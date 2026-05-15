// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiff.h"
#include "libtiff/tiffio.h"

/* Define TIFFroundup_32 macro if not already defined */
#ifndef TIFFroundup_32
#define TIFFroundup_32(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
#endif

static TIFFCIELabToRGB* init_CIELabToRGB_struct(const uint8_t *data, size_t size) {
    if (size < sizeof(TIFFDisplay) + 3*sizeof(float)) {
        return NULL;
    }
    
    TIFFCIELabToRGB *labToRGB = (TIFFCIELabToRGB*)malloc(sizeof(TIFFCIELabToRGB));
    if (!labToRGB) return NULL;
    
    TIFFDisplay display;
    memcpy(&display, data, sizeof(TIFFDisplay));
    
    float refWhite[3];
    memcpy(refWhite, data + sizeof(TIFFDisplay), 3*sizeof(float));
    
    int init_result = TIFFCIELabToRGBInit(labToRGB, &display, refWhite);
    if (init_result != 1) {
        free(labToRGB);
        return NULL;
    }
    
    return labToRGB;
}

static TIFFYCbCrToRGB* init_YCbCrToRGB_struct(const uint8_t *data, size_t size) {
    if (size < 6*sizeof(float)) {
        return NULL;
    }
    
    size_t buffer_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                         4 * 256 * sizeof(TIFFRGBValue) +
                         2 * 256 * sizeof(int) +
                         3 * 256 * sizeof(int32_t);
    
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB*)_TIFFmalloc(buffer_size);
    if (!ycbcr) return NULL;
    
    float luma[3];
    float refBlackWhite[3];
    
    memcpy(luma, data, 3*sizeof(float));
    memcpy(refBlackWhite, data + 3*sizeof(float), 3*sizeof(float));
    
    int init_result = TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);
    if (init_result != 1) {
        _TIFFfree(ycbcr);
        return NULL;
    }
    
    return ycbcr;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t function_selector = Data[0] % 6;
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;
    
    switch (function_selector) {
        case 0: {  // TIFFXYZToRGB
            if (func_size < 3*sizeof(float)) break;
            
            TIFFCIELabToRGB *labToRGB = init_CIELabToRGB_struct(func_data, func_size);
            if (!labToRGB) break;
            
            float x, y, z;
            memcpy(&x, func_data, sizeof(float));
            memcpy(&y, func_data + sizeof(float), sizeof(float));
            memcpy(&z, func_data + 2*sizeof(float), sizeof(float));
            
            uint32_t r, g, b;
            TIFFXYZToRGB(labToRGB, x, y, z, &r, &g, &b);
            
            free(labToRGB);
            break;
        }
        
        case 1: {  // XYZtoRGB24
            if (func_size < 3*sizeof(float)) break;
            
            float xyz[3];
            memcpy(xyz, func_data, 3*sizeof(float));
            
            uint8_t rgb[3];
            XYZtoRGB24(xyz, rgb);
            break;
        }
        
        case 2: {  // TIFFYCbCrtoRGB
            if (func_size < 3*sizeof(uint32_t)) break;
            
            TIFFYCbCrToRGB *ycbcr = init_YCbCrToRGB_struct(func_data, func_size);
            if (!ycbcr) break;
            
            uint32_t Y, Cb, Cr;
            memcpy(&Y, func_data, sizeof(uint32_t));
            memcpy(&Cb, func_data + sizeof(uint32_t), sizeof(uint32_t));
            memcpy(&Cr, func_data + 2*sizeof(uint32_t), sizeof(uint32_t));
            
            uint32_t r, g, b;
            TIFFYCbCrtoRGB(ycbcr, Y, (int32_t)Cb, (int32_t)Cr, &r, &g, &b);
            
            _TIFFfree(ycbcr);
            break;
        }
        
        case 3: {  // TIFFCIELabToXYZ
            if (func_size < 3*sizeof(uint32_t)) break;
            
            TIFFCIELabToRGB *labToRGB = init_CIELabToRGB_struct(func_data, func_size);
            if (!labToRGB) break;
            
            uint32_t L, a, b_val;
            memcpy(&L, func_data, sizeof(uint32_t));
            memcpy(&a, func_data + sizeof(uint32_t), sizeof(uint32_t));
            memcpy(&b_val, func_data + 2*sizeof(uint32_t), sizeof(uint32_t));
            
            float x, y, z;
            TIFFCIELabToXYZ(labToRGB, L, (int32_t)a, (int32_t)b_val, &x, &y, &z);
            
            free(labToRGB);
            break;
        }
        
        case 4: {  // TIFFCIELabToRGBInit
            if (func_size < sizeof(TIFFDisplay) + 3*sizeof(float)) break;
            
            TIFFCIELabToRGB labToRGB;
            TIFFDisplay display;
            float refWhite[3];
            
            memcpy(&display, func_data, sizeof(TIFFDisplay));
            memcpy(refWhite, func_data + sizeof(TIFFDisplay), 3*sizeof(float));
            
            TIFFCIELabToRGBInit(&labToRGB, &display, refWhite);
            break;
        }
        
        case 5: {  // TIFFYCbCrToRGBInit
            if (func_size < 6*sizeof(float)) break;
            
            size_t buffer_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                                 4 * 256 * sizeof(TIFFRGBValue) +
                                 2 * 256 * sizeof(int) +
                                 3 * 256 * sizeof(int32_t);
            
            TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB*)_TIFFmalloc(buffer_size);
            if (!ycbcr) break;
            
            float luma[3];
            float refBlackWhite[3];
            
            memcpy(luma, func_data, 3*sizeof(float));
            memcpy(refBlackWhite, func_data + 3*sizeof(float), 3*sizeof(float));
            
            TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);
            
            _TIFFfree(ycbcr);
            break;
        }
    }
    
    return 0;
}