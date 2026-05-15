// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFAccessTagMethods at tif_extension.c:58:17 in tiffio.h
// TIFFCreateDirectory at tif_dir.c:1689:5 in tiffio.h
// TIFFFreeDirectory at tif_dir.c:1619:6 in tiffio.h
// TIFFCreateCustomDirectory at tif_dir.c:1704:5 in tiffio.h
// TIFFWriteCustomDirectory at tif_dirwrite.c:303:5 in tiffio.h
// TIFFReadCustomDirectory at tif_dirread.c:5278:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClientOpen at tif_open.c:289:7 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "libtiff/tiffio.h"

typedef struct mem_buffer {
    uint8_t* data;
    tsize_t size;
    tsize_t pos;
} mem_buffer;

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static tsize_t dummy_read(thandle_t handle, tdata_t buf, tsize_t size) {
    mem_buffer* mb = (mem_buffer*)handle;
    tsize_t to_read = size;
    if (mb->pos >= mb->size) return 0;
    if (mb->pos + to_read > mb->size) to_read = mb->size - mb->pos;
    memcpy(buf, mb->data + mb->pos, to_read);
    mb->pos += to_read;
    return to_read;
}

static tsize_t dummy_write(thandle_t handle, tdata_t buf, tsize_t size) {
    mem_buffer* mb = (mem_buffer*)handle;
    // For simplicity, just track position without actual writing
    mb->pos += size;
    return size;
}

static toff_t dummy_seek(thandle_t handle, toff_t offset, int whence) {
    mem_buffer* mb = (mem_buffer*)handle;
    switch (whence) {
        case SEEK_SET:
            mb->pos = offset;
            break;
        case SEEK_CUR:
            mb->pos += offset;
            break;
        case SEEK_END:
            mb->pos = mb->size + offset;
            break;
    }
    if (mb->pos > mb->size) mb->pos = mb->size;
    if (mb->pos < 0) mb->pos = 0;
    return mb->pos;
}

static int dummy_close(thandle_t handle) {
    mem_buffer* mb = (mem_buffer*)handle;
    free(mb->data);
    free(mb);
    return 0;
}

static toff_t dummy_size(thandle_t handle) {
    mem_buffer* mb = (mem_buffer*)handle;
    return mb->size;
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size, const char* mode) {
    mem_buffer* mb = (mem_buffer*)malloc(sizeof(mem_buffer));
    if (!mb) return NULL;
    
    mb->data = (uint8_t*)malloc(size);
    if (!mb->data) {
        free(mb);
        return NULL;
    }
    memcpy(mb->data, data, size);
    mb->size = size;
    mb->pos = 0;
    
    TIFF* tif = TIFFClientOpen("dummy", mode, (thandle_t)mb,
                               dummy_read, dummy_write,
                               dummy_seek, dummy_close,
                               dummy_size, NULL, NULL);
    if (!tif) {
        free(mb->data);
        free(mb);
    }
    return tif;
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Create a dummy TIFF handle with random data
    TIFF* tif = create_dummy_tiff(Data, Size, "w");
    if (!tif) return 0;
    
    // Test TIFFAccessTagMethods
    TIFFTagMethods* methods = TIFFAccessTagMethods(tif);
    (void)methods; // Use result to avoid unused variable warning
    
    // Test TIFFCreateDirectory
    if (TIFFCreateDirectory(tif) == 1) {
        // Test TIFFFreeDirectory
        TIFFFreeDirectory(tif);
        
        // Test TIFFCreateCustomDirectory - use NULL for field array
        TIFFCreateCustomDirectory(tif, NULL);
        
        // Test TIFFWriteCustomDirectory
        uint64_t dir_offset = 0;
        TIFFWriteCustomDirectory(tif, &dir_offset);
        
        // Test TIFFReadCustomDirectory with random offset
        if (Size >= sizeof(toff_t)) {
            toff_t diroff;
            memcpy(&diroff, Data, sizeof(toff_t));
            // Ensure offset is within reasonable bounds
            diroff = diroff % (Size > 1000 ? 1000 : Size);
            TIFFReadCustomDirectory(tif, diroff, NULL);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    
    return 0;
}