// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Define TIFFroundup_32 macro if not already defined */
#ifndef TIFFroundup_32
#define TIFFroundup_32(x, y) ((((x)+((y)-1))/(y))*(y))
#endif

static int read_float(const uint8_t **data, size_t *size, float *value) {
    if (*size < sizeof(float)) return 0;
    memcpy(value, *data, sizeof(float));
    *data += sizeof(float);
    *size -= sizeof(float);
    return 1;
}

static int read_int32(const uint8_t **data, size_t *size, int32_t *value) {
    if (*size < sizeof(int32_t)) return 0;
    memcpy(value, *data, sizeof(int32_t));
    *data += sizeof(int32_t);
    *size -= sizeof(int32_t);
    return 1;
}

static int read_uint32(const uint8_t **data, size_t *size, uint32_t *value) {
    if (*size < sizeof(uint32_t)) return 0;
    memcpy(value, *data, sizeof(uint32_t));
    *data += sizeof(uint32_t);
    *size -= sizeof(uint32_t);
    return 1;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    /* Test LogLuv24fromXYZ */
    if (remaining >= 3 * sizeof(float) + sizeof(int)) {
        float xyz[3];
        int encoding;
        
        for (int i = 0; i < 3; i++) {
            if (!read_float(&ptr, &remaining, &xyz[i])) break;
        }
        if (!read_int32(&ptr, &remaining, &encoding)) return 0;
        
        uint32_t result = LogLuv24fromXYZ(xyz, encoding);
        (void)result; /* Avoid unused variable warning */
    }
    
    /* Test XYZtoRGB24 */
    if (remaining >= 3 * sizeof(float)) {
        float xyz[3];
        uint8_t rgb[3];
        
        for (int i = 0; i < 3; i++) {
            if (!read_float(&ptr, &remaining, &xyz[i])) break;
        }
        
        XYZtoRGB24(xyz, rgb);
    }
    
    /* Test TIFFYCbCrToRGBInit */
    if (remaining >= 6 * sizeof(float)) {
        /* Allocate buffer as specified in documentation */
        size_t buffer_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                             4 * 256 * sizeof(TIFFRGBValue) +
                             2 * 256 * sizeof(int) +
                             3 * 256 * sizeof(int32_t);
        
        TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(buffer_size);
        if (ycbcr) {
            float luma[3];
            float refBlackWhite[6];
            
            for (int i = 0; i < 3; i++) {
                if (!read_float(&ptr, &remaining, &luma[i])) break;
            }
            for (int i = 0; i < 6; i++) {
                if (!read_float(&ptr, &remaining, &refBlackWhite[i])) break;
            }
            
            int init_result = TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);
            (void)init_result;
            
            _TIFFfree(ycbcr);
        }
    }
    
    /* Test TIFFCIELabToRGBInit and related functions */
    if (remaining >= sizeof(TIFFDisplay) + 3 * sizeof(float)) {
        /* Allocate and initialize TIFFCIELabToRGB structure */
        TIFFCIELabToRGB *cielab = (TIFFCIELabToRGB *)_TIFFmalloc(sizeof(TIFFCIELabToRGB));
        TIFFDisplay *display = (TIFFDisplay *)_TIFFmalloc(sizeof(TIFFDisplay));
        
        if (cielab && display) {
            /* Read display parameters from fuzzer input */
            memcpy(display, ptr, sizeof(TIFFDisplay) > remaining ? remaining : sizeof(TIFFDisplay));
            ptr += sizeof(TIFFDisplay) > remaining ? remaining : sizeof(TIFFDisplay);
            remaining -= sizeof(TIFFDisplay) > remaining ? remaining : sizeof(TIFFDisplay);
            
            float refWhite[3];
            for (int i = 0; i < 3; i++) {
                if (!read_float(&ptr, &remaining, &refWhite[i])) break;
            }
            
            /* Initialize CIELab to RGB conversion */
            int init_result = TIFFCIELabToRGBInit(cielab, display, refWhite);
            (void)init_result;
            
            /* Test TIFFCIELabToXYZ if we have enough data */
            if (remaining >= sizeof(uint32_t) + 2 * sizeof(int32_t) + 3 * sizeof(float)) {
                uint32_t L;
                int32_t a, b;
                float X, Y, Z;
                
                if (read_uint32(&ptr, &remaining, &L) &&
                    read_int32(&ptr, &remaining, &a) &&
                    read_int32(&ptr, &remaining, &b)) {
                    
                    TIFFCIELabToXYZ(cielab, L, a, b, &X, &Y, &Z);
                    
                    /* Test TIFFXYZToRGB if we have the initialized structure */
                    uint32_t R, G, B;
                    TIFFXYZToRGB(cielab, X, Y, Z, &R, &G, &B);
                }
            }
            
            _TIFFfree(cielab);
            _TIFFfree(display);
        } else {
            if (cielab) _TIFFfree(cielab);
            if (display) _TIFFfree(display);
        }
    }
    
    return 0;
}