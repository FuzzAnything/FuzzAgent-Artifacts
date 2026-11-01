// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
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

static void fuzz_tiffswabarrayoflong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    size_t n = size / sizeof(uint32_t);
    if (n == 0) return;
    uint32_t *arr = malloc(n * sizeof(uint32_t));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, n);
    free(arr);
}

static void fuzz_tiffreversebits(const uint8_t *data, size_t size) {
    if (size == 0) return;
    uint8_t *arr = malloc(size);
    if (!arr) return;
    memcpy(arr, data, size);
    TIFFReverseBits(arr, size);
    free(arr);
}

static void fuzz_tiffswabarrayofdouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    size_t n = size / sizeof(double);
    if (n == 0) return;
    double *arr = malloc(n * sizeof(double));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(double));
    TIFFSwabArrayOfDouble(arr, n);
    free(arr);
}

static void fuzz_tiffswabarrayoflong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    size_t n = size / sizeof(uint64_t);
    if (n == 0) return;
    uint64_t *arr = malloc(n * sizeof(uint64_t));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(arr, n);
    free(arr);
}

static void fuzz_tiffswabdouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    double val;
    memcpy(&val, data, sizeof(double));
    TIFFSwabDouble(&val);
}

static void fuzz_tiffswabarrayoftriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    size_t n = size / 3;
    if (n == 0) return;
    uint8_t *arr = malloc(size);
    if (!arr) return;
    memcpy(arr, data, size);
    TIFFSwabArrayOfTriples(arr, n);
    free(arr);
}

int LLVMFuzzerTestOneInput_98(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_tiffswabarrayoflong(Data, Size);
    fuzz_tiffreversebits(Data, Size);
    fuzz_tiffswabarrayofdouble(Data, Size);
    fuzz_tiffswabarrayoflong8(Data, Size);
    fuzz_tiffswabdouble(Data, Size);
    fuzz_tiffswabarrayoftriples(Data, Size);

    return 0;
}