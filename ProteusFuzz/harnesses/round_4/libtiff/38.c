// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    // Parse input data
    const uint8_t *ptr = Data;
    
    // Extract parameters for LogLuv32fromXYZ and LogLuv24fromXYZ
    // First 12 bytes: 3 floats (XYZ)
    float xyz[3];
    memcpy(xyz, ptr, sizeof(float) * 3);
    ptr += sizeof(float) * 3;
    
    // Next byte: exponent multiplier / error mode
    int em = 0;
    if (Size > 13) {
        em = (int)(ptr[0] & 0xFF);
        ptr++;
    }

    // Test LogLuv32fromXYZ
    uint32_t logluv32 = LogLuv32fromXYZ(xyz, em);
    (void)logluv32; // Use result to avoid unused variable warning

    // Test LogLuv24fromXYZ
    uint32_t logluv24 = LogLuv24fromXYZ(xyz, em);
    (void)logluv24;

    // Extract double for LogL16fromY and LogL10fromY
    double y_val = 0.0;
    if (Size > 13 + sizeof(double)) {
        memcpy(&y_val, ptr, sizeof(double));
        ptr += sizeof(double);
    }

    // Test LogL16fromY
    int logl16 = LogL16fromY(y_val, em);
    (void)logl16;

    // Test LogL10fromY
    int logl10 = LogL10fromY(y_val, em);
    (void)logl10;

    // Extract int for LogL10toY
    int p10 = 0;
    if (Size > 13 + sizeof(double) + sizeof(int)) {
        memcpy(&p10, ptr, sizeof(int));
        ptr += sizeof(int);
    }
    // Clamp p10 to valid range [0, 1023]
    if (p10 < 0) p10 = 0;
    if (p10 > 1023) p10 = 1023;

    // Test LogL10toY
    double y_from_logl10 = LogL10toY(p10);
    (void)y_from_logl10;

    // Extract int for uv_decode
    int uv_index = 0;
    if (Size > 13 + sizeof(double) + sizeof(int) + sizeof(int)) {
        memcpy(&uv_index, ptr, sizeof(int));
        ptr += sizeof(int);
    }
    
    // Clamp uv_index to valid range [0, UV_NDIVS-1]
    // UV_NDIVS is typically 32 in libtiff, but we use a safe upper bound
    if (uv_index < 0) uv_index = 0;
    if (uv_index > 31) uv_index = 31;

    double u, v;
    int uv_result = uv_decode(&u, &v, uv_index);
    (void)uv_result;
    (void)u;
    (void)v;

    // Additional fuzzing: try edge cases
    // Zero XYZ
    float xyz_zero[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32fromXYZ(xyz_zero, 0);
    LogLuv24fromXYZ(xyz_zero, 0);

    // Extreme Y values
    LogL16fromY(1e308, 0);
    LogL16fromY(-1e308, 0);
    LogL10fromY(1e308, 0);
    LogL10fromY(-1e308, 0);

    // Invalid uv_index (should return -1)
    uv_decode(&u, &v, -1);
    uv_decode(&u, &v, 1000);

    return 0;
}