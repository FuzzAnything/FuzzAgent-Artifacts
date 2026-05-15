// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFIsUpSampled at tif_open.c:889:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRGBATile at tif_getimage.c:3416:5 in tiffio.h
// TIFFGetFieldDefaulted at tif_aux.c:380:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFReadRGBAStrip at tif_getimage.c:3341:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "libtiff/tiffio.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(data, 1, size, fp);
    fclose(fp);
    return 1;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }
    
    if (!write_dummy_file(Data, Size)) {
        return 0;
    }
    
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }
    
    uint32_t width, height;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    int is_tiled = TIFFIsTiled(tif);
    int is_upsampled = TIFFIsUpSampled(tif);
    
    if (is_tiled) {
        uint32_t tile_width, tile_length;
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_length);
        
        uint32_t tile_count = TIFFNumberOfTiles(tif);
        
        if (tile_count > 0) {
            uint32_t tile_index = Data[0] % tile_count;
            
            uint32_t x = (Data[1] * 256 + Data[2]) % width;
            uint32_t y = (Data[3] * 256 + Data[4]) % height;
            uint32_t z = 0;
            uint16_t s = Data[5] % 256;
            
            int check_result = TIFFCheckTile(tif, x, y, z, s);
            
            tmsize_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0) {
                void *tile_buffer = malloc(tile_size);
                if (tile_buffer) {
                    TIFFReadEncodedTile(tif, tile_index, tile_buffer, tile_size);
                    free(tile_buffer);
                }
            }
            
            uint32_t *rgba_buffer = malloc(tile_width * tile_length * sizeof(uint32_t));
            if (rgba_buffer) {
                uint32_t tile_x = (x / tile_width) * tile_width;
                uint32_t tile_y = (y / tile_length) * tile_length;
                TIFFReadRGBATile(tif, tile_x, tile_y, rgba_buffer);
                free(rgba_buffer);
            }
        }
    } else {
        uint32_t rows_per_strip;
        TIFFGetFieldDefaulted(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
        
        uint32_t strip_count = TIFFNumberOfStrips(tif);
        
        if (strip_count > 0) {
            uint32_t strip_index = Data[0] % strip_count;
            uint32_t *strip_buffer = malloc(width * rows_per_strip * sizeof(uint32_t));
            if (strip_buffer) {
                TIFFReadRGBAStrip(tif, strip_index * rows_per_strip, strip_buffer);
                free(strip_buffer);
            }
        }
    }
    
    TIFFClose(tif);
    unlink("./dummy_file");
    
    return 0;
}