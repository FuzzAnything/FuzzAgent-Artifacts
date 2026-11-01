// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t n = Size / sizeof(uint16_t);
    uint16_t *buf = (uint16_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfShort(buf, n);
    free(buf);
}

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t n = Size / 3;
    uint8_t *buf = (uint8_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfTriples(buf, n);
    free(buf);
}

static void fuzz_TIFFSwabArrayOfDouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t n = Size / sizeof(double);
    double *buf = (double *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfDouble(buf, n);
    free(buf);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t n = Size / sizeof(float);
    float *buf = (float *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfFloat(buf, n);
    free(buf);
}

static void fuzz_TIFFSwabFloat(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    float *buf = (float *)malloc(sizeof(float));
    if (!buf) return;
    memcpy(buf, Data, sizeof(float));
    TIFFSwabFloat(buf);
    free(buf);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t n = Size / sizeof(uint64_t);
    uint64_t *buf = (uint64_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfLong8(buf, n);
    free(buf);
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz TIFFSwabArrayOfShort
    fuzz_TIFFSwabArrayOfShort(Data, Size);

    // Fuzz TIFFSwabArrayOfTriples
    fuzz_TIFFSwabArrayOfTriples(Data, Size);

    // Fuzz TIFFSwabArrayOfDouble
    fuzz_TIFFSwabArrayOfDouble(Data, Size);

    // Fuzz TIFFSwabArrayOfFloat
    fuzz_TIFFSwabArrayOfFloat(Data, Size);

    // Fuzz TIFFSwabFloat
    fuzz_TIFFSwabFloat(Data, Size);

    // Fuzz TIFFSwabArrayOfLong8
    fuzz_TIFFSwabArrayOfLong8(Data, Size);

    return 0;
}