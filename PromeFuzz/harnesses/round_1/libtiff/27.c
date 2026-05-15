// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = size / sizeof(uint64_t);
    if (max_elements > 1000) max_elements = 1000;
    
    uint64_t *array = (uint64_t *)malloc(max_elements * sizeof(uint64_t));
    if (!array) return;
    
    memcpy(array, data, max_elements * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(array, max_elements);
    free(array);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = size / sizeof(uint16_t);
    if (max_elements > 1000) max_elements = 1000;
    
    uint16_t *array = (uint16_t *)malloc(max_elements * sizeof(uint16_t));
    if (!array) return;
    
    memcpy(array, data, max_elements * sizeof(uint16_t));
    TIFFSwabArrayOfShort(array, max_elements);
    free(array);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_elements = size / sizeof(float);
    if (max_elements > 1000) max_elements = 1000;
    
    float *array = (float *)malloc(max_elements * sizeof(float));
    if (!array) return;
    
    memcpy(array, data, max_elements * sizeof(float));
    TIFFSwabArrayOfFloat(array, max_elements);
    free(array);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = size / sizeof(uint32_t);
    if (max_elements > 1000) max_elements = 1000;
    
    uint32_t *array = (uint32_t *)malloc(max_elements * sizeof(uint32_t));
    if (!array) return;
    
    memcpy(array, data, max_elements * sizeof(uint32_t));
    TIFFSwabArrayOfLong(array, max_elements);
    free(array);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t max_triples = size / 3;
    if (max_triples > 1000) max_triples = 1000;
    
    uint8_t *array = (uint8_t *)malloc(max_triples * 3);
    if (!array) return;
    
    memcpy(array, data, max_triples * 3);
    TIFFSwabArrayOfTriples(array, max_triples);
    free(array);
}

static void fuzz_TIFFSwabFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    float value;
    memcpy(&value, data, sizeof(float));
    TIFFSwabFloat(&value);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0:
            fuzz_TIFFSwabArrayOfLong8(fuzz_data, fuzz_size);
            break;
        case 1:
            fuzz_TIFFSwabArrayOfShort(fuzz_data, fuzz_size);
            break;
        case 2:
            fuzz_TIFFSwabArrayOfFloat(fuzz_data, fuzz_size);
            break;
        case 3:
            fuzz_TIFFSwabArrayOfLong(fuzz_data, fuzz_size);
            break;
        case 4:
            fuzz_TIFFSwabArrayOfTriples(fuzz_data, fuzz_size);
            break;
        case 5:
            fuzz_TIFFSwabFloat(fuzz_data, fuzz_size);
            break;
    }
    
    return 0;
}