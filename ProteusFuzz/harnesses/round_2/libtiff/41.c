// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFReverseBits at tif_swab.c:310:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
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

static void fuzz_tiff_reverse_bits(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;
    uint8_t *buf = (uint8_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFReverseBits(buf, (tmsize_t)Size);
    free(buf);
}

static void fuzz_tiff_swab_double(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    double val;
    memcpy(&val, Data, sizeof(double));
    TIFFSwabDouble(&val);
}

static void fuzz_tiff_swab_array_of_triples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t count = Size / 3;
    if (count == 0) return;
    uint8_t *buf = (uint8_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfTriples(buf, (tmsize_t)count);
    free(buf);
}

static void fuzz_tiff_swab_array_of_double(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t count = Size / sizeof(double);
    if (count == 0) return;
    double *buf = (double *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfDouble(buf, (tmsize_t)count);
    free(buf);
}

static void fuzz_tiff_swab_array_of_long(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    size_t count = Size / sizeof(uint32_t);
    if (count == 0) return;
    uint32_t *buf = (uint32_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfLong(buf, (tmsize_t)count);
    free(buf);
}

static void fuzz_tiff_swab_array_of_long8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t count = Size / sizeof(uint64_t);
    if (count == 0) return;
    uint64_t *buf = (uint64_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    TIFFSwabArrayOfLong8(buf, (tmsize_t)count);
    free(buf);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_tiff_reverse_bits(Data, Size);
    fuzz_tiff_swab_double(Data, Size);
    fuzz_tiff_swab_array_of_triples(Data, Size);
    fuzz_tiff_swab_array_of_double(Data, Size);
    fuzz_tiff_swab_array_of_long(Data, Size);
    fuzz_tiff_swab_array_of_long8(Data, Size);

    return 0;
}