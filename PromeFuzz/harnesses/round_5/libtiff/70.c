// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
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
    size_t n = max_elements > 0 ? (data[0] % max_elements) + 1 : 0;
    
    if (n > 0) {
        uint64_t *arr = (uint64_t *)malloc(n * sizeof(uint64_t));
        if (arr) {
            memcpy(arr, data, n * sizeof(uint64_t));
            TIFFSwabArrayOfLong8(arr, n);
            free(arr);
        }
    }
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = size / sizeof(uint16_t);
    size_t n = max_elements > 0 ? (data[0] % max_elements) + 1 : 0;
    
    if (n > 0) {
        uint16_t *arr = (uint16_t *)malloc(n * sizeof(uint16_t));
        if (arr) {
            memcpy(arr, data, n * sizeof(uint16_t));
            TIFFSwabArrayOfShort(arr, n);
            free(arr);
        }
    }
}

static void fuzz_TIFFSwabLong8(const uint8_t *data, size_t size) {
    if (size >= sizeof(uint64_t)) {
        uint64_t value;
        memcpy(&value, data, sizeof(uint64_t));
        TIFFSwabLong8(&value);
    }
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t max_triples = size / 3;
    size_t n = max_triples > 0 ? (data[0] % max_triples) + 1 : 0;
    
    if (n > 0) {
        uint8_t *arr = (uint8_t *)malloc(n * 3);
        if (arr) {
            memcpy(arr, data, n * 3);
            TIFFSwabArrayOfTriples(arr, n);
            free(arr);
        }
    }
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_elements = size / sizeof(float);
    size_t n = max_elements > 0 ? (data[0] % max_elements) + 1 : 0;
    
    if (n > 0) {
        float *arr = (float *)malloc(n * sizeof(float));
        if (arr) {
            memcpy(arr, data, n * sizeof(float));
            TIFFSwabArrayOfFloat(arr, n);
            free(arr);
        }
    }
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = size / sizeof(uint32_t);
    size_t n = max_elements > 0 ? (data[0] % max_elements) + 1 : 0;
    
    if (n > 0) {
        uint32_t *arr = (uint32_t *)malloc(n * sizeof(uint32_t));
        if (arr) {
            memcpy(arr, data, n * sizeof(uint32_t));
            TIFFSwabArrayOfLong(arr, n);
            free(arr);
        }
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabLong8(Data, Size);
    fuzz_TIFFSwabArrayOfTriples(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    
    return 0;
}