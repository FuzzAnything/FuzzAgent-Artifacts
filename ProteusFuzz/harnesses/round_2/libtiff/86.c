// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFCurrentDirectory at tif_open.c:869:8 in tiffio.h
// TIFFNumberOfDirectories at tif_dir.c:2032:8 in tiffio.h
// TIFFSetDirectory at tif_dir.c:2057:5 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFRasterScanlineSize64 at tif_strip.c:357:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Attempt to read directory to initialize internal state */
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    /* Fuzz TIFFGetStrileByteCount with varying strile indices */
    uint32_t strile = 0;
    if (Size >= 4) {
        strile = *(const uint32_t *)Data;
    }
    (void)TIFFGetStrileByteCount(tif, strile);

    /* Fuzz TIFFScanlineSize64 */
    (void)TIFFScanlineSize64(tif);

    /* Fuzz TIFFTileRowSize64 */
    (void)TIFFTileRowSize64(tif);

    /* Fuzz TIFFRasterScanlineSize64 */
    (void)TIFFRasterScanlineSize64(tif);

    /* Fuzz TIFFStripSize64 */
    (void)TIFFStripSize64(tif);

    /* Fuzz TIFFVTileSize64 with varying nrows */
    uint32_t nrows = 0;
    if (Size >= 8) {
        nrows = *(const uint32_t *)(Data + 4);
    }
    (void)TIFFVTileSize64(tif, nrows);

    /* Try with different directory indices if multiple exist */
    tdir_t curdir = TIFFCurrentDirectory(tif);
    tdir_t dircount = TIFFNumberOfDirectories(tif);
    if (dircount > 1 && Size >= 12) {
        uint32_t dir_idx = *(const uint32_t *)(Data + 8);
        dir_idx = dir_idx % dircount;
        if (TIFFSetDirectory(tif, dir_idx)) {
            (void)TIFFGetStrileByteCount(tif, strile);
            (void)TIFFScanlineSize64(tif);
            (void)TIFFTileRowSize64(tif);
            (void)TIFFRasterScanlineSize64(tif);
            (void)TIFFStripSize64(tif);
            (void)TIFFVTileSize64(tif, nrows);
        }
    }

    TIFFClose(tif);
    return 0;
}