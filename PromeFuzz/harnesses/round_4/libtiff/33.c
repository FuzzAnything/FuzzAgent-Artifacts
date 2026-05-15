// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Maximum array size to avoid excessive memory usage
    const tmsize_t max_elements = 1024;

    // Test TIFFSwabLong8
    if (Size >= sizeof(uint64_t)) {
        uint64_t value;
        memcpy(&value, Data, sizeof(uint64_t));
        TIFFSwabLong8(&value);
    }

    // Test TIFFSwabArrayOfLong8
    if (Size >= sizeof(uint64_t)) {
        tmsize_t n = Size / sizeof(uint64_t);
        if (n > max_elements) n = max_elements;
        
        uint64_t *long8_array = (uint64_t *)malloc(n * sizeof(uint64_t));
        if (long8_array) {
            memcpy(long8_array, Data, n * sizeof(uint64_t));
            TIFFSwabArrayOfLong8(long8_array, n);
            free(long8_array);
        }
    }

    // Test TIFFSwabArrayOfShort
    if (Size >= sizeof(uint16_t)) {
        tmsize_t n = Size / sizeof(uint16_t);
        if (n > max_elements) n = max_elements;
        
        uint16_t *short_array = (uint16_t *)malloc(n * sizeof(uint16_t));
        if (short_array) {
            memcpy(short_array, Data, n * sizeof(uint16_t));
            TIFFSwabArrayOfShort(short_array, n);
            free(short_array);
        }
    }

    // Test TIFFSwabArrayOfFloat
    if (Size >= sizeof(float)) {
        tmsize_t n = Size / sizeof(float);
        if (n > max_elements) n = max_elements;
        
        float *float_array = (float *)malloc(n * sizeof(float));
        if (float_array) {
            memcpy(float_array, Data, n * sizeof(float));
            TIFFSwabArrayOfFloat(float_array, n);
            free(float_array);
        }
    }

    // Test TIFFSwabArrayOfTriples
    if (Size >= 3) {  // Minimum size for one triple
        tmsize_t n = Size / 3;
        if (n > max_elements) n = max_elements;
        
        uint8_t *triples_array = (uint8_t *)malloc(n * 3);
        if (triples_array) {
            memcpy(triples_array, Data, n * 3);
            TIFFSwabArrayOfTriples(triples_array, n);
            free(triples_array);
        }
    }

    // Test TIFFSwabArrayOfLong
    if (Size >= sizeof(uint32_t)) {
        tmsize_t n = Size / sizeof(uint32_t);
        if (n > max_elements) n = max_elements;
        
        uint32_t *long_array = (uint32_t *)malloc(n * sizeof(uint32_t));
        if (long_array) {
            memcpy(long_array, Data, n * sizeof(uint32_t));
            TIFFSwabArrayOfLong(long_array, n);
            free(long_array);
        }
    }

    return 0;
}