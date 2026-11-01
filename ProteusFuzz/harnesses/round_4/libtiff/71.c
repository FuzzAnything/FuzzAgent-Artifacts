// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <tiff.h>

static void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test 1: TIFFYCbCrToRGBInit and TIFFYCbCrtoRGB
    {
        #ifndef TIFFroundup_32
        #define TIFFroundup_32(x, y) (((x) + ((y) - 1)) & ~((y) - 1))
        #endif

        size_t ycbcr_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                            4 * 256 * sizeof(TIFFRGBValue) +
                            2 * 256 * sizeof(int) +
                            3 * 256 * sizeof(int32_t);
        TIFFYCbCrToRGB *ycbcr_ctx = (TIFFYCbCrToRGB *)safe_malloc(ycbcr_size);
        if (!ycbcr_ctx) return 0;

        float luma_coeffs[3] = {0.299f, 0.587f, 0.114f};
        // Fixed: ref_black_white must be size 6 to match library expectations (Y, Cb, Cr min/max)
        float ref_black_white[6] = {0.0f, 255.0f, 0.0f, 255.0f, 0.0f, 255.0f};
        
        if (Size >= 6) {
            luma_coeffs[0] = (float)Data[0] / 255.0f;
            luma_coeffs[1] = (float)Data[1] / 255.0f;
            luma_coeffs[2] = (float)Data[2] / 255.0f;
            // Populate all 6 elements safely if data is available
            if (Size > 3) ref_black_white[0] = (float)Data[3];
            if (Size > 4) ref_black_white[1] = (float)Data[4];
            if (Size > 5) ref_black_white[2] = (float)Data[5];
            if (Size > 6) ref_black_white[3] = (float)Data[6];
            if (Size > 7) ref_black_white[4] = (float)Data[7];
            if (Size > 8) ref_black_white[5] = (float)Data[8];
        }

        if (TIFFYCbCrToRGBInit(ycbcr_ctx, luma_coeffs, ref_black_white) == 0) {
            uint32_t r, g, b;
            uint32_t y = (Size > 9) ? Data[9] : 128;
            int32_t cb = (Size > 10) ? (int32_t)(Data[10] - 128) : 0;
            int32_t cr = (Size > 11) ? (int32_t)(Data[11] - 128) : 0;
            
            TIFFYCbCrtoRGB(ycbcr_ctx, y, cb, cr, &r, &g, &b);
        }
        free(ycbcr_ctx);
    }

    // Test 2: LogLuv24toXYZ
    {
        float xyz[3];
        uint32_t pixel = 0;
        if (Size >= 4) {
            pixel = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        }
        LogLuv24toXYZ(pixel, xyz);
    }

    // Test 3: TIFFCIELabToRGBInit, TIFFCIELabToXYZ, TIFFXYZToRGB
    {
        TIFFCIELabToRGB *lab_ctx = (TIFFCIELabToRGB *)safe_malloc(sizeof(TIFFCIELabToRGB));
        if (!lab_ctx) return 0;

        TIFFDisplay display;
        memset(&display, 0, sizeof(display));
        
        float white_point[3] = {1.0f, 1.0f, 1.0f};
        if (Size >= 15) {
            white_point[0] = (float)Data[12] / 255.0f;
            white_point[1] = (float)Data[13] / 255.0f;
            white_point[2] = (float)Data[14] / 255.0f;
        }

        if (TIFFCIELabToRGBInit(lab_ctx, &display, white_point) == 0) {
            uint32_t r_out, g_out, b_out;
            float x, y, z;
            
            int32_t L = (Size > 15) ? (int32_t)Data[15] : 50;
            int32_t a = (Size > 16) ? (int32_t)(Data[16] - 128) : 0;
            int32_t b_val = (Size > 17) ? (int32_t)(Data[17] - 128) : 0;
            
            TIFFCIELabToXYZ(lab_ctx, L, a, b_val, &x, &y, &z);
            TIFFXYZToRGB(lab_ctx, x, y, z, &r_out, &g_out, &b_out);
        }
        free(lab_ctx);
    }

    return 0;
}