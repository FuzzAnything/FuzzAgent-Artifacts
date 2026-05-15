// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static void dummy_handle_error(const char* module, const char* fmt, va_list ap) {
    // Suppress error messages during fuzzing
}

static void dummy_handle_warning(const char* module, const char* fmt, va_list ap) {
    // Suppress warning messages during fuzzing
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Set error handlers to suppress output
    TIFFSetErrorHandler(dummy_handle_error);
    TIFFSetWarningHandler(dummy_handle_warning);

    // Write fuzzing data to a dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    
    // Write data to file
    ssize_t written = write(fd, Data, Size);
    close(fd);
    
    if (written != (ssize_t)Size) {
        unlink("./dummy_file");
        return 0;
    }

    // Open the TIFF file
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Call TIFFIsTiled
    int is_tiled = TIFFIsTiled(tif);
    (void)is_tiled; // Result not used but function called

    // First _TIFFmalloc call - allocate small buffer
    size_t alloc_size1 = Size > 0 ? (Data[0] % 1024) + 1 : 64;
    void* mem1 = _TIFFmalloc(alloc_size1);
    
    // Second _TIFFmalloc call - allocate another buffer
    size_t alloc_size2 = Size > 1 ? (Data[1] % 2048) + 1 : 128;
    void* mem2 = _TIFFmalloc(alloc_size2);

    // Initialize TIFFRGBAImage structure
    TIFFRGBAImage img;
    char emsg[1024];
    
    // Call TIFFRGBAImageBegin
    int begin_result = TIFFRGBAImageBegin(&img, tif, 0, emsg);
    
    if (begin_result == 1) {
        // Get image dimensions
        uint32_t width = img.width;
        uint32_t height = img.height;
        
        // Allocate raster buffer for TIFFRGBAImageGet
        uint32_t* raster = NULL;
        if (width > 0 && height > 0 && width < 4096 && height < 4096) {
            size_t raster_size = width * height * sizeof(uint32_t);
            if (raster_size > 0 && raster_size < 100 * 1024 * 1024) { // Limit to 100MB
                raster = (uint32_t*)_TIFFmalloc(raster_size);
            }
        }
        
        if (raster) {
            // Call TIFFRGBAImageGet with various parameters based on fuzzing input
            uint32_t row_start = 0;
            uint32_t row_count = height;
            
            // Use fuzzing data to determine parameters if possible
            if (Size > 4) {
                row_start = (Data[2] % height);
                row_count = (Data[3] % (height - row_start)) + 1;
                if (row_count == 0) row_count = 1;
            }
            
            // Call TIFFRGBAImageGet
            int get_result = TIFFRGBAImageGet(&img, raster, row_start, row_count);
            (void)get_result; // Result not used but function called
            
            // Free the raster buffer
            _TIFFfree(raster);
        }
        
        // Call TIFFRGBAImageEnd
        TIFFRGBAImageEnd(&img);
    }

    // Clean up allocated memory in specified order
    if (mem1) _TIFFfree(mem1);
    if (mem2) _TIFFfree(mem2);
    
    // Two additional _TIFFfree calls as specified
    _TIFFfree(NULL); // Safe with NULL
    _TIFFfree(NULL); // Safe with NULL

    // Close TIFF file
    TIFFClose(tif);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}