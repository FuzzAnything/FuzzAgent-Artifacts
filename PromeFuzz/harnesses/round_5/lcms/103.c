// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
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
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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
    // Create a simple gamma tone curve for testing
    return cmsBuildGamma(context, 2.2);
}

static cmsCIExyY* create_test_white_point(const uint8_t* data, size_t size) {
    static cmsCIExyY white_point;
    
    if (size >= sizeof(cmsCIExyY)) {
        // Use fuzzer data to initialize white point
        memcpy(&white_point, data, sizeof(cmsCIExyY));
    } else {
        // Default to D50 if not enough data
        white_point.x = 0.3457;
        white_point.y = 0.3585;
        white_point.Y = 1.0;
    }
    
    return &white_point;
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    // Create a context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) {
        return 0;
    }
    
    // Test cmsCreateLab4ProfileTHR
    cmsCIExyY* white_point1 = create_test_white_point(Data, Size);
    cmsHPROFILE lab4_profile = cmsCreateLab4ProfileTHR(context, white_point1);
    if (lab4_profile) {
        cmsContext ctx1 = cmsGetProfileContextID(lab4_profile);
        (void)ctx1; // Use variable to avoid unused warning
        cmsCloseProfile(lab4_profile);
    }
    
    // Test cmsCreateNULLProfile
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    if (null_profile) {
        cmsContext ctx2 = cmsGetProfileContextID(null_profile);
        (void)ctx2;
        cmsCloseProfile(null_profile);
    }
    
    // Test cmsCreateXYZProfileTHR
    cmsHPROFILE xyz_profile = cmsCreateXYZProfileTHR(context);
    if (xyz_profile) {
        cmsContext ctx3 = cmsGetProfileContextID(xyz_profile);
        (void)ctx3;
        cmsCloseProfile(xyz_profile);
    }
    
    // Test cmsCreateLab2ProfileTHR
    cmsCIExyY* white_point2 = create_test_white_point(Data + sizeof(cmsCIExyY), 
                                                      Size - sizeof(cmsCIExyY));
    cmsHPROFILE lab2_profile = cmsCreateLab2ProfileTHR(context, white_point2);
    if (lab2_profile) {
        cmsContext ctx4 = cmsGetProfileContextID(lab2_profile);
        (void)ctx4;
        cmsCloseProfile(lab2_profile);
    }
    
    // Test cmsCreateGrayProfileTHR
    cmsCIExyY* white_point3 = create_test_white_point(Data + 2 * sizeof(cmsCIExyY), 
                                                      Size - 2 * sizeof(cmsCIExyY));
    cmsToneCurve* transfer_function = create_test_tone_curve(context);
    cmsHPROFILE gray_profile = cmsCreateGrayProfileTHR(context, white_point3, transfer_function);
    if (gray_profile) {
        cmsContext ctx5 = cmsGetProfileContextID(gray_profile);
        (void)ctx5;
        cmsCloseProfile(gray_profile);
    }
    if (transfer_function) {
        cmsFreeToneCurve(transfer_function);
    }
    
    // Cleanup
    cmsDeleteContext(context);
    
    return 0;
}