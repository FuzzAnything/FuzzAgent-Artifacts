// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateGrayProfile at cmsvirt.c:280:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(void) {
    // Create a simple gamma tone curve for testing
    return cmsBuildGamma(NULL, 2.2);
}

static cmsCIExyY create_test_xyY(const uint8_t *data, size_t size) {
    cmsCIExyY xyY;
    
    // Use fuzzer data to initialize xyY values if available
    if (size >= sizeof(cmsCIExyY)) {
        memcpy(&xyY, data, sizeof(cmsCIExyY));
    } else {
        // Default values if not enough data
        xyY.x = 0.3127;
        xyY.y = 0.3290;
        xyY.Y = 1.0;
    }
    
    return xyY;
}

static cmsCIExyYTRIPLE create_test_primaries(const uint8_t *data, size_t size) {
    cmsCIExyYTRIPLE primaries;
    
    // Use fuzzer data to initialize primaries if available
    if (size >= sizeof(cmsCIExyYTRIPLE)) {
        memcpy(&primaries, data, sizeof(cmsCIExyYTRIPLE));
    } else {
        // Default sRGB primaries
        primaries.Red.x = 0.6400;
        primaries.Red.y = 0.3300;
        primaries.Red.Y = 1.0;
        
        primaries.Green.x = 0.3000;
        primaries.Green.y = 0.6000;
        primaries.Green.Y = 1.0;
        
        primaries.Blue.x = 0.1500;
        primaries.Blue.y = 0.0600;
        primaries.Blue.Y = 1.0;
    }
    
    return primaries;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Initialize test structures
    cmsCIExyY whitePoint = create_test_xyY(Data, Size);
    cmsCIExyYTRIPLE primaries = create_test_primaries(Data, Size);
    
    // Create tone curves for RGB profiles
    cmsToneCurve* toneCurve = create_test_tone_curve();
    cmsToneCurve* transferFunctions[3] = {toneCurve, toneCurve, toneCurve};
    
    // Test cmsD50_xyY
    const cmsCIExyY* d50 = cmsD50_xyY();
    (void)d50; // Use to avoid unused variable warning
    
    // Test cmsCreateGrayProfile
    cmsHPROFILE grayProfile = cmsCreateGrayProfile(&whitePoint, toneCurve);
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    
    // Test cmsCreateLab2Profile
    cmsHPROFILE lab2Profile = cmsCreateLab2Profile(&whitePoint);
    if (lab2Profile) {
        cmsCloseProfile(lab2Profile);
    }
    
    // Test cmsCreateRGBProfile
    cmsHPROFILE rgbProfile = cmsCreateRGBProfile(&whitePoint, &primaries, transferFunctions);
    if (rgbProfile) {
        cmsCloseProfile(rgbProfile);
    }
    
    // Test cmsCreateRGBProfileTHR
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context) {
        cmsHPROFILE rgbProfileTHR = cmsCreateRGBProfileTHR(context, &whitePoint, &primaries, transferFunctions);
        if (rgbProfileTHR) {
            cmsCloseProfile(rgbProfileTHR);
        }
        cmsDeleteContext(context);
    }
    
    // Test cmsCreateLab4Profile
    cmsHPROFILE lab4Profile = cmsCreateLab4Profile(&whitePoint);
    if (lab4Profile) {
        cmsCloseProfile(lab4Profile);
    }
    
    // Clean up tone curve
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    
    return 0;
}