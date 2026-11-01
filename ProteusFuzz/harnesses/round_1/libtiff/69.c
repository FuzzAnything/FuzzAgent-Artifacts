// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
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

/*
 * Fuzz driver for libtiff API functions:
 * TIFFTileSize, _TIFFmalloc, TIFFRasterScanlineSize,
 * TIFFStripSize, TIFFRawStripSize, TIFFScanlineSize.
 *
 * The driver writes the fuzz input to a temporary file, opens it with libtiff,
 * and exercises the target APIs. It handles errors gracefully and cleans up
 * resources to prevent leaks or crashes.
 */

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        if (size > 0) {
            fwrite(data, 1, size, fp);
        }
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure we have at least a minimal header to avoid immediate open failure */
    if (Size < 4) {
        return 0;
    }

    /* Write input to a dummy file */
    write_dummy_file(Data, Size);

    /* Open the file in read mode */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (!tif) {
        return 0;
    }

    /* Exercise _TIFFmalloc with various sizes derived from input */
    size_t alloc_size = 0;
    if (Size >= 4) {
        alloc_size = *(const uint32_t *)(Data);
        /* Cap allocation to avoid OOM during fuzzing */
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    } else {
        alloc_size = 64;
    }

    void *buf = _TIFFmalloc(alloc_size);
    if (buf) {
        /* Optionally use the buffer to trigger more code paths */
        memset(buf, 0, alloc_size);
        _TIFFfree(buf);
    }

    /* Exercise TIFFTileSize */
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    /* Exercise TIFFRasterScanlineSize */
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    (void)raster_scanline_size;

    /* Exercise TIFFStripSize */
    tmsize_t strip_size = TIFFStripSize(tif);
    (void)strip_size;

    /* Exercise TIFFRawStripSize with a few strip indices */
    uint32_t strip_idx = 0;
    if (Size >= 4) {
        strip_idx = *(const uint32_t *)(Data + 4);
    }
    /* Limit strip index to avoid excessive iteration */
    if (strip_idx > 100) {
        strip_idx = 100;
    }
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_idx);
    (void)raw_strip_size;

    /* Exercise TIFFScanlineSize */
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    (void)scanline_size;

    /* Close the TIFF handle */
    TIFFClose(tif);

    return 0;
}