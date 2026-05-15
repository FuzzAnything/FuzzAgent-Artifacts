// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFSwabLong at tif_swab.c:45:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_swab_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    uint16_t value;
    memcpy(&value, data, sizeof(uint16_t));
    TIFFSwabShort(&value);
}

static void fuzz_swab_long(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    uint32_t value;
    memcpy(&value, data, sizeof(uint32_t));
    TIFFSwabLong(&value);
}

static void fuzz_swab_array_of_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_elements = size / sizeof(uint16_t);
    if (max_elements == 0) return;
    
    size_t n = (size % 5) + 1;  // Vary between 1-5 elements
    if (n > max_elements) n = max_elements;
    
    uint16_t *arr = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, (tmsize_t)n);
    free(arr);
}

static void fuzz_swab_array_of_long(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_elements = size / sizeof(uint32_t);
    if (max_elements == 0) return;
    
    size_t n = (size % 7) + 1;  // Vary between 1-7 elements
    if (n > max_elements) n = max_elements;
    
    uint32_t *arr = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, (tmsize_t)n);
    free(arr);
}

static void fuzz_swab_array_of_long8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_elements = size / sizeof(uint64_t);
    if (max_elements == 0) return;
    
    size_t n = (size % 3) + 1;  // Vary between 1-3 elements
    if (n > max_elements) n = max_elements;
    
    uint64_t *arr = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!arr) return;
    
    memcpy(arr, data, n * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, (tmsize_t)n);
    free(arr);
}

static void fuzz_swab_array_of_triples(const uint8_t *data, size_t size) {
    if (size < 3) return;  // Minimum one triple
    
    size_t max_triples = size / 3;
    if (max_triples == 0) return;
    
    size_t n = (size % 4) + 1;  // Vary between 1-4 triples
    if (n > max_triples) n = max_triples;
    
    uint8_t *arr = (uint8_t *)malloc(3 * n);
    if (!arr) return;
    
    memcpy(arr, data, 3 * n);
    TIFFSwabArrayOfTriples(arr, (tmsize_t)n);
    free(arr);
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Seed-based variation to explore different states
    unsigned int seed = Data[0];
    
    // Call different functions based on input characteristics
    if (Size % 2 == 0) {
        fuzz_swab_short(Data, Size);
        fuzz_swab_long(Data, Size);
    }
    
    if (Size % 3 == 0) {
        fuzz_swab_array_of_short(Data, Size);
        fuzz_swab_array_of_long(Data, Size);
    }
    
    if (Size % 5 == 0) {
        fuzz_swab_array_of_long8(Data, Size);
    }
    
    if (Size % 7 == 0) {
        fuzz_swab_array_of_triples(Data, Size);
    }
    
    // Always call at least one function
    if (seed % 2 == 0) {
        fuzz_swab_short(Data, Size);
    } else if (seed % 3 == 0) {
        fuzz_swab_long(Data, Size);
    } else if (seed % 5 == 0) {
        fuzz_swab_array_of_short(Data, Size);
    } else if (seed % 7 == 0) {
        fuzz_swab_array_of_long(Data, Size);
    } else if (seed % 11 == 0) {
        fuzz_swab_array_of_long8(Data, Size);
    } else {
        fuzz_swab_array_of_triples(Data, Size);
    }
    
    return 0;
}