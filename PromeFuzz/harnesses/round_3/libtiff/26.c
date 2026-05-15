// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadEncodedStrip at tif_read.c:534:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
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
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        remove_dummy_file();
        return 0;
    }
    
    uint32_t width = 0, height = 0, depth = 0;
    uint16_t samples = 0, planar = 0;
    uint32_t tile_width = 0, tile_height = 0, tile_depth = 0;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_IMAGEDEPTH, &depth);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    TIFFGetField(tif, TIFFTAG_TILEDEPTH, &tile_depth);
    
    uint32_t is_tiled = TIFFIsTiled(tif);
    
    if (is_tiled) {
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < 10485760) {
            void *tile_buf = malloc(tile_size);
            if (tile_buf) {
                uint32_t num_tiles = TIFFNumberOfTiles(tif);
                if (num_tiles > 0) {
                    uint32_t tile_idx = *((uint32_t*)Data) % num_tiles;
                    uint32_t x = *((uint32_t*)(Data + 4)) % (width > 0 ? width : 1);
                    uint32_t y = *((uint32_t*)(Data + 8)) % (height > 0 ? height : 1);
                    uint32_t z = *((uint32_t*)(Data + 12)) % (depth > 0 ? depth : 1);
                    uint16_t s = *((uint16_t*)(Data + 16)) % (samples > 0 ? samples : 1);
                    
                    TIFFReadTile(tif, tile_buf, x, y, z, s);
                    TIFFWriteTile(tif, tile_buf, x, y, z, s);
                }
                free(tile_buf);
            }
        }
    } else {
        tmsize_t strip_size = TIFFStripSize(tif);
        if (strip_size > 0 && strip_size < 10485760) {
            void *strip_buf = malloc(strip_size);
            if (strip_buf) {
                uint32_t num_strips = TIFFNumberOfStrips(tif);
                if (num_strips > 0) {
                    uint32_t strip = *((uint32_t*)Data) % num_strips;
                    tmsize_t size = *((tmsize_t*)(Data + 4)) % (strip_size + 1);
                    if (size == 0) size = -1;
                    TIFFReadEncodedStrip(tif, strip, strip_buf, size);
                }
                free(strip_buf);
            }
        }
        
        uint32_t nrows = *((uint32_t*)(Data + 8)) % (height > 0 ? height : 1);
        if (nrows == 0) nrows = 1;
        TIFFVStripSize(tif, nrows);
    }
    
    TIFFTileSize(tif);
    TIFFNumberOfTiles(tif);
    
    TIFFClose(tif);
    remove_dummy_file();
    return 0;
}