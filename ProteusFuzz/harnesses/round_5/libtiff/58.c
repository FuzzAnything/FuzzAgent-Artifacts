// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write fuzz data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file as a TIFF
    TIFF *tif = TIFFOpen(filename, "rb");
    if (tif == NULL) {
        return 0;
    }

    // Fuzz TIFFTileRowSize64
    uint64_t tile_row_size = TIFFTileRowSize64(tif);
    (void)tile_row_size;

    // Fuzz TIFFVTileSize64 with various nrows
    uint32_t nrows_options[] = {0, 1, 10, 100, 1000, (uint32_t)Size, 0xFFFFFFFF};
    for (size_t i = 0; i < sizeof(nrows_options) / sizeof(nrows_options[0]); i++) {
        uint64_t vtile_size = TIFFVTileSize64(tif, nrows_options[i]);
        (void)vtile_size;
    }

    // Fuzz TIFFScanlineSize64
    uint64_t scanline_size = TIFFScanlineSize64(tif);
    (void)scanline_size;

    // Fuzz TIFFGetStrileByteCount with various strile indices
    uint32_t strile_options[] = {0, 1, 10, 100, (uint32_t)Size, 0xFFFFFFFF};
    for (size_t i = 0; i < sizeof(strile_options) / sizeof(strile_options[0]); i++) {
        uint64_t strile_count = TIFFGetStrileByteCount(tif, strile_options[i]);
        (void)strile_count;
    }

    // Fuzz TIFFTileSize64
    uint64_t tile_size = TIFFTileSize64(tif);
    (void)tile_size;

    // Fuzz TIFFStripSize64
    uint64_t strip_size = TIFFStripSize64(tif);
    (void)strip_size;

    // Cleanup
    cleanup_tiff(tif);

    return 0;
}