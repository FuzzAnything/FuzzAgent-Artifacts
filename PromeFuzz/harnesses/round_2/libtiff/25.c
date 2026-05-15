// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "libtiff/tiffio.h"

static void initialize_xyz_array(float *xyz, const uint8_t *data, size_t size, size_t offset) {
    if (offset + 12 <= size) {
        memcpy(xyz, data + offset, 12);
    } else {
        xyz[0] = 0.5f;
        xyz[1] = 0.5f;
        xyz[2] = 0.5f;
    }
}

static uint32_t extract_uint32(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 4 <= size) {
        uint32_t value;
        memcpy(&value, data + offset, 4);
        return value;
    }
    return 0xFFFFFFFF;
}

static int extract_int(const uint8_t *data, size_t size, size_t offset) {
    if (offset + 4 <= size) {
        int value;
        memcpy(&value, data + offset, 4);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    float xyz[3];
    uint8_t rgb[3];
    uint32_t r, g, b;
    uint32_t logluv_pixel;
    float xyz_out[3];
    
    // Test LogLuv24fromXYZ
    initialize_xyz_array(xyz, Data, Size, 0);
    int white = extract_int(Data, Size, 12);
    LogLuv24fromXYZ(xyz, white);
    
    // Test LogLuv32fromXYZ
    initialize_xyz_array(xyz, Data, Size, 16);
    white = extract_int(Data, Size, 28);
    LogLuv32fromXYZ(xyz, white);
    
    // Test XYZtoRGB24
    initialize_xyz_array(xyz, Data, Size, 32);
    XYZtoRGB24(xyz, rgb);
    
    // Test TIFFXYZToRGB
    TIFFCIELabToRGB cielab;
    if (Size >= 44 + sizeof(TIFFCIELabToRGB)) {
        memcpy(&cielab, Data + 44, sizeof(TIFFCIELabToRGB));
    } else {
        memset(&cielab, 0, sizeof(TIFFCIELabToRGB));
    }
    
    float x = 0.5f, y = 0.5f, z = 0.5f;
    if (Size >= 44 + sizeof(TIFFCIELabToRGB) + 12) {
        memcpy(&x, Data + 44 + sizeof(TIFFCIELabToRGB), 4);
        memcpy(&y, Data + 44 + sizeof(TIFFCIELabToRGB) + 4, 4);
        memcpy(&z, Data + 44 + sizeof(TIFFCIELabToRGB) + 8, 4);
    }
    TIFFXYZToRGB(&cielab, x, y, z, &r, &g, &b);
    
    // Test LogLuv24toXYZ
    logluv_pixel = extract_uint32(Data, Size, 0);
    LogLuv24toXYZ(logluv_pixel, xyz_out);
    
    // Test LogLuv32toXYZ
    logluv_pixel = extract_uint32(Data, Size, 4);
    LogLuv32toXYZ(logluv_pixel, xyz_out);
    
    return 0;
}