// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(cmsContext ctx, double gamma) {
    return cmsBuildGamma(ctx, gamma);
}

static void free_tone_curve(cmsToneCurve* curve) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy white point and primaries
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE primaries = {
        {0.6400, 0.3300, 1.0},  // Red
        {0.3000, 0.6000, 1.0},  // Green
        {0.1500, 0.0600, 1.0}   // Blue
    };
    
    // Create tone curves using fuzzer data to influence gamma values
    double gamma_r = 2.2;
    double gamma_g = 2.2;
    double gamma_b = 2.2;
    
    if (Size >= 3) {
        gamma_r = 1.0 + (Data[0] / 255.0) * 4.0;  // Gamma between 1.0 and 5.0
        gamma_g = 1.0 + (Data[1] / 255.0) * 4.0;
        gamma_b = 1.0 + (Data[2] / 255.0) * 4.0;
    }
    
    cmsToneCurve* transfer[3] = {
        create_test_tone_curve(ctx, gamma_r),
        create_test_tone_curve(ctx, gamma_g),
        create_test_tone_curve(ctx, gamma_b)
    };
    
    if (!transfer[0] || !transfer[1] || !transfer[2]) {
        for (int i = 0; i < 3; i++) {
            free_tone_curve(transfer[i]);
        }
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsCreateRGBProfileTHR
    cmsHPROFILE rgbProfile = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transfer);
    if (!rgbProfile) {
        for (int i = 0; i < 3; i++) {
            free_tone_curve(transfer[i]);
        }
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Test cmsGetDeviceClass and cmsSetDeviceClass
    cmsProfileClassSignature originalClass = cmsGetDeviceClass(rgbProfile);
    
    // Use fuzzer data to select device class
    cmsProfileClassSignature newClass = cmsSigInputClass;
    if (Size >= 4) {
        switch (Data[3] % 6) {
            case 0: newClass = cmsSigInputClass; break;
            case 1: newClass = cmsSigDisplayClass; break;
            case 2: newClass = cmsSigOutputClass; break;
            case 3: newClass = cmsSigLinkClass; break;
            case 4: newClass = cmsSigAbstractClass; break;
            case 5: newClass = cmsSigColorSpaceClass; break;
        }
    }
    
    cmsSetDeviceClass(rgbProfile, newClass);
    cmsProfileClassSignature retrievedClass = cmsGetDeviceClass(rgbProfile);
    
    // Test cmsGetColorSpace and cmsSetColorSpace
    cmsColorSpaceSignature originalColorSpace = cmsGetColorSpace(rgbProfile);
    
    // Use fuzzer data to select color space
    cmsColorSpaceSignature newColorSpace = cmsSigRgbData;
    if (Size >= 5) {
        switch (Data[4] % 10) {
            case 0: newColorSpace = cmsSigRgbData; break;
            case 1: newColorSpace = cmsSigGrayData; break;
            case 2: newColorSpace = cmsSigCmykData; break;
            case 3: newColorSpace = cmsSigLabData; break;
            case 4: newColorSpace = cmsSigLuvData; break;
            case 5: newColorSpace = cmsSigYCbCrData; break;
            case 6: newColorSpace = cmsSigYxyData; break;
            case 7: newColorSpace = cmsSigHsvData; break;
            case 8: newColorSpace = cmsSigHlsData; break;
            case 9: newColorSpace = cmsSigMCH1Data; break;
        }
    }
    
    cmsSetColorSpace(rgbProfile, newColorSpace);
    cmsColorSpaceSignature retrievedColorSpace = cmsGetColorSpace(rgbProfile);
    
    // Test cmsCreateLinearizationDeviceLinkTHR
    cmsColorSpaceSignature linkColorSpace = cmsSigRgbData;
    if (Size >= 6) {
        switch (Data[5] % 5) {
            case 0: linkColorSpace = cmsSigRgbData; break;
            case 1: linkColorSpace = cmsSigGrayData; break;
            case 2: linkColorSpace = cmsSigCmykData; break;
            case 3: linkColorSpace = cmsSigLabData; break;
            case 4: linkColorSpace = cmsSigYCbCrData; break;
        }
    }
    
    cmsHPROFILE deviceLink = cmsCreateLinearizationDeviceLinkTHR(ctx, linkColorSpace, transfer);
    if (deviceLink) {
        // Test getters on device link profile
        cmsProfileClassSignature linkClass = cmsGetDeviceClass(deviceLink);
        cmsColorSpaceSignature linkSpace = cmsGetColorSpace(deviceLink);
        
        // Clean up device link
        cmsCloseProfile(deviceLink);
    }
    
    // Clean up
    cmsCloseProfile(rgbProfile);
    for (int i = 0; i < 3; i++) {
        free_tone_curve(transfer[i]);
    }
    cmsDeleteContext(ctx);
    
    return 0;
}