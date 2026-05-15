// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Seed random number generator for diverse program states
    static int initialized = 0;
    if (!initialized) {
        srand(0);
        initialized = 1;
    }

    // Calculate array sizes based on input data
    size_t max_elements = Size / 8; // Use 8 as base for 64-bit types
    
    // Test TIFFSwabArrayOfLong8
    if (max_elements > 0) {
        uint64_t *long8_array = (uint64_t *)malloc(max_elements * sizeof(uint64_t));
        if (long8_array) {
            // Initialize with fuzzer data
            size_t bytes_to_copy = max_elements * sizeof(uint64_t);
            if (bytes_to_copy > Size) bytes_to_copy = Size;
            memcpy(long8_array, Data, bytes_to_copy);
            
            // Call target function
            TIFFSwabArrayOfLong8(long8_array, (tmsize_t)max_elements);
            
            free(long8_array);
        }
    }

    // Test TIFFSwabArrayOfDouble
    if (max_elements > 0) {
        double *double_array = (double *)malloc(max_elements * sizeof(double));
        if (double_array) {
            size_t bytes_to_copy = max_elements * sizeof(double);
            if (bytes_to_copy > Size) bytes_to_copy = Size;
            memcpy(double_array, Data, bytes_to_copy);
            
            TIFFSwabArrayOfDouble(double_array, (tmsize_t)max_elements);
            
            free(double_array);
        }
    }

    // Test TIFFSwabFloat
    if (Size >= sizeof(float)) {
        float float_value;
        memcpy(&float_value, Data, sizeof(float));
        TIFFSwabFloat(&float_value);
    }

    // Test TIFFSwabArrayOfTriples
    // Each triple is 3 bytes, so n is number of triples
    size_t triple_count = Size / 3;
    if (triple_count > 0) {
        uint8_t *triples = (uint8_t *)malloc(triple_count * 3);
        if (triples) {
            size_t bytes_to_copy = triple_count * 3;
            if (bytes_to_copy > Size) bytes_to_copy = Size;
            memcpy(triples, Data, bytes_to_copy);
            
            TIFFSwabArrayOfTriples(triples, (tmsize_t)triple_count);
            
            free(triples);
        }
    }

    // Test TIFFSwabArrayOfFloat
    size_t float_count = Size / sizeof(float);
    if (float_count > 0) {
        float *float_array = (float *)malloc(float_count * sizeof(float));
        if (float_array) {
            size_t bytes_to_copy = float_count * sizeof(float);
            if (bytes_to_copy > Size) bytes_to_copy = Size;
            memcpy(float_array, Data, bytes_to_copy);
            
            TIFFSwabArrayOfFloat(float_array, (tmsize_t)float_count);
            
            free(float_array);
        }
    }

    // Test TIFFSwabArrayOfLong
    size_t long_count = Size / sizeof(uint32_t);
    if (long_count > 0) {
        uint32_t *long_array = (uint32_t *)malloc(long_count * sizeof(uint32_t));
        if (long_array) {
            size_t bytes_to_copy = long_count * sizeof(uint32_t);
            if (bytes_to_copy > Size) bytes_to_copy = Size;
            memcpy(long_array, Data, bytes_to_copy);
            
            TIFFSwabArrayOfLong(long_array, (tmsize_t)long_count);
            
            free(long_array);
        }
    }

    // Test with different array sizes to explore more states
    for (int i = 0; i < 3; i++) {
        size_t test_size = (rand() % 10) + 1;
        if (test_size * sizeof(uint64_t) <= Size) {
            uint64_t *test_array = (uint64_t *)malloc(test_size * sizeof(uint64_t));
            if (test_array) {
                memcpy(test_array, Data, test_size * sizeof(uint64_t));
                TIFFSwabArrayOfLong8(test_array, (tmsize_t)test_size);
                free(test_array);
            }
        }
    }

    return 0;
}