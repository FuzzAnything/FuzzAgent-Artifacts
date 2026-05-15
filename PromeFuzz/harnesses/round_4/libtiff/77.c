// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = size / sizeof(uint64_t);
    size_t n = max_elements > 100 ? 100 : max_elements; // Limit to reasonable size
    
    uint64_t *arr = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, n);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = size / sizeof(uint16_t);
    size_t n = max_elements > 200 ? 200 : max_elements;
    
    uint16_t *arr = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, n);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_elements = size / sizeof(float);
    size_t n = max_elements > 100 ? 100 : max_elements;
    
    float *arr = (float *)malloc(n * sizeof(float));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(float));
    TIFFSwabArrayOfFloat(arr, n);
    free(arr);
}

static void fuzz_TIFFSwabFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    float value;
    memcpy(&value, data, sizeof(float));
    TIFFSwabFloat(&value);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    size_t max_elements = size / sizeof(double);
    size_t n = max_elements > 50 ? 50 : max_elements;
    
    double *arr = (double *)malloc(n * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(double));
    TIFFSwabArrayOfDouble(arr, n);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = size / sizeof(uint32_t);
    size_t n = max_elements > 100 ? 100 : max_elements;
    
    uint32_t *arr = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, n);
    free(arr);
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Seed random number generator with input data for diverse testing
    unsigned int seed = 0;
    if (Size >= sizeof(unsigned int)) {
        memcpy(&seed, Data, sizeof(unsigned int));
    } else {
        seed = (unsigned int)Size;
    }
    srand(seed);
    
    // Test all functions with different input patterns
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    fuzz_TIFFSwabFloat(Data, Size);
    fuzz_TIFFSwabArrayOfDouble(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    
    // Test with offset data to explore different program states
    if (Size > 4) {
        size_t offset = rand() % (Size / 2);
        fuzz_TIFFSwabArrayOfLong8(Data + offset, Size - offset);
        fuzz_TIFFSwabArrayOfShort(Data + offset, Size - offset);
    }
    
    // Test with zero elements (edge case)
    uint64_t dummy64 = 0;
    uint16_t dummy16 = 0;
    float dummy_float = 0.0f;
    double dummy_double = 0.0;
    uint32_t dummy32 = 0;
    
    TIFFSwabArrayOfLong8(&dummy64, 0);
    TIFFSwabArrayOfShort(&dummy16, 0);
    TIFFSwabArrayOfFloat(&dummy_float, 0);
    TIFFSwabArrayOfDouble(&dummy_double, 0);
    TIFFSwabArrayOfLong(&dummy32, 0);
    
    // Test single element arrays
    if (Size >= sizeof(uint64_t)) {
        uint64_t single64;
        memcpy(&single64, Data, sizeof(uint64_t));
        TIFFSwabArrayOfLong8(&single64, 1);
    }
    
    if (Size >= sizeof(uint16_t)) {
        uint16_t single16;
        memcpy(&single16, Data, sizeof(uint16_t));
        TIFFSwabArrayOfShort(&single16, 1);
    }
    
    if (Size >= sizeof(float)) {
        float single_float;
        memcpy(&single_float, Data, sizeof(float));
        TIFFSwabArrayOfFloat(&single_float, 1);
    }
    
    if (Size >= sizeof(double)) {
        double single_double;
        memcpy(&single_double, Data, sizeof(double));
        TIFFSwabArrayOfDouble(&single_double, 1);
    }
    
    if (Size >= sizeof(uint32_t)) {
        uint32_t single32;
        memcpy(&single32, Data, sizeof(uint32_t));
        TIFFSwabArrayOfLong(&single32, 1);
    }
    
    return 0;
}