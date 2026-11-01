// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t count = Size / sizeof(uint64_t);
    if (count == 0) return;
    
    uint64_t *arr = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!arr) return;
    
    memcpy(arr, Data, count * sizeof(uint64_t));
    
    TIFFSwabArrayOfLong8(arr, count);
    
    free(arr);
}

static void fuzz_TIFFReverseBits(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    
    uint8_t *arr = (uint8_t *)malloc(Size);
    if (!arr) return;
    
    memcpy(arr, Data, Size);
    
    TIFFReverseBits(arr, Size);
    
    free(arr);
}

static void fuzz_TIFFSwabDouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    
    double val;
    memcpy(&val, Data, sizeof(double));
    
    TIFFSwabDouble(&val);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    size_t count = Size / sizeof(uint32_t);
    if (count == 0) return;
    
    uint32_t *arr = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!arr) return;
    
    memcpy(arr, Data, count * sizeof(uint32_t));
    
    TIFFSwabArrayOfLong(arr, count);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t count = Size / 3;
    if (count == 0) return;
    
    uint8_t *arr = (uint8_t *)malloc(Size);
    if (!arr) return;
    
    memcpy(arr, Data, Size);
    
    TIFFSwabArrayOfTriples(arr, count);
    
    free(arr);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t count = Size / sizeof(double);
    if (count == 0) return;
    
    double *arr = (double *)malloc(count * sizeof(double));
    if (!arr) return;
    
    memcpy(arr, Data, count * sizeof(double));
    
    TIFFSwabArrayOfDouble(arr, count);
    
    free(arr);
}

int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all target functions
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    fuzz_TIFFReverseBits(Data, Size);
    fuzz_TIFFSwabDouble(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    fuzz_TIFFSwabArrayOfTriples(Data, Size);
    fuzz_TIFFSwabArrayOfDouble(Data, Size);
    
    return 0;
}