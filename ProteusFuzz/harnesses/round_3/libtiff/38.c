// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFReadDirectory at tif_dirread.c:4236:5 in tiffio.h
// TIFFDefaultStripSize at tif_strip.c:218:10 in tiffio.h
// TIFFNumberOfTiles at tif_tile.c:108:10 in tiffio.h
// TIFFVStripSize64 at tif_strip.c:88:10 in tiffio.h
// TIFFNumberOfStrips at tif_strip.c:65:10 in tiffio.h
// TIFFGetStrileByteCount at tif_dirread.c:8366:10 in tiffio.h
// TIFFComputeStrip at tif_strip.c:35:10 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "tiffio.h"

static void cleanup_tiff(TIFF **tif) {
    if (*tif != NULL) {
        TIFFClose(*tif);
        *tif = NULL;
    }
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file with the fuzz input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    TIFF *tif = NULL;
    uint32_t request_size = 0;
    uint32_t row = 0;
    uint16_t sample = 0;
    uint32_t strile = 0;
    uint32_t nrows = 0;

    // Extract some values from input to drive fuzzing parameters
    if (Size >= 4) {
        request_size = *(const uint32_t *)(Data);
        if (Size >= 8) {
            row = *(const uint32_t *)(Data + 4);
            if (Size >= 10) {
                sample = *(const uint16_t *)(Data + 8);
                if (Size >= 14) {
                    strile = *(const uint32_t *)(Data + 10);
                    if (Size >= 18) {
                        nrows = *(const uint32_t *)(Data + 14);
                    }
                }
            }
        }
    }

    // Open the file in read mode
    tif = TIFFOpen(filename, "r");
    if (tif == NULL) {
        unlink(filename);
        return 0;
    }

    // Try to read the directory to ensure state is valid for most functions
    if (!TIFFReadDirectory(tif)) {
        // Directory might be invalid, but we can still try some functions
    }

    // Fuzz TIFFDefaultStripSize
    uint32_t strip_size = TIFFDefaultStripSize(tif, request_size);
    (void)strip_size;

    // Fuzz TIFFNumberOfTiles
    uint32_t num_tiles = TIFFNumberOfTiles(tif);
    (void)num_tiles;

    // Fuzz TIFFVStripSize64
    uint64_t vstrip_size = TIFFVStripSize64(tif, nrows);
    (void)vstrip_size;

    // Fuzz TIFFNumberOfStrips
    uint32_t num_strips = TIFFNumberOfStrips(tif);
    (void)num_strips;

    // Fuzz TIFFGetStrileByteCount
    uint64_t byte_count = TIFFGetStrileByteCount(tif, strile);
    (void)byte_count;

    // Fuzz TIFFComputeStrip
    uint32_t strip_idx = TIFFComputeStrip(tif, row, sample);
    (void)strip_idx;

    // Cleanup
    cleanup_tiff(&tif);
    unlink(filename);

    return 0;
}