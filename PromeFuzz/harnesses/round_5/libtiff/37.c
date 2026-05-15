// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_TIFFSwabDouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    double value;
    memcpy(&value, data, sizeof(double));
    TIFFSwabDouble(&value);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = size / sizeof(uint16_t);
    if (max_elements > 1024) max_elements = 1024;
    
    uint16_t *arr = (uint16_t *)malloc(max_elements * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = size / sizeof(uint64_t);
    if (max_elements > 1024) max_elements = 1024;
    
    uint64_t *arr = (uint64_t *)malloc(max_elements * sizeof(uint64_t));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    size_t max_elements = size / sizeof(double);
    if (max_elements > 1024) max_elements = 1024;
    
    double *arr = (double *)malloc(max_elements * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(double));
    TIFFSwabArrayOfDouble(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t max_triples = size / 3;
    if (max_triples > 1024) max_triples = 1024;
    
    uint8_t *arr = (uint8_t *)malloc(max_triples * 3);
    if (!arr) return;
    
    memcpy(arr, data, max_triples * 3);
    TIFFSwabArrayOfTriples(arr, (tmsize_t)max_triples);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_elements = size / sizeof(float);
    if (max_elements > 1024) max_elements = 1024;
    
    float *arr = (float *)malloc(max_elements * sizeof(float));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(float));
    TIFFSwabArrayOfFloat(arr, (tmsize_t)max_elements);
    free(arr);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Seed random number generator with input data for diverse testing
    unsigned int seed = 0;
    for (size_t i = 0; i < Size && i < sizeof(seed); i++) {
        seed = (seed << 8) | Data[i];
    }
    srand(seed);
    
    // Randomly select which functions to test based on input
    int test_mask = (Size > 0) ? (Data[0] % 63) : 0;
    
    if (test_mask & 1) fuzz_TIFFSwabDouble(Data, Size);
    if (test_mask & 2) fuzz_TIFFSwabArrayOfShort(Data, Size);
    if (test_mask & 4) fuzz_TIFFSwabArrayOfLong8(Data, Size);
    if (test_mask & 8) fuzz_TIFFSwabArrayOfDouble(Data, Size);
    if (test_mask & 16) fuzz_TIFFSwabArrayOfTriples(Data, Size);
    if (test_mask & 32) fuzz_TIFFSwabArrayOfFloat(Data, Size);
    
    // Also test with different offsets into the data
    if (Size > 1) {
        size_t offset = Data[0] % (Size - 1);
        if (offset > 0 && Size - offset > 0) {
            fuzz_TIFFSwabArrayOfShort(Data + offset, Size - offset);
            fuzz_TIFFSwabArrayOfFloat(Data + offset, Size - offset);
        }
    }
    
    return 0;
}