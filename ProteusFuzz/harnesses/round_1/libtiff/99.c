// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFSetField at tif_dir.c:1146:5 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
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

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
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

    // Ensure we have a valid directory
    if (!TIFFReadDirectory(tif)) {
        cleanup_tiff(tif);
        return 0;
    }

    // Check if the image is tiled
    uint32_t width, height;
    uint16_t bitspersample, samplesperpixel;
    if (!TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width) ||
        !TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height) ||
        !TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample) ||
        !TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &samplesperpixel)) {
        cleanup_tiff(tif);
        return 0;
    }

    // Allocate buffers for tile operations
    tmsize_t tileSize = TIFFTileSize(tif);
    if (tileSize <= 0) {
        cleanup_tiff(tif);
        return 0;
    }
    void *buf = malloc(tileSize);
    if (buf == NULL) {
        cleanup_tiff(tif);
        return 0;
    }
    memset(buf, 0, tileSize);

    // Fuzz TIFFReadTile
    // Use coordinates derived from input data to explore different states
    uint32_t x = (Size > 0) ? Data[0] % width : 0;
    uint32_t y = (Size > 1) ? Data[1] % height : 0;
    uint32_t z = (Size > 2) ? Data[2] : 0;
    uint16_t s = (Size > 3) ? Data[3] % (samplesperpixel > 0 ? samplesperpixel : 1) : 0;

    (void)TIFFReadTile(tif, buf, x, y, z, s);

    // Fuzz TIFFReadEncodedTile
    uint32_t numTiles = TIFFNumberOfTiles(tif);
    if (numTiles > 0) {
        uint32_t tileIndex = (Size > 4) ? Data[4] % numTiles : 0;
        (void)TIFFReadEncodedTile(tif, tileIndex, buf, -1);
    }

    // Fuzz TIFFTileRowSize
    (void)TIFFTileRowSize(tif);

    // Fuzz TIFFVTileSize
    uint32_t nrows = (Size > 5) ? Data[5] % 100 : 1;
    (void)TIFFVTileSize(tif, nrows);

    // Fuzz TIFFReadRawTile
    if (numTiles > 0) {
        uint32_t rawTileIndex = (Size > 6) ? Data[6] % numTiles : 0;
        (void)TIFFReadRawTile(tif, rawTileIndex, buf, tileSize);
    }

    // Prepare for write operations by opening in write mode if possible
    // Note: We cannot easily switch modes on the same handle, so we close and reopen
    cleanup_tiff(tif);
    tif = TIFFOpen(filename, "w");
    if (tif != NULL) {
        // Set basic tags to make the file valid for writing
        TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_IMAGELENGTH, height);
        TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, bitspersample);
        TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, samplesperpixel);
        TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
        TIFFSetField(tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
        TIFFSetField(tif, TIFFTAG_TILEWIDTH, width);
        TIFFSetField(tif, TIFFTAG_TILELENGTH, height);

        // Fuzz TIFFWriteTile
        (void)TIFFWriteTile(tif, buf, x, y, z, s);

        cleanup_tiff(tif);
    }

    free(buf);
    return 0;
}