// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <tiffio.h>

static float extract_float(const uint8_t *data, size_t size) {
    if (size < 4) return 0.0f;
    float val;
    memcpy(&val, data, 4);
    return val;
}

static int extract_int(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    int val;
    memcpy(&val, data, 4);
    return val;
}

static double extract_double(const uint8_t *data, size_t size) {
    if (size < 8) return 0.0;
    double val;
    memcpy(&val, data, 8);
    return val;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    float xyz[3];
    xyz[0] = extract_float(Data, Size);
    xyz[1] = extract_float(Data + 4, Size - 4);
    xyz[2] = extract_float(Data + 8, Size - 8);

    int mode = (Size > 12) ? extract_int(Data + 12, Size - 12) : 0;
    int em = (Size > 16) ? extract_int(Data + 16, Size - 16) : 0;

    // Test LogLuv32fromXYZ
    uint32_t result32 = LogLuv32fromXYZ(xyz, mode);
    (void)result32;

    // Test LogLuv24fromXYZ
    uint32_t result24 = LogLuv24fromXYZ(xyz, mode);
    (void)result24;

    // Test LogL16fromY with Y from xyz[1]
    double y_val = xyz[1];
    int result16 = LogL16fromY(y_val, em);
    (void)result16;

    // Test LogL10toY with a value derived from input
    int p10 = (Size > 20) ? extract_int(Data + 20, Size - 20) & 1023 : 0;
    double y_from_10 = LogL10toY(p10);
    (void)y_from_10;

    // Test LogL16toY with result16
    double y_from_16 = LogL16toY(result16);
    (void)y_from_16;

    // Test LogL10fromY
    int result10 = LogL10fromY(y_val, em);
    (void)result10;

    return 0;
}