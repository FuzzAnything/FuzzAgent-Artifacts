// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"

/* 
 * TIFFroundup_32 is often a macro or internal function. 
 * If not available in tiffio.h, we define a simple inline version.
 * It rounds 'x' up to the nearest multiple of 'align'.
 */
#ifndef TIFFroundup_32
#define TIFFroundup_32(x, align) (((x) + ((align) - 1)) & ~((align) - 1))
#endif

static void safe_free(void *ptr) {
    if (ptr) {
        _TIFFfree(ptr);
    }
}

static int init_cielab_context(TIFFCIELabToRGB *cielab, const uint8_t *data, size_t size) {
    TIFFDisplay display;
    float *white_point = NULL;
    int ret = 0;

    memset(&display, 0, sizeof(display));
    
    // Attempt to extract white point from fuzz data if available
    if (size >= 12) {
        white_point = (float *)malloc(3 * sizeof(float));
        if (white_point) {
            white_point[0] = ((float)data[0]) / 255.0f;
            white_point[1] = ((float)data[1]) / 255.0f;
            white_point[2] = ((float)data[2]) / 255.0f;
            
            // Pass the white_point array to the init function.
            // The display struct is passed but members might be ignored if white_point is provided.
            ret = TIFFCIELabToRGBInit(cielab, &display, white_point);
        }
    } else {
        // Default D50 white point
        float fallback_wp[3] = {0.95047f, 1.00000f, 1.08883f};
        ret = TIFFCIELabToRGBInit(cielab, &display, fallback_wp);
    }

    if (white_point) {
        free(white_point);
    }
    
    return ret;
}

static int init_ycbcr_context(TIFFYCbCrToRGB *ycbcr, const uint8_t *data, size_t size) {
    float *luma_coeff = NULL;
    float *ref_black_white = NULL;
    int ret = 0;

    if (size >= 6) {
        luma_coeff = (float *)malloc(3 * sizeof(float));
        ref_black_white = (float *)malloc(2 * sizeof(float));
        if (luma_coeff && ref_black_white) {
            luma_coeff[0] = ((float)data[0]) / 255.0f;
            luma_coeff[1] = ((float)data[1]) / 255.0f;
            luma_coeff[2] = ((float)data[2]) / 255.0f;
            ref_black_white[0] = ((float)data[3]) / 255.0f;
            ref_black_white[1] = ((float)data[4]) / 255.0f;
            
            ret = TIFFYCbCrToRGBInit(ycbcr, luma_coeff, ref_black_white);
        }
    }

    if (luma_coeff) free(luma_coeff);
    if (ref_black_white) free(ref_black_white);

    return ret;
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Contexts
    TIFFCIELabToRGB *cielab_ctx = NULL;
    TIFFYCbCrToRGB *ycbcr_ctx = NULL;
    
    // Allocate memory for contexts
    // Note: libtiff documentation suggests specific allocation sizes for ycbcr
    // For cielab, the struct size is internal, but we allocate a reasonable buffer.
    size_t cielab_size = sizeof(TIFFCIELabToRGB) + 4096; 
    size_t ycbcr_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                        4 * 256 * sizeof(TIFFRGBValue) + 
                        2 * 256 * sizeof(int) + 
                        3 * 256 * sizeof(int32_t);

    cielab_ctx = (TIFFCIELabToRGB *)_TIFFmalloc(cielab_size);
    ycbcr_ctx = (TIFFYCbCrToRGB *)_TIFFmalloc(ycbcr_size);

    if (!cielab_ctx || !ycbcr_ctx) {
        safe_free(cielab_ctx);
        safe_free(ycbcr_ctx);
        return 0;
    }

    // Initialize contexts based on fuzz data
    // We use the first part of data for initialization parameters
    int cielab_init_ok = init_cielab_context(cielab_ctx, Data, Size);
    int ycbcr_init_ok = init_ycbcr_context(ycbcr_ctx, Data, Size);

    // Fuzzing Logic
    uint32_t r, g, b;
    float x, y, z;
    float xyz_out[3];
    int32_t l, a, b_val;
    uint32_t logluv_pixel;
    int32_t y_val, cb_val, cr_val;

    // Extract fuzz data for conversion inputs
    if (Size >= 12) {
        l = (int32_t)Data[0]; // 0-100 ideally
        a = (int32_t)(Data[1] - 128); // -128 to 127
        b_val = (int32_t)(Data[2] - 128); // -128 to 127
        
        x = (float)Data[3] / 255.0f;
        y = (float)Data[4] / 255.0f;
        z = (float)Data[5] / 255.0f;

        logluv_pixel = (uint32_t)((Data[6] << 16) | (Data[7] << 8) | Data[8]);
        
        y_val = (int32_t)Data[9];
        cb_val = (int32_t)(Data[10] - 128);
        cr_val = (int32_t)(Data[11] - 128);
    } else {
        // Default values if data is too short
        l = 50; a = 0; b_val = 0;
        x = 0.5f; y = 0.5f; z = 0.5f;
        logluv_pixel = 0;
        y_val = 128; cb_val = 0; cr_val = 0;
    }

    // 1. Test TIFFCIELabToXYZ
    if (cielab_init_ok) {
        TIFFCIELabToXYZ(cielab_ctx, (uint32_t)l, a, b_val, &x, &y, &z);
    }

    // 2. Test LogLuv24toXYZ (No context needed)
    LogLuv24toXYZ(logluv_pixel, xyz_out);

    // 3. Test TIFFYCbCrtoRGB
    if (ycbcr_init_ok) {
        TIFFYCbCrtoRGB(ycbcr_ctx, (uint32_t)y_val, cb_val, cr_val, &r, &g, &b);
    }

    // 4. Test TIFFXYZToRGB
    // Ensure cielab_ctx is valid and initialized before calling
    if (cielab_ctx && cielab_init_ok) {
        TIFFXYZToRGB(cielab_ctx, x, y, z, &r, &g, &b);
    }

    // Cleanup
    safe_free(cielab_ctx);
    safe_free(ycbcr_ctx);

    return 0;
}