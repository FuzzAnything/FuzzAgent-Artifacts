// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel = *(const uint32_t *)Data;
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv24toXYZ(pixel, xyz);
}

static void fuzz_TIFFXYZToRGB(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    TIFFCIELabToRGB *state = NULL;
    if (state == NULL) return;
    
    float x = 0.0f, y = 0.0f, z = 0.0f;
    if (Size >= 12) {
        memcpy(&x, Data, sizeof(float));
        memcpy(&y, Data + 4, sizeof(float));
        memcpy(&z, Data + 8, sizeof(float));
    }
    
    uint32_t r, g, b;
    TIFFXYZToRGB(state, x, y, z, &r, &g, &b);
}

static void fuzz_XYZtoRGB24(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    uint8_t rgb[3] = {0, 0, 0};
    
    memcpy(xyz, Data, sizeof(float) * 3);
    XYZtoRGB24(xyz, rgb);
}

static void fuzz_TIFFCIELabToXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    TIFFCIELabToRGB *state = NULL;
    if (state == NULL) return;
    
    uint32_t L = 0;
    int32_t a = 0, b = 0;
    float x, y, z;
    
    if (Size >= 12) {
        memcpy(&L, Data, sizeof(uint32_t));
        memcpy(&a, Data + 4, sizeof(int32_t));
        memcpy(&b, Data + 8, sizeof(int32_t));
    }
    
    TIFFCIELabToXYZ(state, L, a, b, &x, &y, &z);
}

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 12) return;
    float xyz[3];
    int exponent = 1;
    
    memcpy(xyz, Data, sizeof(float) * 3);
    if (Size >= 16) {
        memcpy(&exponent, Data + 12, sizeof(int));
    }
    
    uint32_t result = LogLuv32fromXYZ(xyz, exponent);
    (void)result;
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return;
    uint32_t pixel = *(const uint32_t *)Data;
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32toXYZ(pixel, xyz);
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_LogLuv24toXYZ(Data, Size);
    fuzz_TIFFXYZToRGB(Data, Size);
    fuzz_XYZtoRGB24(Data, Size);
    fuzz_TIFFCIELabToXYZ(Data, Size);
    fuzz_LogLuv32fromXYZ(Data, Size);
    fuzz_LogLuv32toXYZ(Data, Size);
    
    return 0;
}