// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_TIFFSwabArrayOfTriples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    
    size_t max_triples = size / 3;
    if (max_triples > 1024) max_triples = 1024;
    
    uint8_t *triples = (uint8_t *)malloc(max_triples * 3);
    if (!triples) return;
    
    memcpy(triples, data, max_triples * 3);
    TIFFSwabArrayOfTriples(triples, (tmsize_t)max_triples);
    
    free(triples);
}

static void fuzz_TIFFSwabArrayOfLong(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    size_t max_longs = size / sizeof(uint32_t);
    if (max_longs > 1024) max_longs = 1024;
    
    uint32_t *longs = (uint32_t *)malloc(max_longs * sizeof(uint32_t));
    if (!longs) return;
    
    memcpy(longs, data, max_longs * sizeof(uint32_t));
    TIFFSwabArrayOfLong(longs, (tmsize_t)max_longs);
    
    free(longs);
}

static void fuzz_TIFFSwabArrayOfLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    size_t max_long8s = size / sizeof(uint64_t);
    if (max_long8s > 1024) max_long8s = 1024;
    
    uint64_t *long8s = (uint64_t *)malloc(max_long8s * sizeof(uint64_t));
    if (!long8s) return;
    
    memcpy(long8s, data, max_long8s * sizeof(uint64_t));
    TIFFSwabArrayOfLong8(long8s, (tmsize_t)max_long8s);
    
    free(long8s);
}

static void fuzz_TIFFSwabArrayOfShort(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    
    size_t max_shorts = size / sizeof(uint16_t);
    if (max_shorts > 1024) max_shorts = 1024;
    
    uint16_t *shorts = (uint16_t *)malloc(max_shorts * sizeof(uint16_t));
    if (!shorts) return;
    
    memcpy(shorts, data, max_shorts * sizeof(uint16_t));
    TIFFSwabArrayOfShort(shorts, (tmsize_t)max_shorts);
    
    free(shorts);
}

static void fuzz_TIFFSwabLong8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    
    uint64_t value;
    memcpy(&value, data, sizeof(uint64_t));
    TIFFSwabLong8(&value);
}

static void fuzz_TIFFSwabArrayOfFloat(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    size_t max_floats = size / sizeof(float);
    if (max_floats > 1024) max_floats = 1024;
    
    float *floats = (float *)malloc(max_floats * sizeof(float));
    if (!floats) return;
    
    memcpy(floats, data, max_floats * sizeof(float));
    TIFFSwabArrayOfFloat(floats, (tmsize_t)max_floats);
    
    free(floats);
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_TIFFSwabArrayOfTriples(Data, Size);
    fuzz_TIFFSwabArrayOfLong(Data, Size);
    fuzz_TIFFSwabArrayOfLong8(Data, Size);
    fuzz_TIFFSwabArrayOfShort(Data, Size);
    fuzz_TIFFSwabLong8(Data, Size);
    fuzz_TIFFSwabArrayOfFloat(Data, Size);
    
    return 0;
}