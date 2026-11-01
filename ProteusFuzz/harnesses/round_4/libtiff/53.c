// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
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

static void fuzz_floats(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    size_t count = size / sizeof(float);
    if (count == 0) return;
    
    float *fp = malloc(count * sizeof(float));
    if (!fp) return;
    memcpy(fp, data, count * sizeof(float));
    
    TIFFSwabArrayOfFloat(fp, count);
    
    free(fp);
}

static void fuzz_long8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    size_t count = size / sizeof(uint64_t);
    if (count == 0) return;
    
    uint64_t *lp = malloc(count * sizeof(uint64_t));
    if (!lp) return;
    memcpy(lp, data, count * sizeof(uint64_t));
    
    TIFFSwabArrayOfLong8(lp, count);
    
    free(lp);
}

static void fuzz_shorts(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    size_t count = size / sizeof(uint16_t);
    if (count == 0) return;
    
    uint16_t *wp = malloc(count * sizeof(uint16_t));
    if (!wp) return;
    memcpy(wp, data, count * sizeof(uint16_t));
    
    TIFFSwabArrayOfShort(wp, count);
    
    free(wp);
}

static void fuzz_doubles(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    size_t count = size / sizeof(double);
    if (count == 0) return;
    
    double *dp = malloc(count * sizeof(double));
    if (!dp) return;
    memcpy(dp, data, count * sizeof(double));
    
    TIFFSwabArrayOfDouble(dp, count);
    
    free(dp);
}

static void fuzz_single_double(const uint8_t *data, size_t size) {
    if (size < sizeof(double)) return;
    
    double d;
    memcpy(&d, data, sizeof(double));
    
    TIFFSwabDouble(&d);
}

static void fuzz_triples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    size_t count = size / 3;
    if (count == 0) return;
    
    uint8_t *tp = malloc(count * 3);
    if (!tp) return;
    memcpy(tp, data, count * 3);
    
    TIFFSwabArrayOfTriples(tp, count);
    
    free(tp);
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all target functions with the input data
    fuzz_floats(Data, Size);
    fuzz_long8(Data, Size);
    fuzz_shorts(Data, Size);
    fuzz_doubles(Data, Size);
    fuzz_single_double(Data, Size);
    fuzz_triples(Data, Size);
    
    return 0;
}