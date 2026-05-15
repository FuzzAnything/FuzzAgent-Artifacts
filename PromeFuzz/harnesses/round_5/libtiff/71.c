// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
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

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    tmsize_t max_elements = size / sizeof(uint16_t);
    if (max_elements > 1024) max_elements = 1024;
    
    uint16_t *arr = (uint16_t *)malloc(max_elements * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, max_elements);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    tmsize_t max_elements = size / sizeof(double);
    if (max_elements > 1024) max_elements = 1024;
    
    double *arr = (double *)malloc(max_elements * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(double));
    TIFFSwabArrayOfDouble(arr, max_elements);
    
    free(arr);
}

static void fuzz_TIFFSwabFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    float value;
    memcpy(&value, data, sizeof(float));
    TIFFSwabFloat(&value);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    tmsize_t max_triples = size / 3;
    if (max_triples > 1024) max_triples = 1024;
    
    uint8_t *arr = (uint8_t *)malloc(max_triples * 3);
    if (!arr) return;
    
    memcpy(arr, data, max_triples * 3);
    TIFFSwabArrayOfTriples(arr, max_triples);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    tmsize_t max_elements = size / sizeof(float);
    if (max_elements > 1024) max_elements = 1024;
    
    float *arr = (float *)malloc(max_elements * sizeof(float));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(float));
    TIFFSwabArrayOfFloat(arr, max_elements);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    tmsize_t max_elements = size / sizeof(uint32_t);
    if (max_elements > 1024) max_elements = 1024;
    
    uint32_t *arr = (uint32_t *)malloc(max_elements * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, data, max_elements * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, max_elements);
    
    free(arr);
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabArrayOfDouble(Data, Size);
    fuzz_TIFFSwabFloat(Data, Size);
    fuzz_TIFFSwabArrayOfTriples(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    
    return 0;
}