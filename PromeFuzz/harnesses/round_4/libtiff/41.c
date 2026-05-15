// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFRGBAImageOK at tif_getimage.c:83:5 in tiffio.h
// TIFFReadRGBAImageOriented at tif_getimage.c:631:5 in tiffio.h
// TIFFReadRGBAImage at tif_getimage.c:656:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBATileExt at tif_getimage.c:3422:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFReadRGBAStripExt at tif_getimage.c:3347:5 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff error/warning messages during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors to avoid output pollution
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzing data to a dummy file
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    // Get image dimensions if available
    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    // Limit dimensions to avoid excessive memory allocation
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    // Allocate buffer for RGBA data
    uint32_t* raster = NULL;
    if (width > 0 && height > 0 && width * height < 1000000) {
        raster = (uint32_t*)_TIFFmalloc(width * height * sizeof(uint32_t));
    }
    
    // Test TIFFRGBAImageOK
    char emsg[1024];
    TIFFRGBAImageOK(tif, emsg);
    
    // Test TIFFReadRGBAImageOriented if we have valid dimensions
    if (raster && width > 0 && height > 0) {
        TIFFReadRGBAImageOriented(tif, width, height, raster, ORIENTATION_TOPLEFT, 0);
    }
    
    // Test TIFFReadRGBAImage
    if (raster && width > 0 && height > 0) {
        TIFFReadRGBAImage(tif, width, height, raster, 0);
    }
    
    // Test tile-based functions if image is tiled
    uint32_t is_tiled = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &is_tiled);
    
    if (is_tiled && raster) {
        uint32_t tile_width = 0, tile_height = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        
        if (tile_width > 0 && tile_height > 0 && tile_width * tile_height < 100000) {
            // Test TIFFReadRGBATileExt for first tile
            TIFFReadRGBATileExt(tif, 0, 0, raster, 0);
        }
    }
    
    // Test strip-based functions
    uint32_t rows_per_strip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    if (rows_per_strip > 0 && rows_per_strip < 10000 && raster) {
        // Test TIFFReadRGBAStripExt for first strip
        TIFFReadRGBAStripExt(tif, 0, raster, 0);
        
        // Test TIFFReadRGBAStrip for first strip
        TIFFReadRGBAStrip(tif, 0, raster);
    }
    
    // Cleanup
    if (raster) {
        _TIFFfree(raster);
    }
    
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}