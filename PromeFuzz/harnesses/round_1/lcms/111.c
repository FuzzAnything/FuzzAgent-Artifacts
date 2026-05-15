// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreate_OkLabProfile at cmsvirt.c:690:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(cmsContext ctx) {
    // Create a simple gamma tone curve for testing
    return cmsBuildGamma(ctx, 2.2);
}

static void generate_test_white_point(const uint8_t* data, size_t size, cmsCIExyY* wp) {
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(wp, data, sizeof(cmsCIExyY));
    } else {
        // Use D50 as default if not enough data
        const cmsCIExyY* d50 = cmsD50_xyY();
        *wp = *d50;
    }
}

static void generate_test_primaries(const uint8_t* data, size_t size, cmsCIExyYTRIPLE* prim) {
    if (size >= sizeof(cmsCIExyYTRIPLE)) {
        memcpy(prim, data, sizeof(cmsCIExyYTRIPLE));
    } else {
        // Use sRGB primaries as default
        prim->Red.x = 0.6400;
        prim->Red.y = 0.3300;
        prim->Red.Y = 1.0;
        prim->Green.x = 0.3000;
        prim->Green.y = 0.6000;
        prim->Green.Y = 1.0;
        prim->Blue.x = 0.1500;
        prim->Blue.y = 0.0600;
        prim->Blue.Y = 1.0;
    }
}

int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    cmsContext ctx = NULL;
    cmsHPROFILE profile = NULL;
    
    // Create a context for functions that need it
    ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) {
        return 0;
    }
    
    // Test 1: cmsCreate_OkLabProfile
    profile = cmsCreate_OkLabProfile(ctx);
    if (profile) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    
    // Test 2: cmsCreateLab2Profile
    cmsCIExyY whitePoint1;
    generate_test_white_point(Data, Size, &whitePoint1);
    profile = cmsCreateLab2Profile(&whitePoint1);
    if (profile) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    
    // Test 3: cmsD50_xyY (just call it)
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50; // Use the result to avoid unused variable warning
    
    // Test 4: cmsCreateRGBProfileTHR
    cmsCIExyY whitePoint2;
    cmsCIExyYTRIPLE primaries;
    cmsToneCurve* transfer[3] = {NULL, NULL, NULL};
    
    generate_test_white_point(Data, Size, &whitePoint2);
    generate_test_primaries(Data, Size, &primaries);
    
    // Create tone curves
    for (int i = 0; i < 3; i++) {
        transfer[i] = create_test_tone_curve(ctx);
    }
    
    profile = cmsCreateRGBProfileTHR(ctx, &whitePoint2, &primaries, transfer);
    if (profile) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    
    // Clean up tone curves
    for (int i = 0; i < 3; i++) {
        if (transfer[i]) {
            cmsFreeToneCurve(transfer[i]);
        }
    }
    
    // Test 5: cmsCreateLab4ProfileTHR
    cmsCIExyY whitePoint3;
    generate_test_white_point(Data, Size, &whitePoint3);
    profile = cmsCreateLab4ProfileTHR(ctx, &whitePoint3);
    if (profile) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    
    // Test 6: cmsCreateLab4Profile
    cmsCIExyY whitePoint4;
    generate_test_white_point(Data, Size, &whitePoint4);
    profile = cmsCreateLab4Profile(&whitePoint4);
    if (profile) {
        cmsCloseProfile(profile);
        profile = NULL;
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}