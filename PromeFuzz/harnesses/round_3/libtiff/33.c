// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void init_display_params(TIFFDisplay *display) {
    if (!display) return;
    
    display->d_mat[0][0] = 0.412453f;
    display->d_mat[0][1] = 0.357580f;
    display->d_mat[0][2] = 0.180423f;
    display->d_mat[1][0] = 0.212671f;
    display->d_mat[1][1] = 0.715160f;
    display->d_mat[1][2] = 0.072169f;
    display->d_mat[2][0] = 0.019334f;
    display->d_mat[2][1] = 0.119193f;
    display->d_mat[2][2] = 0.950227f;
    
    display->d_YCR = 0.0f;
    display->d_YCG = 0.0f;
    display->d_YCB = 0.0f;
    
    display->d_Vrwr = 0.0f;
    display->d_Vrwg = 0.0f;
    display->d_Vrwb = 0.0f;
    
    display->d_Y0R = 0.0f;
    display->d_Y0G = 0.0f;
    display->d_Y0B = 0.0f;
    
    display->d_gammaR = 2.2f;
    display->d_gammaG = 2.2f;
    display->d_gammaB = 2.2f;
}

static void init_ycbcr_params(float *luma, float *refBlackWhite) {
    if (!luma || !refBlackWhite) return;
    
    // Luma coefficients
    luma[0] = 0.299f;
    luma[1] = 0.587f;
    luma[2] = 0.114f;
    
    // Reference black and white values (6 floats: Y, Cb, Cr each have black and white)
    refBlackWhite[0] = 0.0f;    // Y black
    refBlackWhite[1] = 255.0f;  // Y white
    refBlackWhite[2] = 128.0f;  // Cb black
    refBlackWhite[3] = 255.0f;  // Cb white
    refBlackWhite[4] = 128.0f;  // Cr black
    refBlackWhite[5] = 255.0f;  // Cr white
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size < 24) return 0;
    
    // Initialize structures
    TIFFCIELabToRGB cielab;
    TIFFDisplay display;
    TIFFYCbCrToRGB *ycbcr = NULL;
    
    // Initialize display parameters
    init_display_params(&display);
    
    // Initialize white point (3 floats)
    float whitePoint[3];
    memcpy(whitePoint, Data, sizeof(float) * 3);
    Data += sizeof(float) * 3;
    Size -= sizeof(float) * 3;
    
    // Call TIFFCIELabToRGBInit
    TIFFCIELabToRGBInit(&cielab, &display, whitePoint);
    
    // Prepare for LogLuv24fromXYZ
    float xyz[3];
    if (Size >= sizeof(float) * 3 + sizeof(int)) {
        memcpy(xyz, Data, sizeof(float) * 3);
        Data += sizeof(float) * 3;
        Size -= sizeof(float) * 3;
        
        int flag = *(const int*)Data;
        Data += sizeof(int);
        Size -= sizeof(int);
        
        // Call LogLuv24fromXYZ
        uint32_t logluv = LogLuv24fromXYZ(xyz, flag);
        (void)logluv;
    }
    
    // Prepare for TIFFXYZToRGB
    if (Size >= sizeof(float) * 3) {
        float X = *(const float*)Data;
        Data += sizeof(float);
        float Y = *(const float*)Data;
        Data += sizeof(float);
        float Z = *(const float*)Data;
        Data += sizeof(float);
        Size -= sizeof(float) * 3;
        
        uint32_t R, G, B;
        TIFFXYZToRGB(&cielab, X, Y, Z, &R, &G, &B);
    }
    
    // Allocate buffer for TIFFYCbCrToRGBInit
    // Calculate buffer size without TIFFroundup_32 macro
    size_t roundup_size = (sizeof(TIFFYCbCrToRGB) + sizeof(long) - 1) / sizeof(long) * sizeof(long);
    size_t buffer_size = roundup_size +
                         4 * 256 * sizeof(TIFFRGBValue) +
                         2 * 256 * sizeof(int) +
                         3 * 256 * sizeof(int32_t);
    
    ycbcr = (TIFFYCbCrToRGB *)malloc(buffer_size);
    if (ycbcr) {
        float luma[3];
        float refBlackWhite[6];  // Changed from ref[2] to refBlackWhite[6]
        
        init_ycbcr_params(luma, refBlackWhite);
        
        // Call TIFFYCbCrToRGBInit
        int ret = TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);
        (void)ret;
    }
    
    // Prepare for TIFFCIELabToXYZ
    if (Size >= sizeof(uint32_t) + 2 * sizeof(int32_t)) {
        uint32_t L = *(const uint32_t*)Data;
        Data += sizeof(uint32_t);
        int32_t a = *(const int32_t*)Data;
        Data += sizeof(int32_t);
        int32_t b = *(const int32_t*)Data;
        Data += sizeof(int32_t);
        Size -= sizeof(uint32_t) + 2 * sizeof(int32_t);
        
        float X, Y, Z;
        TIFFCIELabToXYZ(&cielab, L, a, b, &X, &Y, &Z);
    }
    
    // Prepare for XYZtoRGB24
    if (Size >= sizeof(float) * 3) {
        float xyz2[3];
        memcpy(xyz2, Data, sizeof(float) * 3);
        Data += sizeof(float) * 3;
        Size -= sizeof(float) * 3;
        
        uint8_t rgb[3];
        XYZtoRGB24(xyz2, rgb);
    }
    
    // Cleanup
    if (ycbcr) {
        free(ycbcr);
    }
    
    return 0;
}