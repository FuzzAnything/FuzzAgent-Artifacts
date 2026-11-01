// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tiffio.h"

static void test_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int exp_mult = (int)(Data[12] % 10) - 5; // Range -5 to 4
    
    uint32_t result = LogLuv32fromXYZ(xyz, exp_mult);
    (void)result;
}

static void test_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int mode = (int)(Data[12] % 2); // 0 or 1
    
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void test_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    int p10 = (int)(Data[0] | (Data[1] << 8));
    // Clamp to valid range 0-1023
    if (p10 < 0) p10 = 0;
    if (p10 > 1023) p10 = 1023;
    
    double result = LogL10toY(p10);
    (void)result;
}

static void test_uv_decode(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    int c = (int)(Data[0] | (Data[1] << 8));
    // Clamp to valid range 0-1023 (UV_NDIVS is typically 32x32=1024)
    if (c < 0) c = 0;
    if (c > 1023) c = 1023;
    
    double u, v;
    int result = uv_decode(&u, &v, c);
    (void)result;
}

static void test_uv_encode(const uint8_t *Data, size_t Size) {
    if (Size < 16) return;
    
    double u, v;
    memcpy(&u, Data, sizeof(double));
    memcpy(&v, Data + 8, sizeof(double));
    int mode = (int)(Data[16] % 2);
    
    int result = uv_encode(u, v, mode);
    (void)result;
}

static void test_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    float xyz[3];
    LogLuv32toXYZ(pixel, xyz);
    (void)xyz;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Determine which function to test based on the first byte
    int selector = Data[0] % 6;
    
    switch (selector) {
        case 0:
            test_LogLuv32fromXYZ(Data, Size);
            break;
        case 1:
            test_LogLuv24fromXYZ(Data, Size);
            break;
        case 2:
            test_LogL10toY(Data, Size);
            break;
        case 3:
            test_uv_decode(Data, Size);
            break;
        case 4:
            test_uv_encode(Data, Size);
            break;
        case 5:
            test_LogLuv32toXYZ(Data, Size);
            break;
    }
    
    return 0;
}