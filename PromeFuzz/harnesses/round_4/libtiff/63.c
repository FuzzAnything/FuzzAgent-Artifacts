// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
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
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzz data to a temporary file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Try to open the file as TIFF
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        // If opening in read mode fails, try write mode
        tif = TIFFOpen("./dummy_file", "w");
        if (!tif) {
            unlink("./dummy_file");
            return 0;
        }
        
        // For write mode, set some basic tags to make the file valid
        uint32_t width = 256;
        uint32_t height = 256;
        uint16_t samplesperpixel = 1;
        uint16_t bitspersample = 8;
        uint16_t photometric = PHOTOMETRIC_MINISBLACK;
        uint16_t planarconfig = PLANARCONFIG_CONTIG;
        uint16_t compression = COMPRESSION_NONE;
        
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, planarconfig);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
        
        // Try to set tiling parameters
        uint32_t tilewidth = 64;
        uint32_t tilelength = 64;
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tilewidth);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tilelength);
    }

    // Use fuzz data to generate parameters for API calls
    uint32_t param1 = 0, param2 = 0, param3 = 0;
    uint16_t param4 = 0;
    
    if (Size >= sizeof(uint32_t) * 3 + sizeof(uint16_t)) {
        memcpy(&param1, Data, sizeof(uint32_t));
        memcpy(&param2, Data + sizeof(uint32_t), sizeof(uint32_t));
        memcpy(&param3, Data + sizeof(uint32_t) * 2, sizeof(uint32_t));
        memcpy(&param4, Data + sizeof(uint32_t) * 3, sizeof(uint16_t));
    } else if (Size > 0) {
        // Use available data
        size_t offset = 0;
        if (Size >= sizeof(uint32_t)) {
            memcpy(&param1, Data, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        if (Size - offset >= sizeof(uint32_t)) {
            memcpy(&param2, Data + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        if (Size - offset >= sizeof(uint32_t)) {
            memcpy(&param3, Data + offset, sizeof(uint32_t));
            offset += sizeof(uint32_t);
        }
        if (Size - offset >= sizeof(uint16_t)) {
            memcpy(&param4, Data + offset, sizeof(uint16_t));
        }
    }

    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Test TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    
    // Test TIFFVStripSize with different row counts
    uint32_t nrows = param1 % 256;  // Limit to reasonable values
    if (nrows == 0) nrows = 1;
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows);
    
    // Test TIFFReadEncodedStrip if we have strips
    if (!TIFFIsTiled(tif) && num_tiles == 0) {
        uint32_t strip = param2 % 100;  // Reasonable strip number
        if (vstrip_size > 0 && vstrip_size < 1048576) {  // Limit to 1MB
            void *buf = malloc(vstrip_size);
            if (buf) {
                TIFFReadEncodedStrip(tif, strip, buf, vstrip_size);
                free(buf);
            }
        }
    }
    
    // Test tile-related functions if the file is tiled
    if (TIFFIsTiled(tif) && tile_size > 0 && tile_size < 1048576) {  // Limit to 1MB
        // Allocate buffer for tile operations
        void *tile_buf = malloc(tile_size);
        if (tile_buf) {
            // Initialize buffer with some data
            memset(tile_buf, 0xAA, tile_size);
            
            // Test TIFFReadTile
            uint32_t x = param1 % 1024;
            uint32_t y = param2 % 1024;
            uint32_t z = param3 % 16;
            uint16_t s = param4 % 4;
            TIFFReadTile(tif, tile_buf, x, y, z, s);
            
            // Test TIFFWriteTile
            TIFFWriteTile(tif, tile_buf, x, y, z, s);
            
            free(tile_buf);
        }
    }
    
    // Cleanup
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}