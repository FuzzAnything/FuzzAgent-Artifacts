// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <string.h>

static void fuzz_LogLuv24fromXYZ(const uint8_t *data, size_t size) {
    if (size < 4 * sizeof(float)) return;
    
    float xyz[3];
    memcpy(&xyz[0], data, sizeof(float));
    memcpy(&xyz[1], data + sizeof(float), sizeof(float));
    memcpy(&xyz[2], data + 2 * sizeof(float), sizeof(float));
    int em = *(int*)(data + 3 * sizeof(float));
    
    LogLuv24fromXYZ(xyz, em);
}

static void fuzz_XYZtoRGB24(const uint8_t *data, size_t size) {
    if (size < 3 * sizeof(float)) return;
    
    float xyz[3];
    uint8_t rgb[3];
    
    memcpy(&xyz[0], data, sizeof(float));
    memcpy(&xyz[1], data + sizeof(float), sizeof(float));
    memcpy(&xyz[2], data + 2 * sizeof(float), sizeof(float));
    
    XYZtoRGB24(xyz, rgb);
}

static void fuzz_TIFFXYZToRGB(const uint8_t *data, size_t size) {
    if (size < 3 * sizeof(float)) return;
    
    TIFFCIELabToRGB cielabToRGB;
    memset(&cielabToRGB, 0, sizeof(cielabToRGB));
    
    float x, y, z;
    uint32_t r, g, b;
    
    memcpy(&x, data, sizeof(float));
    memcpy(&y, data + sizeof(float), sizeof(float));
    memcpy(&z, data + 2 * sizeof(float), sizeof(float));
    
    TIFFXYZToRGB(&cielabToRGB, x, y, z, &r, &g, &b);
}

static void fuzz_TIFFCIELabToXYZ(const uint8_t *data, size_t size) {
    if (size < 3 * sizeof(uint32_t)) return;
    
    TIFFCIELabToRGB cielabToRGB;
    memset(&cielabToRGB, 0, sizeof(cielabToRGB));
    
    uint32_t L = *(uint32_t*)data;
    int32_t a = *(int32_t*)(data + sizeof(uint32_t));
    int32_t b_val = *(int32_t*)(data + 2 * sizeof(uint32_t));
    float x, y, z;
    
    TIFFCIELabToXYZ(&cielabToRGB, L, a, b_val, &x, &y, &z);
}

static void fuzz_LogLuv24toXYZ(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    uint32_t logLuv24 = *(uint32_t*)data;
    float xyz[3];
    
    LogLuv24toXYZ(logLuv24, xyz);
}

static void fuzz_LogLuv32toXYZ(const uint8_t *data, size_t size) {
    if (size < sizeof(uint32_t)) return;
    
    uint32_t logLuv32 = *(uint32_t*)data;
    float xyz[3];
    
    LogLuv32toXYZ(logLuv32, xyz);
}

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_LogLuv24fromXYZ(Data, Size);
    fuzz_XYZtoRGB24(Data, Size);
    fuzz_TIFFXYZToRGB(Data, Size);
    fuzz_TIFFCIELabToXYZ(Data, Size);
    fuzz_LogLuv24toXYZ(Data, Size);
    fuzz_LogLuv32toXYZ(Data, Size);
    
    return 0;
}