// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tiffio.h>

static void fuzz_TIFFCIELabToRGB(const uint8_t *Data, size_t Size) {
    TIFFCIELabToRGB *cielab = NULL;
    TIFFDisplay display = {0};
    float whitepoint[3] = {1.0f, 1.0f, 1.0f};
    float xyz[3];
    uint32_t r, g, b;

    // Allocate memory for TIFFCIELabToRGB structure
    // Note: The exact size is internal, but we allocate a safe buffer.
    // In real usage, libtiff might allocate internally or expect specific sizing.
    // Here we assume the struct pointer is valid after init.
    // Since the internal structure size is opaque, we rely on the init function 
    // to handle allocation or we allocate a large buffer if the struct is embedded.
    // Based on libtiff source, TIFFCIELabToRGB usually requires malloc inside init or 
    // the user allocates a specific size. Let's try to allocate a reasonable buffer.
    // However, TIFFCIELabToRGBInit takes a pointer. If it allocates internally, we pass a pointer to a pointer?
    // No, the signature is TIFFCIELabToRGB *. It likely expects a pre-allocated struct or allocates members.
    // Let's allocate a large buffer to be safe for internal tables.
    cielab = (TIFFCIELabToRGB *)malloc(sizeof(TIFFCIELabToRGB) + 4096);
    if (!cielab) return;
    memset(cielab, 0, sizeof(TIFFCIELabToRGB) + 4096);

    // Initialize display parameters if needed
    // We don't have full control over TIFFDisplay fields, so we zero it.
    
    int init_result = TIFFCIELabToRGBInit(cielab, &display, whitepoint);
    
    if (init_result == 0) {
        // Init failed, try to free and return
        free(cielab);
        return;
    }

    // Fuzz TIFFCIELabToXYZ
    if (Size >= 12) {
        uint32_t l = (Data[0] << 16) | (Data[1] << 8) | Data[2];
        int32_t a = (int32_t)((Data[3] << 16) | (Data[4] << 8) | Data[5]);
        int32_t b_val = (int32_t)((Data[6] << 16) | (Data[7] << 8) | Data[8]);
        
        // Clamp inputs to valid ranges to avoid undefined behavior in math
        if (l > 100) l = 100;
        if (a < -128) a = -128;
        if (a > 127) a = 127;
        if (b_val < -128) b_val = -128;
        if (b_val > 127) b_val = 127;

        TIFFCIELabToXYZ(cielab, l, a, b_val, &xyz[0], &xyz[1], &xyz[2]);

        // Fuzz TIFFXYZToRGB
        TIFFXYZToRGB(cielab, xyz[0], xyz[1], xyz[2], &r, &g, &b);
    }

    free(cielab);
}

static void fuzz_TIFFYCbCrToRGB(const uint8_t *Data, size_t Size) {
    // Allocate buffer for TIFFYCbCrToRGB as per documentation
    // TIFFroundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) + 4 * 256 * sizeof(TIFFRGBValue) + 2 * 256 * sizeof(int) + 3 * 256 * sizeof(int32_t)
    // TIFFRGBValue is typically uint16 or uint32, let's assume uint32 for safety or check typedef.
    // Usually TIFFRGBValue is uint16. Let's use a safe large allocation.
    size_t ycbcr_size = sizeof(TIFFYCbCrToRGB) + 4 * 256 * sizeof(uint32_t) + 2 * 256 * sizeof(int) + 3 * 256 * sizeof(int32_t);
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)malloc(ycbcr_size);
    if (!ycbcr) return;
    memset(ycbcr, 0, ycbcr_size);

    float luma_coeffs[3] = {0.299f, 0.587f, 0.114f}; // Standard BT.601
    float ref_black_white[2] = {0.0f, 255.0f};

    int init_result = TIFFYCbCrToRGBInit(ycbcr, luma_coeffs, ref_black_white);
    if (init_result == 0) {
        free(ycbcr);
        return;
    }

    // Fuzz TIFFYCbCrtoRGB
    if (Size >= 3) {
        uint32_t y = Data[0];
        int32_t cb = (int32_t)Data[1];
        int32_t cr = (int32_t)Data[2];
        uint32_t r, g, b;

        TIFFYCbCrtoRGB(ycbcr, y, cb, cr, &r, &g, &b);
    }

    free(ycbcr);
}

static void fuzz_LogLuv24toXYZ(const uint8_t *Data, size_t Size) {
    float xyz[3];
    uint32_t pixel;

    if (Size >= 4) {
        pixel = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        LogLuv24toXYZ(pixel, xyz);
    } else {
        // Fuzz with random data if size is small
        pixel = 0;
        if (Size > 0) pixel |= Data[0];
        if (Size > 1) pixel |= (Data[1] << 8);
        if (Size > 2) pixel |= (Data[2] << 16);
        if (Size > 3) pixel |= (Data[3] << 24);
        
        LogLuv24toXYZ(pixel, xyz);
    }
}

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use the first byte to decide which function to fuzz
    uint8_t mode = Data[0];

    switch (mode % 4) {
        case 0:
            fuzz_TIFFCIELabToRGB(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 1:
            fuzz_TIFFYCbCrToRGB(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 2:
            fuzz_LogLuv24toXYZ(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
        case 3:
            // Combine all for more coverage
            fuzz_TIFFCIELabToRGB(Data + 1, Size > 1 ? Size - 1 : 0);
            fuzz_TIFFYCbCrToRGB(Data + 1, Size > 1 ? Size - 1 : 0);
            fuzz_LogLuv24toXYZ(Data + 1, Size > 1 ? Size - 1 : 0);
            break;
    }

    return 0;
}