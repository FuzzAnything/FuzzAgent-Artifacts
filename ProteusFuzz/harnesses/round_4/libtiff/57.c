// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    float xyz[3];
    int exponent;
    
    if (Size < 12) return;
    memcpy(xyz, Data, sizeof(float) * 3);
    
    if (Size < 16) return;
    memcpy(&exponent, Data + 12, sizeof(int));
    
    uint32_t result = LogLuv32fromXYZ(xyz, exponent);
    
    (void)result;
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    
    LogLuv32toXYZ(pixel, xyz);
    
    (void)xyz;
}

static void fuzz_uv_encode(const uint8_t *Data, size_t Size) {
    if (Size < 20) return;
    
    double u, v;
    int mode;
    
    if (Size < 8) return;
    memcpy(&u, Data, sizeof(double));
    
    if (Size < 16) return;
    memcpy(&v, Data + 8, sizeof(double));
    
    if (Size < 20) return;
    memcpy(&mode, Data + 16, sizeof(int));
    
    int result = uv_encode(u, v, mode);
    
    (void)result;
}

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    
    LogLuv24toXYZ(pixel, xyz);
    
    (void)xyz;
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    float xyz[3];
    int mode;
    
    if (Size < 12) return;
    memcpy(xyz, Data, sizeof(float) * 3);
    
    if (Size < 16) return;
    memcpy(&mode, Data + 12, sizeof(int));
    
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    
    (void)result;
}

static void fuzz_uv_decode(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    int index;
    memcpy(&index, Data, sizeof(int));
    
    double u, v;
    
    int result = uv_decode(&u, &v, index);
    
    (void)result;
    (void)u;
    (void)v;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;
    
    uint8_t action = Data[0] % 6;
    
    switch (action) {
        case 0:
            fuzz_LogLuv32fromXYZ(Data + 1, Size - 1);
            break;
        case 1:
            fuzz_LogLuv32toXYZ(Data + 1, Size - 1);
            break;
        case 2:
            fuzz_uv_encode(Data + 1, Size - 1);
            break;
        case 3:
            fuzz_LogLuv24toXYZ(Data + 1, Size - 1);
            break;
        case 4:
            fuzz_LogLuv24fromXYZ(Data + 1, Size - 1);
            break;
        case 5:
            fuzz_uv_decode(Data + 1, Size - 1);
            break;
    }
    
    return 0;
}