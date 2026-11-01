// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogL16fromY at tif_luv.c:816:5 in tiffio.h
// LogL10fromY at tif_luv.c:894:5 in tiffio.h
// LogL10toY at tif_luv.c:883:5 in tiffio.h
// LogL16toY at tif_luv.c:801:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <tiffio.h>

static double fuzz_double(const uint8_t *data, size_t size) {
    if (size < 8) {
        return 0.0;
    }
    double val;
    memcpy(&val, data, 8);
    // Avoid NaN and Inf for stability, though the library should handle them
    if (!isfinite(val)) {
        return 0.0;
    }
    return val;
}

static int fuzz_int(const uint8_t *data, size_t size) {
    if (size < 4) {
        return 0;
    }
    int val;
    memcpy(&val, data, 4);
    return val;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Prepare XYZ array (3 floats)
    float xyz[3];
    xyz[0] = fuzz_double(Data, Size);
    xyz[1] = fuzz_double(Data + 8, Size > 16 ? Size - 8 : 0);
    xyz[2] = fuzz_double(Data + 16, Size > 24 ? Size - 16 : 0);

    // Prepare Y value (double)
    double y_val = fuzz_double(Data, Size);

    // Prepare integer flags (em, mode)
    int em = fuzz_int(Data, Size) & 0xFF; // Use lower byte for error mode
    int mode = fuzz_int(Data + 4, Size > 4 ? Size - 4 : 0) & 0xFF;

    // Fuzz LogLuv32fromXYZ
    uint32_t res32 = LogLuv32fromXYZ(xyz, em);
    (void)res32;

    // Fuzz LogLuv24fromXYZ
    uint32_t res24 = LogLuv24fromXYZ(xyz, mode);
    (void)res24;

    // Fuzz LogL16fromY
    int res16 = LogL16fromY(y_val, em);
    (void)res16;

    // Fuzz LogL10fromY
    int res10 = LogL10fromY(y_val, em);
    (void)res10;

    // Fuzz LogL10toY (input range 0-1023)
    int input10 = (res10 < 0) ? 0 : (res10 > 1023 ? 1023 : res10);
    double out10 = LogL10toY(input10);
    (void)out10;

    // Fuzz LogL16toY (input range -32768 to 32767)
    int input16 = res16;
    double out16 = LogL16toY(input16);
    (void)out16;

    return 0;
}