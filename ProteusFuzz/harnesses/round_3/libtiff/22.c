// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int exp_mult = 0;
    if (Size >= 16) {
        memcpy(&exp_mult, Data + 12, sizeof(int));
    }
    uint32_t result = LogLuv32fromXYZ(xyz, exp_mult);
    (void)result;
}

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int mode = 0;
    if (Size >= 16) {
        memcpy(&mode, Data + 12, sizeof(int));
    }
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32toXYZ(pixel, xyz);
}

static void fuzz_LogL10toY(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int p10;
    memcpy(&p10, Data, sizeof(int));
    double result = LogL10toY(p10);
    (void)result;
}

static void fuzz_uv_encode(const uint8_t *Data, size_t Size) {
    if (Size < 20) return;
    double u, v;
    int mode;
    memcpy(&u, Data, sizeof(double));
    memcpy(&v, Data + 8, sizeof(double));
    memcpy(&mode, Data + 16, sizeof(int));
    int result = uv_encode(u, v, mode);
    (void)result;
}

static void fuzz_uv_decode(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    int c;
    memcpy(&c, Data, sizeof(int));
    double u, v;
    int result = uv_decode(&u, &v, c);
    (void)result;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_LogLuv32fromXYZ(Data, Size);
    fuzz_LogLuv24fromXYZ(Data, Size);
    fuzz_LogLuv32toXYZ(Data, Size);
    fuzz_LogL10toY(Data, Size);
    fuzz_uv_encode(Data, Size);
    fuzz_uv_decode(Data, Size);

    return 0;
}