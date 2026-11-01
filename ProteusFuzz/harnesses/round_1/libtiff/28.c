// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <tiffio.h>

static void safe_fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel = 0;
    memcpy(&pixel, Data, 4);
    float xyz[3];
    LogLuv32toXYZ(pixel, xyz);
}

static void safe_fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel = 0;
    memcpy(&pixel, Data, 4);
    float xyz[3];
    LogLuv24toXYZ(pixel, xyz);
}

static void safe_fuzz_XYZtoRGB24(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, 12);
    uint8_t rgb[3];
    XYZtoRGB24(xyz, rgb);
}

static void safe_fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, 12);
    int exp_mult = 0;
    if (Size >= 16) {
        memcpy(&exp_mult, Data + 12, 4);
    }
    uint32_t result = LogLuv32fromXYZ(xyz, exp_mult);
    (void)result;
}

static void safe_fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    memcpy(xyz, Data, 12);
    int mode = 0;
    if (Size >= 16) {
        memcpy(&mode, Data + 12, 4);
    }
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void safe_fuzz_TIFFXYZToRGB(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float x, y, z;
    memcpy(&x, Data, 4);
    memcpy(&y, Data + 4, 4);
    memcpy(&z, Data + 8, 4);
    
    TIFFCIELabToRGB state;
    // Initialize a valid dummy display structure to prevent NULL pointer dereference
    TIFFDisplay display = {0};
    float *lut = NULL;
    
    if (TIFFCIELabToRGBInit(&state, &display, lut) == 0) {
        return;
    }
    
    uint32_t r, g, b;
    TIFFXYZToRGB(&state, x, y, z, &r, &g, &b);
    
    // Note: Memory management for state is handled internally or ignored for fuzzing stability
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t op = Data[0] % 6;
    
    switch (op) {
        case 0:
            safe_fuzz_LogLuv32toXYZ(Data, Size);
            break;
        case 1:
            safe_fuzz_LogLuv24toXYZ(Data, Size);
            break;
        case 2:
            safe_fuzz_XYZtoRGB24(Data, Size);
            break;
        case 3:
            safe_fuzz_LogLuv32fromXYZ(Data, Size);
            break;
        case 4:
            safe_fuzz_LogLuv24fromXYZ(Data, Size);
            break;
        case 5:
            safe_fuzz_TIFFXYZToRGB(Data, Size);
            break;
        default:
            break;
    }
    
    return 0;
}