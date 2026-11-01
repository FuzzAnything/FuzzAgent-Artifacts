// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFRawStripSize at tif_strip.c:168:10 in tiffio.h
// TIFFStripSize at tif_strip.c:204:10 in tiffio.h
// TIFFTileSize at tif_tile.c:253:10 in tiffio.h
// TIFFScanlineSize at tif_strip.c:343:10 in tiffio.h
// TIFFRasterScanlineSize at tif_strip.c:375:10 in tiffio.h
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
    if (f == NULL) {
        return;
    }
    /* Write at least a minimal TIFF header if input is too small */
    if (size == 0) {
        /* Minimal TIFF header: II (little endian), 42, 8 (offset) */
        uint8_t header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
        fwrite(header, 1, sizeof(header), f);
    } else {
        fwrite(data, 1, size, f);
    }
    fclose(f);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test _TIFFmalloc directly with various sizes derived from input */
    size_t alloc_size = 0;
    if (Size >= 4) {
        alloc_size = *(const uint32_t *)Data;
        /* Avoid huge allocations to prevent OOM in fuzzer */
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    } else if (Size > 0) {
        alloc_size = Data[0];
    }

    void *ptr = _TIFFmalloc(alloc_size);
    if (ptr != NULL) {
        memset(ptr, 0, alloc_size);
        /* Simulate usage */
        if (alloc_size > 0) {
            ((uint8_t *)ptr)[0] = 1;
        }
        _TIFFfree(ptr);
    }

    /* Prepare a dummy file for TIFF operations */
    write_dummy_file(Data, Size);

    /* Open the dummy file */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        /* File might be invalid, try to open in write mode to create a valid one if needed */
        /* But for fuzzing, we just proceed if open fails, as the API calls will return errors */
        return 0;
    }

    /* Test TIFFRawStripSize with various strip indices */
    uint32_t strip_index = 0;
    if (Size >= 4) {
        strip_index = *(const uint32_t *)(Data + (Size >= 8 ? 4 : 0));
    } else if (Size > 0) {
        strip_index = Data[0];
    }
    /* Limit strip index to reasonable range */
    if (strip_index > 1000) {
        strip_index = 1000;
    }
    tmsize_t raw_strip_size = TIFFRawStripSize(tif, strip_index);
    (void)raw_strip_size; /* Suppress unused variable warning */

    /* Test TIFFStripSize */
    tmsize_t strip_size = TIFFStripSize(tif);
    (void)strip_size;

    /* Test TIFFTileSize */
    tmsize_t tile_size = TIFFTileSize(tif);
    (void)tile_size;

    /* Test TIFFScanlineSize */
    tmsize_t scanline_size = TIFFScanlineSize(tif);
    (void)scanline_size;

    /* Test TIFFRasterScanlineSize */
    tmsize_t raster_scanline_size = TIFFRasterScanlineSize(tif);
    (void)raster_scanline_size;

    /* Cleanup */
    TIFFClose(tif);

    return 0;
}