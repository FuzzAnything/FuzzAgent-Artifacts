// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_dummy_tiffdisplay(TIFFDisplay *display) {
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
    display->d_YCR = 195.0f;
    display->d_YCG = 195.0f;
    display->d_YCB = 195.0f;
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

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 128) return 0;

    // 1. Test TIFFYCbCrToRGBInit
    {
        // Calculate allocation size manually without TIFFroundup_32
        size_t base_size = sizeof(TIFFYCbCrToRGB);
        size_t align = sizeof(long);
        size_t rounded_size = ((base_size + align - 1) / align) * align;
        
        size_t alloc_size = rounded_size +
                            4 * 256 * sizeof(TIFFRGBValue) +
                            2 * 256 * sizeof(int) +
                            3 * 256 * sizeof(int32_t);
        TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)malloc(alloc_size);
        if (ycbcr) {
            float refBlackWhite[6];
            memcpy(refBlackWhite, Data, sizeof(refBlackWhite));
            TIFFYCbCrToRGBInit(ycbcr, refBlackWhite, refBlackWhite + 3);
            free(ycbcr);
        }
    }

    // 2. Test TIFFCIELabToXYZ
    {
        TIFFCIELabToRGB *labrgb = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        if (labrgb) {
            memset(labrgb, 0, sizeof(TIFFCIELabToRGB));
            uint32_t l;
            int32_t a, b;
            memcpy(&l, Data + 24, sizeof(uint32_t));
            memcpy(&a, Data + 28, sizeof(int32_t));
            memcpy(&b, Data + 32, sizeof(int32_t));
            float X, Y, Z;
            TIFFCIELabToXYZ(labrgb, l, a, b, &X, &Y, &Z);
            free(labrgb);
        }
    }

    // 3. Test LogLuv24fromXYZ
    {
        float xyz[3];
        memcpy(xyz, Data + 36, sizeof(float) * 3);
        int em = *(Data + 48) & 1;
        LogLuv24fromXYZ(xyz, em);
    }

    // 4. Test XYZtoRGB24
    {
        float xyz[3];
        uint8_t rgb[3];
        memcpy(xyz, Data + 52, sizeof(float) * 3);
        XYZtoRGB24(xyz, rgb);
    }

    // 5. Test TIFFCIELabToRGBInit
    {
        TIFFCIELabToRGB *labrgb = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        TIFFDisplay display;
        if (labrgb) {
            init_dummy_tiffdisplay(&display);
            float refWhite[3];
            memcpy(refWhite, Data + 64, sizeof(float) * 3);
            TIFFCIELabToRGBInit(labrgb, &display, refWhite);
            free(labrgb);
        }
    }

    // 6. Test TIFFXYZToRGB
    {
        TIFFCIELabToRGB *labrgb = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        if (labrgb) {
            memset(labrgb, 0, sizeof(TIFFCIELabToRGB));
            TIFFDisplay display;
            init_dummy_tiffdisplay(&display);
            float refWhite[3] = {0.9505f, 1.0000f, 1.0890f};
            TIFFCIELabToRGBInit(labrgb, &display, refWhite);
            
            float X, Y, Z;
            memcpy(&X, Data + 76, sizeof(float));
            memcpy(&Y, Data + 80, sizeof(float));
            memcpy(&Z, Data + 84, sizeof(float));
            uint32_t R, G, B;
            TIFFXYZToRGB(labrgb, X, Y, Z, &R, &G, &B);
            free(labrgb);
        }
    }

    return 0;
}