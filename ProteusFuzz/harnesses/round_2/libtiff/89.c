// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
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

static void fuzz_swab_short(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t n = Size / sizeof(uint16_t);
    if (n == 0) return;
    
    uint16_t *buf = (uint16_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfShort(buf, n);
    
    free(buf);
}

static void fuzz_swab_triples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t n = Size / 3;
    if (n == 0) return;
    
    uint8_t *buf = (uint8_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfTriples(buf, n);
    
    free(buf);
}

static void fuzz_swab_double(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t n = Size / sizeof(double);
    if (n == 0) return;
    
    double *buf = (double *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfDouble(buf, n);
    
    free(buf);
}

static void fuzz_swab_float(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t n = Size / sizeof(float);
    if (n == 0) return;
    
    float *buf = (float *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfFloat(buf, n);
    
    free(buf);
}

static void fuzz_swab_long8_single(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    
    uint64_t val;
    memcpy(&val, Data, sizeof(uint64_t));
    
    TIFFSwabLong8(&val);
}

static void fuzz_swab_long8_array(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t n = Size / sizeof(uint64_t);
    if (n == 0) return;
    
    uint64_t *buf = (uint64_t *)malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfLong8(buf, n);
    
    free(buf);
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_swab_short(Data, Size);
    fuzz_swab_triples(Data, Size);
    fuzz_swab_double(Data, Size);
    fuzz_swab_float(Data, Size);
    fuzz_swab_long8_single(Data, Size);
    fuzz_swab_long8_array(Data, Size);
    
    return 0;
}