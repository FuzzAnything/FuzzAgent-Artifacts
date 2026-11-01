// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
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

/*
 * Fuzz driver for libtiff API functions:
 * TIFFRawStripSize, TIFFRasterScanlineSize, TIFFStripSize,
 * TIFFScanlineSize, TIFFTileSize, _TIFFmalloc.
 *
 * This driver creates a temporary TIFF file from the input data,
 * opens it, and exercises the target functions with various parameters.
 */

static void cleanup_tiff(TIFF *tif) {
    if (tif != NULL) {
        TIFFClose(tif);
    }
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Write input data to a dummy file */
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    /* Open the file as a TIFF */
    TIFF *tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        return 0;
    }

    /* Exercise TIFFRasterScanlineSize */
    tmsize_t scanline_size = TIFFRasterScanlineSize(tif);
    (void)scanline_size;

    /* Exercise TIFFScanlineSize */
    tmsize_t scanline_size2 = TIFFScanlineSize(tif);
    (void)scanline_size2;

    /* Exercise TIFFTileSize */
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    /* Exercise TIFFStripSize */
    tmsize_t strip_size = TIFFStripSize(tif);
    (void)strip_size;

    /* Exercise TIFFRawStripSize with various strip indices */
    /* Try strip 0 */
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, 0);
    (void)raw_strip_size;

    /* Try a strip index derived from input data to explore more states */
    if (Size >= 4) {
        uint32_t strip_idx = ((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]) % 1000;
        raw_strip_size = TIFFRawStripSize(tif, strip_idx);
        (void)raw_strip_size;
    }

    /* Exercise _TIFFmalloc with various sizes */
    /* Size 0 */
    void *ptr0 = _TIFFmalloc(0);
    if (ptr0 != NULL) {
        _TIFFfree(ptr0);
    }

    /* Size derived from input */
    if (Size >= 4) {
        tmsize_t alloc_size = ((Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3]) % 1048576; /* Max 1MB */
        void *ptr = _TIFFmalloc(alloc_size);
        if (ptr != NULL) {
            /* Write some data to the allocated buffer to ensure it's usable */
            memset(ptr, 0, alloc_size);
            _TIFFfree(ptr);
        }
    }

    /* Cleanup */
    cleanup_tiff(tif);

    return 0;
}