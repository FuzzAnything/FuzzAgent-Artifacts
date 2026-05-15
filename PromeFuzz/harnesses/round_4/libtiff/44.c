// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void init_float_array(float* arr, size_t len, const uint8_t* data, size_t size, size_t* offset) {
    for (size_t i = 0; i < len && *offset + sizeof(float) <= size; i++) {
        memcpy(&arr[i], data + *offset, sizeof(float));
        *offset += sizeof(float);
    }
}

static int extract_int(const uint8_t* data, size_t size, size_t* offset) {
    int value = 0;
    if (*offset + sizeof(int) <= size) {
        memcpy(&value, data + *offset, sizeof(int));
        *offset += sizeof(int);
    }
    return value;
}

static double extract_double(const uint8_t* data, size_t size, size_t* offset) {
    double value = 0.0;
    if (*offset + sizeof(double) <= size) {
        memcpy(&value, data + *offset, sizeof(double));
        *offset += sizeof(double);
    }
    return value;
}

static uint32_t extract_uint32(const uint8_t* data, size_t size, size_t* offset) {
    uint32_t value = 0;
    if (*offset + sizeof(uint32_t) <= size) {
        memcpy(&value, data + *offset, sizeof(uint32_t));
        *offset += sizeof(uint32_t);
    }
    return value;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test LogL10fromY
    double y1 = extract_double(Data, Size, &offset);
    int clamp1 = extract_int(Data, Size, &offset);
    int result1 = LogL10fromY(y1, clamp1);
    (void)result1;
    
    // Test LogL16toY
    int logl16 = extract_int(Data, Size, &offset);
    double result2 = LogL16toY(logl16);
    (void)result2;
    
    // Test LogLuv32fromXYZ
    float xyz[3] = {0.0f, 0.0f, 0.0f};
    init_float_array(xyz, 3, Data, Size, &offset);
    int clamp2 = extract_int(Data, Size, &offset);
    uint32_t result3 = LogLuv32fromXYZ(xyz, clamp2);
    (void)result3;
    
    // Test LogL16fromY
    double y2 = extract_double(Data, Size, &offset);
    int clamp3 = extract_int(Data, Size, &offset);
    int result4 = LogL16fromY(y2, clamp3);
    (void)result4;
    
    // Test LogL10toY
    int logl10 = extract_int(Data, Size, &offset);
    double result5 = LogL10toY(logl10);
    (void)result5;
    
    // Test LogLuv24toXYZ
    uint32_t logluv24 = extract_uint32(Data, Size, &offset);
    float xyz_out[3] = {0.0f, 0.0f, 0.0f};
    LogLuv24toXYZ(logluv24, xyz_out);
    
    return 0;
}