// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 13) return;
    float xyz[3];
    if (Size >= 12) {
        memcpy(xyz, Data, sizeof(float) * 3);
    } else {
        xyz[0] = xyz[1] = xyz[2] = 0.0f;
    }
    int exp_mult = (int)(Data[12] % 100) - 50;
    LogLuv32fromXYZ(xyz, exp_mult);
}

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv24toXYZ(pixel, xyz);
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 13) return;
    float xyz[3];
    if (Size >= 12) {
        memcpy(xyz, Data, sizeof(float) * 3);
    } else {
        xyz[0] = xyz[1] = xyz[2] = 0.0f;
    }
    int mode = (int)(Data[12] % 2);
    LogLuv24fromXYZ(xyz, mode);
}

static void fuzz_uv_encode(const uint8_t *Data, size_t Size) {
    if (Size < 17) return;
    double u, v;
    if (Size >= 16) {
        memcpy(&u, Data, sizeof(double));
        memcpy(&v, Data + 8, sizeof(double));
    } else {
        u = v = 0.0;
    }
    int mode = (int)(Data[16] % 2);
    uv_encode(u, v, mode);
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32toXYZ(pixel, xyz);
}

static void fuzz_uv_decode(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int index;
    memcpy(&index, Data, sizeof(int));
    double u = 0.0, v = 0.0;
    uv_decode(&u, &v, index);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t op = Data[0] % 6;
    const uint8_t *payload = Data + 1;
    size_t payload_size = Size - 1;

    switch (op) {
        case 0:
            fuzz_LogLuv32fromXYZ(payload, payload_size);
            break;
        case 1:
            fuzz_LogLuv24toXYZ(payload, payload_size);
            break;
        case 2:
            fuzz_LogLuv24fromXYZ(payload, payload_size);
            break;
        case 3:
            fuzz_uv_encode(payload, payload_size);
            break;
        case 4:
            fuzz_LogLuv32toXYZ(payload, payload_size);
            break;
        case 5:
            fuzz_uv_decode(payload, payload_size);
            break;
        default:
            break;
    }
    return 0;
}