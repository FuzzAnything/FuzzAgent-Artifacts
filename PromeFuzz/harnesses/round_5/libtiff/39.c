// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFOpen at tif_unix.c:232:7 in tiffio.h
// TIFFIsByteSwapped at tif_open.c:884:5 in tiffio.h
// TIFFClose at tif_close.c:155:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
    (void)module;
    (void)fmt;
    (void)ap;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Initialize libtiff error handlers to suppress output
    TIFFSetErrorHandler(dummy_handler);
    TIFFSetWarningHandler(dummy_handler);

    // Test TIFFSwabShort
    if (Size >= sizeof(uint16_t)) {
        uint16_t val = 0;
        memcpy(&val, Data, sizeof(uint16_t));
        TIFFSwabShort(&val);
    }

    // Test TIFFSwabLong
    if (Size >= sizeof(uint32_t)) {
        uint32_t val = 0;
        memcpy(&val, Data, sizeof(uint32_t));
        TIFFSwabLong(&val);
    }

    // Test TIFFSwabArrayOfShort
    if (Size >= 2 * sizeof(uint16_t)) {
        size_t max_elements = Size / sizeof(uint16_t);
        if (max_elements > 1024) max_elements = 1024; // Limit size
        uint16_t *arr = malloc(max_elements * sizeof(uint16_t));
        if (arr) {
            memcpy(arr, Data, max_elements * sizeof(uint16_t));
            TIFFSwabArrayOfShort(arr, max_elements);
            free(arr);
        }
    }

    // Test TIFFSwabArrayOfLong8
    if (Size >= 2 * sizeof(uint64_t)) {
        size_t max_elements = Size / sizeof(uint64_t);
        if (max_elements > 1024) max_elements = 1024; // Limit size
        uint64_t *arr = malloc(max_elements * sizeof(uint64_t));
        if (arr) {
            memcpy(arr, Data, max_elements * sizeof(uint64_t));
            TIFFSwabArrayOfLong8(arr, max_elements);
            free(arr);
        }
    }

    // Test TIFFSwabArrayOfFloat
    if (Size >= 2 * sizeof(float)) {
        size_t max_elements = Size / sizeof(float);
        if (max_elements > 1024) max_elements = 1024; // Limit size
        float *arr = malloc(max_elements * sizeof(float));
        if (arr) {
            memcpy(arr, Data, max_elements * sizeof(float));
            TIFFSwabArrayOfFloat(arr, max_elements);
            free(arr);
        }
    }

    // Test TIFFIsByteSwapped - requires a TIFF* handle
    if (Size > 0) {
        // Create a dummy TIFF file with fuzzer data
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
            
            TIFF *tif = TIFFOpen("./dummy_file", "r");
            if (tif) {
                TIFFIsByteSwapped(tif);
                TIFFClose(tif);
            }
            remove("./dummy_file");
        }
    }

    return 0;
}