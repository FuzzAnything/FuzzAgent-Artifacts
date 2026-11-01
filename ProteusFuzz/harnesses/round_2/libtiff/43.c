// This fuzz driver is generated for library libtiff, aiming to fuzz the following functions:
// TIFFYCbCrtoRGB at tif_color.c:199:6 in tiffio.h
// TIFFCIELabToRGBInit at tif_color.c:135:5 in tiffio.h
// TIFFCIELabToXYZ at tif_color.c:43:6 in tiffio.h
// TIFFXYZToRGB at tif_color.c:89:6 in tiffio.h
// LogLuv24toXYZ at tif_luv.c:1032:5 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFfree at tif_unix.c:349:6 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// _TIFFmalloc at tif_unix.c:333:7 in tiffio.h
// TIFFYCbCrToRGBInit at tif_color.c:251:5 in tiffio.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <tiffio.h>
#include <tiff.h>

/* 
 * Fuzz driver for libtiff color conversion functions.
 * Targets: TIFFCIELabToXYZ, LogLuv24toXYZ, TIFFCIELabToRGBInit,
 *          TIFFYCbCrtoRGB, TIFFYCbCrToRGBInit, TIFFXYZToRGB.
 */

static void cleanup_ycc(TIFFYCbCrToRGB *ycbcr) {
    if (ycbcr) {
        _TIFFfree(ycbcr);
    }
}

static void cleanup_lab(TIFFCIELabToRGB *lab) {
    if (lab) {
        _TIFFfree(lab);
    }
}

/* Helper to replicate TIFFroundup_32 if not available in public headers */
static size_t my_tiff_roundup_32(size_t n, size_t align) {
    return (n + align - 1) & ~(align - 1);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    /* 
     * Prepare TIFFYCbCrToRGB structure and buffer.
     */
    size_t ycbcr_size = my_tiff_roundup_32(sizeof(TIFFYCbCrToRGB), sizeof(long)) +
                        4 * 256 * sizeof(TIFFRGBValue) +
                        2 * 256 * sizeof(int) +
                        3 * 256 * sizeof(int32_t);
    
    TIFFYCbCrToRGB *ycbcr = (TIFFYCbCrToRGB *)_TIFFmalloc(ycbcr_size);
    if (!ycbcr) {
        return 0;
    }
    memset(ycbcr, 0, ycbcr_size);

    /* 
     * Prepare TIFFCIELabToRGB structure.
     */
    TIFFCIELabToRGB *lab = (TIFFCIELabToRGB *)_TIFFmalloc(sizeof(TIFFCIELabToRGB) + 1024);
    if (!lab) {
        cleanup_ycc(ycbcr);
        return 0;
    }
    memset(lab, 0, sizeof(TIFFCIELabToRGB) + 1024);

    /* 
     * Prepare TIFFDisplay structure for TIFFCIELabToRGBInit.
     */
    TIFFDisplay display;
    memset(&display, 0, sizeof(display));
    
    /* 
     * Fix: libtiff tif_color.c accesses refBlackWhite[5] (6 elements total).
     * The previous driver only allocated 5 elements, causing a buffer overflow.
     * We now allocate 6 floats to safely satisfy the library's access pattern.
     */
    float luma_coeffs[3] = {0.299f, 0.587f, 0.114f};
    float ref_data[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}; 
    
    if (Size >= 10) {
        luma_coeffs[0] = (float)(Data[0] & 0xFF) / 255.0f;
        luma_coeffs[1] = (float)(Data[1] & 0xFF) / 255.0f;
        luma_coeffs[2] = (float)(Data[2] & 0xFF) / 255.0f;
        
        ref_data[0] = (float)(Data[3] & 0xFF) / 255.0f;
        ref_data[1] = (float)(Data[4] & 0xFF) / 255.0f;
        ref_data[2] = (float)(Data[5] & 0xFF) / 255.0f;
        ref_data[3] = (float)(Data[6] & 0xFF) / 255.0f;
        ref_data[4] = (float)(Data[7] & 0xFF) / 255.0f;
        ref_data[5] = (float)(Data[8] & 0xFF) / 255.0f; // Added index 5
    }

    /* 
     * 1. Test TIFFYCbCrToRGBInit
     */
    int ycc_init_result = TIFFYCbCrToRGBInit(ycbcr, luma_coeffs, ref_data);

    /* 
     * 2. Test TIFFYCbCrtoRGB
     */
    uint32_t Y, Cb, Cr;
    uint32_t R, G, B;
    
    if (Size >= 13) { // Adjusted size check due to index shift
        Y = (uint32_t)(Data[9] & 0xFF);
        Cb = (uint32_t)(Data[10] & 0xFF);
        Cr = (uint32_t)(Data[11] & 0xFF);
        
        Y = Y & 0xFF;
        Cb = Cb & 0xFF;
        Cr = Cr & 0xFF;
        
        TIFFYCbCrtoRGB(ycbcr, Y, Cb, Cr, &R, &G, &B);
    }

    /* 
     * 3. Test TIFFCIELabToRGBInit
     */
    float white_point[3] = {1.0f, 1.0f, 1.0f};
    if (Size >= 16) {
        white_point[0] = (float)(Data[12] & 0xFF) / 255.0f;
        white_point[1] = (float)(Data[13] & 0xFF) / 255.0f;
        white_point[2] = (float)(Data[14] & 0xFF) / 255.0f;
    }
    
    int lab_init_result = TIFFCIELabToRGBInit(lab, &display, white_point);

    /* 
     * 4. Test TIFFCIELabToXYZ
     */
    float X, Y_val, Z;
    uint32_t L;
    int32_t a, b;

    if (Size >= 19) {
        L = (uint32_t)(Data[15] & 0xFF);
        L = (L * 100) / 255;
        
        a = (int32_t)(Data[16] & 0xFF) - 128;
        b = (int32_t)(Data[17] & 0xFF) - 128;
        
        TIFFCIELabToXYZ(lab, L, a, b, &X, &Y_val, &Z);
    }

    /* 
     * 5. Test TIFFXYZToRGB
     */
    uint32_t R_out, G_out, B_out;
    float X_in, Y_in, Z_in;
    
    if (Size >= 22) {
        X_in = (float)(Data[18] & 0xFF) / 255.0f;
        Y_in = (float)(Data[19] & 0xFF) / 255.0f;
        Z_in = (float)(Data[20] & 0xFF) / 255.0f;
        
        TIFFXYZToRGB(lab, X_in, Y_in, Z_in, &R_out, &G_out, &B_out);
    }

    /* 
     * 6. Test LogLuv24toXYZ
     */
    uint32_t logluv_pixel;
    float xyz_out[3];
    
    if (Size >= 25) {
        logluv_pixel = ((uint32_t)Data[21] << 24) | ((uint32_t)Data[22] << 16) | 
                       ((uint32_t)Data[23] << 8) | ((uint32_t)Data[24] & 0xFF);
        
        LogLuv24toXYZ(logluv_pixel, xyz_out);
    }

    /* Cleanup */
    cleanup_ycc(ycbcr);
    cleanup_lab(lab);

    return 0;
}