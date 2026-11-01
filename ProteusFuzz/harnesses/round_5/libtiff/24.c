// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open TIFF for reading
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        return 0;
    }

    // Test TIFFTileRowSize
    tmsize_t row_size = TIFFTileRowSize(tif);
    (void)row_size;

    // Test TIFFVTileSize with various nrows derived from input
    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
    } else if (Size > 0) {
        nrows = Data[0];
    }
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
    (void)vtile_size;

    // Determine number of tiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    if (num_tiles > 0) {
        // Allocate buffer for tile data
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0 && tile_size < (1024 * 1024 * 10)) { // Limit allocation
            uint8_t *tile_buf = (uint8_t *)malloc(tile_size);
            if (tile_buf != NULL) {
                // Test TIFFReadEncodedTile
                for (uint32_t i = 0; i < num_tiles; i++) {
                    TIFFReadEncodedTile(tif, i, tile_buf, -1);
                }

                // Test TIFFReadRawTile
                for (uint32_t i = 0; i < num_tiles; i++) {
                    TIFFReadRawTile(tif, i, tile_buf, tile_size);
                }

                // Test TIFFReadTile with coordinates
                // Use input data to generate coordinates if possible
                uint32_t x = (Size >= 4) ? *(const uint32_t *)(Data + 4) : 0;
                uint32_t y = (Size >= 8) ? *(const uint32_t *)(Data + 8) : 0;
                uint32_t z = (Size >= 12) ? *(const uint32_t *)(Data + 12) : 0;
                uint16_t s = (Size >= 14) ? *(const uint16_t *)(Data + 14) : 0;

                TIFFReadTile(tif, tile_buf, x, y, z, s);
                
                // Test with out-of-bounds or varied coordinates
                TIFFReadTile(tif, tile_buf, 0, 0, 0, 0);
                TIFFReadTile(tif, tile_buf, UINT32_MAX, UINT32_MAX, UINT32_MAX, UINT16_MAX);

                free(tile_buf);
            }
        }
    }

    // Test write operations if the file can be opened for writing
    // We reopen the file in write mode to test TIFFWriteTile
    TIFFClose(tif);
    tif = TIFFOpen(filename, "w");
    if (tif != NULL) {
        // Set basic tags to make it a valid tiled TIFF
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, 64);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, 64);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 8);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, 32);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, 32);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);

        tmsize_t write_tile_size = TIFFTileSize(tif);
        if (write_tile_size > 0 && write_tile_size < (1024 * 1024 * 10)) {
            uint8_t *write_buf = (uint8_t *)malloc(write_tile_size);
            if (write_buf != NULL) {
                // Fill buffer with input data or zeros
                memset(write_buf, 0, write_tile_size);
                if (Size > 0) {
                    size_t copy_len = (Size < write_tile_size) ? Size : write_tile_size;
                    memcpy(write_buf, Data, copy_len);
                }

                // Test TIFFWriteTile
                TIFFWriteTile(tif, write_buf, 0, 0, 0, 0);
                TIFFWriteTile(tif, write_buf, 32, 0, 0, 0);
                TIFFWriteTile(tif, write_buf, 0, 32, 0, 0);
                TIFFWriteTile(tif, write_buf, UINT32_MAX, UINT32_MAX, 0, 0);

                free(write_buf);
            }
        }
        TIFFClose(tif);
    }

    // Cleanup dummy file
    remove(filename);

    return 0;
}