// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabLong8 at tif_swab.c:60:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
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
    
    size_t count = Size / sizeof(uint16_t);
    if (count == 0) return;
    
    uint16_t *buf = (uint16_t *)malloc(count * sizeof(uint16_t));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(uint16_t));
    
    TIFFSwabArrayOfShort(buf, count);
    
    free(buf);
}

static void fuzz_longs(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    
    size_t count = Size / sizeof(uint32_t);
    if (count == 0) return;
    
    uint32_t *buf = (uint32_t *)malloc(count * sizeof(uint32_t));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(uint32_t));
    
    TIFFSwabArrayOfLong(buf, count);
    
    free(buf);
}

static void fuzz_floats(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    
    size_t count = Size / sizeof(float);
    if (count == 0) return;
    
    float *buf = (float *)malloc(count * sizeof(float));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(float));
    
    TIFFSwabArrayOfFloat(buf, count);
    
    free(buf);
}

static void fuzz_long8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    
    uint64_t val;
    memcpy(&val, Data, sizeof(uint64_t));
    
    TIFFSwabLong8(&val);
}

static void fuzz_long8_array(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    
    size_t count = Size / sizeof(uint64_t);
    if (count == 0) return;
    
    uint64_t *buf = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(uint64_t));
    
    TIFFSwabArrayOfLong8(buf, count);
    
    free(buf);
}

static void fuzz_doubles(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    
    size_t count = Size / sizeof(double);
    if (count == 0) return;
    
    double *buf = (double *)malloc(count * sizeof(double));
    if (!buf) return;
    
    memcpy(buf, Data, count * sizeof(double));
    
    TIFFSwabArrayOfDouble(buf, count);
    
    free(buf);
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all functions with the input data
    fuzz_shorts(Data, Size);
    fuzz_longs(Data, Size);
    fuzz_floats(Data, Size);
    fuzz_long8(Data, Size);
    fuzz_long8_array(Data, Size);
    fuzz_doubles(Data, Size);
    
    return 0;
}