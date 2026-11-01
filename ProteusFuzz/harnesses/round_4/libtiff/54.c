// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabShort at tif_swab.c:33:6 in tiffio.h
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

static void fuzz_floats(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    size_t n = size / sizeof(float);
    if (n == 0) return;
    
    float *fp = (float *)malloc(n * sizeof(float));
    if (!fp) return;
    
    memcpy(fp, data, n * sizeof(float));
    
    TIFFSwabArrayOfFloat(fp, n);
    
    free(fp);
}

static void fuzz_long8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    size_t n = size / sizeof(uint64_t);
    if (n == 0) return;
    
    uint64_t *lp = (uint64_t *)malloc(n * sizeof(uint64_t));
    if (!lp) return;
    
    memcpy(lp, data, n * sizeof(uint64_t));
    
    TIFFSwabArrayOfLong8(lp, n);
    
    free(lp);
}

static void fuzz_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    size_t n = size / sizeof(uint16_t);
    if (n == 0) return;
    
    uint16_t *wp = (uint16_t *)malloc(n * sizeof(uint16_t));
    if (!wp) return;
    
    memcpy(wp, data, n * sizeof(uint16_t));
    
    TIFFSwabArrayOfShort(wp, n);
    
    free(wp);
}

static void fuzz_double(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    size_t n = size / sizeof(double);
    if (n == 0) return;
    
    double *dp = (double *)malloc(n * sizeof(double));
    if (!dp) return;
    
    memcpy(dp, data, n * sizeof(double));
    
    TIFFSwabArrayOfDouble(dp, n);
    
    free(dp);
}

static void fuzz_single_short(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    uint16_t val;
    memcpy(&val, data, sizeof(uint16_t));
    
    TIFFSwabShort(&val);
}

static void fuzz_long(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    size_t n = size / sizeof(uint32_t);
    if (n == 0) return;
    
    uint32_t *lp = (uint32_t *)malloc(n * sizeof(uint32_t));
    if (!lp) return;
    
    memcpy(lp, data, n * sizeof(uint32_t));
    
    TIFFSwabArrayOfLong(lp, n);
    
    free(lp);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all target functions with the input data
    fuzz_floats(Data, Size);
    fuzz_long8(Data, Size);
    fuzz_short(Data, Size);
    fuzz_double(Data, Size);
    fuzz_single_short(Data, Size);
    fuzz_long(Data, Size);
    
    return 0;
}