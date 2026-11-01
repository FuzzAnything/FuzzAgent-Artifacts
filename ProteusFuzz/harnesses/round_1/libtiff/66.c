// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabFloat at tif_swab.c:165:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfTriples at tif_swab.c:99:6 in tiffio.h
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

static void fuzz_swab_shorts(const uint8_t *data, size_t size) {
    if (size < sizeof(uint16_t)) return;
    size_t n = size / sizeof(uint16_t);
    if (n == 0) return;
    
    uint16_t *buf = (uint16_t *)malloc(size);
    if (!buf) return;
    
    memcpy(buf, data, size);
    TIFFSwabArrayOfShort(buf, n);
    
    free(buf);
}

static void fuzz_swab_float(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    
    float *buf = (float *)malloc(sizeof(float));
    if (!buf) return;
    
    memcpy(buf, data, sizeof(float));
    TIFFSwabFloat(buf);
    
    free(buf);
}

static void fuzz_swab_longs(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    size_t n = size / sizeof(uint32_t);
    if (n == 0) return;
    
    uint32_t *buf = (uint32_t *)malloc(size);
    if (!buf) return;
    
    memcpy(buf, data, size);
    TIFFSwabArrayOfLong(buf, n);
    
    free(buf);
}

static void fuzz_swab_floats(const uint8_t *data, size_t size) {
    if (size < sizeof(float)) return;
    size_t n = size / sizeof(float);
    if (n == 0) return;
    
    float *buf = (float *)malloc(size);
    if (!buf) return;
    
    memcpy(buf, data, size);
    TIFFSwabArrayOfFloat(buf, n);
    
    free(buf);
}

static void fuzz_swab_triples(const uint8_t *data, size_t size) {
    if (size < 3) return;
    size_t n = size / 3;
    if (n == 0) return;
    
    uint8_t *buf = (uint8_t *)malloc(size);
    if (!buf) return;
    
    memcpy(buf, data, size);
    TIFFSwabArrayOfTriples(buf, n);
    
    free(buf);
}

static void fuzz_swab_longs8(const uint8_t *data, size_t size) {
    if (size < sizeof(uint64_t)) return;
    size_t n = size / sizeof(uint64_t);
    if (n == 0) return;
    
    uint64_t *buf = (uint64_t *)malloc(size);
    if (!buf) return;
    
    memcpy(buf, data, size);
    TIFFSwabArrayOfLong8(buf, n);
    
    free(buf);
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_swab_shorts(Data, Size);
    fuzz_swab_float(Data, Size);
    fuzz_swab_longs(Data, Size);
    fuzz_swab_floats(Data, Size);
    fuzz_swab_triples(Data, Size);
    fuzz_swab_longs8(Data, Size);
    
    return 0;
}