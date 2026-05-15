// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_TIFFSwabLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    uint64_t value;
    memcpy(&value, data, sizeof(uint64_t));
    TIFFSwabLong8(&value);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    tmsize_t count = size / sizeof(float);
    float *array = (float *)malloc(size);
    if (!array) return;
    
    memcpy(array, data, size);
    TIFFSwabArrayOfFloat(array, count);
    free(array);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    tmsize_t count = size / sizeof(uint16_t);
    uint16_t *array = (uint16_t *)malloc(size);
    if (!array) return;
    
    memcpy(array, data, size);
    TIFFSwabArrayOfShort(array, count);
    free(array);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    tmsize_t count = size / 3;
    uint8_t *array = (uint8_t *)malloc(size);
    if (!array) return;
    
    memcpy(array, data, size);
    TIFFSwabArrayOfTriples(array, count);
    free(array);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    tmsize_t count = size / sizeof(uint32_t);
    uint32_t *array = (uint32_t *)malloc(size);
    if (!array) return;
    
    memcpy(array, data, size);
    TIFFSwabArrayOfLong(array, count);
    free(array);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    tmsize_t count = size / sizeof(uint64_t);
    uint64_t *array = (uint64_t *)malloc(size);
    if (!array) return;
    
    memcpy(array, data, size);
    TIFFSwabArrayOfLong8(array, count);
    free(array);
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_TIFFSwabLong8(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabArrayOfTriples(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    
    return 0;
}