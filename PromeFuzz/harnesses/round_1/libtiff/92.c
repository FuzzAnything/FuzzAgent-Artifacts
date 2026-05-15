// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper macro to round up to multiple of alignment */
#define ROUNDUP_32(x, align) (((x) + (align) - 1) & ~((align) - 1))

static int read_int32(const uint8_t **data, size_t *size) {
    if (*size < sizeof(int32_t)) return 0;
    int32_t value;
    memcpy(&value, *data, sizeof(int32_t));
    *data += sizeof(int32_t);
    *size -= sizeof(int32_t);
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

static float read_float(const uint8_t **data, size_t *size) {
    if (*size < sizeof(float)) return 0.0f;
    float value;
    memcpy(&value, *data, sizeof(float));
    *data += sizeof(float);
    *size -= sizeof(float);
    return value;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    const uint8_t *ptr = Data;
    size_t remaining = Size;

    /* Allocate TIFFYCbCrToRGB buffer as per documentation */
    size_t ycbcr_size = ROUNDUP_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(ycbcr_size);
    if (!ycbcr) return 0;

    /* Allocate TIFFCIELabToRGB structure */
    TIFFCIELabToRGB *cielab = (TIFFCIELabToRGB *)_TIFFmalloc(sizeof(TIFFCIELabToRGB));
    if (!cielab) {
        _TIFFfree(ycbcr);
        return 0;
    }

    /* Allocate TIFFDisplay structure */
    TIFFDisplay *display = (TIFFDisplay *)_TIFFmalloc(sizeof(TIFFDisplay));
    if (!display) {
        _TIFFfree(ycbcr);
        _TIFFfree(cielab);
        return 0;
    }

    /* Initialize with fuzzer data */
    float luma[6];
    float refBlackWhite[6];
    for (int i = 0; i < 6 && remaining > 0; i++) {
        luma[i] = read_float(&ptr, &remaining);
        refBlackWhite[i] = read_float(&ptr, &remaining);
    }

    /* Initialize YCbCr to RGB conversion */
    TIFFYCbCrToRGBInit(ycbcr, luma, refBlackWhite);

    /* Initialize CIELab to RGB conversion */
    float whitePoint[3];
    for (int i = 0; i < 3 && remaining > 0; i++) {
        whitePoint[i] = read_float(&ptr, &remaining);
    }
    TIFFCIELabToRGBInit(cielab, display, whitePoint);

    /* Perform conversions using remaining fuzzer data */
    while (remaining >= sizeof(uint32_t) + 2 * sizeof(int32_t)) {
        uint32_t y = read_uint32(&ptr, &remaining);
        int32_t cb = read_int32(&ptr, &remaining);
        int32_t cr = read_int32(&ptr, &remaining);
        uint32_t r, g, b;

        /* YCbCr to RGB conversion */
        TIFFYCbCrtoRGB(ycbcr, y, cb, cr, &r, &g, &b);

        /* CIELab to XYZ conversion */
        float X, Y, Z;
        TIFFCIELabToXYZ(cielab, y, cb, cr, &X, &Y, &Z);

        /* XYZ to RGB24 conversion */
        float xyz[3] = {X, Y, Z};
        uint8_t rgb24[3];
        XYZtoRGB24(xyz, rgb24);

        /* XYZ to RGB conversion */
        uint32_t rgb_r, rgb_g, rgb_b;
        TIFFXYZToRGB(cielab, X, Y, Z, &rgb_r, &rgb_g, &rgb_b);
    }

    /* Cleanup */
    _TIFFfree(display);
    _TIFFfree(cielab);
    _TIFFfree(ycbcr);
    return 0;
}