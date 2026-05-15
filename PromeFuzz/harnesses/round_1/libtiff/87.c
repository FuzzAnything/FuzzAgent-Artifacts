// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 200) {
        return 0;
    }

    /* Manually implement TIFFroundup_32 functionality */
    #define TIFFroundup_32(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
    
    /* Allocate buffer for TIFFYCbCrToRGB as per documentation */
    size_t ycbcr_buf_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                            4 * 256 * sizeof(TIFFRGBValue) +
                            2 * 256 * sizeof(int) +
                            3 * 256 * sizeof(int32_t);
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(ycbcr_buf_size);
    if (!ycbcr) {
        return 0;
    }

    /* Prepare float arrays for TIFFYCbCrToRGBInit - ensure we have enough data */
    if (Size < 24) {
        _TIFFfree(ycbcr);
        return 0;
    }
    
    float refWhite[3], refBlack[3];
    /* Copy safely without reading beyond Data bounds */
    for (int i = 0; i < 3; i++) {
        float temp_white, temp_black;
        if (Size >= (i+1)*sizeof(float)) {
            memcpy(&temp_white, Data + i*sizeof(float), sizeof(float));
            refWhite[i] = temp_white;
        } else {
            refWhite[i] = 0.0f;
        }
        
        if (Size >= 3*sizeof(float) + (i+1)*sizeof(float)) {
            memcpy(&temp_black, Data + 3*sizeof(float) + i*sizeof(float), sizeof(float));
            refBlack[i] = temp_black;
        } else {
            refBlack[i] = 0.0f;
        }
    }
    
    /* Initialize YCbCr to RGB conversion */
    int init_result = TIFFYCbCrToRGBInit(ycbcr, refWhite, refBlack);
    
    if (init_result == 0) {
        /* Test TIFFYCbCrtoRGB with fuzzed values */
        uint32_t y = 0;
        int32_t cb = 0, cr = 0;
        
        if (Size >= 24 + sizeof(uint32_t)) {
            y = *(uint32_t*)(Data + 24) % 256;
        }
        if (Size >= 28 + sizeof(int32_t)) {
            cb = *(int32_t*)(Data + 28) % 256;
        }
        if (Size >= 32 + sizeof(int32_t)) {
            cr = *(int32_t*)(Data + 32) % 256;
        }
        
        uint32_t r, g, b;
        TIFFYCbCrtoRGB(ycbcr, y, cb, cr, &r, &g, &b);
    }

    /* Test XYZtoRGB24 */
    float xyz[3];
    if (Size >= 36 + 3*sizeof(float)) {
        for (int i = 0; i < 3; i++) {
            memcpy(&xyz[i], Data + 36 + i*sizeof(float), sizeof(float));
        }
    } else {
        for (int i = 0; i < 3; i++) {
            xyz[i] = 0.0f;
        }
    }
    
    uint8_t rgb24[3];
    XYZtoRGB24(xyz, rgb24);

    /* Prepare TIFFCIELabToRGB structure */
    TIFFCIELabToRGB cielab;
    memset(&cielab, 0, sizeof(cielab));
    
    /* Prepare TIFFDisplay structure */
    TIFFDisplay display;
    memset(&display, 0, sizeof(display));
    
    /* Initialize display with some fuzzed values */
    if (Size >= 48 + sizeof(TIFFDisplay)) {
        size_t copy_size = sizeof(TIFFDisplay) < 100 ? sizeof(TIFFDisplay) : 100;
        if (Size >= 48 + copy_size) {
            memcpy(&display, Data + 48, copy_size);
        }
    }
    
    /* Prepare white point for CIELab init */
    float whitePoint[3];
    if (Size >= 148 + 3*sizeof(float)) {
        for (int i = 0; i < 3; i++) {
            memcpy(&whitePoint[i], Data + 148 + i*sizeof(float), sizeof(float));
        }
    } else {
        for (int i = 0; i < 3; i++) {
            whitePoint[i] = 0.0f;
        }
    }
    
    /* Initialize CIELab to RGB conversion */
    int cielab_init_result = TIFFCIELabToRGBInit(&cielab, &display, whitePoint);
    
    if (cielab_init_result == 0) {
        /* Test TIFFCIELabToXYZ */
        uint32_t L = 0;
        int32_t a = 0, b_val = 0;
        
        if (Size >= 160 + sizeof(uint32_t)) {
            L = *(uint32_t*)(Data + 160) % 256;
        }
        if (Size >= 164 + sizeof(int32_t)) {
            a = *(int32_t*)(Data + 164);
        }
        if (Size >= 168 + sizeof(int32_t)) {
            b_val = *(int32_t*)(Data + 168);
        }
        
        float X, Y, Z;
        TIFFCIELabToXYZ(&cielab, L, a, b_val, &X, &Y, &Z);
        
        /* Test TIFFXYZToRGB */
        float x = 0.0f, y_val = 0.0f, z = 0.0f;
        
        if (Size >= 172 + sizeof(float)) {
            memcpy(&x, Data + 172, sizeof(float));
        }
        if (Size >= 176 + sizeof(float)) {
            memcpy(&y_val, Data + 176, sizeof(float));
        }
        if (Size >= 180 + sizeof(float)) {
            memcpy(&z, Data + 180, sizeof(float));
        }
        
        uint32_t rgb_r, rgb_g, rgb_b;
        TIFFXYZToRGB(&cielab, x, y_val, z, &rgb_r, &rgb_g, &rgb_b);
    }

    /* Cleanup */
    _TIFFfree(ycbcr);
    
    return 0;
}