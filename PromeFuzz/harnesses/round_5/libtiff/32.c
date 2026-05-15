// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// uv_decode at tif_luv.c:997:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static double readDoubleFromData(const uint8_t *data, size_t size, size_t *offset) {
    double value = 0.0;
    if (*offset + sizeof(double) <= size) {
        memcpy(&value, data + *offset, sizeof(double));
        *offset += sizeof(double);
    }
    return value;
}

static float readFloatFromData(const uint8_t *data, size_t size, size_t *offset) {
    float value = 0.0f;
    if (*offset + sizeof(float) <= size) {
        memcpy(&value, data + *offset, sizeof(float));
        *offset += sizeof(float);
    }
    return value;
}

static int readIntFromData(const uint8_t *data, size_t size, size_t *offset) {
    int value = 0;
    if (*offset + sizeof(int) <= size) {
        memcpy(&value, data + *offset, sizeof(int));
        *offset += sizeof(int);
    }
    return value;
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test LogL16fromY
    double y1 = readDoubleFromData(Data, Size, &offset);
    int em1 = readIntFromData(Data, Size, &offset);
    int result1 = LogL16fromY(y1, em1);
    (void)result1;
    
    // Test LogLuv24fromXYZ
    float xyz[3];
    for (int i = 0; i < 3; i++) {
        xyz[i] = readFloatFromData(Data, Size, &offset);
    }
    int em2 = readIntFromData(Data, Size, &offset);
    uint32_t result2 = LogLuv24fromXYZ(xyz, em2);
    (void)result2;
    
    // Test uv_decode
    double up = 0.0, vp = 0.0;
    int c = readIntFromData(Data, Size, &offset);
    int result3 = uv_decode(&up, &vp, c);
    (void)result3;
    
    // Test uv_encode
    double u = readDoubleFromData(Data, Size, &offset);
    double v = readDoubleFromData(Data, Size, &offset);
    int color_table_index = readIntFromData(Data, Size, &offset);
    int result4 = uv_encode(u, v, color_table_index);
    (void)result4;
    
    // Test LogL10toY
    int logl10 = readIntFromData(Data, Size, &offset);
    double result5 = LogL10toY(logl10);
    (void)result5;
    
    // Test LogL10fromY
    double y2 = readDoubleFromData(Data, Size, &offset);
    int em3 = readIntFromData(Data, Size, &offset);
    int result6 = LogL10fromY(y2, em3);
    (void)result6;
    
    return 0;
}