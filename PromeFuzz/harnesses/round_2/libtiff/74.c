// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFCurrentTile at tif_open.c:879:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
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
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
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

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

static TIFF* create_dummy_tiff(const uint8_t* data, size_t size) {
    // Write data to dummy file
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return NULL;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    
    if (written != (ssize_t)size) {
        return NULL;
    }
    
    // Set error/warning handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);
    
    // Try to open in read mode first
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (tif) return tif;
    
    // If read fails, try to create a new file for writing
    tif = TIFFOpen("./dummy_file", "w");
    if (!tif) return NULL;
    
    // Set some basic tags to make the TIFF handle somewhat valid
    // Use values derived from fuzzer input to explore different states
    uint32_t width = 1, height = 1;
    if (size >= 8) {
        width = *(uint32_t*)data;
        height = *((uint32_t*)data + 1);
        if (width == 0) width = 1;
        if (height == 0) height = 1;
    }
    
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 3);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    
    // Randomly choose between tiled and stripped layout
    if (size > 0 && (data[0] & 0x01)) {
        uint32_t tileWidth = 256, tileHeight = 256;
        if (size >= 12) {
            tileWidth = *((uint32_t*)data + 2);
            tileHeight = *((uint32_t*)data + 3);
            if (tileWidth == 0) tileWidth = 256;
            if (tileHeight == 0) tileHeight = 256;
        }
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tileWidth);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tileHeight);
    } else {
        uint32_t rowsPerStrip = 1;
        if (size >= 4) {
            rowsPerStrip = *(uint32_t*)data;
            if (rowsPerStrip == 0) rowsPerStrip = 1;
        }
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsPerStrip);
    }
    
    return tif;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    TIFF* tif = create_dummy_tiff(Data, Size);
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Extract parameters from fuzzer input
    uint32_t param1 = 1, param2 = 1;
    if (Size >= 8) {
        param1 = *(uint32_t*)Data;
        param2 = *((uint32_t*)Data + 1);
    } else if (Size >= 4) {
        param1 = *(uint32_t*)Data;
    }
    
    // Call target functions with various parameters
    // 1. TIFFVTileSize
    tmsize_t tile_size = TIFFVTileSize(tif, param1);
    (void)tile_size;
    
    // 2. TIFFDefaultStripSize
    uint32_t strip_size = TIFFDefaultStripSize(tif, param1);
    (void)strip_size;
    
    // 3. TIFFVTileSize64
    uint64_t tile_size64 = TIFFVTileSize64(tif, param1);
    (void)tile_size64;
    
    // 4. TIFFCurrentTile
    uint32_t current_tile = TIFFCurrentTile(tif);
    (void)current_tile;
    
    // 5. TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;
    
    // 6. TIFFDefaultTileSize
    uint32_t tile_width = 0, tile_height = 0;
    TIFFDefaultTileSize(tif, &tile_width, &tile_height);
    (void)tile_width;
    (void)tile_height;
    
    // Try calling functions with different parameters to explore more states
    if (Size >= 12) {
        uint32_t param3 = *((uint32_t*)Data + 2);
        TIFFVTileSize(tif, param3);
        TIFFDefaultStripSize(tif, param3);
        TIFFVTileSize64(tif, param3);
    }
    
    // Clean up
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}