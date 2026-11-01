// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_TIFFSwabShort(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    uint16_t val;
    memcpy(&val, Data, sizeof(uint16_t));
    TIFFSwabShort(&val);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t count = Size / sizeof(uint16_t);
    if (count == 0) return;
    uint16_t *arr = (uint16_t *)malloc(count * sizeof(uint16_t));
    if (!arr) return;
    memcpy(arr, Data, count * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, (tmsize_t)count);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t count = Size / sizeof(double);
    if (count == 0) return;
    double *arr = (double *)malloc(count * sizeof(double));
    if (!arr) return;
    memcpy(arr, Data, count * sizeof(double));
    TIFFSwabArrayOfDouble(arr, (tmsize_t)count);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t count = Size / sizeof(uint64_t);
    if (count == 0) return;
    uint64_t *arr = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!arr) return;
    memcpy(arr, Data, count * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, (tmsize_t)count);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t count = Size / sizeof(float);
    if (count == 0) return;
    float *arr = (float *)malloc(count * sizeof(float));
    if (!arr) return;
    memcpy(arr, Data, count * sizeof(float));
    TIFFSwabArrayOfFloat(arr, (tmsize_t)count);
    free(arr);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t count = Size / 3;
    if (count == 0) return;
    uint8_t *arr = (uint8_t *)malloc(count * 3);
    if (!arr) return;
    memcpy(arr, Data, count * 3);
    TIFFSwabArrayOfTriples(arr, (tmsize_t)count);
    free(arr);
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_TIFFSwabShort(Data, Size);
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabArrayOfDouble(Data, Size);
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    fuzz_TIFFSwabArrayOfTriples(Data, Size);

    return 0;
}