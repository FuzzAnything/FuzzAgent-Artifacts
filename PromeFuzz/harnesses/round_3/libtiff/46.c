// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFFileno at tif_open.c:813:5 in tiffio.h
// TIFFSetFileno at tif_open.c:818:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpenExt at tif_unix.c:214:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "libtiff/tiffio.h"

// Structure to hold fuzzing data for custom I/O
typedef struct {
    const uint8_t* buffer;
    size_t size;
    size_t pos;
} fuzz_data;

// Custom I/O handlers for TIFFClientOpen/TIFFClientOpenExt
static tmsize_t fuzz_readproc(thandle_t handle, void* buf, tmsize_t size) {
    fuzz_data* data = (fuzz_data*)handle;
    if (data->pos >= data->size) return 0;
    
    tmsize_t to_read = size;
    if (data->pos + to_read > data->size) {
        to_read = data->size - data->pos;
    }
    
    memcpy(buf, data->buffer + data->pos, to_read);
    data->pos += to_read;
    return to_read;
}

static tmsize_t fuzz_writeproc(thandle_t handle, void* buf, tmsize_t size) {
    return size;
}

static toff_t fuzz_seekproc(thandle_t handle, toff_t offset, int whence) {
    fuzz_data* data = (fuzz_data*)handle;
    
    switch (whence) {
        case SEEK_SET:
            data->pos = offset;
            break;
        case SEEK_CUR:
            data->pos += offset;
            break;
        case SEEK_END:
            data->pos = data->size + offset;
            break;
    }
    
    if (data->pos > data->size) data->pos = data->size;
    if (data->pos < 0) data->pos = 0;
    
    return data->pos;
}

static int fuzz_closeproc(thandle_t handle) {
    return 0;
}

static toff_t fuzz_sizeproc(thandle_t handle) {
    fuzz_data* data = (fuzz_data*)handle;
    return data->size;
}

static int fuzz_mapproc(thandle_t handle, void** base, toff_t* size) {
    fuzz_data* data = (fuzz_data*)handle;
    *base = (void*)data->buffer;
    *size = data->size;
    return 1;
}

static void fuzz_unmapproc(thandle_t handle, void* base, toff_t size) {
    // Nothing to do
}

static int create_dummy_file(const uint8_t* data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    // Create dummy file for file descriptor based operations
    if (create_dummy_file(Data, Size) < 0) {
        return 0;
    }
    
    // Test 1: TIFFFdOpen and TIFFFdOpenExt
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        // Test TIFFFdOpen
        TIFF* tif1 = TIFFFdOpen(fd, "./dummy_file", "r");
        if (tif1) {
            // Test TIFFFileno
            int retrieved_fd = TIFFFileno(tif1);
            (void)retrieved_fd; // Suppress unused variable warning
            
            // Test TIFFSetFileno
            int old_fd = TIFFSetFileno(tif1, fd);
            (void)old_fd; // Suppress unused variable warning
            
            TIFFClose(tif1);
        } else {
            close(fd);
        }
        
        // Reopen for TIFFFdOpenExt
        fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            TIFF* tif2 = TIFFFdOpenExt(fd, "./dummy_file", "r", NULL);
            if (tif2) {
                TIFFClose(tif2);
            } else {
                close(fd);
            }
        }
    }
    
    // Test 2: TIFFClientOpen and TIFFClientOpenExt with custom I/O
    fuzz_data fdata = {Data, Size, 0};
    
    // Test TIFFClientOpen
    TIFF* tif3 = TIFFClientOpen("fuzz", "r", (thandle_t)&fdata,
                               fuzz_readproc, fuzz_writeproc,
                               fuzz_seekproc, fuzz_closeproc,
                               fuzz_sizeproc, fuzz_mapproc, fuzz_unmapproc);
    if (tif3) {
        // Try to read some data to exercise the handlers
        uint32_t width = 0;
        TIFFGetField(tif3, TIFFTAG_IMAGEWIDTH, &width);
        
        TIFFClose(tif3);
    }
    
    // Reset position for next test
    fdata.pos = 0;
    
    // Test TIFFClientOpenExt
    TIFF* tif4 = TIFFClientOpenExt("fuzz_ext", "r", (thandle_t)&fdata,
                                  fuzz_readproc, fuzz_writeproc,
                                  fuzz_seekproc, fuzz_closeproc,
                                  fuzz_sizeproc, fuzz_mapproc, fuzz_unmapproc,
                                  NULL);
    if (tif4) {
        // Exercise the TIFF handle
        uint16_t samplesperpixel = 0;
        TIFFGetField(tif4, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
        
        TIFFClose(tif4);
    }
    
    // Clean up
    unlink("./dummy_file");
    
    return 0;
}