// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "libtiff/tiffio.h"

#define MAX_ELEMENTS 1024

static void fuzz_swab_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t) * 2) return;
    
    size_t max_n = size / sizeof(uint16_t);
    size_t n = max_n > MAX_ELEMENTS ? MAX_ELEMENTS : max_n;
    
    uint16_t *arr = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, n);
    free(arr);
}

static void fuzz_swab_float(const uint8_t *data, size_t size) {
    if (size < sizeof(float) * 2) return;
    
    size_t max_n = size / sizeof(float);
    size_t n = max_n > MAX_ELEMENTS ? MAX_ELEMENTS : max_n;
    
    float *arr = (float *)malloc(n * sizeof(float));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(float));
    TIFFSwabArrayOfFloat(arr, n);
    free(arr);
}

static void fuzz_swab_long(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t) * 2) return;
    
    size_t max_n = size / sizeof(uint32_t);
    size_t n = max_n > MAX_ELEMENTS ? MAX_ELEMENTS : max_n;
    
    uint32_t *arr = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, n);
    free(arr);
}

static void fuzz_swab_double_array(const uint8_t *data, size_t size) {
    if (size < sizeof(double) * 2) return;
    
    size_t max_n = size / sizeof(double);
    size_t n = max_n > MAX_ELEMENTS ? MAX_ELEMENTS : max_n;
    
    double *arr = (double *)malloc(n * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(double));
    TIFFSwabArrayOfDouble(arr, n);
    free(arr);
}

static void fuzz_swab_triples(const uint8_t *data, size_t size) {
    if (size < 3 * 2) return;
    
    size_t max_n = size / 3;
    size_t n = max_n > MAX_ELEMENTS ? MAX_ELEMENTS : max_n;
    
    uint8_t *arr = (uint8_t *)malloc(n * 3);
    if (!arr) return;
    
    memcpy(arr, data, n * 3);
    TIFFSwabArrayOfTriples(arr, n);
    free(arr);
}

static void fuzz_swab_single_double(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    double value;
    memcpy(&value, data, sizeof(double));
    TIFFSwabDouble(&value);
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_swab_short(Data, Size);
    fuzz_swab_float(Data, Size);
    fuzz_swab_long(Data, Size);
    fuzz_swab_double_array(Data, Size);
    fuzz_swab_triples(Data, Size);
    fuzz_swab_single_double(Data, Size);
    
    return 0;
}