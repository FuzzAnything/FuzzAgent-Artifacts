// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// _cmsICCcolorSpace at cmspcs.c:764:34 in lcms2.h
// cmsChannelsOfColorSpace at cmspcs.c:877:26 in lcms2.h
// cmsChannelsOf at cmspcs.c:945:27 in lcms2.h
// _cmsLCMScolorSpace at cmspcs.c:810:15 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsSetColorSpace at cmsio0.c:1087:16 in lcms2.h
// cmsGetColorSpace at cmsio0.c:1081:34 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    // Use first byte for OurNotation parameter
    int OurNotation = 0;
    if (Size > 0) {
        OurNotation = Data[0];
    }
    
    // Test _cmsICCcolorSpace
    cmsColorSpaceSignature iccSig = _cmsICCcolorSpace(OurNotation);
    
    // Test cmsChannelsOfColorSpace and cmsChannelsOf with the generated signature
    cmsInt32Number channels1 = cmsChannelsOfColorSpace(iccSig);
    cmsUInt32Number channels2 = cmsChannelsOf(iccSig);
    
    // Test _cmsLCMScolorSpace with the generated signature
    int lcmsCode = _cmsLCMScolorSpace(iccSig);
    
    // Create a dummy profile for testing cmsGetColorSpace and cmsSetColorSpace
    // cmsCreateRGBProfile expects 3 arguments: WhitePoint, Primaries, and TransferFunction[3]
    cmsToneCurve* TransferFunction[3] = { NULL, NULL, NULL };
    cmsHPROFILE hProfile = cmsCreateRGBProfile(NULL, NULL, TransferFunction);
    
    if (hProfile != NULL) {
        // Test cmsGetColorSpace on the created profile
        cmsColorSpaceSignature currentSig = cmsGetColorSpace(hProfile);
        
        // Test cmsSetColorSpace with a signature derived from fuzzer input
        // Use the signature we got from _cmsICCcolorSpace or create one from data
        cmsColorSpaceSignature newSig;
        if (Size >= 4) {
            // Use bytes 1-4 to form a signature (if available)
            memcpy(&newSig, Data + 1, 4);
        } else {
            // Use the signature we already have
            newSig = iccSig;
        }
        
        cmsSetColorSpace(hProfile, newSig);
        
        // Verify the change
        cmsColorSpaceSignature updatedSig = cmsGetColorSpace(hProfile);
        
        // Clean up
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}