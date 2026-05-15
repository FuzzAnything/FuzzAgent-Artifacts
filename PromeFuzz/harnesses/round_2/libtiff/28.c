// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    // Suppress libtiff error/warning messages
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write fuzzer input to a temporary file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the TIFF file
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Get image dimensions and other metadata
        uint32_t width = 0, height = 0;
        uint16_t samplesperpixel = 0, bitspersample = 0;
        uint16_t planarconfig = 0, photometric = 0;
        uint32_t rowsperstrip = 0;
        uint32_t tilewidth = 0, tileheight = 0;
        uint32_t strile = 0;
        
        TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
        TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
        TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
        TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);
        TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planarconfig);
        TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric);
        TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tileheight);
        
        // Determine if tiled or stripped
        int isTiled = TIFFIsTiled(tif);
        
        // Call target functions with various parameters
        // 1. TIFFGetStrileByteCount
        if (isTiled) {
            uint32_t ntiles = TIFFNumberOfTiles(tif);
            if (ntiles > 0) {
                // Use first few bytes to determine which tile to query
                strile = (Size > 0) ? (Data[0] % ntiles) : 0;
                (void)TIFFGetStrileByteCount(tif, strile);
            }
        } else {
            uint32_t nstrips = TIFFNumberOfStrips(tif);
            if (nstrips > 0) {
                strile = (Size > 1) ? (Data[1] % nstrips) : 0;
                (void)TIFFGetStrileByteCount(tif, strile);
            }
        }
        
        // 2. TIFFStripSize64
        (void)TIFFStripSize64(tif);
        
        // 3. TIFFVStripSize64
        if (rowsperstrip > 0 && height > 0) {
            uint32_t nrows = (Size > 2) ? (Data[2] % height) : 1;
            if (nrows == 0) nrows = 1;
            (void)TIFFVStripSize64(tif, nrows);
        }
        
        // 4. TIFFRawStripSize64
        if (!isTiled) {
            uint32_t nstrips = TIFFNumberOfStrips(tif);
            if (nstrips > 0) {
                uint32_t strip = (Size > 3) ? (Data[3] % nstrips) : 0;
                (void)TIFFRawStripSize64(tif, strip);
            }
        }
        
        // 5. TIFFVTileSize64
        if (isTiled && tileheight > 0) {
            uint32_t nrows = (Size > 4) ? (Data[4] % tileheight) : 1;
            if (nrows == 0) nrows = 1;
            (void)TIFFVTileSize64(tif, nrows);
        }
        
        // 6. TIFFTileSize64
        if (isTiled) {
            (void)TIFFTileSize64(tif);
        }
        
        TIFFClose(tif);
    }
    
    // Clean up temporary file
    unlink("./dummy_file");
    
    return 0;
}