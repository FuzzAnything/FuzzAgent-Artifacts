// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* Include the required header file with double quotes and full relative path */
#include "libtiff/tiffio.h"

static void fuzz_LogLuv24fromXYZ(const uint8_t *data, size_t size) {
    if (size < 3 * sizeof(float)) return;
    
    float xyz[3];
    memcpy(xyz, data, 3 * sizeof(float));
    int lum = (size >= 3 * sizeof(float) + sizeof(int)) ? 
              *(int *)(data + 3 * sizeof(float)) : 0;
    
    LogLuv24fromXYZ(xyz, lum);
}

static void fuzz_XYZtoRGB24(const uint8_t *data, size_t size) {
    if (size < 3 * sizeof(float)) return;
    
    float xyz[3];
    uint8_t rgb[3];
    memcpy(xyz, data, 3 * sizeof(float));
    
    XYZtoRGB24(xyz, rgb);
}

static void fuzz_TIFFYCbCrToRGBInit(const uint8_t *data, size_t size) {
    /* Calculate required buffer size as per documentation */
    size_t base_size = sizeof(TIFFYCbCrToRGB);
    size_t aligned_base = (base_size + sizeof(long) - 1) & ~(sizeof(long) - 1);
    size_t total_size = aligned_base +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);
    
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)malloc(total_size);
    if (!ycbcr) return;
    
    /* Initialize the entire buffer to zero to avoid uninitialized reads */
    memset(ycbcr, 0, total_size);
    
    /* Use a single array of 6 floats as expected by the function */
    float refBlackWhite[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    
    /* Copy only if we have enough data, otherwise use zeros */
    if (size >= 6 * sizeof(float)) {
        memcpy(refBlackWhite, data, 6 * sizeof(float));
    } else if (size > 0) {
        /* Copy only the available bytes */
        memcpy(refBlackWhite, data, (size > 6 * sizeof(float)) ? 6 * sizeof(float) : size);
    }
    
    /* Call the function with the correct parameters */
    TIFFYCbCrToRGBInit(ycbcr, refBlackWhite, refBlackWhite + 3);
    free(ycbcr);
}

static void fuzz_TIFFXYZToRGB(const uint8_t *data, size_t size) {
    /* We need to initialize TIFFCIELabToRGB first */
    TIFFCIELabToRGB cielab;
    TIFFDisplay display;
    float whitePoint[3] = {0.9505f, 1.0000f, 1.0890f}; /* D65 */
    
    /* Initialize with default values */
    memset(&display, 0, sizeof(TIFFDisplay));
    display.d_mat[0][0] = 0.4124f; display.d_mat[0][1] = 0.3576f; display.d_mat[0][2] = 0.1805f;
    display.d_mat[1][0] = 0.2126f; display.d_mat[1][1] = 0.7152f; display.d_mat[1][2] = 0.0722f;
    display.d_mat[2][0] = 0.0193f; display.d_mat[2][1] = 0.1192f; display.d_mat[2][2] = 0.9505f;
    
    if (TIFFCIELabToRGBInit(&cielab, &display, whitePoint) != 0) {
        float x = 0.0f, y = 0.0f, z = 0.0f;
        uint32_t r, g, b;
        
        if (size >= 3 * sizeof(float)) {
            memcpy(&x, data, sizeof(float));
            memcpy(&y, data + sizeof(float), sizeof(float));
            memcpy(&z, data + 2 * sizeof(float), sizeof(float));
        }
        
        TIFFXYZToRGB(&cielab, x, y, z, &r, &g, &b);
    }
}

