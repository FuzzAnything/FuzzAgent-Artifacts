// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadTile at tif_read.c:940:10 in tiffio.h
// TIFFReadEncodedTile at tif_read.c:953:10 in tiffio.h
// TIFFTileRowSize at tif_tile.c:177:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFReadRawTile at tif_read.c:1176:10 in tiffio.h
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

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to work with
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }

    // Write the fuzzed data to the file
    if (fwrite(Data, 1, Size, fp) != Size) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    // Open the file for reading
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        return 0;
    }

    // Allocate a buffer for reading tiles
    size_t max_tile_size = 65536; // Reasonable upper limit for fuzzing
    uint8_t *buf = (uint8_t *)malloc(max_tile_size);
    if (buf == NULL) {
        cleanup_tiff(tif);
        return 0;
    }

    // Fuzz TIFFReadTile
    // Extract coordinates from input if available, otherwise use defaults
    uint32_t x = (Size > 0) ? (uint32_t)Data[0] : 0;
    uint32_t y = (Size > 1) ? (uint32_t)Data[1] : 0;
    uint32_t z = (Size > 2) ? (uint32_t)Data[2] : 0;
    uint16_t s = (Size > 3) ? (uint16_t)Data[3] : 0;

    tmsize_t ret = TIFFReadTile(tif, buf, x, y, z, s);
    (void)ret; // Suppress unused variable warning

    // Fuzz TIFFReadEncodedTile
    uint32_t tile = (Size > 4) ? (uint32_t)Data[4] : 0;
    tmsize_t size = -1; // Let libtiff determine size
    ret = TIFFReadEncodedTile(tif, tile, buf, size);
    (void)ret;

    // Fuzz TIFFTileRowSize
    tmsize_t row_size = TIFFTileRowSize(tif);
    (void)row_size;

    // Fuzz TIFFVTileSize
    uint32_t nrows = (Size > 5) ? (uint32_t)Data[5] : 1;
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows);
    (void)vtile_size;

    // Fuzz TIFFReadRawTile
    ret = TIFFReadRawTile(tif, tile, buf, size);
    (void)ret;

    // Cleanup
    free(buf);
    cleanup_tiff(tif);
    remove(filename);

    return 0;
}