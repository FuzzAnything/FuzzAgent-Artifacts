// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for various operations
    if (Size < 20) return 0;
    
    // Prepare dummy file for any potential file operations
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Extract test values from fuzzer input safely
    uint32_t logluv24_val, logluv32_val;
    float xyz_array[3];
    
    // Use memcpy to avoid alignment issues and ensure we don't read beyond buffer
    memcpy(&logluv24_val, Data, sizeof(uint32_t));
    memcpy(&logluv32_val, Data + sizeof(uint32_t), sizeof(uint32_t));
    
    // Ensure we have enough data for the 3 floats
    if (Size >= (sizeof(uint32_t) * 2 + sizeof(float) * 3)) {
        memcpy(xyz_array, Data + (sizeof(uint32_t) * 2), sizeof(float) * 3);
    } else {
        // If not enough data, use zeros
        xyz_array[0] = xyz_array[1] = xyz_array[2] = 0.0f;
    }
    
    // Test LogLuv24toXYZ
    float xyz_out1[3];
    LogLuv24toXYZ(logluv24_val, xyz_out1);
    
    // Test LogLuv32toXYZ
    float xyz_out2[3];
    LogLuv32toXYZ(logluv32_val, xyz_out2);
    
    // Test XYZtoRGB24
    uint8_t rgb24_out[3];
    XYZtoRGB24(xyz_array, rgb24_out);
    
    // Test LogLuv24fromXYZ
    uint32_t logluv24_result = LogLuv24fromXYZ(xyz_array, 0);
    logluv24_result = LogLuv24fromXYZ(xyz_array, 1);
    
    // Test LogLuv32fromXYZ
    uint32_t logluv32_result = LogLuv32fromXYZ(xyz_array, 0);
    logluv32_result = LogLuv32fromXYZ(xyz_array, 1);
    
    // Test TIFFXYZToRGB
    TIFFCIELabToRGB cielab;
    memset(&cielab, 0, sizeof(cielab));
    
    uint32_t r, g, b;
    TIFFXYZToRGB(&cielab, xyz_array[0], xyz_array[1], xyz_array[2], &r, &g, &b);
    TIFFXYZToRGB(&cielab, xyz_out1[0], xyz_out1[1], xyz_out1[2], &r, &g, &b);
    TIFFXYZToRGB(&cielab, xyz_out2[0], xyz_out2[1], xyz_out2[2], &r, &g, &b);
    
    // Test edge cases with extreme values
    float extreme_xyz[3] = {0.0f, 0.0f, 0.0f};
    LogLuv24fromXYZ(extreme_xyz, 0);
    LogLuv32fromXYZ(extreme_xyz, 0);
    XYZtoRGB24(extreme_xyz, rgb24_out);
    
    extreme_xyz[0] = 1.0f; extreme_xyz[1] = 1.0f; extreme_xyz[2] = 1.0f;
    LogLuv24fromXYZ(extreme_xyz, 1);
    LogLuv32fromXYZ(extreme_xyz, 1);
    XYZtoRGB24(extreme_xyz, rgb24_out);
    
    extreme_xyz[0] = -1.0f; extreme_xyz[1] = -1.0f; extreme_xyz[2] = -1.0f;
    LogLuv24fromXYZ(extreme_xyz, 0);
    LogLuv32fromXYZ(extreme_xyz, 0);
    XYZtoRGB24(extreme_xyz, rgb24_out);
    
    extreme_xyz[0] = 1000.0f; extreme_xyz[1] = 1000.0f; extreme_xyz[2] = 1000.0f;
    LogLuv24fromXYZ(extreme_xyz, 1);
    LogLuv32fromXYZ(extreme_xyz, 1);
    XYZtoRGB24(extreme_xyz, rgb24_out);
    
    TIFFXYZToRGB(&cielab, extreme_xyz[0], extreme_xyz[1], extreme_xyz[2], &r, &g, &b);
    
    // Clean up
    remove(dummy_file);
    
    return 0;
}