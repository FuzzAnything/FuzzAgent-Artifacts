// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data for basic operations
    if (Size < sizeof(cmsColorSpaceSignature)) {
        return 0;
    }

    // Extract color space signature from fuzzed data
    cmsColorSpaceSignature colorSpaceSig;
    memcpy(&colorSpaceSig, Data, sizeof(cmsColorSpaceSignature));
    
    // Test cmsChannelsOfColorSpace
    cmsInt32Number channels1 = cmsChannelsOfColorSpace(colorSpaceSig);
    (void)channels1; // Use result to avoid unused variable warning
    
    // Test cmsChannelsOf (deprecated but still available)
    cmsUInt32Number channels2 = cmsChannelsOf(colorSpaceSig);
    (void)channels2;
    
    // Test _cmsICCcolorSpace - use part of data as integer input
    int ourNotation = 0;
    if (Size >= sizeof(int)) {
        memcpy(&ourNotation, Data, sizeof(int));
    } else {
        ourNotation = (int)*Data;
    }
    cmsColorSpaceSignature iccResult = _cmsICCcolorSpace(ourNotation);
    (void)iccResult;
    
    // Test _cmsLCMScolorSpace
    int lcmsResult = _cmsLCMScolorSpace(colorSpaceSig);
    (void)lcmsResult;
    
    // Create a dummy profile for profile-related functions
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    
    if (hProfile != NULL) {
        // Test cmsGetColorSpace
        cmsColorSpaceSignature retrievedSig = cmsGetColorSpace(hProfile);
        (void)retrievedSig;
        
        // Test cmsSetColorSpace
        cmsSetColorSpace(hProfile, colorSpaceSig);
        
        // Clean up
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}