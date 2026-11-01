// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabDouble at tif_swab.c:201:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
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

static void fuzz_tiffswabarrayofshort(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t n = Size / sizeof(uint16_t);
    if (n == 0) return;
    
    uint16_t *buf = malloc(Size);
    if (!buf) return;
    
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfShort(buf, n);
    
    free(buf);
}

static void fuzz_tiffswabarrayofdouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t n = Size / sizeof(double);
    if (n == 0) return;
    
    double *buf = malloc(Size);
    if (!buf) return;
    
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfDouble(buf, n);
    
    free(buf);
}

static void fuzz_tiffswabdouble(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    
    double val;
    memcpy(&val, Data, sizeof(double));
    
    TIFFSwabDouble(&val);
}

static void fuzz_tiffswabarrayoffloat(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t n = Size / sizeof(float);
    if (n == 0) return;
    
    float *buf = malloc(Size);
    if (!buf) return;
    
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfFloat(buf, n);
    
    free(buf);
}

static void fuzz_tiffswabarrayoflong8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t n = Size / sizeof(uint64_t);
    if (n == 0) return;
    
    uint64_t *buf = malloc(Size);
    if (!buf) return;
    
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfLong8(buf, n);
    
    free(buf);
}

static void fuzz_tiffswabarrayoftriples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t n = Size / 3;
    if (n == 0) return;
    
    uint8_t *buf = malloc(Size);
    if (!buf) return;
    
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfTriples(buf, n);
    
    free(buf);
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz TIFFSwabArrayOfShort
    fuzz_tiffswabarrayofshort(Data, Size);
    
    // Fuzz TIFFSwabArrayOfDouble
    fuzz_tiffswabarrayofdouble(Data, Size);
    
    // Fuzz TIFFSwabDouble
    fuzz_tiffswabdouble(Data, Size);
    
    // Fuzz TIFFSwabArrayOfFloat
    fuzz_tiffswabarrayoffloat(Data, Size);
    
    // Fuzz TIFFSwabArrayOfLong8
    fuzz_tiffswabarrayoflong8(Data, Size);
    
    // Fuzz TIFFSwabArrayOfTriples
    fuzz_tiffswabarrayoftriples(Data, Size);
    
    return 0;
}