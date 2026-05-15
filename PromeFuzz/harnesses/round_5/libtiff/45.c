// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRGBAImageBegin at tif_getimage.c:310:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// TIFFRGBAImageGet at tif_getimage.c:587:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFRGBAImageEnd at tif_getimage.c:253:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Test TIFFRGBAImageOK
    char emsg[1024];
    int ok = TIFFRGBAImageOK(tif, emsg);
    
    // Test TIFFReadRGBAImage if dimensions are available
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        
        // Limit dimensions to prevent excessive memory allocation
        if (width > 0 && width < 4096 && height > 0 && height < 4096) {
            uint32_t *raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
            if (raster) {
                // Test TIFFReadRGBAImage
                TIFFReadRGBAImage(tif, width, height, raster, 0);
                
                // Test TIFFReadRGBAImageOriented with different orientations
                for (int orientation = 1; orientation <= 8; orientation++) {
                    TIFFReadRGBAImageOriented(tif, width, height, raster, orientation, 0);
                }
                
                _TIFFfree(raster);
            }
        }
    }
    
    // Test TIFFRGBAImage functions if RGBA conversion is possible
    if (ok) {
        TIFFRGBAImage img;
        memset(&img, 0, sizeof(TIFFRGBAImage));
        
        // Test TIFFRGBAImageBegin
        if (TIFFRGBAImageBegin(&img, tif, 0, emsg)) {
            // Test TIFFRGBAImageGet with various parameters
            if (img.width > 0 && img.height > 0) {
                // Limit dimensions for memory safety
                uint32_t test_width = img.width > 1024 ? 1024 : img.width;
                uint32_t test_height = img.height > 1024 ? 1024 : img.height;
                
                uint32_t *raster = (uint32_t*)_TIFFmalloc(test_width * test_height * sizeof(uint32_t));
                if (raster) {
                    // Test with full image area
                    TIFFRGBAImageGet(&img, raster, img.width, img.height);
                    
                    // Test with partial area
                    if (test_width > 10 && test_height > 10) {
                        TIFFRGBAImageGet(&img, raster, 10, 10);
                    }
                    
                    _TIFFfree(raster);
                }
            }
            
            // Test TIFFRGBAImageEnd
            TIFFRGBAImageEnd(&img);
        }
        
        // Test TIFFRGBAImageEnd on uninitialized structure
        TIFFRGBAImage img2;
        memset(&img2, 0, sizeof(TIFFRGBAImage));
        TIFFRGBAImageEnd(&img2);
    }
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}