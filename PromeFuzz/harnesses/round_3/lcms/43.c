// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx) {
    // Create a simple gamma curve
    return cmsBuildGamma(ctx, 2.2);
}

static void free_tone_curve(cmsToneCurve* curve, cmsContext ctx) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    // Initialize structures with data from fuzzer input
    cmsCIExyY whitePoint;
    cmsCIExyYTRIPLE primaries;
    cmsCIEXYZ xyz;
    cmsCIExyY dest_xyY;
    
    // Use fuzzer data to fill structures, with fallback defaults
    if (Size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, Data, sizeof(cmsCIExyY));
    } else {
        // Default D50 white point
        whitePoint.x = 0.3457;
        whitePoint.y = 0.3585;
        whitePoint.Y = 1.0;
    }
    
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsCIExyYTRIPLE)) {
        memcpy(&primaries, Data + sizeof(cmsCIExyY), sizeof(cmsCIExyYTRIPLE));
    } else {
        // Default sRGB primaries
        primaries.Red.x = 0.64;
        primaries.Red.y = 0.33;
        primaries.Red.Y = 1.0;
        primaries.Green.x = 0.30;
        primaries.Green.y = 0.60;
        primaries.Green.Y = 1.0;
        primaries.Blue.x = 0.15;
        primaries.Blue.y = 0.06;
        primaries.Blue.Y = 1.0;
    }
    
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsCIExyYTRIPLE) + sizeof(cmsCIEXYZ)) {
        memcpy(&xyz, Data + sizeof(cmsCIExyY) + sizeof(cmsCIExyYTRIPLE), sizeof(cmsCIEXYZ));
    } else {
        // Default XYZ values
        xyz.X = 0.9505;
        xyz.Y = 1.0000;
        xyz.Z = 1.0891;
    }
    
    // Create context for THR functions
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    // Create tone curves for transfer functions
    cmsToneCurve* gray_curve = create_dummy_tone_curve(NULL);
    cmsToneCurve* rgb_curve = create_dummy_tone_curve(ctx);
    cmsToneCurve* transfer_functions[3] = {rgb_curve, rgb_curve, rgb_curve};
    
    // Test cmsCreateGrayProfile
    cmsHPROFILE gray_profile = cmsCreateGrayProfile(&whitePoint, gray_curve);
    if (gray_profile) {
        cmsCloseProfile(gray_profile);
    }
    
    // Test cmsCreateLab2Profile
    cmsHPROFILE lab_profile = cmsCreateLab2Profile(&whitePoint);
    if (lab_profile) {
        cmsCloseProfile(lab_profile);
    }
    
    // Test cmsCreateRGBProfileTHR
    cmsHPROFILE rgb_profile_thr = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transfer_functions);
    if (rgb_profile_thr) {
        cmsCloseProfile(rgb_profile_thr);
    }
    
    // Test cmsXYZ2xyY
    cmsXYZ2xyY(&dest_xyY, &xyz);
    
    // Test cmsCreateRGBProfile
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(&whitePoint, &primaries, transfer_functions);
    if (rgb_profile) {
        cmsCloseProfile(rgb_profile);
    }
    
    // Test cmsCreateGrayProfileTHR
    cmsHPROFILE gray_profile_thr = cmsCreateGrayProfileTHR(ctx, &whitePoint, rgb_curve);
    if (gray_profile_thr) {
        cmsCloseProfile(gray_profile_thr);
    }
    
    // Cleanup
    free_tone_curve(gray_curve, NULL);
    free_tone_curve(rgb_curve, ctx);
    cmsDeleteContext(ctx);
    
    return 0;
}