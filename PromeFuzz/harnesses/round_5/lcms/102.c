// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsGetProfileContextID at cmsio0.c:571:22 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_tone_curve(cmsContext context) {
    // Create a simple gamma curve for testing
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(context, gamma);
}

static void cleanup_tone_curve(cmsToneCurve* curve, cmsContext context) {
    if (curve) {
        cmsFreeToneCurve(curve);
    }
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    // Initialize white point structure
    cmsCIExyY white_point;
    
    // Use fuzzing data to populate white point if we have enough bytes
    if (Size >= sizeof(cmsCIExyY)) {
        memcpy(&white_point, Data, sizeof(cmsCIExyY));
    } else {
        // Default values if not enough data
        white_point.x = 0.3127;
        white_point.y = 0.3290;
        white_point.Y = 1.0;
    }
    
    // Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Test cmsCreateLab4ProfileTHR
    cmsHPROFILE lab4_profile = cmsCreateLab4ProfileTHR(context, &white_point);
    if (lab4_profile) {
        // Test cmsGetProfileContextID on the created profile
        cmsContext profile_ctx = cmsGetProfileContextID(lab4_profile);
        // Clean up
        cmsCloseProfile(lab4_profile);
    }
    
    // Test cmsCreateNULLProfile
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    if (null_profile) {
        cmsContext null_profile_ctx = cmsGetProfileContextID(null_profile);
        cmsCloseProfile(null_profile);
    }
    
    // Test cmsCreateXYZProfileTHR
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    if (xyz_profile) {
        cmsContext xyz_profile_ctx = cmsGetProfileContextID(xyz_profile);
        cmsCloseProfile(xyz_profile);
    }
    
    // Test cmsCreateLab2ProfileTHR
    cmsHPROFILE lab2_profile = cmsCreateLab2ProfileTHR(context, &white_point);
    if (lab2_profile) {
        cmsContext lab2_profile_ctx = cmsGetProfileContextID(lab2_profile);
        cmsCloseProfile(lab2_profile);
    }
    
    // Test cmsCreateGrayProfileTHR
    cmsToneCurve* tone_curve = create_test_tone_curve(context);
    cmsHPROFILE gray_profile = cmsCreateGrayProfileTHR(context, &white_point, tone_curve);
    if (gray_profile) {
        cmsContext gray_profile_ctx = cmsGetProfileContextID(gray_profile);
        cmsCloseProfile(gray_profile);
    }
    
    // Cleanup
    cleanup_tone_curve(tone_curve, context);
    cmsDeleteContext(context);
    
    return 0;
}