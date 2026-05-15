// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer input to file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove("./dummy_file");
        return 0;
    }
    
    // Get image dimensions if available
    uint32_t width = 0, height = 0;
    if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) &&
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height)) {
        
        // Allocate buffer for RGBA data
        uint32_t *raster = NULL;
        if (width > 0 && height > 0 && width < 10000 && height < 10000) {
            size_t raster_size = (size_t)width * (size_t)height;
            if (raster_size < 1000000) { // Limit to reasonable size
                raster = (uint32_t *)_TIFFmalloc(raster_size * sizeof(uint32_t));
            }
        }
        
        if (raster) {
            // Test TIFFReadRGBAImage
            TIFFReadRGBAImage(tif, width, height, raster, 0);
            
            // Test TIFFReadRGBAImageOriented with different orientations
            for (int orientation = 0; orientation <= 8; orientation++) {
                TIFFReadRGBAImageOriented(tif, width, height, raster, orientation, 0);
            }
            
            // Test strip-based functions if image is stripped
            if (!TIFFIsTiled(tif)) {
                uint32_t rows_per_strip = 0;
                if (TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip)) {
                    if (rows_per_strip > 0 && rows_per_strip <= height) {
                        // Test TIFFReadRGBAStrip
                        for (uint32_t row = 0; row < height; row += rows_per_strip) {
                            TIFFReadRGBAStrip(tif, row, raster);
                        }
                        
                        // Test TIFFReadRGBAStripExt
                        for (uint32_t row = 0; row < height; row += rows_per_strip) {
                            TIFFReadRGBAStripExt(tif, row, raster, 0);
                        }
                    }
                }
            }
            
            // Test tile-based functions if image is tiled
            if (TIFFIsTiled(tif)) {
                uint32_t tile_width = 0, tile_height = 0;
                if (TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width) &&
                    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height)) {
                    
                    if (tile_width > 0 && tile_height > 0) {
                        // Test TIFFReadRGBATileExt
                        for (uint32_t row = 0; row < height; row += tile_height) {
                            for (uint32_t col = 0; col < width; col += tile_width) {
                                TIFFReadRGBATileExt(tif, col, row, raster, 0);
                            }
                        }
                    }
                }
            }
            
            _TIFFfree(raster);
        }
    }
    
    // Test TIFFRGBAImageOK
    char emsg[1024];
    TIFFRGBAImageOK(tif, emsg);
    
    // Cleanup
    TIFFClose(tif);
    remove("./dummy_file");
    
    return 0;
}