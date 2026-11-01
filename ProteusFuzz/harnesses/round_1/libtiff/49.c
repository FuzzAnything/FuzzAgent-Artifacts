// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFWriteRawTile at tif_write.c:533:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawStrip at tif_read.c:717:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFWriteRawStrip at tif_write.c:328:10 in tiffio.h
// _TIFFmemcpy at tif_unix.c:355:6 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static void setup_tiff_for_write(TIFF *tif, int tiled) {
    if (tif == NULL) return;

    uint16_t bitspersample = 8;
    uint16_t photometric = PHOTOMETRIC_MINISBLACK;
    uint32_t width = 1;
    uint32_t height = 1;
    uint16_t compression = COMPRESSION_NONE;
    uint16_t samplesperpixel = 1;

    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
    TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, photometric);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, compression);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);

    if (tiled) {
        uint32_t tilewidth = 16;
        uint32_t tileheight = 16;
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, tilewidth);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, tileheight);
    } else {
        uint32_t rowsperstrip = 1;
        TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    }

    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const char *filename = "./dummy_file";
    TIFF *tif = NULL;
    uint8_t *buffer = NULL;
    tmsize_t buf_size = 0;

    /* Allocate a buffer for operations, capping size to avoid excessive memory usage */
    if (Size > 0 && Size < (1024 * 1024)) {
        buffer = (uint8_t *)malloc(Size);
        if (buffer == NULL) return 0;
        memcpy(buffer, Data, Size);
        buf_size = Size;
    } else if (Size > 0) {
        /* If input is too large, use a smaller fixed buffer for testing */
        buf_size = 1024;
        buffer = (uint8_t *)malloc(buf_size);
        if (buffer == NULL) return 0;
        memcpy(buffer, Data, buf_size);
    } else {
        return 0;
    }

    /* Test Write Operations */
    /* Create a file for writing */
    tif = TIFFOpen(filename, "w");
    if (tif != NULL) {
        setup_tiff_for_write(tif, 0); /* Strip based */

        /* Fuzz TIFFWriteRawStrip */
        if (buf_size > 0) {
            TIFFWriteRawStrip(tif, 0, buffer, buf_size);
        }

        /* Test _TIFFmemcpy on the buffer */
        if (buf_size > 0) {
            _TIFFmemcpy(buffer, buffer, buf_size);
        }

        /* Test TIFFVStripSize */
        if (buf_size > 0) {
            uint32_t nrows = 1;
            if (buf_size > 1) nrows = Data[0] % 100;
            if (nrows == 0) nrows = 1;
            TIFFVStripSize(tif, nrows);
        }

        TIFFClose(tif);
        tif = NULL;
    }

    /* Test Tile-based Write and Read */
    tif = TIFFOpen(filename, "w");
    if (tif != NULL) {
        setup_tiff_for_write(tif, 1); /* Tile based */

        /* Fuzz TIFFWriteRawTile */
        if (buf_size > 0) {
            TIFFWriteRawTile(tif, 0, buffer, buf_size);
        }

        TIFFClose(tif);
        tif = NULL;
    }

    /* Test Read Operations */
    /* Re-open for reading */
    tif = TIFFOpen(filename, "r");
    if (tif != NULL) {
        /* Ensure buffer is large enough for reading if possible */
        if (buf_size == 0) {
            free(buffer);
            buffer = (uint8_t *)malloc(4096);
            buf_size = 4096;
        }

        /* Fuzz TIFFReadRawStrip */
        /* Try reading with different sizes */
        TIFFReadRawStrip(tif, 0, buffer, buf_size);
        TIFFReadRawStrip(tif, 0, buffer, -1);
        TIFFReadRawStrip(tif, 9999, buffer, buf_size); /* Invalid strip */

        /* Fuzz TIFFReadRawTile */
        TIFFReadRawTile(tif, 0, buffer, buf_size);
        TIFFReadRawTile(tif, 9999, buffer, buf_size); /* Invalid tile */

        /* Fuzz TIFFVStripSize on read handle */
        TIFFVStripSize(tif, 1);

        TIFFClose(tif);
        tif = NULL;
    }

    if (buffer != NULL) {
        free(buffer);
    }

    return 0;
}