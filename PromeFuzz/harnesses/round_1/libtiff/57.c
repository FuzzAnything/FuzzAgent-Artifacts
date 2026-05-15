// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#ifndef TIFFroundup_32
#define TIFFroundup_32(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
#endif

static float read_float(const uint8_t **data, size_t *size) {
    if (*size < sizeof(float)) return 0.0f;
    float value;
    memcpy(&value, *data, sizeof(float));
    *data += sizeof(float);
    *size -= sizeof(float);
    return value;
}

static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(uint32_t)) return 0;
    uint32_t value;
    memcpy(&value, *data, sizeof(uint32_t));
    *data += sizeof(uint32_t);
    *size -= sizeof(uint32_t);
    return value;
}

static int32_t read_int32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int32_t)) return 0;
    int32_t value;
    memcpy(&value, *data, sizeof(int32_t));
    *data += sizeof(int32_t);
    *size -= sizeof(int32_t);
    return value;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0;
    
    const uint8_t *data_ptr = Data;
    size_t data_size = Size;
    
    // 1. Test TIFFYCbCrToRGBInit
    {
        // Allocate buffer as per documentation
        size_t buffer_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                             4 * 256 * sizeof(TIFFRGBValue) +
                             2 * 256 * sizeof(int) +
                             3 * 256 * sizeof(int32_t);
        TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(buffer_size);
        if (ycbcr) {
            float refWhite[3];
            float luma[3];
            for (int i = 0; i < 3; i++) {
                refWhite[i] = read_float(&data_ptr, &data_size);
                luma[i] = read_float(&data_ptr, &data_size);
            }
            TIFFYCbCrToRGBInit(ycbcr, refWhite, luma);
            _TIFFfree(ycbcr);
        }
    }
    
    // 2. Test TIFFCIELabToXYZ
    {
        TIFFCIELabToRGB *lab = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        if (lab) {
            uint32_t l = read_uint32(&data_ptr, &data_size);
            int32_t a = read_int32(&data_ptr, &data_size);
            int32_t b = read_int32(&data_ptr, &data_size);
            float X, Y, Z;
            TIFFCIELabToXYZ(lab, l, a, b, &X, &Y, &Z);
            free(lab);
        }
    }
    
    // 3. Test LogLuv24fromXYZ
    {
        float xyz[3];
        for (int i = 0; i < 3; i++) {
            xyz[i] = read_float(&data_ptr, &data_size);
        }
        int code = (int)read_uint32(&data_ptr, &data_size);
        LogLuv24fromXYZ(xyz, code);
    }
    
    // 4. Test XYZtoRGB24
    {
        float xyz[3];
        uint8_t rgb[3];
        for (int i = 0; i < 3; i++) {
            xyz[i] = read_float(&data_ptr, &data_size);
        }
        XYZtoRGB24(xyz, rgb);
    }
    
    // 5. Test TIFFCIELabToRGBInit
    {
        TIFFCIELabToRGB *lab = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        TIFFDisplay display;
        if (lab) {
            // Initialize display with fuzzer data
            memset(&display, 0, sizeof(TIFFDisplay));
            if (data_size >= sizeof(TIFFDisplay)) {
                memcpy(&display, data_ptr, sizeof(TIFFDisplay));
                data_ptr += sizeof(TIFFDisplay);
                data_size -= sizeof(TIFFDisplay);
            }
            
            float refWhite[3];
            for (int i = 0; i < 3; i++) {
                refWhite[i] = read_float(&data_ptr, &data_size);
            }
            TIFFCIELabToRGBInit(lab, &display, refWhite);
            free(lab);
        }
    }
    
    // 6. Test TIFFXYZToRGB
    {
        TIFFCIELabToRGB *lab = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB));
        if (lab) {
            // Initialize with some defaults
            TIFFDisplay display;
            memset(&display, 0, sizeof(TIFFDisplay));
            float refWhite[3] = {0.0f, 0.0f, 0.0f};
            TIFFCIELabToRGBInit(lab, &display, refWhite);
            
            float X = read_float(&data_ptr, &data_size);
            float Y = read_float(&data_ptr, &data_size);
            float Z = read_float(&data_ptr, &data_size);
            uint32_t r, g, b;
            TIFFXYZToRGB(lab, X, Y, Z, &r, &g, &b);
            free(lab);
        }
    }
    
    return 0;
}