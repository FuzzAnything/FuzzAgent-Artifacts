// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = size / sizeof(uint64_t);
    if (max_elements == 0) return;
    
    uint64_t *arr = (uint64_t *)malloc(size);
    if (!arr) return;
    
    memcpy(arr, data, size);
    TIFFSwabArrayOfLong8(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = size / sizeof(uint16_t);
    if (max_elements == 0) return;
    
    uint16_t *arr = (uint16_t *)malloc(size);
    if (!arr) return;
    
    memcpy(arr, data, size);
    TIFFSwabArrayOfShort(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    uint64_t value;
    memcpy(&value, data, sizeof(uint64_t));
    TIFFSwabLong8(&value);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_elements = size / sizeof(float);
    if (max_elements == 0) return;
    
    float *arr = (float *)malloc(size);
    if (!arr) return;
    
    memcpy(arr, data, size);
    TIFFSwabArrayOfFloat(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = size / sizeof(uint32_t);
    if (max_elements == 0) return;
    
    uint32_t *arr = (uint32_t *)malloc(size);
    if (!arr) return;
    
    memcpy(arr, data, size);
    TIFFSwabArrayOfLong(arr, (tmsize_t)max_elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t max_triples = size / 3;
    if (max_triples == 0) return;
    
    uint8_t *triples = (uint8_t *)malloc(size);
    if (!triples) return;
    
    memcpy(triples, data, size);
    TIFFSwabArrayOfTriples(triples, (tmsize_t)max_triples);
    free(triples);
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use a safe method to seed randomness without reading beyond buffer bounds
    unsigned int seed = 0;
    if (Size >= sizeof(unsigned int)) {
        memcpy(&seed, Data, sizeof(unsigned int));
    } else {
        // For small inputs, construct seed from available bytes
        for (size_t i = 0; i < Size && i < sizeof(unsigned int); i++) {
            seed = (seed << 8) | Data[i];
        }
    }
    srand(seed);
    
    // Call each function with different strategies
    int strategy = Size % 7;
    
    switch (strategy) {
        case 0:
            fuzz_TIFFSwabArrayOfLong8(Data, Size);
            break;
        case 1:
            fuzz_TIFFSwabArrayOfShort(Data, Size);
            break;
        case 2:
            fuzz_TIFFSwabLong8(Data, Size);
            break;
        case 3:
            fuzz_TIFFSwabArrayOfFloat(Data, Size);
            break;
        case 4:
            fuzz_TIFFSwabArrayOfLong(Data, Size);
            break;
        case 5:
            fuzz_TIFFSwabArrayOfTriples(Data, Size);
            break;
        case 6:
            // Test all functions with a subset of data
            if (Size > 100) {
                size_t subset = Size / 2;
                fuzz_TIFFSwabArrayOfLong8(Data, subset);
                fuzz_TIFFSwabArrayOfShort(Data + subset, Size - subset);
            }
            break;
    }
    
    return 0;
}