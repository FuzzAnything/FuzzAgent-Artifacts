// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tiffio.h"

static int get_int_from_data(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) {
        return 0;
    }
    int val = 0;
    memcpy(&val, Data + *offset, sizeof(int));
    *offset += 4;
    return val;
}

static double get_double_from_data(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 8 > Size) {
        return 0.0;
    }
    double val = 0.0;
    memcpy(&val, Data + *offset, sizeof(double));
    *offset += 8;
    return val;
}

static float get_float_from_data(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) {
        return 0.0f;
    }
    float val = 0.0f;
    memcpy(&val, Data + *offset, sizeof(float));
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    size_t offset = 0;
    int mode = get_int_from_data(Data, Size, &offset);
    
    // Ensure mode is within reasonable bounds for error handling
    if (mode < 0 || mode > 10) {
        mode = mode % 11;
    }

    // Test LogL10fromY and LogL10toY
    if (offset + 8 <= Size) {
        double y_val = get_double_from_data(Data, Size, &offset);
        int logl10_val = LogL10fromY(y_val, mode);
        double y_recovered = LogL10toY(logl10_val);
        (void)y_recovered;
    }

    // Test LogL16fromY and LogL16toY
    if (offset + 8 <= Size) {
        double y_val = get_double_from_data(Data, Size, &offset);
        int logl16_val = LogL16fromY(y_val, mode);
        double y_recovered = LogL16toY(logl16_val);
        (void)y_recovered;
    }

    // Test LogLuv32fromXYZ
    if (offset + 12 <= Size) {
        float xyz[3];
        xyz[0] = get_float_from_data(Data, Size, &offset);
        xyz[1] = get_float_from_data(Data, Size, &offset);
        xyz[2] = get_float_from_data(Data, Size, &offset);
        uint32_t logluv32 = LogLuv32fromXYZ(xyz, mode);
        (void)logluv32;
    }

    // Test LogLuv24fromXYZ
    if (offset + 12 <= Size) {
        float xyz[3];
        xyz[0] = get_float_from_data(Data, Size, &offset);
        xyz[1] = get_float_from_data(Data, Size, &offset);
        xyz[2] = get_float_from_data(Data, Size, &offset);
        uint32_t logluv24 = LogLuv24fromXYZ(xyz, mode);
        (void)logluv24;
    }

    return 0;
}