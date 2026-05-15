// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "libtiff/tiffio.h"

static float getFloatFromData(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(float) <= size) {
        float value;
        memcpy(&value, data + *offset, sizeof(float));
        *offset += sizeof(float);
        return value;
    }
    return 0.0f;
}

static uint32_t getUint32FromData(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(uint32_t) <= size) {
        uint32_t value;
        memcpy(&value, data + *offset, sizeof(uint32_t));
        *offset += sizeof(uint32_t);
        return value;
    }
    return 0;
}

static int32_t getInt32FromData(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int32_t) <= size) {
        int32_t value;
        memcpy(&value, data + *offset, sizeof(int32_t));
        *offset += sizeof(int32_t);
        return value;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;

    size_t offset = 0;
    
    // Initialize TIFFCIELabToRGB structure
    TIFFCIELabToRGB *cielabToRGB = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
    if (!cielabToRGB) return 0;
    
    // Initialize TIFFDisplay structure
    TIFFDisplay *display = (TIFFDisplay *)malloc(sizeof(TIFFDisplay));
    if (!display) {
        free(cielabToRGB);
        return 0;
    }
    
    // Populate display structure with fuzzer data
    memset(display, 0, sizeof(TIFFDisplay));
    if (offset + 12 * sizeof(float) <= Size) {
        for (int i = 0; i < 12; i++) {
            ((float *)display)[i] = getFloatFromData(Data, Size, &offset);
        }
    }
    
    // Initialize white point array
    float whitePoint[3];
    for (int i = 0; i < 3; i++) {
        whitePoint[i] = getFloatFromData(Data, Size, &offset);
    }
    
    // Call TIFFCIELabToRGBInit
    TIFFCIELabToRGBInit(cielabToRGB, display, whitePoint);
    
    // Test TIFFXYZToRGB
    float x = getFloatFromData(Data, Size, &offset);
    float y = getFloatFromData(Data, Size, &offset);
    float z = getFloatFromData(Data, Size, &offset);
    uint32_t r_val, g_val, b_val;
    TIFFXYZToRGB(cielabToRGB, x, y, z, &r_val, &g_val, &b_val);
    
    // Test XYZtoRGB24
    float xyz[3];
    for (int i = 0; i < 3; i++) {
        xyz[i] = getFloatFromData(Data, Size, &offset);
    }
    uint8_t rgb24[3];
    XYZtoRGB24(xyz, rgb24);
    
    // Test TIFFCIELabToXYZ
    uint32_t L = getUint32FromData(Data, Size, &offset) % 256;
    int32_t a_val = getInt32FromData(Data, Size, &offset);
    int32_t b_lab = getInt32FromData(Data, Size, &offset);
    float X, Y, Z;
    TIFFCIELabToXYZ(cielabToRGB, L, a_val, b_lab, &X, &Y, &Z);
    
    // Calculate buffer size for TIFFYCbCrToRGB
    size_t bufferSize = (((sizeof(TIFFYCbCrToRGB) + (sizeof(long) - 1)) / sizeof(long)) * sizeof(long)) +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);
    
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(bufferSize);
    if (!ycbcr) {
        free(display);
        free(cielabToRGB);
        return 0;
    }
    
    // Initialize TIFFYCbCrToRGB
    float refBlackWhite[6];
    float ycbcrCoeffs[3];
    
    for (int i = 0; i < 6; i++) {
        refBlackWhite[i] = getFloatFromData(Data, Size, &offset);
    }
    for (int i = 0; i < 3; i++) {
        ycbcrCoeffs[i] = getFloatFromData(Data, Size, &offset);
    }
    
    TIFFYCbCrToRGBInit(ycbcr, ycbcrCoeffs, refBlackWhite);
    
    // Test TIFFYCbCrtoRGB
    uint32_t Y_val = getUint32FromData(Data, Size, &offset) % 256;
    int32_t Cb = getInt32FromData(Data, Size, &offset);
    int32_t Cr = getInt32FromData(Data, Size, &offset);
    uint32_t R, G, B;
    TIFFYCbCrtoRGB(ycbcr, Y_val, Cb, Cr, &R, &G, &B);
    
    // Cleanup
    _TIFFfree(ycbcr);
    free(display);
    free(cielabToRGB);
    
    return 0;
}