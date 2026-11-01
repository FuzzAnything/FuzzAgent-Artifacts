// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFOpenOptionsAlloc at tif_open.c:80:18 in tiffio.h
// TIFFOpenOptionsSetMaxSingleMemAlloc at tif_open.c:94:6 in tiffio.h
// TIFFOpenOptionsSetMaxCumulatedMemAlloc at tif_open.c:106:6 in tiffio.h
// TIFFOpenOptionsSetWarnAboutUnknownTags at tif_open.c:115:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenExt at tif_unix.c:237:7 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
// TIFFOpenOptionsFree at tif_open.c:87:6 in tiffio.h
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
        if (size > 0) {
            fwrite(data, 1, size, f);
        }
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write input data to a dummy file for TIFFOpenExt to read
    write_dummy_file(Data, Size);

    // Allocate options
    TIFFOpenOptions *opts = TIFFOpenOptionsAlloc();
    if (opts == NULL) {
        return 0;
    }

    // Derive values from input data to explore diverse states
    uint32_t val1 = 0;
    uint32_t val2 = 0;
    uint32_t val3 = 0;

    if (Size >= 4) {
        val1 = *(const uint32_t *)(Data);
        val2 = *(const uint32_t *)(Data + 1);
        if (Size >= 8) {
            val3 = *(const uint32_t *)(Data + 4);
        }
    }

    // Set max single memory allocation
    // Use val1, ensure it's not 0 to test limits, or 0 for unlimited
    tmsize_t max_single = (tmsize_t)(val1 & 0x7FFFFFFF);
    TIFFOpenOptionsSetMaxSingleMemAlloc(opts, max_single);

    // Set max cumulated memory allocation
    tmsize_t max_cumulated = (tmsize_t)(val2 & 0x7FFFFFFF);
    TIFFOpenOptionsSetMaxCumulatedMemAlloc(opts, max_cumulated);

    // Set warn about unknown tags (0 or 1)
    int warn_flag = (val3 & 1);
    TIFFOpenOptionsSetWarnAboutUnknownTags(opts, warn_flag);

    // Try opening the file in read mode
    TIFF *tif = TIFFOpenExt("./dummy_file", "r", opts);
    if (tif != NULL) {
        // Perform minimal operations to exercise state
        // Just closing is enough to trigger cleanup logic
        TIFFClose(tif);
    }

    // Try opening in write mode (might fail if file exists, but tests the path)
    // We reuse the same file name; behavior depends on OS and libtiff version
    TIFF *tif_w = TIFFOpenExt("./dummy_file", "w", opts);
    if (tif_w != NULL) {
        TIFFClose(tif_w);
    }

    // Free the options structure
    TIFFOpenOptionsFree(opts);

    return 0;
}