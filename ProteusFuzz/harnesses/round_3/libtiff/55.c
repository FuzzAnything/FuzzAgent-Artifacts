// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (Size > 0) {
        fwrite(Data, 1, Size, f);
    } else {
        // Write minimal valid TIFF header if input is empty
        uint16_t header[] = {42, 0, 0, 0, 8, 0, 0, 0}; // Little endian, offset 8
        fwrite(header, 1, sizeof(header), f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Try to read directory to initialize internal structures
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz TIFFRasterScanlineSize64
    (void)TIFFRasterScanlineSize64(tif);

    // Fuzz TIFFVStripSize64 with various row counts
    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
    }
    (void)TIFFVStripSize64(tif, nrows);
    
    // Also try with -1 (full height) logic if supported by casting
    (void)TIFFVStripSize64(tif, 0xFFFFFFFFU);

    // Fuzz TIFFVTileSize64
    (void)TIFFVTileSize64(tif, nrows);

    // Fuzz TIFFStripSize64
    (void)TIFFStripSize64(tif);

    // Fuzz TIFFScanlineSize64
    (void)TIFFScanlineSize64(tif);

    // Fuzz TIFFTileRowSize64
    (void)TIFFTileRowSize64(tif);

    TIFFClose(tif);
    return 0;
}