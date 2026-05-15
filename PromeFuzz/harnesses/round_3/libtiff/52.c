// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
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
    
    size_t max_elements = size / sizeof(uint16_t);
    size_t n = max_elements > 1024 ? 1024 : max_elements;
    
    uint16_t *arr = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, n);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t max_triples = size / 3;
    size_t n = max_triples > 1024 ? 1024 : max_triples;
    
    uint8_t *triples = (uint8_t *)malloc(n * 3);
    if (!triples) return;
    
    memcpy(triples, data, n * 3);
    TIFFSwabArrayOfTriples(triples, n);
    
    free(triples);
}

static void fuzz_TIFFSwabDouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    double d;
    memcpy(&d, data, sizeof(double));
    TIFFSwabDouble(&d);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    size_t max_elements = size / sizeof(double);
    size_t n = max_elements > 1024 ? 1024 : max_elements;
    
    double *arr = (double *)malloc(n * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(double));
    TIFFSwabArrayOfDouble(arr, n);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = size / sizeof(uint32_t);
    size_t n = max_elements > 1024 ? 1024 : max_elements;
    
    uint32_t *arr = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, n);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = size / sizeof(uint64_t);
    size_t n = max_elements > 1024 ? 1024 : max_elements;
    
    uint64_t *arr = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, n);
    
    free(arr);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabArrayOfTriples(Data, Size);
    fuzz_TIFFSwabDouble(Data, Size);
    fuzz_TIFFSwabArrayOfDouble(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    
    return 0;
}