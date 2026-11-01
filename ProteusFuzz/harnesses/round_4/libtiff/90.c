// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        return 0;
    }
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return (written == size) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    if (!write_dummy_file(Data, Size)) {
        return 0;
    }

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    /* Try to read the directory to ensure the handle is in a valid state */
    if (TIFFReadDirectory(tif) > 0) {
        /* Fuzz TIFFStripSize64 */
        (void)TIFFStripSize64(tif);

        /* Fuzz TIFFScanlineSize64 */
        (void)TIFFScanlineSize64(tif);

        /* Fuzz TIFFTileRowSize64 */
        (void)TIFFTileRowSize64(tif);

        /* Fuzz TIFFVStripSize64 with various nrows */
        uint32_t nrows = 0;
        if (Size >= 4) {
            nrows = *(const uint32_t *)(Data + 10);
        }
        (void)TIFFVStripSize64(tif, nrows);

        /* Fuzz TIFFVStripSize64 with full height (-1 cast to uint32_t) */
        (void)TIFFVStripSize64(tif, (uint32_t)-1);

        /* Fuzz TIFFRasterScanlineSize64 */
        (void)TIFFRasterScanlineSize64(tif);

        /* Fuzz TIFFVTileSize64 */
        (void)TIFFVTileSize64(tif, nrows);
        (void)TIFFVTileSize64(tif, (uint32_t)-1);
    }

    TIFFClose(tif);
    return 0;
}