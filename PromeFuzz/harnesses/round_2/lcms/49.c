// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateNULLProfile at cmsvirt.c:1018:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateXYZProfileTHR at cmsvirt.c:577:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to create a white point structure if we have enough data
    cmsCIExyY whitePoint;
    int hasWhitePoint = 0;
    
    if (Size >= sizeof(cmsCIExyY)) {
        // Copy enough bytes for the white point structure
        memcpy(&whitePoint, Data, sizeof(cmsCIExyY));
        hasWhitePoint = 1;
        // Move Data pointer forward for any additional usage
        Data += sizeof(cmsCIExyY);
        Size -= sizeof(cmsCIExyY);
    }
    
    // Create a context - use NULL for default context
    cmsContext context = NULL;
    
    // 1. Test cmsCreateNULLProfile
    cmsHPROFILE nullProfile = cmsCreateNULLProfile();
    if (nullProfile != NULL) {
        // Test cmsGetColorSpace on the NULL profile
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(nullProfile);
        (void)colorSpace; // Avoid unused variable warning
        
        // Test cmsSetDeviceClass if we have enough data for signature
        if (Size >= sizeof(cmsProfileClassSignature)) {
            cmsProfileClassSignature sig = *(cmsProfileClassSignature*)Data;
            cmsSetDeviceClass(nullProfile, sig);
        }
        cmsCloseProfile(nullProfile);
    }
    
    // 2. Test cmsCreateLab4ProfileTHR
    cmsHPROFILE lab4Profile = cmsCreateLab4ProfileTHR(context, 
        hasWhitePoint ? &whitePoint : NULL);
    if (lab4Profile != NULL) {
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(lab4Profile);
        (void)colorSpace;
        cmsCloseProfile(lab4Profile);
    }
    
    // 3. Test cmsCreateXYZProfileTHR
    cmsHPROFILE xyzProfile = cmsCreateXYZProfileTHR(context);
    if (xyzProfile != NULL) {
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(xyzProfile);
        (void)colorSpace;
        cmsCloseProfile(xyzProfile);
    }
    
    // 4. Test cmsCreateLab2ProfileTHR
    cmsHPROFILE lab2Profile = cmsCreateLab2ProfileTHR(context, 
        hasWhitePoint ? &whitePoint : NULL);
    if (lab2Profile != NULL) {
        cmsColorSpaceSignature colorSpace = cmsGetColorSpace(lab2Profile);
        (void)colorSpace;
        cmsCloseProfile(lab2Profile);
    }
    
    return 0;
}