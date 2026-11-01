// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFVStripSize at tif_strip.c:142:10 in tiffio.h
// TIFFVTileSize at tif_tile.c:238:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        } else {
            // Write a minimal valid TIFF header if input is empty
            uint16_t header[8] = {
                0x002A, // Little Endian
                42,     // Magic number
                8,      // Offset to first IFD
                0,      // Reserved
                0,      // Reserved
                0,      // Reserved
                0,      // Reserved
                0       // Reserved
            };
            fwrite(header, sizeof(uint16_t), 8, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    setup_dummy_file(Data, Size);

    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    // Read directory to ensure valid state for size calculations
    if (!TIFFReadDirectory(tif)) {
        TIFFClose(tif);
        return 0;
    }

    // Fuzz TIFFVStripSize
    uint32_t nrows_strip = 0;
    if (Size >= 4) {
        nrows_strip = *(uint32_t *)Data;
    } else if (Size > 0) {
        nrows_strip = Data[0];
    }
    // Limit nrows to prevent excessive allocation or invalid states
    if (nrows_strip > 10000) nrows_strip = 10000;
    tmsize_t vstrip_size = TIFFVStripSize(tif, nrows_strip);
    (void)vstrip_size;

    // Fuzz TIFFVTileSize
    uint32_t nrows_tile = 0;
    if (Size >= 8) {
        nrows_tile = *(uint32_t *)(Data + 4);
    } else if (Size > 4) {
        nrows_tile = Data[4];
    }
    if (nrows_tile > 10000) nrows_tile = 10000;
    tmsize_t vtile_size = TIFFVTileSize(tif, nrows_tile);
    (void)vtile_size;

    // Fuzz TIFFRawStripSize
    uint32_t strip_idx = 0;
    if (Size >= 12) {
        strip_idx = *(uint32_t *)(Data + 8);
    } else if (Size > 8) {
        strip_idx = Data[8];
    }
    // Limit strip index to reasonable bounds
    if (strip_idx > 1000) strip_idx = 1000;
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_idx);
    (void)raw_strip_size;

    // Fuzz _TIFFmalloc with size derived from input
    tmsize_t alloc_size = 0;
    if (Size >= 16) {
        alloc_size = *(tmsize_t *)(Data + 12);
    } else if (Size > 12) {
        alloc_size = Data[12];
    }
    // Prevent huge allocations or zero size
    if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
    if (alloc_size == 0) alloc_size = 1;
    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr) {
        _TIFFfree(ptr);
    }

    // Fuzz TIFFStripSize
    tmsize_t strip_size = TIFFStripSize(tif);
    (void)strip_size;

    // Fuzz TIFFRasterScanlineSize
    tmsize_t scanline_size = TIFFRasterScanlineSize(tif);
    (void)scanline_size;

    TIFFClose(tif);
    return 0;
}