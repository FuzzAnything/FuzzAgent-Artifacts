// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

/* Helper to allocate memory for TIFFYCbCrToRGB as per libtiff requirements */
static TIFFYCbCrToRGB *allocate_ycbcr_struct(void) {
    size_t base_size = sizeof(TIFFYCbCrToRGB);
    size_t align = sizeof(long);
    size_t padded_size = (base_size + align - 1) & ~(align - 1);
    
    size_t total_size = padded_size +
        4 * 256 * sizeof(TIFFRGBValue) +
        2 * 256 * sizeof(int) +
        3 * 256 * sizeof(int32_t);

    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(total_size);
    if (ycbcr == NULL) {
        return NULL;
    }
    memset(ycbcr, 0, total_size);
    return ycbcr;
}

/* Helper to allocate memory for TIFFCIELabToRGB and TIFFDisplay */
static TIFFCIELabToRGB *allocate_cielab_struct(TIFFDisplay **display_out) {
    size_t display_size = sizeof(TIFFDisplay) + 1024;
    size_t cielab_size = sizeof(TIFFCIELabToRGB) + 1024;
    
    void *mem = _TIFFmalloc(display_size + cielab_size);
    if (mem == NULL) {
        return NULL;
    }
    memset(mem, 0, display_size + cielab_size);
    
    TIFFDisplay *display = (TIFFDisplay *)mem;
    TIFFCIELabToRGB *cielab = (TIFFCIELabToRGB *)((char *)mem + display_size);
    
    *display_out = display;
    return cielab;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    /* --- Test 1: TIFFYCbCrToRGBInit and TIFFYCbCrtoRGB --- */
    TIFFYCbCrToRGB *ycbcr = allocate_ycbcr_struct();
    if (ycbcr != NULL) {
        /* refBlackWhite array requires 6 floats: [Ymin, Ymax, Cbmin, Cbmax, Crmin, Crmax] */
        float refBlackWhite[6] = {0.0f, 255.0f, 0.0f, 255.0f, 0.0f, 255.0f};
        float luma_coeffs[3] = {0.299f, 0.587f, 0.114f};
        
        /* Use input data to perturb coefficients slightly for fuzzing */
        if (Size > 2) {
            luma_coeffs[0] += ((float)Data[0] / 255.0f) - 0.5f;
            luma_coeffs[1] += ((float)Data[1] / 255.0f) - 0.5f;
            luma_coeffs[2] += ((float)Data[2] / 255.0f) - 0.5f;
        }
        
        /* Pass the 6-element array instead of 3 */
        if (TIFFYCbCrToRGBInit(ycbcr, luma_coeffs, refBlackWhite) != 0) {
            uint32_t Y = (Size > 3) ? (Data[3] & 0xFF) : 0;
            int32_t Cb = (Size > 4) ? (int32_t)((Data[4] << 24) >> 24) : 0;
            int32_t Cr = (Size > 5) ? (int32_t)((Data[5] << 24) >> 24) : 0;
            
            uint32_t R, G, B;
            TIFFYCbCrtoRGB(ycbcr, Y, Cb, Cr, &R, &G, &B);
        }
        _TIFFfree(ycbcr);
    }

    /* --- Test 2: LogLuv24toXYZ --- */
    if (Size >= 4) {
        uint32_t logluv_pixel = 0;
        if (Size > 9) {
            logluv_pixel = Data[6] | (Data[7] << 8) | (Data[8] << 16) | (Data[9] << 24);
        }
        float xyz_out[3];
        LogLuv24toXYZ(logluv_pixel, xyz_out);
    }

    /* --- Test 3: TIFFCIELabToRGBInit, TIFFCIELabToXYZ, TIFFXYZToRGB --- */
    TIFFDisplay *display = NULL;
    TIFFCIELabToRGB *cielab = allocate_cielab_struct(&display);
    if (cielab != NULL && display != NULL) {
        float white_point[3] = {0.95047f, 1.0f, 1.08883f};
        
        /* Perturb white point with fuzz data */
        if (Size >= 12) {
            white_point[0] += ((float)Data[10] / 255.0f) - 0.5f;
            white_point[1] += ((float)Data[11] / 255.0f) - 0.5f;
        }
        
        if (TIFFCIELabToRGBInit(cielab, display, white_point) != 0) {
            /* Generate Lab values from fuzz input */
            uint32_t L = (Size > 12) ? (Data[12] % 101) : 50;
            int32_t a = (Size > 14) ? (int32_t)((Data[14] << 24) >> 24) - 64 : 0;
            int32_t b = (Size > 15) ? (int32_t)((Data[15] << 24) >> 24) - 64 : 0;
            
            float X, Y, Z;
            TIFFCIELabToXYZ(cielab, L, a, b, &X, &Y, &Z);
            
            uint32_t r_out, g_out, b_out;
            TIFFXYZToRGB(cielab, X, Y, Z, &r_out, &g_out, &b_out);
        }
        
        _TIFFfree(display); /* Free the combined block */
    }

    return 0;
}