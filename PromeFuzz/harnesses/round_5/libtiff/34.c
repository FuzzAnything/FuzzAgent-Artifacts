// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// uv_encode at tif_luv.c:967:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "libtiff/tiffio.h"

static double readDouble(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(double) > size) {
        return 0.0;
    }
    double value;
    memcpy(&value, data + *offset, sizeof(double));
    *offset += sizeof(double);
    return value;
}

static int readInt(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int value;
    memcpy(&value, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return value;
}

static uint32_t readUint32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint32_t) > size) {
        return 0;
    }
    uint32_t value;
    memcpy(&value, data + *offset, sizeof(uint32_t));
    *offset += sizeof(uint32_t);
    return value;
}

static float readFloat(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(float) > size) {
        return 0.0f;
    }
    float value;
    memcpy(&value, data + *offset, sizeof(float));
    *offset += sizeof(float);
    return value;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    size_t offset = 0;
    
    // Test LogL16fromY
    double y1 = readDouble(Data, Size, &offset);
    int em1 = readInt(Data, Size, &offset);
    int result1 = LogL16fromY(y1, em1);
    (void)result1; // Suppress unused variable warning
    
    // Test LogLuv24toXYZ
    uint32_t pixel24 = readUint32(Data, Size, &offset);
    float xyz1[3];
    for (int i = 0; i < 3; i++) {
        xyz1[i] = readFloat(Data, Size, &offset);
    }
    LogLuv24toXYZ(pixel24, xyz1);
    
    // Test LogLuv24fromXYZ
    float xyz2[3];
    for (int i = 0; i < 3; i++) {
        xyz2[i] = readFloat(Data, Size, &offset);
    }
    int em2 = readInt(Data, Size, &offset);
    uint32_t result2 = LogLuv24fromXYZ(xyz2, em2);
    (void)result2; // Suppress unused variable warning
    
    // Test uv_encode
    double u = readDouble(Data, Size, &offset);
    double v = readDouble(Data, Size, &offset);
    int em3 = readInt(Data, Size, &offset);
    int result3 = uv_encode(u, v, em3);
    (void)result3; // Suppress unused variable warning
    
    // Test LogLuv32toXYZ
    uint32_t pixel32 = readUint32(Data, Size, &offset);
    float xyz3[3];
    for (int i = 0; i < 3; i++) {
        xyz3[i] = readFloat(Data, Size, &offset);
    }
    LogLuv32toXYZ(pixel32, xyz3);
    
    // Test LogL10fromY
    double y2 = readDouble(Data, Size, &offset);
    int em4 = readInt(Data, Size, &offset);
    int result4 = LogL10fromY(y2, em4);
    (void)result4; // Suppress unused variable warning
    
    return 0;
}