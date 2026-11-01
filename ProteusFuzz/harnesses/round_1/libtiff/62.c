// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFTileRowSize64 at tif_tile.c:140:10 in tiffio.h
// TIFFVTileSize64 at tif_tile.c:188:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFTileSize64 at tif_tile.c:249:10 in tiffio.h
// TIFFScanlineSize64 at tif_strip.c:257:10 in tiffio.h
// TIFFStripSize64 at tif_strip.c:196:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "tiffio.h"

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = TIFFOpen(filename, "r");
    if (!tif) {
        return 0;
    }

    uint64_t tile_row_size = TIFFTileRowSize64(tif);
    (void)tile_row_size;

    uint32_t nrows = 0;
    if (Size >= 4) {
        nrows = *(const uint32_t *)(Data);
    }
    uint64_t vtile_size = TIFFVTileSize64(tif, nrows);
    (void)vtile_size;

    uint32_t strile = 0;
    if (Size >= 8) {
        strile = *(const uint32_t *)(Data + 4);
    }
    uint64_t strile_byte_count = TIFFGetStrileByteCount(tif, strile);
    (void)strile_byte_count;

    uint64_t tile_size = TIFFTileSize64(tif);
    (void)tile_size;

    uint64_t scanline_size = TIFFScanlineSize64(tif);
    (void)scanline_size;

    uint64_t strip_size = TIFFStripSize64(tif);
    (void)strip_size;

    TIFFClose(tif);

    return 0;
}