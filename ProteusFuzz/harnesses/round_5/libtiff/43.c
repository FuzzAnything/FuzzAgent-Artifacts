// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

static void fuzzer_initialize(void) {
    // No specific global initialization required for these standalone functions
}

static void fuzzer_cleanup(void) {
    // No specific global cleanup required
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    fuzzer_initialize();

    // Parse input data
    const float *xyz_input = (const float *)Data;
    const float x = xyz_input[0];
    const float y = xyz_input[1];
    const float z = xyz_input[2];
    
    // Ensure we have enough data for additional parameters if needed
    int exponent = 0;
    int mode = 0;
    uint32_t packed_pixel = 0;
    
    if (Size >= 16) {
        exponent = *(const int *)(Data + 12);
        mode = *(const int *)(Data + 16);
    }
    
    if (Size >= 20) {
        packed_pixel = *(const uint32_t *)(Data + 20);
    }

    // Prepare buffers
    float xyz_out[3];
    uint8_t rgb_out[3];
    uint32_t r_out, g_out, b_out;
    uint32_t result_32;
    uint32_t result_24;

    // 1. Fuzz LogLuv32fromXYZ
    // Converts XYZ to 32-bit LogLuv
    result_32 = LogLuv32fromXYZ((float[]){x, y, z}, exponent);
    
    // 2. Fuzz LogLuv24fromXYZ
    // Converts XYZ to 24-bit LogLuv
    result_24 = LogLuv24fromXYZ((float[]){x, y, z}, mode);

    // 3. Fuzz LogLuv24toXYZ
    // Converts 24-bit LogLuv back to XYZ
    LogLuv24toXYZ(result_24, xyz_out);

    // 4. Fuzz LogLuv32toXYZ
    // Converts 32-bit LogLuv back to XYZ
    LogLuv32toXYZ(result_32, xyz_out);

    // 5. Fuzz XYZtoRGB24
    // Converts XYZ to 8-bit RGB
    XYZtoRGB24((float[]){x, y, z}, rgb_out);

    // 6. Fuzz TIFFXYZToRGB
    // The original driver passed NULL, causing a crash because the function
    // dereferences the pointer immediately. We skip this call or require a
    // valid TIFFCIELabToRGB structure. Since constructing a valid one is
    // complex and requires file I/O, we skip this specific test case to
    // prevent the null pointer dereference crash.
    // TIFFXYZToRGB(NULL, x, y, z, &r_out, &g_out, &b_out); // Removed

    // Additional diverse states:
    // Test with zero luminance
    float zero_xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv32fromXYZ(zero_xyz, exponent);
    LogLuv24fromXYZ(zero_xyz, mode);
    LogLuv24toXYZ(0, xyz_out);
    LogLuv32toXYZ(0, xyz_out);
    XYZtoRGB24(zero_xyz, rgb_out);

    // Test with extreme values (NaN/Inf handling if applicable)
    float extreme_xyz[3] = {INFINITY, -INFINITY, NAN};
    LogLuv32fromXYZ(extreme_xyz, exponent);
    LogLuv24fromXYZ(extreme_xyz, mode);
    XYZtoRGB24(extreme_xyz, rgb_out);

    fuzzer_cleanup();
    return 0;
}