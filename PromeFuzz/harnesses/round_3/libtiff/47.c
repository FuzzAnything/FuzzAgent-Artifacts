// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"

static void init_display_from_data(TIFFDisplay *display, const uint8_t *data, size_t size) {
    if (size >= sizeof(TIFFDisplay)) {
        memcpy(display, data, sizeof(TIFFDisplay));
    } else {
        memset(display, 0, sizeof(TIFFDisplay));
        display->d_mat[0][0] = 2.0f;
        display->d_mat[0][1] = 1.0f;
        display->d_mat[0][2] = 0.0f;
        display->d_mat[1][0] = 1.0f;
    }
}

static void init_ycbcr_params(float *luma, float *refBlackWhite, const uint8_t *data, size_t size) {
    if (size >= 9 * sizeof(float)) {
        memcpy(luma, data, 3 * sizeof(float));
        memcpy(refBlackWhite, data + 3 * sizeof(float), 6 * sizeof(float));
    } else {
        luma[0] = 0.299f;
        luma[1] = 0.587f;
        luma[2] = 0.114f;
        refBlackWhite[0] = 0.0f;
        refBlackWhite[1] = 255.0f;
        refBlackWhite[2] = 128.0f;
        refBlackWhite[3] = 255.0f;
        refBlackWhite[4] = 128.0f;
        refBlackWhite[5] = 255.0f;
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    TIFFCIELabToRGB cielabToRGB;
    TIFFDisplay display;
    float range[3];
    
    init_display_from_data(&display, Data, Size);
    size_t offset = sizeof(TIFFDisplay) < Size ? sizeof(TIFFDisplay) : Size;
    
    if (Size - offset >= 3 * sizeof(float)) {
        memcpy(range, Data + offset, 3 * sizeof(float));
        offset += 3 * sizeof(float);
    } else {
        range[0] = 0.0f;
        range[1] = 255.0f;
        range[2] = 255.0f;
    }
    
    TIFFCIELabToRGBInit(&cielabToRGB, &display, range);
    
    if (Size - offset >= 3 * sizeof(float)) {
        float X, Y, Z;
        uint32_t R, G, B;
        memcpy(&X, Data + offset, sizeof(float));
        memcpy(&Y, Data + offset + sizeof(float), sizeof(float));
        memcpy(&Z, Data + offset + 2 * sizeof(float), sizeof(float));
        offset += 3 * sizeof(float);
        
        TIFFXYZToRGB(&cielabToRGB, X, Y, Z, &R, &G, &B);
    }
    
    size_t ycbcr_buffer_size = ((sizeof(TIFFYCbCrToRGB) + sizeof(long) - 1) / sizeof(long)) * sizeof(long) +
                               4 * 256 * sizeof(unsigned char) + 2 * 256 * sizeof(int) +
                               3 * 256 * sizeof(int32_t);
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(ycbcr_buffer_size);
    if (ycbcr) {
        float luma[3];
        float refBlackWhite[6];
        init_ycbcr_params(luma, refBlackWhite, Data + offset, Size - offset);
        
        TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);
        
        if (Size - offset >= 3 * sizeof(uint32_t)) {
            uint32_t Y, Cb, Cr;
            uint32_t R, G, B;
            memcpy(&Y, Data + offset, sizeof(uint32_t));
            memcpy(&Cb, Data + offset + sizeof(uint32_t), sizeof(uint32_t));
            memcpy(&Cr, Data + offset + 2 * sizeof(uint32_t), sizeof(uint32_t));
            
            TIFFYCbCrtoRGB(ycbcr, Y, Cb, Cr, &R, &G, &B);
        }
        
        _TIFFfree(ycbcr);
    }
    
    if (Size - offset >= 3 * sizeof(uint32_t)) {
        uint32_t L, a, b;
        float X, Y, Z;
        memcpy(&L, Data + offset, sizeof(uint32_t));
        memcpy(&a, Data + offset + sizeof(uint32_t), sizeof(uint32_t));
        memcpy(&b, Data + offset + 2 * sizeof(uint32_t), sizeof(uint32_t));
        offset += 3 * sizeof(uint32_t);
        
        TIFFCIELabToXYZ(&cielabToRGB, L, a, b, &X, &Y, &Z);
        
        if (Size - offset >= 3 * sizeof(float)) {
            float xyz[3];
            uint8_t rgb[3];
            memcpy(xyz, Data + offset, 3 * sizeof(float));
            XYZtoRGB24(xyz, rgb);
        }
    }
    
    return 0;
}