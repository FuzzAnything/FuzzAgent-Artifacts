// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_swab_functions(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    // For array functions, use a reasonable maximum element count
    const tmsize_t max_elements = 1024;
    
    // Calculate how many elements we can fit in the available data
    size_t element_size;
    tmsize_t n;
    
    // TIFFSwabArrayOfLong8
    element_size = sizeof(uint64_t);
    n = (tmsize_t)(size / element_size);
    if (n > max_elements) n = max_elements;
    if (n > 0) {
        uint64_t *long8_array = (uint64_t*)malloc(n * element_size);
        if (long8_array) {
            memcpy(long8_array, data, n * element_size);
            TIFFSwabArrayOfLong8(long8_array, n);
            free(long8_array);
        }
    }
    
    // TIFFSwabArrayOfFloat
    element_size = sizeof(float);
    n = (tmsize_t)(size / element_size);
    if (n > max_elements) n = max_elements;
    if (n > 0) {
        float *float_array = (float*)malloc(n * element_size);
        if (float_array) {
            memcpy(float_array, data, n * element_size);
            TIFFSwabArrayOfFloat(float_array, n);
            free(float_array);
        }
    }
    
    // TIFFSwabDouble (single double)
    if (size >= sizeof(double)) {
        double d;
        memcpy(&d, data, sizeof(double));
        TIFFSwabDouble(&d);
    }
    
    // TIFFSwabArrayOfDouble
    element_size = sizeof(double);
    n = (tmsize_t)(size / element_size);
    if (n > max_elements) n = max_elements;
    if (n > 0) {
        double *double_array = (double*)malloc(n * element_size);
        if (double_array) {
            memcpy(double_array, data, n * element_size);
            TIFFSwabArrayOfDouble(double_array, n);
            free(double_array);
        }
    }
    
    // TIFFSwabArrayOfTriples
    // Each triple is 3 bytes, n is number of triples
    element_size = 3;
    n = (tmsize_t)(size / element_size);
    if (n > max_elements) n = max_elements;
    if (n > 0) {
        uint8_t *triple_array = (uint8_t*)malloc(n * element_size);
        if (triple_array) {
            memcpy(triple_array, data, n * element_size);
            TIFFSwabArrayOfTriples(triple_array, n);
            free(triple_array);
        }
    }
    
    // TIFFSwabArrayOfLong
    element_size = sizeof(uint32_t);
    n = (tmsize_t)(size / element_size);
    if (n > max_elements) n = max_elements;
    if (n > 0) {
        uint32_t *long_array = (uint32_t*)malloc(n * element_size);
        if (long_array) {
            memcpy(long_array, data, n * element_size);
            TIFFSwabArrayOfLong(long_array, n);
            free(long_array);
        }
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    fuzz_swab_functions(Data, Size);
    return 0;
}