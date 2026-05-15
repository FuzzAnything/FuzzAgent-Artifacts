// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

static void init_cielab_to_rgb(TIFFCIELabToRGB *cr) {
    // Initialize with reasonable default values
    cr->range = 0.5f;
    cr->rstep = 0.01f;
    cr->gstep = 0.01f;
    cr->bstep = 0.01f;
    cr->X0 = 0.0f;
    cr->Y0 = 0.0f;
    cr->Z0 = 0.0f;
    // Don't set display field as it has a specific type
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for operations
    if (Size < 16) return 0;
    
    // Initialize variables
    uint32_t logluv_val;
    float xyz[3];
    float xyz2[3];
    uint32_t rgb_r, rgb_g, rgb_b;
    uint8_t rgb24[3];
    uint32_t result_logluv;
    TIFFCIELabToRGB cielab;
    
    // Initialize CIELab structure
    init_cielab_to_rgb(&cielab);
    
    // Use first 4 bytes for LogLuv values
    memcpy(&logluv_val, Data, sizeof(uint32_t));
    
    // Test LogLuv24toXYZ
    LogLuv24toXYZ(logluv_val, xyz);
    
    // Test LogLuv32toXYZ
    LogLuv32toXYZ(logluv_val, xyz2);
    
    // Use next 12 bytes for XYZ values (3 floats)
    if (Size >= 16) {
        size_t offset = sizeof(uint32_t);
        memcpy(xyz, Data + offset, sizeof(float) * 3);
        
        // Test TIFFXYZToRGB
        TIFFXYZToRGB(&cielab, xyz[0], xyz[1], xyz[2], &rgb_r, &rgb_g, &rgb_b);
        
        // Test XYZtoRGB24
        XYZtoRGB24(xyz, rgb24);
        
        // Test LogLuv24fromXYZ
        result_logluv = LogLuv24fromXYZ(xyz, 0);
        
        // Test LogLuv32fromXYZ
        result_logluv = LogLuv32fromXYZ(xyz, 0);
    }
    
    return 0;
}