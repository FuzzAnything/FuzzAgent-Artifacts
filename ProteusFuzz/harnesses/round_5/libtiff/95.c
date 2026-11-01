// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_tiffswab_arrayoflong(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    size_t count = Size / sizeof(uint32_t);
    if (count == 0) return;

    uint32_t *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);

    TIFFSwabArrayOfLong(buf, count);

    free(buf);
}

static void fuzz_tiffswab_arrayofshort(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t count = Size / sizeof(uint16_t);
    if (count == 0) return;

    uint16_t *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);

    TIFFSwabArrayOfShort(buf, count);

    free(buf);
}

static void fuzz_tiffswab_arrayofdouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t count = Size / sizeof(double);
    if (count == 0) return;

    double *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);

    TIFFSwabArrayOfDouble(buf, count);

    free(buf);
}

static void fuzz_tiffswab_arrayoflong8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t count = Size / sizeof(uint64_t);
    if (count == 0) return;

    uint64_t *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);

    TIFFSwabArrayOfLong8(buf, count);

    free(buf);
}

static void fuzz_tiffswab_arrayoffloat(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t count = Size / sizeof(float);
    if (count == 0) return;

    float *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);

    TIFFSwabArrayOfFloat(buf, count);

    free(buf);
}

static void fuzz_tiffswabfloat(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;

    float val;
    memcpy(&val, Data, sizeof(float));

    TIFFSwabFloat(&val);
}

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz each function with the input data
    fuzz_tiffswab_arrayoflong(Data, Size);
    fuzz_tiffswab_arrayofshort(Data, Size);
    fuzz_tiffswab_arrayofdouble(Data, Size);
    fuzz_tiffswab_arrayoflong8(Data, Size);
    fuzz_tiffswab_arrayoffloat(Data, Size);
    fuzz_tiffswabfloat(Data, Size);

    return 0;
}