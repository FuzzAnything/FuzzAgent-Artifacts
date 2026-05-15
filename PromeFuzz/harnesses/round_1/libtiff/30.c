// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
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
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings/errors
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);
    
    // Write fuzzer input to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Get basic image information
    uint32_t width = 0, height = 0;
    uint16_t samplesperpixel = 0, bitspersample = 0;
    uint32_t rowsperstrip = 0;
    uint16_t planarconfig = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
    
    // Calculate number of strips/tiles
    uint32_t nstrips = 0;
    if (TIFFIsTiled(tif)) {
        uint32_t tilewidth = 0, tilelength = 0;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength);
        if (tilewidth > 0 && tilelength > 0 && width > 0 && height > 0) {
            uint32_t tilesacross = (width + tilewidth - 1) / tilewidth;
            uint32_t tilesdown = (height + tilelength - 1) / tilelength;
            nstrips = tilesacross * tilesdown;
        }
    } else {
        if (rowsperstrip > 0 && height > 0) {
            nstrips = (height + rowsperstrip - 1) / rowsperstrip;
        }
    }
    
    // Call target functions with various parameters
    if (nstrips > 0) {
        // Test with valid strip/tile indices
        for (uint32_t i = 0; i < nstrips && i < 10; i++) {
            (void)TIFFGetStrileByteCount(tif, i);
            (void)TIFFGetStrileOffset(tif, i);
            (void)TIFFRawStripSize64(tif, i);
        }
        
        // Test with some out-of-bounds indices
        if (nstrips < UINT32_MAX - 100) {
            (void)TIFFGetStrileByteCount(tif, nstrips);
            (void)TIFFGetStrileOffset(tif, nstrips);
            (void)TIFFRawStripSize64(tif, nstrips);
        }
    }
    
    // Test tile size functions
    (void)TIFFTileSize64(tif);
    
    // Test strip size functions
    (void)TIFFStripSize64(tif);
    
    // Test variable strip size functions with various row counts
    if (height > 0) {
        // Test with 1 row
        (void)TIFFVStripSize64(tif, 1);
        
        // Test with typical strip size
        if (rowsperstrip > 0) {
            (void)TIFFVStripSize64(tif, rowsperstrip);
        }
        
        // Test with full image height
        (void)TIFFVStripSize64(tif, height);
        
        // Test with various other values
        for (uint32_t nrows = 1; nrows <= 100 && nrows < height; nrows *= 2) {
            (void)TIFFVStripSize64(tif, nrows);
        }
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}