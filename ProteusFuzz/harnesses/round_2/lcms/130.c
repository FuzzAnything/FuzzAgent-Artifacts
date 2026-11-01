// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE p1, cmsHPROFILE p2, cmsHPROFILE p3, cmsHPROFILE p4, cmsHPROFILE p5, cmsHPROFILE p6) {
    if (p1) cmsCloseProfile(p1);
    if (p2) cmsCloseProfile(p2);
    if (p3) cmsCloseProfile(p3);
    if (p4) cmsCloseProfile(p4);
    if (p5) cmsCloseProfile(p5);
    if (p6) cmsCloseProfile(p6);
}

static void cleanup_curves(cmsContext ctx, cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;

    cmsHPROFILE p1 = NULL;
    cmsHPROFILE p2 = NULL;
    cmsHPROFILE p3 = NULL;
    cmsHPROFILE p4 = NULL;
    cmsHPROFILE p5 = NULL;
    cmsHPROFILE p6 = NULL;

    // 1. cmsCreate_OkLabProfile
    p1 = cmsCreate_OkLabProfile(ctx);

    // Prepare data for other functions based on input
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65 default
    cmsCIExyY whitePointVar = {0.0, 0.0, 0.0};
    
    // Fuzz white point if enough data
    if (Size >= 12) {
        memcpy(&whitePointVar, Data, 12);
        // Clamp values to reasonable ranges to avoid immediate NaN/Inf issues if possible
        if (whitePointVar.x < 0 || whitePointVar.x > 1) whitePointVar.x = 0.5;
        if (whitePointVar.y < 0 || whitePointVar.y > 1) whitePointVar.y = 0.5;
        if (whitePointVar.Y < 0 || whitePointVar.Y > 1) whitePointVar.Y = 0.5;
    }

    // 2. cmsCreateGrayProfileTHR
    cmsToneCurve *toneCurve = NULL;
    toneCurve = cmsBuildGamma(ctx, 2.2);
    if (toneCurve) {
        p2 = cmsCreateGrayProfileTHR(ctx, &whitePointVar, toneCurve);
        cmsFreeToneCurve(toneCurve);
    } else {
        // Fallback with default white point if curve creation failed
        toneCurve = cmsBuildGamma(ctx, 1.0);
        if (toneCurve) {
            p2 = cmsCreateGrayProfileTHR(ctx, &whitePoint, toneCurve);
            cmsFreeToneCurve(toneCurve);
        }
    }

    // 3. cmsCreateRGBProfileTHR
    cmsCIExyYTRIPLE primaries = {{{0.64, 0.33, 1.0}, {0.30, 0.60, 1.0}, {0.15, 0.06, 1.0}}}; // sRGB
    cmsToneCurve *rgbCurves[3] = {NULL, NULL, NULL};
    
    // Try to create curves from input data if possible, otherwise defaults
    if (Size >= 4) {
        double gamma = (double)(Data[0] & 0xFF) / 255.0 + 0.1;
        rgbCurves[0] = cmsBuildGamma(ctx, gamma);
        rgbCurves[1] = cmsBuildGamma(ctx, gamma);
        rgbCurves[2] = cmsBuildGamma(ctx, gamma);
    }
    
    if (rgbCurves[0] && rgbCurves[1] && rgbCurves[2]) {
        p3 = cmsCreateRGBProfileTHR(ctx, &whitePointVar, &primaries, rgbCurves);
        cleanup_curves(ctx, rgbCurves[0], rgbCurves[1], rgbCurves[2]);
        rgbCurves[0] = rgbCurves[1] = rgbCurves[2] = NULL;
    } else {
        // Fallback with defaults
        rgbCurves[0] = cmsBuildGamma(ctx, 2.2);
        rgbCurves[1] = cmsBuildGamma(ctx, 2.2);
        rgbCurves[2] = cmsBuildGamma(ctx, 2.2);
        if (rgbCurves[0] && rgbCurves[1] && rgbCurves[2]) {
            p3 = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, rgbCurves);
            cleanup_curves(ctx, rgbCurves[0], rgbCurves[1], rgbCurves[2]);
            rgbCurves[0] = rgbCurves[1] = rgbCurves[2] = NULL;
        }
    }

    // 4. cmsCreateLab2ProfileTHR
    p4 = cmsCreateLab2ProfileTHR(ctx, &whitePointVar);
    if (!p4) {
        p4 = cmsCreateLab2ProfileTHR(ctx, NULL); // Try with NULL
    }

    // 5. cmsCreateNULLProfileTHR
    p5 = cmsCreateNULLProfileTHR(ctx);

    // 6. cmsCreateLab4ProfileTHR
    p6 = cmsCreateLab4ProfileTHR(ctx, &whitePointVar);
    if (!p6) {
        p6 = cmsCreateLab4ProfileTHR(ctx, NULL); // Try with NULL
    }

    cleanup_profiles(p1, p2, p3, p4, p5, p6);
    cmsDeleteContext(ctx);

    return 0;
}