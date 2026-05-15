// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Include the required header file with full path */
#include "libtiff/tiffio.h"

/* Helper function to allocate and initialize TIFFYCbCrToRGB buffer as described */
static TIFFYCbCrToRGB* allocate_ycbcr_buffer(void) {
    /* Calculate buffer size as per the documentation */
    size_t base_size = sizeof(TIFFYCbCrToRGB);
    /* Align to long boundary */
    size_t aligned_size = (base_size + sizeof(long) - 1) & ~(sizeof(long) - 1);
    size_t total_size = aligned_size +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);
    
    return (TIFFYCbCrToRGB*)_TIFFmalloc(total_size);
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    /* Ensure we have minimum data for operations */
    if (Size < 50) return 0;
    
    /* Initialize structures */
    TIFFCIELabToRGB cielab_rgb;
    TIFFDisplay display;
    TIFFYCbCrToRGB *ycbcr_rgb = NULL;
    
    /* Initialize display parameters with fuzzer data */
    memset(&display, 0, sizeof(display));
    display.d_mat[0][0] = *((float*)Data + 0);
    display.d_mat[0][1] = *((float*)Data + 1);
    display.d_mat[0][2] = *((float*)Data + 2);
    display.d_mat[1][0] = *((float*)Data + 3);
    display.d_mat[1][1] = *((float*)Data + 4);
    display.d_mat[1][2] = *((float*)Data + 5);
    display.d_mat[2][0] = *((float*)Data + 6);
    display.d_mat[2][1] = *((float*)Data + 7);
    display.d_mat[2][2] = *((float*)Data + 8);
    
    /* Initialize white point */
    float whitePoint[3];
    whitePoint[0] = *((float*)Data + 9);
    whitePoint[1] = *((float*)Data + 10);
    whitePoint[2] = *((float*)Data + 11);
    
    /* Initialize TIFFCIELabToRGB */
    if (TIFFCIELabToRGBInit(&cielab_rgb, &display, whitePoint) != 0) {
        /* Initialization succeeded, test conversion functions */
        
        /* Test TIFFXYZToRGB */
        float X = *((float*)Data + 12);
        float Y = *((float*)Data + 13);
        float Z = *((float*)Data + 14);
        uint32_t R, G, B;
        TIFFXYZToRGB(&cielab_rgb, X, Y, Z, &R, &G, &B);
        
        /* Test XYZtoRGB24 */
        float xyz[3];
        xyz[0] = *((float*)Data + 15);
        xyz[1] = *((float*)Data + 16);
        xyz[2] = *((float*)Data + 17);
        uint8_t rgb24[3];
        XYZtoRGB24(xyz, rgb24);
        
        /* Test TIFFCIELabToXYZ */
        uint32_t L = Data[18] & 0xFF;
        int32_t a = (int32_t)((Data[19] - 128) * 2);
        int32_t b = (int32_t)((Data[20] - 128) * 2);
        float outX, outY, outZ;
        TIFFCIELabToXYZ(&cielab_rgb, L, a, b, &outX, &outY, &outZ);
    }
    
    /* Test YCbCr functions */
    ycbcr_rgb = allocate_ycbcr_buffer();
    if (ycbcr_rgb != NULL) {
        /* Initialize luma and refBlackWhite parameters */
        float luma[3];
        float refBlackWhite[6];
        
        /* Use fuzzer data for initialization parameters */
        for (int i = 0; i < 3 && (21 + i) * sizeof(float) < Size; i++) {
            luma[i] = *((float*)Data + 21 + i);
        }
        for (int i = 0; i < 6 && (24 + i) * sizeof(float) < Size; i++) {
            refBlackWhite[i] = *((float*)Data + 24 + i);
        }
        
        /* Initialize YCbCr to RGB conversion */
        if (TIFFYCbCrToRGBInit(ycbcr_rgb, luma, refBlackWhite) != 0) {
            /* Test TIFFYCbCrtoRGB */
            uint32_t Y = Data[30] & 0xFF;
            int32_t Cb = (int32_t)(Data[31] - 128);
            int32_t Cr = (int32_t)(Data[32] - 128);
            uint32_t outR, outG, outB;
            TIFFYCbCrtoRGB(ycbcr_rgb, Y, Cb, Cr, &outR, &outG, &outB);
        }
        
        /* Clean up YCbCr buffer */
        _TIFFfree(ycbcr_rgb);
    }
    
    return 0;
}