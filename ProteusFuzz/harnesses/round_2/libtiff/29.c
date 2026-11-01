// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_tiffswab_arrayofshort(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t n = size / sizeof(uint16_t);
    if (n == 0) return;
    
    uint16_t *buf = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!buf) return;
    
    memcpy(buf, data, n * sizeof(uint16_t));
    TIFFSwabArrayOfShort(buf, n);
    free(buf);
}

static void fuzz_tiffswab_arrayoftriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    size_t n = size / 3;
    if (n == 0) return;
    
    uint8_t *buf = (uint8_t *)malloc(n * 3);
    if (!buf) return;
    
    memcpy(buf, data, n * 3);
    TIFFSwabArrayOfTriples(buf, n);
    free(buf);
}

static void fuzz_tiffswab_arrayofdouble(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    size_t n = size / sizeof(double);
    if (n == 0) return;
    
    double *buf = (double *)malloc(n * sizeof(double));
    if (!buf) return;
    
    memcpy(buf, data, n * sizeof(double));
    TIFFSwabArrayOfDouble(buf, n);
    free(buf);
}

static void fuzz_tiffswab_arrayoflong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    size_t n = size / sizeof(uint32_t);
    if (n == 0) return;
    
    uint32_t *buf = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!buf) return;
    
    memcpy(buf, data, n * sizeof(uint32_t));
    TIFFSwabArrayOfLong(buf, n);
    free(buf);
}

static void fuzz_tiffswab_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    uint16_t val;
    memcpy(&val, data, sizeof(uint16_t));
    TIFFSwabShort(&val);
}

static void fuzz_tiffswab_arrayoffloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    size_t n = size / sizeof(float);
    if (n == 0) return;
    
    float *buf = (float *)malloc(n * sizeof(float));
    if (!buf) return;
    
    memcpy(buf, data, n * sizeof(float));
    TIFFSwabArrayOfFloat(buf, n);
    free(buf);
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_tiffswab_arrayofshort(Data, Size);
    fuzz_tiffswab_arrayoftriples(Data, Size);
    fuzz_tiffswab_arrayofdouble(Data, Size);
    fuzz_tiffswab_arrayoflong(Data, Size);
    fuzz_tiffswab_short(Data, Size);
    fuzz_tiffswab_arrayoffloat(Data, Size);

    return 0;
}