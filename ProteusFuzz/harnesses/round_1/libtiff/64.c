// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tiffio.h>

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test LogL10toY
    // Input range: 0-1023
    int logl10_input = Data[0] % 1024;
    double y_result = LogL10toY(logl10_input);
    (void)y_result; // Use result to prevent unused warnings

    // Test LogLuv32toXYZ
    // Need a uint32_t pixel and float array for output
    if (Size >= 4) {
        uint32_t pixel32 = 0;
        memcpy(&pixel32, Data, 4);
        float xyz_out[3] = {0.0f, 0.0f, 0.0f};
        LogLuv32toXYZ(pixel32, xyz_out);
        (void)xyz_out;
    }

    // Test uv_decode
    // Input index c must be 0 to UV_NDIVS-1. 
    // We'll use a safe modulo to keep within likely range, function handles overflow.
    if (Size >= 3) {
        int c = (Data[1] * 256 + Data[2]) % 256; 
        double u_res, v_res;
        int decode_ret = uv_decode(&u_res, &v_res, c);
        (void)decode_ret;
        (void)u_res;
        (void)v_res;
    }

    // Test LogLuv32fromXYZ
    // Needs float array and int exponent
    if (Size >= 13) {
        float xyz_in[3];
        memcpy(xyz_in, Data + 4, 12);
        int exp_mult = Data[12] % 100;
        uint32_t packed32 = LogLuv32fromXYZ(xyz_in, exp_mult);
        (void)packed32;
    }

    // Test LogLuv24fromXYZ
    // Needs float array and int mode
    if (Size >= 13) {
        float xyz_in2[3];
        memcpy(xyz_in2, Data + 4, 12);
        int mode = Data[12] % 2; // Mode is often 0 or 1
        uint32_t packed24 = LogLuv24fromXYZ(xyz_in2, mode);
        (void)packed24;
    }

    // Test uv_encode
    // Needs double u, double v, int mode
    if (Size >= 25) {
        double u_val = 0.0;
        double v_val = 0.0;
        // Interpret next 8 bytes as double
        memcpy(&u_val, Data + 12, sizeof(double));
        memcpy(&v_val, Data + 20, sizeof(double));
        int mode_enc = Data[24] % 2;
        int enc_ret = uv_encode(u_val, v_val, mode_enc);
        (void)enc_ret;
    }

    return 0;
}