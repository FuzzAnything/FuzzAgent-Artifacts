// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
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
 * - TIFFGetStrileByteCount
 * - TIFFDefaultStripSize
 * - TIFFComputeStrip
 * - TIFFNumberOfTiles
 * - TIFFNumberOfStrips
 * - TIFFVStripSize64
 *
 * The driver creates a temporary TIFF file from the fuzz input, opens it,
 * and calls the target functions with various parameters derived from the input.
 */

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        /* Write at least a minimal valid TIFF header to avoid immediate failure */
        /* Minimal TIFF header: 2 bytes magic, 2 bytes offset to first IFD */
        uint16_t magic = 0x4949; /* Little-endian */
        uint16_t offset = 8;
        fwrite(&magic, sizeof(uint16_t), 1, f);
        fwrite(&offset, sizeof(uint16_t), 1, f);
        
        /* Write the fuzz input as raw data */
        if (size > 0) {
            fwrite(data, 1, size, f);
        } else {
            /* Write some dummy bytes if input is empty */
            uint8_t dummy[] = {0, 0, 0, 0, 0, 0, 0, 0};
            fwrite(dummy, 1, sizeof(dummy), f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare the environment: create a dummy file with the fuzz input */
    write_dummy_file(Data, Size);

    /* Open the TIFF file for reading */
    TIFF *tif = TIFFOpen("./dummy_file", "r");
    if (tif == NULL) {
        return 0;
    }

    /* Extract some parameters from the input data for diverse testing */
    uint32_t param1 = 0;
    uint32_t param2 = 0;
    uint16_t param3 = 0;
    uint32_t param4 = 0;

    if (Size >= 4) {
        param1 = *(const uint32_t *)(Data);
        if (Size >= 8) {
            param2 = *(const uint32_t *)(Data + 4);
        }
        if (Size >= 10) {
            param3 = *(const uint16_t *)(Data + 8);
        }
        if (Size >= 12) {
            param4 = *(const uint32_t *)(Data + 10);
        }
    }

    /* Ensure parameters are within reasonable bounds to avoid immediate crashes */
    if (param1 > 1000000) param1 = 1000000;
    if (param2 > 1000000) param2 = 1000000;
    if (param3 > 10000) param3 = 10000;
    if (param4 > 1000000) param4 = 1000000;

    /* 1. TIFFGetStrileByteCount */
    /* Test with various strile indices */
    (void)TIFFGetStrileByteCount(tif, param1);
    (void)TIFFGetStrileByteCount(tif, 0);
    (void)TIFFGetStrileByteCount(tif, 1);
    (void)TIFFGetStrileByteCount(tif, param1 % 100);

    /* 2. TIFFDefaultStripSize */
    /* Test with various request sizes */
    (void)TIFFDefaultStripSize(tif, param2);
    (void)TIFFDefaultStripSize(tif, 0);
    (void)TIFFDefaultStripSize(tif, 1);
    (void)TIFFDefaultStripSize(tif, param2 % 1000);

    /* 3. TIFFComputeStrip */
    /* Test with various row and sample values */
    (void)TIFFComputeStrip(tif, param1, param3);
    (void)TIFFComputeStrip(tif, 0, 0);
    (void)TIFFComputeStrip(tif, 1, 0);
    (void)TIFFComputeStrip(tif, param1 % 1000, param3 % 10);

    /* 4. TIFFNumberOfTiles */
    /* This function does not take arguments, just call it */
    (void)TIFFNumberOfTiles(tif);

    /* 5. TIFFNumberOfStrips */
    /* This function does not take arguments, just call it */
    (void)TIFFNumberOfStrips(tif);

    /* 6. TIFFVStripSize64 */
    /* Test with various nrows values */
    (void)TIFFVStripSize64(tif, param4);
    (void)TIFFVStripSize64(tif, 0);
    (void)TIFFVStripSize64(tif, 1);
    (void)TIFFVStripSize64(tif, param4 % 1000);

    /* Cleanup: close the TIFF file */
    TIFFClose(tif);

    return 0;
}