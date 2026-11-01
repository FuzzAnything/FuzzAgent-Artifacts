// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSwabArrayOfShort at tif_swab.c:81:6 in tiffio.h
// TIFFSwabArrayOfLong at tif_swab.c:117:6 in tiffio.h
// TIFFSwabArrayOfDouble at tif_swab.c:222:6 in tiffio.h
// TIFFSwabArrayOfFloat at tif_swab.c:180:6 in tiffio.h
// TIFFSwabArrayOfLong8 at tif_swab.c:138:6 in tiffio.h
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

static void fuzz_shorts(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint16_t)) return;
    size_t n = Size / sizeof(uint16_t);
    if (n == 0) return;
    
    uint16_t *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfShort(buf, n);
    
    free(buf);
}

static void fuzz_longs(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    size_t n = Size / sizeof(uint32_t);
    if (n == 0) return;
    
    uint32_t *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfLong(buf, n);
    
    free(buf);
}

static void fuzz_doubles(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(double)) return;
    size_t n = Size / sizeof(double);
    if (n == 0) return;
    
    double *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfDouble(buf, n);
    
    free(buf);
}

static void fuzz_floats(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    size_t n = Size / sizeof(float);
    if (n == 0) return;
    
    float *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfFloat(buf, n);
    
    free(buf);
}

static void fuzz_longs8(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint64_t)) return;
    size_t n = Size / sizeof(uint64_t);
    if (n == 0) return;
    
    uint64_t *buf = malloc(Size);
    if (!buf) return;
    memcpy(buf, Data, Size);
    
    TIFFSwabArrayOfLong8(buf, n);
    
    free(buf);
}

static void fuzz_single_float(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(float)) return;
    
    float val;
    memcpy(&val, Data, sizeof(float));
    
    TIFFSwabFloat(&val);
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Fuzz all functions with the input data
    fuzz_shorts(Data, Size);
    fuzz_longs(Data, Size);
    fuzz_doubles(Data, Size);
    fuzz_floats(Data, Size);
    fuzz_longs8(Data, Size);
    fuzz_single_float(Data, Size);
    
    return 0;
}