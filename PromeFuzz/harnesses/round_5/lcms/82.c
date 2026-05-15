// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetPCS at cmsio0.c:1075:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCreateGrayProfileTHR at cmsvirt.c:227:23 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx) {
    // Create a simple gamma tone curve
    cmsFloat64Number gamma = 2.2;
    return cmsBuildGamma(ctx, gamma);
}

static cmsCIExyY create_dummy_white_point(void) {
    // D65 white point
    cmsCIExyY whitePoint;
    whitePoint.x = 0.3127;
    whitePoint.y = 0.3290;
    whitePoint.Y = 1.0;
    return whitePoint;
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create context
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    // Create dummy tone curve
    cmsToneCurve* toneCurve = create_dummy_tone_curve(ctx);
    if (!toneCurve) {
        cmsDeleteContext(ctx);
        return 0;
    }
    
    // Create dummy white point
    cmsCIExyY whitePoint = create_dummy_white_point();
    
    // Use fuzzer data to determine which functions to call
    uint8_t selector = Data[0];
    
    // Create profiles
    cmsHPROFILE grayProfile = NULL;
    cmsHPROFILE xyzProfile = NULL;
    
    // Create gray profile (if we have enough data)
    if (Size >= 1) {
        grayProfile = cmsCreateGrayProfileTHR(ctx, &whitePoint, toneCurve);
    }
    
    // Create XYZ profile
    xyzProfile = cmsCreateXYZProfile();
    
    // Test functions based on selector
    switch (selector % 6) {
        case 0:
            if (grayProfile) {
                cmsColorSpaceSignature cs = cmsGetColorSpace(grayProfile);
                (void)cs; // Use result to avoid unused variable warning
            }
            break;
            
        case 1:
            if (grayProfile) {
                // Use fuzzer data for device class if available
                cmsProfileClassSignature devClass = cmsSigInputClass;
                if (Size >= 2) {
                    devClass = (cmsProfileClassSignature)(Data[1] % 256);
                }
                cmsSetDeviceClass(grayProfile, devClass);
            }
            break;
            
        case 2:
            if (grayProfile) {
                cmsProfileClassSignature devClass = cmsGetDeviceClass(grayProfile);
                (void)devClass;
            }
            break;
            
        case 3:
            // Already created gray profile above, just verify it was created
            if (grayProfile) {
                // Do nothing, creation was successful
            }
            break;
            
        case 4:
            if (xyzProfile) {
                // Do nothing, creation was successful
            }
            break;
            
        case 5:
            if (grayProfile) {
                // Use fuzzer data for PCS if available
                cmsColorSpaceSignature pcs = cmsSigXYZData;
                if (Size >= 2) {
                    pcs = (cmsColorSpaceSignature)(Data[1] % 256);
                }
                cmsSetPCS(grayProfile, pcs);
            }
            break;
    }
    
    // Clean up
    if (toneCurve) {
        cmsFreeToneCurve(toneCurve);
    }
    if (grayProfile) {
        cmsCloseProfile(grayProfile);
    }
    if (xyzProfile) {
        cmsCloseProfile(xyzProfile);
    }
    cmsDeleteContext(ctx);
    
    return 0;
}