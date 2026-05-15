// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFCheckTile at tif_tile.c:71:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFWriteScanline at tif_write.c:47:5 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    const char* filename = "./dummy_file";
    
    FILE* fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF* tif = TIFFOpen(filename, "r");
    if (!tif) {
        unlink(filename);
        return 0;
    }

    uint32_t width = 0, height = 0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    
    uint16_t samplesperpixel = 1;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel);
    
    uint16_t bitspersample = 8;
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample);

    uint32_t tile_width = 0, tile_height = 0;
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);

    uint32_t config = PLANARCONFIG_CONTIG;
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &config);

    uint32_t rowsperstrip = 0;
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rowsperstrip);

    uint32_t tile = 0;
    if (Size > 4) {
        tile = *(uint32_t*)Data % (TIFFNumberOfTiles(tif) + 1);
    }

    uint32_t x = 0, y = 0, z = 0;
    uint16_t s = 0;
    if (Size > 14) {
        x = *(uint32_t*)(Data + 0) % (width + 1);
        y = *(uint32_t*)(Data + 4) % (height + 1);
        z = *(uint32_t*)(Data + 8) % 2;
        s = *(uint16_t*)(Data + 12) % (samplesperpixel + 1);
    }

    uint32_t row = 0;
    if (Size > 18) {
        row = *(uint32_t*)(Data + 16) % (height + 1);
    }

    tmsize_t tile_size = TIFFTileSize(tif);
    if (tile_size > 0) {
        void* buf = malloc(tile_size);
        if (buf) {
            TIFFReadEncodedTile(tif, tile, buf, tile_size);
            free(buf);
        }
    }

    TIFFCheckTile(tif, x, y, z, s);

    TIFFTileSize(tif);

    if (config == PLANARCONFIG_CONTIG) {
        tmsize_t scanline_size = TIFFScanlineSize(tif);
        if (scanline_size > 0) {
            void* scanline_buf = malloc(scanline_size);
            if (scanline_buf) {
                memset(scanline_buf, 0, scanline_size);
                TIFFWriteScanline(tif, scanline_buf, row, 0);
                free(scanline_buf);
            }
        }
    } else {
        for (uint16_t sample = 0; sample < samplesperpixel; sample++) {
            tmsize_t scanline_size = TIFFScanlineSize(tif);
            if (scanline_size > 0) {
                void* scanline_buf = malloc(scanline_size);
                if (scanline_buf) {
                    memset(scanline_buf, 0, scanline_size);
                    TIFFWriteScanline(tif, scanline_buf, row, sample);
                    free(scanline_buf);
                }
            }
        }
    }

    TIFFNumberOfTiles(tif);

    if (tile_size > 0) {
        void* write_buf = malloc(tile_size);
        if (write_buf) {
            memset(write_buf, 0, tile_size);
            TIFFWriteEncodedTile(tif, tile, write_buf, tile_size);
            free(write_buf);
        }
    }

    TIFFClose(tif);
    unlink(filename);
    
    return 0;
}