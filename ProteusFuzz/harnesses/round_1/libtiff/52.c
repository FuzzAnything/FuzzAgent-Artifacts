// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    uint32_t pixel = *(const uint32_t *)Data;
    float xyz[3];
    LogLuv32toXYZ(pixel, xyz);
}

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uint32_t)) return;
    uint32_t pixel = *(const uint32_t *)Data;
    float xyz[3];
    LogLuv24toXYZ(pixel, xyz);
}

static void fuzz_XYZtoRGB24(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(float)) return;
    float xyz[3];
    memcpy(xyz, Data, 3 * sizeof(float));
    uint8_t rgb[3];
    XYZtoRGB24(xyz, rgb);
}

static void fuzz_TIFFCIELabToXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(int32_t)) return;
    int32_t L, a, b;
    memcpy(&L, Data, sizeof(int32_t));
    memcpy(&a, Data + sizeof(int32_t), sizeof(int32_t));
    memcpy(&b, Data + 2 * sizeof(int32_t), sizeof(int32_t));
    
    TIFFCIELabToRGB state;
    TIFFDisplay display;
    float gamma = 2.2f;
    
    // Initialize display structure safely
    memset(&display, 0, sizeof(display));
    
    // Initialize state without relying on internal display fields
    if (!TIFFCIELabToRGBInit(&state, &display, &gamma)) return;
    
    float x, y, z;
    TIFFCIELabToXYZ(&state, L, a, b, &x, &y, &z);
}

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(float)) return;
    float xyz[3];
    memcpy(xyz, Data, 3 * sizeof(float));
    int exponent = 1;
    if (Size >= 4 * sizeof(float)) {
        memcpy(&exponent, Data + 3 * sizeof(float), sizeof(int));
    }
    uint32_t pixel = LogLuv32fromXYZ(xyz, exponent);
    (void)pixel;
}

static void fuzz_TIFFXYZToRGB(const uint8_t *Data, size_t Size) {
    if (Size < 3 * sizeof(float)) return;
    float x, y, z;
    memcpy(&x, Data, sizeof(float));
    memcpy(&y, Data + sizeof(float), sizeof(float));
    memcpy(&z, Data + 2 * sizeof(float), sizeof(float));
    
    TIFFCIELabToRGB state;
    TIFFDisplay display;
    float gamma = 2.2f;
    
    // Initialize display structure safely
    memset(&display, 0, sizeof(display));
    
    if (!TIFFCIELabToRGBInit(&state, &display, &gamma)) return;
    
    uint32_t r, g, b;
    TIFFXYZToRGB(&state, x, y, z, &r, &g, &b);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_LogLuv32toXYZ(Data, Size);
    fuzz_LogLuv24toXYZ(Data, Size);
    fuzz_XYZtoRGB24(Data, Size);
    fuzz_TIFFCIELabToXYZ(Data, Size);
    fuzz_LogLuv32fromXYZ(Data, Size);
    fuzz_TIFFXYZToRGB(Data, Size);
    
    return 0;
}