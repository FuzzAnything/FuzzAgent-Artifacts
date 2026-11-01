// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv32fromXYZ at tif_luv.c:1206:5 in tiffio.h
// LogLuv32toXYZ at tif_luv.c:1180:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tiffio.h"

static void fuzz_LogLuv24fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return; // Need 3 floats (12 bytes) + 1 int (4 bytes) = 16 bytes minimum

    float xyz[3];
    int mode;
    
    // Parse XYZ values (3 floats = 12 bytes)
    memcpy(xyz, Data, 12);
    
    // Parse mode (1 int = 4 bytes)
    memcpy(&mode, Data + 12, sizeof(int));
    
    uint32_t result = LogLuv24fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_XYZtoRGB24(const uint8_t *Data, size_t Size) {
    if (Size < 12) return; // Need 3 floats = 12 bytes
    
    float xyz[3];
    uint8_t rgb[3];
    
    memcpy(xyz, Data, 12);
    
    XYZtoRGB24(xyz, rgb);
    (void)rgb;
}

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return; // Need 1 uint32_t = 4 bytes
    
    uint32_t pixel;
    float xyz[3];
    
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    LogLuv24toXYZ(pixel, xyz);
    (void)xyz;
}

static void fuzz_LogLuv32fromXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 16) return; // Need 3 floats (12 bytes) + 1 int (4 bytes) = 16 bytes minimum

    float xyz[3];
    int mode;
    
    // Parse XYZ values
    memcpy(xyz, Data, 12);
    
    // Parse mode
    memcpy(&mode, Data + 12, sizeof(int));
    
    uint32_t result = LogLuv32fromXYZ(xyz, mode);
    (void)result;
}

static void fuzz_LogLuv32toXYZ(const uint8_t *Data, size_t Size) {
    if (Size < 4) return; // Need 1 uint32_t = 4 bytes
    
    uint32_t pixel;
    float xyz[3];
    
    memcpy(&pixel, Data, sizeof(uint32_t));
    
    LogLuv32toXYZ(pixel, xyz);
    (void)xyz;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use the first byte to select which function to fuzz
    int choice = Data[0] % 5; // Only 5 safe functions
    
    switch (choice) {
        case 0:
            fuzz_LogLuv24fromXYZ(Data, Size);
            break;
        case 1:
            fuzz_XYZtoRGB24(Data, Size);
            break;
        case 2:
            fuzz_LogLuv24toXYZ(Data, Size);
            break;
        case 3:
            fuzz_LogLuv32fromXYZ(Data, Size);
            break;
        case 4:
            fuzz_LogLuv32toXYZ(Data, Size);
            break;
    }
    
    return 0;
}