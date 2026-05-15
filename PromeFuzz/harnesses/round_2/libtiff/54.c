// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    write_dummy_file(Data, Size);
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Get TIFF characteristics
    uint32_t width = 0, height = 0, tile_width = 0, tile_height = 0;
    uint32_t tiles_across = 0, tiles_down = 0;
    uint16_t samples_per_pixel = 1, bits_per_sample = 8;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    
    // Calculate number of tiles
    if (tile_width > 0 && tile_height > 0) {
        tiles_across = (width + tile_width - 1) / tile_width;
        tiles_down = (height + tile_height - 1) / tile_height;
    }
    
    uint32_t total_tiles = tiles_across * tiles_down;
    
    // Test TIFFTileSize
    tmsize_t tile_size = TIFFTileSize(tif);
    
    // Test TIFFVTileSize with various row counts
    if (tile_height > 0) {
        for (uint32_t nrows = 1; nrows <= tile_height; nrows *= 2) {
            if (nrows > tile_height) break;
            TIFFVTileSize(tif, nrows);
        }
    }
    
    // Test tile read/write functions if we have tiles
    if (total_tiles > 0 && tile_size > 0) {
        // Allocate buffers
        void *read_buf = malloc(tile_size);
        void *write_buf = malloc(tile_size);
        
        if (read_buf && write_buf) {
            // Use fuzz data to initialize write buffer
            size_t copy_size = Size < tile_size ? Size : tile_size;
            memcpy(write_buf, Data, copy_size);
            
            // Test with different tile indices
            uint32_t tile_indices[] = {0, total_tiles/2, total_tiles-1};
            for (size_t i = 0; i < sizeof(tile_indices)/sizeof(tile_indices[0]); i++) {
                uint32_t tile = tile_indices[i];
                if (tile >= total_tiles) continue;
                
                // Test TIFFReadEncodedTile
                TIFFReadEncodedTile(tif, tile, read_buf, tile_size);
                
                // Test TIFFWriteEncodedTile (need to open in write mode)
                TIFF *tif_write = TIFFOpen("./dummy_file", "a");
                if (tif_write) {
                    TIFFWriteEncodedTile(tif_write, tile, write_buf, tile_size);
                    TIFFClose(tif_write);
                }
                
                // Test TIFFReadRawTile
                TIFFReadRawTile(tif, tile, read_buf, tile_size);
                
                // Test TIFFWriteRawTile
                tif_write = TIFFOpen("./dummy_file", "a");
                if (tif_write) {
                    TIFFWriteRawTile(tif_write, tile, write_buf, tile_size);
                    TIFFClose(tif_write);
                }
            }
        }
        
        free(read_buf);
        free(write_buf);
    }
    
    TIFFClose(tif);
    cleanup_dummy_file();
    
    return 0;
}