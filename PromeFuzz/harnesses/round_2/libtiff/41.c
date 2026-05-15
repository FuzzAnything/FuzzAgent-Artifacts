// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "libtiff/tiffio.h"

static void fuzz_TIFFSwabFloat(const uint8_t* data, size_t size) {
    if (size < sizeof(float)) return;
    
    float value;
    memcpy(&value, data, sizeof(float));
    TIFFSwabFloat(&value);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t* data, size_t size) {
    if (size < sizeof(double)) return;
    
    size_t max_elements = 100;
    size_t elements = size / sizeof(double);
    if (elements > max_elements) elements = max_elements;
    
    double* arr = (double*)malloc(elements * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, data, elements * sizeof(double));
    TIFFSwabArrayOfDouble(arr, elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t* data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = 100;
    size_t elements = size / sizeof(uint32_t);
    if (elements > max_elements) elements = max_elements;
    
    uint32_t* arr = (uint32_t*)malloc(elements * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, data, elements * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t* data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = 100;
    size_t elements = size / sizeof(uint64_t);
    if (elements > max_elements) elements = max_elements;
    
    uint64_t* arr = (uint64_t*)malloc(elements * sizeof(uint64_t));
    if (!arr) return;
    
    memcpy(arr, data, elements * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t* data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = 100;
    size_t elements = size / sizeof(uint16_t);
    if (elements > max_elements) elements = max_elements;
    
    uint16_t* arr = (uint16_t*)malloc(elements * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, elements * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, elements);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t* data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_elements = 100;
    size_t elements = size / sizeof(float);
    if (elements > max_elements) elements = max_elements;
    
    float* arr = (float*)malloc(elements * sizeof(float));
    if (!arr) return;
    
    memcpy(arr, data, elements * sizeof(float));
    TIFFSwabArrayOfFloat(arr, elements);
    free(arr);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_TIFFSwabFloat(Data, Size);
    fuzz_TIFFSwabArrayOfDouble(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    
    return 0;
}