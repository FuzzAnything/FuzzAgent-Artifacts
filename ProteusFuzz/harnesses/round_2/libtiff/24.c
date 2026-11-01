// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFWriteTile at tif_write.c:387:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFGetField at tif_dir.c:1582:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f == NULL) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    } else {
        // Write a minimal valid TIFF header if input is empty
        uint8_t header[8] = {0x49, 0x49, 0x2a, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, 8, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data to create a minimal file structure if needed
    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "rb");
    if (tif == NULL) {
        return 0;
    }

    // Try to read directory to initialize state
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Check if tiled
    uint16_t photometric, bitspersample;
    uint32_t tilewidth, tilelength, tilecount;
    tmsize_t tilesize;

    if (!TIFFGetField(tif, TIFFTAG_PHOTOMETRIC, &photometric) ||
        !TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bitspersample) ||
        !TIFFGetField(tif, TIFFTAG_TILEWIDTH, &tilewidth) ||
        !TIFFGetField(tif, TIFFTAG_TILELENGTH, &tilelength)) {
        TIFFClose(tif);
        return 0;
    }

    tilecount = TIFFNumberOfTiles(tif);
    if (tilecount == 0) {
        TIFFClose(tif);
        return 0;
    }

    tilesize = TIFFTileSize(tif);
    if (tilesize == 0) {
        TIFFClose(tif);
        return 0;
    }

    // Allocate buffer for tile data
    void *buf = malloc(tilesize);
    if (buf == NULL) {
        TIFFClose(tif);
        return 0;
    }
    memset(buf, 0, tilesize);

    // Fuzz TIFFReadRawTile
    for (uint32_t i = 0; i < tilecount; i++) {
        TIFFReadRawTile(tif, i, buf, tilesize);
    }
    // Try out of bounds
    TIFFReadRawTile(tif, tilecount + 100, buf, tilesize);

    // Fuzz TIFFVTileSize
    for (uint32_t nrows = 0; nrows < 100; nrows++) {
        TIFFVTileSize(tif, nrows);
    }
    // Try large nrows
    TIFFVTileSize(tif, 0xFFFFFFFF);

    // Fuzz TIFFReadEncodedTile
    for (uint32_t i = 0; i < tilecount; i++) {
        TIFFReadEncodedTile(tif, i, buf, tilesize);
        // Try with -1 size (auto)
        TIFFReadEncodedTile(tif, i, buf, -1);
    }
    // Try out of bounds
    TIFFReadEncodedTile(tif, tilecount + 100, buf, tilesize);

    // Fuzz TIFFReadTile
    // Use coordinates based on tile dimensions
    for (uint32_t x = 0; x < 10; x++) {
        for (uint32_t y = 0; y < 10; y++) {
            TIFFReadTile(tif, buf, x * tilewidth, y * tilelength, 0, 0);
        }
    }
    // Try out of bounds coordinates
    TIFFReadTile(tif, buf, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);

    // Fuzz TIFFTileRowSize
    TIFFTileRowSize(tif);

    // Fuzz TIFFWriteTile (will likely fail on read-only file, but tests logic)
    for (uint32_t x = 0; x < 10; x++) {
        for (uint32_t y = 0; y < 10; y++) {
            TIFFWriteTile(tif, buf, x * tilewidth, y * tilelength, 0, 0);
        }
    }
    // Try out of bounds coordinates
    TIFFWriteTile(tif, buf, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0);

    free(buf);
    TIFFClose(tif);

    return 0;
}