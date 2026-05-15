// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClientOpenExt at tif_open.c:300:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFFileName at tif_open.c:798:13 in tiffio.h
// TIFFSetFileName at tif_open.c:803:13 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFFdOpen at tif_unix.c:209:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static tmsize_t myReadProc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fread(buf, 1, size, file);
}

static tmsize_t myWriteProc(thandle_t handle, void* buf, tmsize_t size) {
    FILE* file = (FILE*)handle;
    return fwrite(buf, 1, size, file);
}

static uint64_t mySeekProc(thandle_t handle, uint64_t offset, int whence) {
    FILE* file = (FILE*)handle;
    if (fseek(file, (long)offset, whence) == 0)
        return ftell(file);
    return (uint64_t)-1;
}

static int myCloseProc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    return fclose(file);
}

static uint64_t mySizeProc(thandle_t handle) {
    FILE* file = (FILE*)handle;
    long pos = ftell(file);
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, pos, SEEK_SET);
    return (uint64_t)size;
}

static int myMapProc(thandle_t handle, void** base, uint64_t* size) {
    (void)handle;
    (void)base;
    (void)size;
    return 0;
}

static void myUnmapProc(thandle_t handle, void* base, uint64_t size) {
    (void)handle;
    (void)base;
    (void)size;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const char* filename = "./dummy_file";
    
    // Write fuzzing data to file
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Test different modes
    const char* modes[] = {"r", "w", "a"};
    for (size_t m = 0; m < sizeof(modes)/sizeof(modes[0]); m++) {
        // Test TIFFOpen
        TIFF* tif = TIFFOpen(filename, modes[m]);
        if (tif) {
            // Test TIFFFileName
            const char* name = TIFFFileName(tif);
            (void)name; // Use result to avoid unused variable warning
            
            // Test TIFFSetFileName
            const char* new_name = "test_name.tif";
            const char* old_name = TIFFSetFileName(tif, new_name);
            (void)old_name; // Use result
            
            TIFFClose(tif);
        }
        
        // Test TIFFOpenExt with NULL options
        tif = TIFFOpenExt(filename, modes[m], NULL);
        if (tif) {
            TIFFClose(tif);
        }
        
        // Test TIFFFdOpen (POSIX version)
        int fd = open(filename, O_RDONLY);
        if (fd >= 0) {
            tif = TIFFFdOpen(fd, filename, modes[m]);
            if (tif) {
                TIFFClose(tif);
            } else {
                close(fd);
            }
        }
    }
    
    // Test TIFFClientOpenExt with custom I/O and NULL options
    FILE* custom_file = fopen(filename, "rb");
    if (custom_file) {
        TIFF* tif = TIFFClientOpenExt(filename, "r", 
                                     (thandle_t)custom_file,
                                     myReadProc,
                                     myWriteProc,
                                     mySeekProc,
                                     myCloseProc,
                                     mySizeProc,
                                     myMapProc,
                                     myUnmapProc,
                                     NULL);
        if (tif) {
            TIFFClose(tif);
        } else {
            fclose(custom_file);
        }
    }
    
    // Clean up
    unlink(filename);
    
    return 0;
}