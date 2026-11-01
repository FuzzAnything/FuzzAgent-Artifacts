// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCreateNULLProfileTHR at cmsvirt.c:960:23 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsContext create_fuzz_context(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return NULL;
    return ctx;
}

static void cleanup_profiles(cmsHPROFILE p1, cmsHPROFILE p2, cmsHPROFILE p3, 
                             cmsHPROFILE p4, cmsHPROFILE p5, cmsHPROFILE p6,
                             cmsContext ctx) {
    if (p1) cmsCloseProfile(p1);
    if (p2) cmsCloseProfile(p2);
    if (p3) cmsCloseProfile(p3);
    if (p4) cmsCloseProfile(p4);
    if (p5) cmsCloseProfile(p5);
    if (p6) cmsCloseProfile(p6);
    if (ctx) cmsDeleteContext(ctx);
}

int LLVMFuzzerTestOneInput_182(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsContext ctx = create_fuzz_context(Data, Size);
    if (ctx == NULL) return 0;

    cmsHPROFILE p1 = NULL, p2 = NULL, p3 = NULL, p4 = NULL, p5 = NULL, p6 = NULL;
    
    // Prepare data for cmsCreateGrayProfileTHR
    cmsCIExyY whitePoint = {0.3127, 0.3290, 1.0}; // D65
    cmsToneCurve *toneCurve = cmsBuildGamma(ctx, 1.0); // Identity curve
    if (toneCurve != NULL) {
        p1 = cmsCreateGrayProfileTHR(ctx, &whitePoint, toneCurve);
        cmsFreeToneCurve(toneCurve);
    }

    // cmsCreate_OkLabProfile
    p2 = cmsCreate_OkLabProfile(ctx);

    // cmsCreateNULLProfileTHR
    p3 = cmsCreateNULLProfileTHR(ctx);

    // cmsCreateLab2ProfileTHR
    p4 = cmsCreateLab2ProfileTHR(ctx, &whitePoint);

    // Prepare data for cmsCreateRGBProfileTHR
    cmsCIExyYTRIPLE primaries = {
        {0.64, 0.33, 1.0}, // Red
        {0.30, 0.60, 1.0}, // Green
        {0.15, 0.06, 1.0}  // Blue
    };
    cmsToneCurve *transfer[3] = {NULL, NULL, NULL};
    for (int i = 0; i < 3; i++) {
        transfer[i] = cmsBuildGamma(ctx, 1.0);
    }
    if (transfer[0] && transfer[1] && transfer[2]) {
        p5 = cmsCreateRGBProfileTHR(ctx, &whitePoint, &primaries, transfer);
        cmsFreeToneCurve(transfer[0]);
        cmsFreeToneCurve(transfer[1]);
        cmsFreeToneCurve(transfer[2]);
    }

    // cmsCreateLab4ProfileTHR
    p6 = cmsCreateLab4ProfileTHR(ctx, &whitePoint);

    // Cleanup
    cleanup_profiles(p1, p2, p3, p4, p5, p6, ctx);

    return 0;
}