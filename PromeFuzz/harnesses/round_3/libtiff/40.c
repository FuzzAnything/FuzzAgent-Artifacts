// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFWriteEncodedTile at tif_write.c:414:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFWriteEncodedStrip at tif_write.c:215:10 in tiffio.h
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
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Initialize error handlers
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Write fuzz data to a file
    int fd = open("./dummy_file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return 0;
    }
    write(fd, Data, Size);
    close(fd);

    // Try to open the file as TIFF
    TIFF* tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        unlink("./dummy_file");
        return 0;
    }

    // Get image dimensions and characteristics
    uint32_t width = 0, height = 0;
    uint32_t tile_width = 0, tile_height = 0;
    uint32_t rows_per_strip = 0;
    uint16_t planar_config = 0;
    uint16_t samples_per_pixel = 0;
    uint16_t bits_per_sample = 0;

    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tile_width);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &tile_height);
    TIFFGetField(tif, TIFFTAG_ROWSPERSTRIP, &rows_per_strip);
    TIFFGetField(tif, TIFFTAG_PLANARCONFIG, &planar_config);
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samples_per_pixel);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bits_per_sample);

    // Calculate number of strips and tiles
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    uint32_t num_tiles = TIFFNumberOfTiles(tif);

    // Use fuzz data to generate parameters
    // Use first 4 bytes for strip/tile indices, next 4 for nrows
    uint32_t param1 = 0, param2 = 0;
    if (Size >= 4) {
        memcpy(&param1, Data, 4);
    }
    if (Size >= 8) {
        memcpy(&param2, Data + 4, 4);
    }

    // Test TIFFRawStripSize
    if (num_strips > 0) {
        uint32_t strip_idx = param1 % num_strips;
        tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_idx);
        (void)raw_strip_size; // Use result to avoid compiler warnings
    }

    // Test TIFFReadEncodedTile
    if (num_tiles > 0) {
        uint32_t tile_idx = param1 % num_tiles;
        tmsize_t tile_size = TIFFTileSize(tif);
        if (tile_size > 0) {
            void* buf = malloc(tile_size);
            if (buf) {
                tmsize_t bytes_read = TIFFReadEncodedTile(tif, tile_idx, buf, tile_size);
                (void)bytes_read;
                free(buf);
            }
        }
    }

    // Test TIFFVTileSize
    if (tile_height > 0 && height > 0) {
        uint32_t nrows = param2 % (tile_height + 1);
        if (nrows == 0) nrows = 1;
        tmsize_t v_tile_size = TIFFVTileSize(tif, nrows);
        (void)v_tile_size;
    }

    // Test TIFFVStripSize
    if (rows_per_strip > 0 && height > 0) {
        uint32_t nrows = param2 % (rows_per_strip + 1);
        if (nrows == 0) nrows = 1;
        tmsize_t v_strip_size = TIFFVStripSize(tif, nrows);
        (void)v_strip_size;
    }

    // Test TIFFWriteEncodedTile - need to open in write mode
    TIFFClose(tif);
    tif = TIFFOpen("./dummy_file", "r+");
    if (tif) {
        if (num_tiles > 0) {
            uint32_t tile_idx = param1 % num_tiles;
            tmsize_t tile_size = TIFFTileSize(tif);
            if (tile_size > 0 && tile_size <= (tmsize_t)Size) {
                void* data = malloc(tile_size);
                if (data) {
                    size_t copy_size = tile_size < Size ? tile_size : Size;
                    memcpy(data, Data, copy_size);
                    tmsize_t bytes_written = TIFFWriteEncodedTile(tif, tile_idx, data, tile_size);
                    (void)bytes_written;
                    free(data);
                }
            }
        }

        // Test TIFFWriteEncodedStrip
        if (num_strips > 0) {
            uint32_t strip_idx = param1 % num_strips;
            tmsize_t strip_size = TIFFStripSize(tif);
            if (strip_size > 0 && strip_size <= (tmsize_t)Size) {
                void* data = malloc(strip_size);
                if (data) {
                    size_t copy_size = strip_size < Size ? strip_size : Size;
                    memcpy(data, Data, copy_size);
                    tmsize_t bytes_written = TIFFWriteEncodedStrip(tif, strip_idx, data, strip_size);
                    (void)bytes_written;
                    free(data);
                }
            }
        }
    }

    // Cleanup
    if (tif) {
        TIFFClose(tif);
    }
    unlink("./dummy_file");

    return 0;
}