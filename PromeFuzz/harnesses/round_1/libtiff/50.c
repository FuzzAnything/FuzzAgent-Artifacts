// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "libtiff/tiffio.h"

static int readInt(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) return 0;
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

static double readDouble(const uint8_t **data, size_t *size) {
    if (*size < sizeof(double)) return 0.0;
    double value;
    memcpy(&value, *data, sizeof(double));
    *data += sizeof(double);
    *size -= sizeof(double);
    return value;
}

static float readFloat(const uint8_t **data, size_t *size) {
    if (*size < sizeof(float)) return 0.0f;
    float value;
    memcpy(&value, *data, sizeof(float));
    *data += sizeof(float);
    *size -= sizeof(float);
    return value;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    // Test LogL16fromY
    double y1 = readDouble(&ptr, &remaining);
    int clamp1 = readInt(&ptr, &remaining);
    (void)LogL16fromY(y1, clamp1);
    
    // Test LogL10toY
    int logl10 = readInt(&ptr, &remaining);
    (void)LogL10toY(logl10);
    
    // Test LogLuv24fromXYZ
    float xyz[3];
    for (int i = 0; i < 3 && remaining > 0; i++) {
        xyz[i] = readFloat(&ptr, &remaining);
    }
    int em = readInt(&ptr, &remaining);
    (void)LogLuv24fromXYZ(xyz, em);
    
    // Test LogL10fromY
    double y2 = readDouble(&ptr, &remaining);
    int clamp2 = readInt(&ptr, &remaining);
    (void)LogL10fromY(y2, clamp2);
    
    // Test uv_decode
    double up, vp;
    int c = readInt(&ptr, &remaining);
    (void)uv_decode(&up, &vp, c);
    
    // Test uv_encode
    double u = readDouble(&ptr, &remaining);
    double v = readDouble(&ptr, &remaining);
    int color = readInt(&ptr, &remaining);
    (void)uv_encode(u, v, color);
    
    return 0;
}