static void fuzz_TIFFCIELabToXYZ(const uint8_t *data, size_t size) {
    /* We need to initialize TIFFCIELabToRGB first */
    TIFFCIELabToRGB cielab;
    TIFFDisplay display;
    float whitePoint[3] = {0.9505f, 1.0000f, 1.0890f}; /* D65 */
    
    /* Initialize with default values */
    memset(&display, 0, sizeof(TIFFDisplay));
    display.d_mat[0][0] = 0.4124f; display.d_mat[0][1] = 0.3576f; display.d_mat[0][2] = 0.1805f;
    display.d_mat[1][0] = 0.2126f; display.d_mat[1][1] = 0.7152f; display.d_mat[1][2] = 0.0722f;
    display.d_mat[2][0] = 0.0193f; display.d_mat[2][1] = 0.1192f; display.d_mat[2][2] = 0.9505f;
    
    if (TIFFCIELabToRGBInit(&cielab, &display, whitePoint) != 0) {
        uint32_t L = 0;
        int32_t a = 0, b = 0;
        float x, y, z;
        
        if (size >= sizeof(uint32_t) + 2 * sizeof(int32_t)) {
            memcpy(&L, data, sizeof(uint32_t));
            memcpy(&a, data + sizeof(uint32_t), sizeof(int32_t));
            memcpy(&b, data + sizeof(uint32_t) + sizeof(int32_t), sizeof(int32_t));
        }
        
        TIFFCIELabToXYZ(&cielab, L, a, b, &x, &y, &z);
    }
}

static void fuzz_TIFFCIELabToRGBInit(const uint8_t *data, size_t size) {
    TIFFCIELabToRGB cielab;
    TIFFDisplay display;
    float whitePoint[3] = {0.9505f, 1.0000f, 1.0890f}; /* D65 */
    
    /* Initialize display with zeros first */
    memset(&display, 0, sizeof(TIFFDisplay));
    
    /* Only copy from data if we have enough bytes */
    if (size >= sizeof(TIFFDisplay)) {
        memcpy(&display, data, sizeof(TIFFDisplay));
        data += sizeof(TIFFDisplay);
        size -= sizeof(TIFFDisplay);
    } else if (size > 0) {
        /* Copy only the available bytes */
        memcpy(&display, data, size);
        size = 0;
    }
    
    /* Copy whitePoint if we have enough data */
    if (size >= 3 * sizeof(float)) {
        memcpy(whitePoint, data, 3 * sizeof(float));
    } else if (size > 0) {
        /* Copy only the available bytes for whitePoint */
        memcpy(whitePoint, data, size);
    }
    
    TIFFCIELabToRGBInit(&cielab, &display, whitePoint);
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Use different parts of the input for different functions */
    size_t chunk_size = Size / 6;
    if (chunk_size == 0) {
        /* If Size is too small, test all functions with the entire input */
        fuzz_LogLuv24fromXYZ(Data, Size);
        fuzz_XYZtoRGB24(Data, Size);
        fuzz_TIFFYCbCrToRGBInit(Data, Size);
        fuzz_TIFFXYZToRGB(Data, Size);
        fuzz_TIFFCIELabToXYZ(Data, Size);
        fuzz_TIFFCIELabToRGBInit(Data, Size);
        return 0;
    }
    
    /* Test LogLuv24fromXYZ */
    fuzz_LogLuv24fromXYZ(Data, chunk_size);
    
    /* Test XYZtoRGB24 */
    if (Size >= 2 * chunk_size) {
        fuzz_XYZtoRGB24(Data + chunk_size, chunk_size);
    }
    
    /* Test TIFFYCbCrToRGBInit */
    if (Size >= 3 * chunk_size) {
        fuzz_TIFFYCbCrToRGBInit(Data + 2 * chunk_size, chunk_size);
    }
    
    /* Test TIFFXYZToRGB */
    if (Size >= 4 * chunk_size) {
        fuzz_TIFFXYZToRGB(Data + 3 * chunk_size, chunk_size);
    }
    
    /* Test TIFFCIELabToXYZ */
    if (Size >= 5 * chunk_size) {
        fuzz_TIFFCIELabToXYZ(Data + 4 * chunk_size, chunk_size);
    }
    
    /* Test TIFFCIELabToRGBInit with the remaining data */
    if (Size > 5 * chunk_size) {
        fuzz_TIFFCIELabToRGBInit(Data + 5 * chunk_size, Size - 5 * chunk_size);
    }
    
    return 0;
}