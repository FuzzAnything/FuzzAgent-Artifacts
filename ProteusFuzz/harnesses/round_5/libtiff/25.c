// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == NULL) {
        return;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Fuzz TIFFTileRowSize64 */
    (void)TIFFTileRowSize64(tif);

    /* Fuzz TIFFVTileSize64 with various row counts */
    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
    }
    (void)TIFFVTileSize64(tif, nrows);

    /* Fuzz TIFFVStripSize64 */
    (void)TIFFVStripSize64(tif, nrows);
    (void)TIFFVStripSize64(tif, 0);
    (void)TIFFVStripSize64(tif, 1);

    /* Fuzz TIFFScanlineSize64 */
    (void)TIFFScanlineSize64(tif);

    /* Fuzz TIFFRasterScanlineSize64 */
    (void)TIFFRasterScanlineSize64(tif);

    /* Fuzz TIFFStripSize64 */
    (void)TIFFStripSize64(tif);

    TIFFClose(tif);
    return 0;
}