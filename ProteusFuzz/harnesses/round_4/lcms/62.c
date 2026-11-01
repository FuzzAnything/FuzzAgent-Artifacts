// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

static cmsToneCurve* create_identity_curve_from_fuzz(cmsContext ctx, const uint8_t *data, size_t size) {
    cmsToneCurve *curve = NULL;
    if (size < 2) return NULL;
    
    // Use first two bytes to determine curve type or just create a simple table
    cmsUInt16Number table[256];
    cmsUInt32Number nEntries = 256;
    
    // Initialize with identity or simple variation based on input
    for (int i = 0; i < 256; i++) {
        if (size > 0) {
            table[i] = (data[i % size] * 256) / 255;
        } else {
            table[i] = i;
        }
    }
    
    if (ctx != NULL) {
        curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, table);
    } else {
        curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    }
    
    return curve;
}

static void cleanup_curves(cmsToneCurve *curves[3]) {
    for (int i = 0; i < 3; i++) {
        if (curves[i] != NULL) {
            cmsFreeToneCurve(curves[i]);
        }
    }
}

static void cleanup_curves_gray(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Test cmsD50_xyY - no arguments, just call it
    const cmsCIExyY *d50 = cmsD50_xyY();
    if (d50 == NULL) {
        // This should not happen, but handle gracefully
        return 0;
    }

    // Initialize context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        // Fallback to NULL context if creation fails
        ctx = NULL;
    }

    // Prepare primaries for RGB profile
    cmsCIExyYTRIPLE primaries;
    memset(&primaries, 0, sizeof(primaries));
    
    // Use fuzz data to vary primaries slightly, but keep them valid-ish
    if (Size >= 12) {
        primaries.Red.x = (Data[0] / 255.0) * 0.7;
        primaries.Red.y = (Data[1] / 255.0) * 0.7;
        primaries.Green.x = (Data[2] / 255.0) * 0.7;
        primaries.Green.y = (Data[3] / 255.0) * 0.7;
        primaries.Blue.x = (Data[4] / 255.0) * 0.7;
        primaries.Blue.y = (Data[5] / 255.0) * 0.7;
    } else {
        // Default sRGB-like primaries
        primaries.Red.x = 0.64; primaries.Red.y = 0.33;
        primaries.Green.x = 0.30; primaries.Green.y = 0.60;
        primaries.Blue.x = 0.15; primaries.Blue.y = 0.06;
    }
    
    // Ensure Y is 1.0 for all
    primaries.Red.Y = 1.0;
    primaries.Green.Y = 1.0;
    primaries.Blue.Y = 1.0;

    // Create tone curves
    cmsToneCurve *rgb_curves[3] = {NULL, NULL, NULL};
    cmsToneCurve *gray_curve = NULL;

    // Create RGB curves
    for (int i = 0; i < 3; i++) {
        size_t offset = i * (Size / 3);
        size_t len = Size / 3;
        if (i == 2) len = Size - offset; // Take remainder
        if (len > 0) {
            rgb_curves[i] = create_identity_curve_from_fuzz(ctx, Data + offset, len);
        }
    }

    // Create gray curve
    if (Size > 0) {
        gray_curve = create_identity_curve_from_fuzz(ctx, Data, Size);
    }

    // Test cmsCreateRGBProfileTHR
    cmsHPROFILE rgb_profile_thr = NULL;
    if (d50 != NULL && rgb_curves[0] != NULL && rgb_curves[1] != NULL && rgb_curves[2] != NULL) {
        rgb_profile_thr = cmsCreateRGBProfileTHR(ctx, d50, &primaries, rgb_curves);
    }
    cleanup_profile(rgb_profile_thr);

    // Test cmsCreateRGBProfile
    cmsHPROFILE rgb_profile = NULL;
    if (d50 != NULL && rgb_curves[0] != NULL && rgb_curves[1] != NULL && rgb_curves[2] != NULL) {
        rgb_profile = cmsCreateRGBProfile(d50, &primaries, rgb_curves);
    }
    cleanup_profile(rgb_profile);

    // Test cmsCreateGrayProfileTHR
    cmsHPROFILE gray_profile_thr = NULL;
    if (d50 != NULL && gray_curve != NULL) {
        gray_profile_thr = cmsCreateGrayProfileTHR(ctx, d50, gray_curve);
    }
    cleanup_profile(gray_profile_thr);

    // Test cmsCreateGrayProfile
    cmsHPROFILE gray_profile = NULL;
    if (d50 != NULL && gray_curve != NULL) {
        gray_profile = cmsCreateGrayProfile(d50, gray_curve);
    }
    cleanup_profile(gray_profile);

    // Test cmsCreateLab4Profile
    cmsHPROFILE lab_profile = NULL;
    if (d50 != NULL) {
        lab_profile = cmsCreateLab4Profile(d50);
    }
    cleanup_profile(lab_profile);

    // Cleanup curves
    cleanup_curves(rgb_curves);
    cleanup_curves_gray(gray_curve);

    // Cleanup context
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }

    return 0;
}