// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 44) {
        return 0;
    }

    // Extract values from fuzzer input
    double y1, y2, u, v;
    uint32_t pixel32, pixel24;
    int clamp1, clamp2, color_type, encoding;
    float xyz[3];
    
    // Use memcpy to avoid alignment issues
    memcpy(&y1, Data, sizeof(double));
    memcpy(&y2, Data + 8, sizeof(double));
    memcpy(&u, Data + 16, sizeof(double));
    memcpy(&v, Data + 24, sizeof(double));
    memcpy(&pixel32, Data + 32, sizeof(uint32_t));
    memcpy(&pixel24, Data + 36, sizeof(uint32_t));
    
    // Extract integer parameters from remaining bytes
    if (Size >= 44) {
        clamp1 = Data[40] % 256;
        clamp2 = Data[41] % 256;
        color_type = Data[42] % 256;
        encoding = Data[43] % 256;
    } else {
        clamp1 = 0;
        clamp2 = 0;
        color_type = 0;
        encoding = 0;
    }

    // Test LogL10fromY
    int result1 = LogL10fromY(y1, clamp1);
    (void)result1;

    // Test LogL16fromY
    int result2 = LogL16fromY(y2, clamp2);
    (void)result2;

    // Test uv_encode
    int result3 = uv_encode(u, v, color_type);
    (void)result3;

    // Test LogLuv32toXYZ
    LogLuv32toXYZ(pixel32, xyz);

    // Test LogLuv24toXYZ
    LogLuv24toXYZ(pixel24, xyz);

    // Prepare XYZ array for LogLuv24fromXYZ
    float xyz_input[3];
    if (Size >= 56) {
        memcpy(&xyz_input[0], Data + 44, sizeof(float));
        memcpy(&xyz_input[1], Data + 48, sizeof(float));
        memcpy(&xyz_input[2], Data + 52, sizeof(float));
    } else {
        xyz_input[0] = 0.0f;
        xyz_input[1] = 0.0f;
        xyz_input[2] = 0.0f;
    }

    // Test LogLuv24fromXYZ
    uint32_t result4 = LogLuv24fromXYZ(xyz_input, encoding);
    (void)result4;

    return 0;
}