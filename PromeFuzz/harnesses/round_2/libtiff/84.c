// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Include the required header file */
#include "libtiff/tiffio.h"

/* Helper function to read a float from fuzzer data */
static float read_float(const uint8_t **data, size_t *size) {
    if (*size < sizeof(float)) {
        *size = 0;
        return 0.0f;
    }
    float value;
    memcpy(&value, *data, sizeof(float));
    *data += sizeof(float);
    *size -= sizeof(float);
    return value;
}

/* Helper function to read an integer from fuzzer data */
static int read_int(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int)) {
        *size = 0;
        return 0;
    }
    int value;
    memcpy(&value, *data, sizeof(int));
    *data += sizeof(int);
    *size -= sizeof(int);
    return value;
}

/* Helper function to read a uint32_t from fuzzer data */
static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(uint32_t)) {
        *size = 0;
        return 0;
    }
    uint32_t value;
    memcpy(&value, *data, sizeof(uint32_t));
    *data += sizeof(uint32_t);
    *size -= sizeof(uint32_t);
    return value;
}

/* Helper function to read a int32_t from fuzzer data */
static int32_t read_int32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int32_t)) {
        *size = 0;
        return 0;
    }
    int32_t value;
    memcpy(&value, *data, sizeof(int32_t));
    *data += sizeof(int32_t);
    *size -= sizeof(int32_t);
    return value;
}

/* Helper macro to round up to multiple of long */
#define TIFFROUNDUP_32(x, y) ((((x) + ((y) - 1)) / (y)) * (y))

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    const uint8_t *data_ptr = Data;
    size_t remaining = Size;

    /* Test LogLuv24fromXYZ */
    float xyz[3];
    for (int i = 0; i < 3; i++) {
        xyz[i] = read_float(&data_ptr, &remaining);
    }
    int logluv_flag = read_int(&data_ptr, &remaining);
    (void)LogLuv24fromXYZ(xyz, logluv_flag);

    /* Test XYZtoRGB24 */
    float xyz2[3];
    uint8_t rgb[3];
    for (int i = 0; i < 3; i++) {
        xyz2[i] = read_float(&data_ptr, &remaining);
    }
    XYZtoRGB24(xyz2, rgb);

    /* Test TIFFYCbCrToRGBInit */
    /* Allocate buffer as specified in the comment */
    size_t buffer_size = TIFFROUNDUP_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                         4 * 256 * sizeof(TIFFRGBValue) +
                         2 * 256 * sizeof(int) +
                         3 * 256 * sizeof(int32_t);
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(buffer_size);
    if (ycbcr) {
        float ycbcr_coeffs[3];
        float rgb_coeffs[3];
        for (int i = 0; i < 3; i++) {
            ycbcr_coeffs[i] = read_float(&data_ptr, &remaining);
            rgb_coeffs[i] = read_float(&data_ptr, &remaining);
        }
        (void)TIFFYCbCrToRGBInit(ycbcr, ycbcr_coeffs, rgb_coeffs);
        _TIFFfree(ycbcr);
    }

    /* Test TIFFCIELabToRGBInit and related functions */
    TIFFCIELabToRGB cielab_rgb;
    TIFFDisplay display;
    float whitePoint[3];

    /* Initialize display structure with fuzzer data */
    size_t display_size = sizeof(TIFFDisplay);
    if (remaining > display_size) {
        memcpy(&display, data_ptr, display_size);
        data_ptr += display_size;
        remaining -= display_size;
    } else {
        memset(&display, 0, sizeof(TIFFDisplay));
    }

    for (int i = 0; i < 3; i++) {
        whitePoint[i] = read_float(&data_ptr, &remaining);
    }

    int init_result = TIFFCIELabToRGBInit(&cielab_rgb, &display, whitePoint);

    if (init_result == 0) {
        /* Test TIFFCIELabToXYZ */
        uint32_t L = read_uint32(&data_ptr, &remaining);
        int32_t a = read_int32(&data_ptr, &remaining);
        int32_t b = read_int32(&data_ptr, &remaining);
        float X, Y, Z;
        TIFFCIELabToXYZ(&cielab_rgb, L, a, b, &X, &Y, &Z);

        /* Test TIFFXYZToRGB */
        float X2 = read_float(&data_ptr, &remaining);
        float Y2 = read_float(&data_ptr, &remaining);
        float Z2 = read_float(&data_ptr, &remaining);
        uint32_t R, G, B;
        TIFFXYZToRGB(&cielab_rgb, X2, Y2, Z2, &R, &G, &B);
    }

    return 0;
}