// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_cleanup(void) {
    // Clean up any resources if needed
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize with dummy values from fuzzer input
    uint32_t seed = *(const uint32_t*)Data;
    srand(seed);
    Data += 4;
    Size -= 4;

    // Test TIFFYCbCrToRGBInit and TIFFYCbCrtoRGB
    {
        // Calculate buffer size without TIFFroundup_32
        size_t base_size = sizeof(TIFFYCbCrToRGB);
        size_t align = sizeof(long);
        size_t rounded_base = ((base_size + align - 1) / align) * align;
        size_t buffer_size = rounded_base +
                             4 * 256 * sizeof(TIFFRGBValue) + 
                             2 * 256 * sizeof(int) +
                             3 * 256 * sizeof(int32_t);
        TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(buffer_size);
        if (ycbcr) {
            float luma[3] = {0.299f, 0.587f, 0.114f};
            float refBlackWhite[6] = {0.0f, 255.0f, 128.0f, 0.0f, 255.0f, 128.0f};
            
            // Use fuzzer data for parameters if available
            if (Size >= 24) {
                memcpy(luma, Data, 12);
                memcpy(refBlackWhite, Data + 12, 12);
                Data += 24;
                Size -= 24;
            }
            
            int init_ret = TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);
            if (init_ret) {
                uint32_t Y = (Size > 0) ? Data[0] : 128;
                int32_t Cb = (Size > 1) ? (int32_t)Data[1] - 128 : 0;
                int32_t Cr = (Size > 2) ? (int32_t)Data[2] - 128 : 0;
                uint32_t R, G, B;
                
                TIFFYCbCrtoRGB(ycbcr, Y, Cb, Cr, &R, &G, &B);
                
                if (Size > 3) {
                    Data += 3;
                    Size -= 3;
                }
            }
            _TIFFfree(ycbcr);
        }
    }

    // Test XYZtoRGB24
    {
        float xyz[3] = {0.5f, 0.5f, 0.5f};
        uint8_t rgb[3];
        
        if (Size >= 12) {
            memcpy(xyz, Data, 12);
            Data += 12;
            Size -= 12;
        }
        
        XYZtoRGB24(xyz, rgb);
    }

    // Test TIFFCIELabToRGBInit, TIFFCIELabToXYZ, and TIFFXYZToRGB
    {
        TIFFCIELabToRGB *cielab = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        TIFFDisplay display;
        float refWhite[3] = {1.0f, 1.0f, 1.0f};
        
        if (cielab) {
            // Initialize display with dummy values
            memset(&display, 0, sizeof(TIFFDisplay));
            display.d_mat[0][0] = 3.0f;
            display.d_mat[0][1] = -1.0f;
            display.d_mat[0][2] = -0.5f;
            display.d_mat[1][0] = -1.0f;
            display.d_mat[1][1] = 1.5f;
            display.d_mat[1][2] = 0.5f;
            display.d_mat[2][0] = 0.5f;
            display.d_mat[2][1] = -0.5f;
            display.d_mat[2][2] = 1.0f;
            
            // Use fuzzer data for refWhite if available
            if (Size >= 12) {
                memcpy(refWhite, Data, 12);
                Data += 12;
                Size -= 12;
            }
            
            int init_ret = TIFFCIELabToRGBInit(cielab, &display, refWhite);
            if (init_ret) {
                // Test TIFFCIELabToXYZ
                uint32_t L = (Size > 0) ? Data[0] : 50;
                int32_t a = (Size > 1) ? (int32_t)Data[1] - 128 : 0;
                int32_t b = (Size > 2) ? (int32_t)Data[2] - 128 : 0;
                float X, Y, Z;
                
                TIFFCIELabToXYZ(cielab, L, a, b, &X, &Y, &Z);
                
                if (Size > 3) {
                    Data += 3;
                    Size -= 3;
                }
                
                // Test TIFFXYZToRGB
                float x = (Size > 0 && Size <= 12) ? *(const float*)Data : 0.5f;
                float y = (Size > 4 && Size <= 12) ? *(const float*)(Data + 4) : 0.5f;
                float z = (Size > 8 && Size <= 12) ? *(const float*)(Data + 8) : 0.5f;
                uint32_t R, G, B;
                
                TIFFXYZToRGB(cielab, x, y, z, &R, &G, &B);
                
                if (Size > 12) {
                    Data += 12;
                    Size -= 12;
                }
            }
            free(cielab);
        }
    }

    dummy_cleanup();
    return 0;
}