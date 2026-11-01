// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
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

static void fuzz_shorts(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t n = Size / sizeof(uint16_t);
    if (n == 0) n = 1;
    
    uint16_t *buf = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!buf) return;
    
    memcpy(buf, Data, n * sizeof(uint16_t));
    
    TIFFSwabArrayOfShort(buf, n);
    
    free(buf);
}

static void fuzz_long8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    
    uint64_t val;
    memcpy(&val, Data, sizeof(uint64_t));
    
    TIFFSwabLong8(&val);
}

static void fuzz_doubles(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t n = Size / sizeof(double);
    if (n == 0) n = 1;
    
    double *buf = (double *)malloc(n * sizeof(double));
    if (!buf) return;
    
    memcpy(buf, Data, n * sizeof(double));
    
    TIFFSwabArrayOfDouble(buf, n);
    
    free(buf);
}

static void fuzz_long8_array(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t n = Size / sizeof(uint64_t);
    if (n == 0) n = 1;
    
    uint64_t *buf = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!buf) return;
    
    memcpy(buf, Data, n * sizeof(uint64_t));
    
    TIFFSwabArrayOfLong8(buf, n);
    
    free(buf);
}

static void fuzz_floats(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t n = Size / sizeof(float);
    if (n == 0) n = 1;
    
    float *buf = (float *)malloc(n * sizeof(float));
    if (!buf) return;
    
    memcpy(buf, Data, n * sizeof(float));
    
    TIFFSwabArrayOfFloat(buf, n);
    
    free(buf);
}

static void fuzz_triples(const uint8_t *Data, size_t Size) {
    if (Size < 3) return;
    size_t n = Size / 3;
    if (n == 0) n = 1;
    
    uint8_t *buf = (uint8_t *)malloc(n * 3);
    if (!buf) return;
    
    memcpy(buf, Data, n * 3);
    
    TIFFSwabArrayOfTriples(buf, n);
    
    free(buf);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz TIFFSwabArrayOfShort
    fuzz_shorts(Data, Size);
    
    // Fuzz TIFFSwabLong8
    fuzz_long8(Data, Size);
    
    // Fuzz TIFFSwabArrayOfDouble
    fuzz_doubles(Data, Size);
    
    // Fuzz TIFFSwabArrayOfLong8
    fuzz_long8_array(Data, Size);
    
    // Fuzz TIFFSwabArrayOfFloat
    fuzz_floats(Data, Size);
    
    // Fuzz TIFFSwabArrayOfTriples
    fuzz_triples(Data, Size);
    
    return 0;
}