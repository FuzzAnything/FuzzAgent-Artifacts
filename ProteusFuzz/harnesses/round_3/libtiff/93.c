// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFDefaultTileSize at tif_tile.c:267:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tiffio.h"

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    } else {
        // Write a minimal valid TIFF header if input is empty to avoid immediate failure
        // Little endian, version 42, first IFD offset 8
        uint8_t header[] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, sizeof(header), fp);
    }
    fclose(fp);
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare the environment: Create a dummy file with the fuzz input
    setup_dummy_file(Data, Size);

    // Open the TIFF file for reading
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    // Ensure we have a valid directory to work with
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Allocate a buffer for reading tiles
    // Size is dynamic based on input to explore different memory states
    size_t buf_size = (Size > 0) ? Size : 1024;
    uint8_t *buf = (uint8_t *)malloc(buf_size);
    if (buf == NULL) {
        TIFFClose(tif);
        return 0;
    }

    // 1. Fuzz TIFFReadTile
    // We try various coordinates. x, y, z, s are derived from input data to explore states.
    uint32_t x = (Size > 0) ? Data[0] : 0;
    uint32_t y = (Size > 1) ? Data[1] : 0;
    uint32_t z = (Size > 2) ? Data[2] : 0;
    uint16_t s = (Size > 3) ? Data[3] : 0;

    // Ensure coordinates are not excessively large to prevent hangs, but allow some variation
    if (x > 10000) x = 10000;
    if (y > 10000) y = 10000;
    if (z > 100) z = 100;
    if (s > 100) s = 100;

    TIFFReadTile(tif, buf, x, y, z, s);

    // 2. Fuzz TIFFCurrentDirectory
    tdir_t dir_idx = TIFFCurrentDirectory(tif);
    (void)dir_idx; // Use variable to suppress warnings

    // 3. Fuzz TIFFDefaultStripSize
    uint32_t request_size = (Size > 4) ? Data[4] : 1024;
    if (request_size == 0) request_size = 1024;
    uint32_t strip_size = TIFFDefaultStripSize(tif, request_size);
    (void)strip_size;

    // 4. Fuzz TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    // 5. Fuzz TIFFVTileSize64
    uint32_t nrows = (Size > 5) ? Data[5] : 10;
    if (nrows == 0) nrows = 1;
    uint64_t vtile_size = TIFFVTileSize64(tif, nrows);
    (void)vtile_size;

    // 6. Fuzz TIFFDefaultTileSize
    uint32_t width, height;
    TIFFDefaultTileSize(tif, &width, &height);
    (void)width;
    (void)height;

    // Cleanup
    free(buf);
    TIFFClose(tif);

    return 0;
}