// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    static const char *dummy_filename = "./dummy_file";
    
    // Write fuzzer input to dummy file
    int fd = open(dummy_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen(dummy_filename, "r");
    if (!tif) {
        unlink(dummy_filename);
        return 0;
    }
    
    // Call functions in specified order
    int is_tiled = TIFFIsTiled(tif);
    
    // Allocate memory blocks
    tmsize_t alloc_size = Size > 0 ? (tmsize_t)Size : 1;
    void *mem1 = _TIFFmalloc(alloc_size);
    void *mem2 = _TIFFmalloc(alloc_size);
    
    // Initialize TIFFRGBAImage structure
    TIFFRGBAImage img;
    char emsg[1024] = {0};
    int begin_result = TIFFRGBAImageBegin(&img, tif, 0, emsg);
    
    if (begin_result == 1) {
        // Try to get image data if initialization succeeded
        uint32_t width = img.width;
        uint32_t height = img.height;
        
        // Limit dimensions to avoid excessive memory allocation
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
        
        // Allocate raster buffer
        uint32_t *raster = NULL;
        if (width > 0 && height > 0 && width <= 4096 && height <= 4096) {
            raster = (uint32_t *)_TIFFmalloc(width * height * sizeof(uint32_t));
            if (raster) {
                // Try to get image data with various parameters
                for (uint32_t y = 0; y < height; y += height/4 + 1) {
                    uint32_t rows_to_read = (height - y) > 100 ? 100 : (height - y);
                    if (rows_to_read > 0) {
                        TIFFRGBAImageGet(&img, raster + y * width, width, rows_to_read);
                    }
                }
                _TIFFfree(raster);
            }
        }
    }
    
    // Clean up
    TIFFRGBAImageEnd(&img);
    
    // Free allocated memory
    _TIFFfree(mem1);
    _TIFFfree(mem2);
    
    // Two more _TIFFfree calls as specified in the order
    _TIFFfree(NULL);  // Safe with NULL
    _TIFFfree(NULL);  // Safe with NULL
    
    // Close TIFF file and clean up
    TIFFClose(tif);
    unlink(dummy_filename);
    
    return 0;
}