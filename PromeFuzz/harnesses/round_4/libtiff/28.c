// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFIsTiled at tif_open.c:859:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFRawStripSize64 at tif_strip.c:153:10 in tiffio.h
// TIFFGetStrileOffset at tif_dirread.c:8349:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <unistd.h>

static int write_to_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    
    const char* filename = "./dummy_file";
    if (!write_to_file(filename, Data, Size)) {
        return 0;
    }
    
    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }
    
    uint32_t width = 0, height = 0, strips_per_image = 0, rows_per_strip = 0;
    uint16_t samples_per_pixel = 1, bits_per_sample = 8;
    uint16_t planar_config = PLANARCONFIG_CONTIG;
    
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    
    if (rows_per_strip == 0 || rows_per_strip > height) {
        rows_per_strip = height;
    }
    
    strips_per_image = TIFFNumberOfStrips(tif);
    uint32_t tiles_across = 0, tiles_down = 0;
    uint32_t tile_width = 0, tile_height = 0;
    uint32_t tiles_per_image = 0;
    
    if (TIFFIsTiled(tif)) {
        TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
        TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
        if (tile_width > 0 && tile_height > 0) {
            tiles_across = (width + tile_width - 1) / tile_width;
            tiles_down = (height + tile_height - 1) / tile_height;
            tiles_per_image = tiles_across * tiles_down;
        }
    }
    
    for (uint32_t i = 0; i < 3; i++) {
        uint32_t test_index = i;
        if (strips_per_image > 0 && test_index >= strips_per_image) {
            test_index = strips_per_image - 1;
        }
        if (tiles_per_image > 0 && test_index >= tiles_per_image) {
            test_index = tiles_per_image - 1;
        }
        
        TIFFGetStrileByteCount(tif, test_index);
        TIFFRawStripSize64(tif, test_index);
        TIFFGetStrileOffset(tif, test_index);
        
        if (i == 0) {
            TIFFStripSize64(tif);
            TIFFTileSize64(tif);
        }
    }
    
    for (uint32_t nrows = 1; nrows <= 3; nrows++) {
        TIFFVStripSize64(tif, nrows);
    }
    
    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}