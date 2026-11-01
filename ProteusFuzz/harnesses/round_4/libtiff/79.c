// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_single_float(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    float f;
    memcpy(&f, data, sizeof(float));
    TIFFSwabFloat(&f);
}

static void fuzz_array_float(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    tmsize_t n = size / sizeof(float);
    if (n == 0) return;
    float *arr = (float *)malloc(n * sizeof(float));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(float));
    TIFFSwabArrayOfFloat(arr, n);
    free(arr);
}

static void fuzz_array_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    tmsize_t n = size / sizeof(uint16_t);
    if (n == 0) return;
    uint16_t *arr = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(uint16_t));
    TIFFSwabArrayOfShort(arr, n);
    free(arr);
}

static void fuzz_array_double(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    tmsize_t n = size / sizeof(double);
    if (n == 0) return;
    double *arr = (double *)malloc(n * sizeof(double));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(double));
    TIFFSwabArrayOfDouble(arr, n);
    free(arr);
}

static void fuzz_array_triples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    tmsize_t n = size / 3;
    if (n == 0) return;
    uint8_t *arr = (uint8_t *)malloc(n * 3);
    if (!arr) return;
    memcpy(arr, data, n * 3);
    TIFFSwabArrayOfTriples(arr, n);
    free(arr);
}

static void fuzz_array_long(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    tmsize_t n = size / sizeof(uint32_t);
    if (n == 0) return;
    uint32_t *arr = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!arr) return;
    memcpy(arr, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(arr, n);
    free(arr);
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz single float
    fuzz_single_float(Data, Size);

    // Fuzz array of floats
    fuzz_array_float(Data, Size);

    // Fuzz array of shorts
    fuzz_array_short(Data, Size);

    // Fuzz array of doubles
    fuzz_array_double(Data, Size);

    // Fuzz array of triples
    fuzz_array_triples(Data, Size);

    // Fuzz array of longs
    fuzz_array_long(Data, Size);

    return 0;
}