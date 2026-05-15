// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFSetWarningHandler at tif_warning.c:32:18 in tiffio.h
// TIFFSetErrorHandler at tif_error.c:32:18 in tiffio.h
// LogLuv24fromXYZ at tif_luv.c:1063:5 in tiffio.h
// XYZtoRGB24 at tif_luv.c:865:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "libtiff/tiffio.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef TIFFroundup_32
#define TIFFroundup_32(x, y) ((((x) + ((y) - 1)) / (y)) * (y))
#endif

static void dummy_handler(const char* module, const char* fmt, va_list ap) {
    // Suppress libtiff warnings/errors during fuzzing
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Suppress libtiff warnings
    TIFFSetWarningHandler(dummy_handler);
    TIFFSetErrorHandler(dummy_handler);

    // Function 1: LogLuv24fromXYZ
    if (Size >= sizeof(float) * 3 + sizeof(int)) {
        float xyz[3];
        int em;
        memcpy(xyz, Data, sizeof(float) * 3);
        memcpy(&em, Data + sizeof(float) * 3, sizeof(int));
        LogLuv24fromXYZ(xyz, em);
    }

    // Function 2: XYZtoRGB24
    if (Size >= sizeof(float) * 3) {
        float xyz[3];
        uint8_t rgb[3];
        memcpy(xyz, Data, sizeof(float) * 3);
        XYZtoRGB24(xyz, rgb);
    }

    // Function 3: TIFFYCbCrToRGBInit
    if (Size >= sizeof(float) * 6) {
        // Allocate buffer as per documentation
        size_t buffer_size = TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                             4 * 256 * sizeof(TIFFRGBValue) +
                             2 * 256 * sizeof(int) +
                             3 * 256 * sizeof(int32_t);
        TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(buffer_size);
        if (ycbcr) {
            float refWhite[3];
            float refBlack[3];
            memcpy(refWhite, Data, sizeof(float) * 3);
            memcpy(refBlack, Data + sizeof(float) * 3, sizeof(float) * 3);
            TIFFYCbCrToRGBInit(ycbcr, refWhite, refBlack);
            _TIFFfree(ycbcr);
        }
    }

    // Functions 4,5,6 require initialized structures
    // Allocate TIFFCIELabToRGB structure
    TIFFCIELabToRGB *labToRGB = (TIFFCIELabToRGB *)_TIFFmalloc(sizeof(TIFFCIELabToRGB));
    if (labToRGB) {
        // Function 6: TIFFCIELabToRGBInit
        if (Size >= sizeof(TIFFDisplay) + sizeof(float) * 3) {
            TIFFDisplay display;
            float whitePoint[3];
            memcpy(&display, Data, sizeof(TIFFDisplay));
            memcpy(whitePoint, Data + sizeof(TIFFDisplay), sizeof(float) * 3);
            TIFFCIELabToRGBInit(labToRGB, &display, whitePoint);
        }

        // Function 5: TIFFCIELabToXYZ (requires initialized labToRGB)
        if (Size >= sizeof(uint32_t) + sizeof(int32_t) * 2 + sizeof(float) * 3) {
            uint32_t L;
            int32_t a, b;
            float X, Y, Z;
            memcpy(&L, Data, sizeof(uint32_t));
            memcpy(&a, Data + sizeof(uint32_t), sizeof(int32_t));
            memcpy(&b, Data + sizeof(uint32_t) + sizeof(int32_t), sizeof(int32_t));
            TIFFCIELabToXYZ(labToRGB, L, a, b, &X, &Y, &Z);
        }

        // Function 4: TIFFXYZToRGB (requires initialized labToRGB)
        if (Size >= sizeof(float) * 3 + sizeof(uint32_t) * 3) {
            float X, Y, Z;
            uint32_t R, G, B;
            memcpy(&X, Data, sizeof(float));
            memcpy(&Y, Data + sizeof(float), sizeof(float));
            memcpy(&Z, Data + sizeof(float) * 2, sizeof(float));
            TIFFXYZToRGB(labToRGB, X, Y, Z, &R, &G, &B);
        }

        _TIFFfree(labToRGB);
    }

    return 0;
}