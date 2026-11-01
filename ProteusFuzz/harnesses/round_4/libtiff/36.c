// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 13) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    int exp_mult = (int)Data[12] % 10;
    LogLuv32fromXYZ(xyz, exp_mult);
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel;
    memcpy(&pixel, Data, sizeof(uint32_t));
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32toXYZ(pixel, xyz);
}

static void fuzz_XYZtoRGB24(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, sizeof(float) * 3);
    uint8_t rgb[3] = {0, 0, 0};
    XYZtoRGB24(xyz, rgb);
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
    memcpy(xyz, Data, sizeof(float) * 3);
    int mode = (int)Data[12] % 2;
    LogLuv24fromXYZ(xyz, mode);
}

static void fuzz_TIFFXYZToRGB(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    TIFFCIELabToRGB *state = NULL;
    if (state == NULL) return;
    
    float x, y, z;
    memcpy(&x, Data, sizeof(float));
    memcpy(&y, Data + 4, sizeof(float));
    memcpy(&z, Data + 8, sizeof(float));
    
    uint32_t r, g, b;
    TIFFXYZToRGB(state, x, y, z, &r, &g, &b);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t op = Data[0] % 6;
    
    switch (op) {
        case 0:
            fuzz_LogLuv32fromXYZ(Data, Size);
            break;
        case 1:
            fuzz_LogLuv32toXYZ(Data, Size);
            break;
        case 2:
            fuzz_XYZtoRGB24(Data, Size);
            break;
        case 3:
            fuzz_LogLuv24toXYZ(Data, Size);
            break;
        case 4:
            fuzz_LogLuv24fromXYZ(Data, Size);
            break;
        case 5:
            fuzz_TIFFXYZToRGB(Data, Size);
            break;
    }
    
    return 0;
}