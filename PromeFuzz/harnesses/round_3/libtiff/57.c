// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
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
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to open the file with libtiff
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif) {
        // Test TIFFCurrentTile
        uint32_t current_tile = TIFFCurrentTile(tif);
        (void)current_tile; // Use result to avoid unused variable warning
        
        // Test TIFFNumberOfTiles
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        (void)num_tiles;
        
        // Generate parameters from fuzz data
        uint32_t nrows = 1;
        uint32_t request = 8192; // Default 8KB request
        
        if (Size >= sizeof(uint32_t)) {
            // Use first 4 bytes for nrows
            memcpy(&nrows, Data, sizeof(uint32_t));
            // Use next 4 bytes for request if available
            if (Size >= 2 * sizeof(uint32_t)) {
                memcpy(&request, Data + sizeof(uint32_t), sizeof(uint32_t));
            }
        }
        
        // Test TIFFVTileSize
        tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
        (void)vtile_size;
        
        // Test TIFFVTileSize64
        uint64_t vtile_size64 = TIFFVTileSize64(tif, nrows);
        (void)vtile_size64;
        
        // Test TIFFDefaultStripSize
        uint32_t strip_size = TIFFDefaultStripSize(tif, request);
        (void)strip_size;
        
        // Test TIFFDefaultTileSize
        uint32_t tile_width = 0, tile_height = 0;
        TIFFDefaultTileSize(tif, &tile_width, &tile_height);
        (void)tile_width;
        (void)tile_height;
        
        // Try to read directory to explore more states
        TIFFReadDirectory(tif);
        
        TIFFClose(tif);
    }
    
    // Also test with write mode
    tif = TIFFOpen("./dummy_file", "w");
    if (tif) {
        // Set some basic fields to make the TIFF valid for operations
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 256);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 256);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, 16);
        TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        
        // Test all functions again in write mode
        uint32_t current_tile = TIFFCurrentTile(tif);
        (void)current_tile;
        
        uint32_t num_tiles = TIFFNumberOfTiles(tif);
        (void)num_tiles;
        
        // Generate parameters again (different offsets)
        uint32_t nrows = 16;
        uint32_t request = 4096;
        
        if (Size >= sizeof(uint32_t)) {
            // Use different bytes for parameters
            size_t offset = Size > 8 ? 8 : 0;
            if (Size >= offset + sizeof(uint32_t)) {
                memcpy(&nrows, Data + offset, sizeof(uint32_t));
            }
            offset = Size > 12 ? 12 : 0;
            if (Size >= offset + sizeof(uint32_t)) {
                memcpy(&request, Data + offset, sizeof(uint32_t));
            }
        }
        
        tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
        (void)vtile_size;
        
        uint64_t vtile_size64 = TIFFVTileSize64(tif, nrows);
        (void)vtile_size64;
        
        uint32_t strip_size = TIFFDefaultStripSize(tif, request);
        (void)strip_size;
        
        uint32_t tile_width = 0, tile_height = 0;
        TIFFDefaultTileSize(tif, &tile_width, &tile_height);
        (void)tile_width;
        (void)tile_height;
        
        // Try setting up as tiled image
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, 64);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, 64);
        
        // Test functions again with tiled configuration
        num_tiles = TIFFNumberOfTiles(tif);
        (void)num_tiles;
        
        vtile_size = TIFFVTileSize(tif, nrows);
        (void)vtile_size;
        
        vtile_size64 = TIFFVTileSize64(tif, nrows);
        (void)vtile_size64;
        
        TIFFClose(tif);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}