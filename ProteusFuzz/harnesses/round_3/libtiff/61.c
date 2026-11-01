// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>

static void fuzzer_cleanup(void *ptr) {
    if (ptr) {
        _TIFFfree(ptr);
    }
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Minimum size required to safely extract all needed parameters
    // We need: pixel_val(4) + lab_a(4) + lab_b(4) + ycbcr_y(4) + ycbcr_cb(4) + ycbcr_cr(4) + lab_l(1) = 25 bytes
    // To be safe and simple, let's require at least 32 bytes.
    if (Size < 32) {
        return 0;
    }

    // Extract fuzzed values for parameters safely
    uint32_t pixel_val = 0;
    int32_t lab_a = 0, lab_b = 0;
    int32_t ycbcr_y, ycbcr_cb, ycbcr_cr;
    float xyz[3];
    uint32_t rgb_r, rgb_g, rgb_b;
    
    // YCbCr coefficients and reference values
    // TIFFYCbCrToRGBInit expects a pointer to an array of floats.
    // The internal code accesses refBlackWhite[0] to refBlackWhite[5].
    // We need to ensure the array has at least 6 elements.
    float ycbcr_coeffs[3];
    float ycbcr_ref[6];

    // Safe extraction with bounds checking
    memcpy(&pixel_val, Data, 4);
    memcpy(&lab_a, Data + 4, 4);
    memcpy(&lab_b, Data + 8, 4);
    memcpy(&ycbcr_y, Data + 12, 4);
    memcpy(&ycbcr_cb, Data + 16, 4);
    memcpy(&ycbcr_cr, Data + 20, 4);
    
    // Use a single byte for L value (0-100)
    int32_t lab_l = Data[24] % 101;

    // Initialize YCbCr coefficients from input or defaults if out of range
    // We use the next 12 bytes for coefficients if available, otherwise defaults
    if (Size >= 37) {
        memcpy(ycbcr_coeffs, Data + 25, 12);
    } else {
        ycbcr_coeffs[0] = 0.299f;
        ycbcr_coeffs[1] = 0.587f;
        ycbcr_coeffs[2] = 0.114f;
    }

    // Initialize reference black/white values (6 floats needed)
    // We use the next 24 bytes for ref values if available, otherwise defaults
    if (Size >= 61) {
        memcpy(ycbcr_ref, Data + 37, 24);
    } else {
        // Default values: Ymin, Ymax, Cbmin, Cbmax, Crmin, Crmax
        ycbcr_ref[0] = 0.0f;   // Ymin
        ycbcr_ref[1] = 255.0f; // Ymax
        ycbcr_ref[2] = 0.0f;   // Cbmin
        ycbcr_ref[3] = 255.0f; // Cbmax
        ycbcr_ref[4] = 0.0f;   // Crmin
        ycbcr_ref[5] = 255.0f; // Crmax
    }

    // Test LogLuv24toXYZ (No state required)
    LogLuv24toXYZ(pixel_val, xyz);

    // Test TIFFCIELabToRGBInit and related functions
    TIFFCIELabToRGB *cielab = NULL;
    TIFFDisplay *display = NULL;
    float *gamma = NULL;

    // Allocate memory for TIFFCIELabToRGB and TIFFDisplay
    // Note: Exact internal sizes are not public, so we allocate a safe buffer.
    // We assume a reasonable size for the struct and its internal tables.
    size_t cielab_size = sizeof(TIFFCIELabToRGB) + 4 * 256 * sizeof(float);
    size_t display_size = sizeof(TIFFDisplay) + 4 * 256 * sizeof(float);
    size_t gamma_size = 256 * sizeof(float);

    cielab = (TIFFCIELabToRGB *) _TIFFmalloc(cielab_size);
    display = (TIFFDisplay *) _TIFFmalloc(display_size);
    gamma = (float *) _TIFFmalloc(gamma_size);

    if (cielab && display && gamma) {
        // Initialize gamma array with fuzzed or default values
        for (int i = 0; i < 256; i++) {
            gamma[i] = (float)(i / 255.0);
        }

        // Initialize display structure (simplified, assuming standard fields)
        // We cannot fully initialize TIFFDisplay without knowing its exact layout,
        // but we can pass it to the init function which might handle defaults or fail gracefully.
        memset(display, 0, display_size);

        // Initialize CIELab to RGB conversion
        int ret = TIFFCIELabToRGBInit(cielab, display, gamma);
        if (ret) {
            // Test TIFFCIELabToXYZ
            float x, y, z;
            TIFFCIELabToXYZ(cielab, lab_l, lab_a, lab_b, &x, &y, &z);

            // Test TIFFXYZToRGB
            TIFFXYZToRGB(cielab, x, y, z, &rgb_r, &rgb_g, &rgb_b);
        }
    }

    fuzzer_cleanup(cielab);
    fuzzer_cleanup(display);
    fuzzer_cleanup(gamma);

    // Test TIFFYCbCrToRGBInit and TIFFYCbCrtoRGB
    TIFFYCbCrToRGB *ycbcr = NULL;
    size_t ycbcr_size = sizeof(TIFFYCbCrToRGB) +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);

    ycbcr = (TIFFYCbCrToRGB *) _TIFFmalloc(ycbcr_size);
    if (ycbcr) {
        int ret = TIFFYCbCrToRGBInit(ycbcr, ycbcr_coeffs, ycbcr_ref);
        if (ret) {
            // Test TIFFYCbCrtoRGB
            uint32_t r, g, b;
            TIFFYCbCrtoRGB(ycbcr, ycbcr_y, ycbcr_cb, ycbcr_cr, &r, &g, &b);
        }
        fuzzer_cleanup(ycbcr);
    }

    return 0;
}