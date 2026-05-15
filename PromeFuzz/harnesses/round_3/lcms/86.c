// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsSetHeaderAttributes at cmsio0.c:1044:16 in lcms2.h
// cmsSetEncodedICCversion at cmsio0.c:1111:16 in lcms2.h
// cmsSetHeaderFlags at cmsio0.c:1002:16 in lcms2.h
// cmsSetHeaderModel at cmsio0.c:1032:16 in lcms2.h
// cmsGetEncodedICCversion at cmsio0.c:1105:27 in lcms2.h
// cmsGetHeaderAttributes at cmsio0.c:1038:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "lcms2.h"

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Create a dummy RGB profile for testing
    cmsToneCurve* Gamma = cmsBuildGamma(NULL, 2.2);
    if (!Gamma) return 0;
    
    cmsCIExyY WhitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE Primaries = {
        {0.640, 0.330, 1.0},
        {0.300, 0.600, 1.0},
        {0.150, 0.060, 1.0}
    };
    
    cmsHPROFILE hProfile = cmsCreateRGBProfile(&WhitePoint, &Primaries, Gamma);
    cmsFreeToneCurve(Gamma);
    
    if (!hProfile) return 0;
    
    // Use fuzzer input to generate parameters for the API calls
    if (Size >= sizeof(cmsUInt64Number) + sizeof(cmsUInt32Number) * 3) {
        cmsUInt64Number flags64;
        cmsUInt32Number version, flags32, model;
        
        // Extract parameters from fuzzer input
        memcpy(&flags64, Data, sizeof(cmsUInt64Number));
        memcpy(&version, Data + sizeof(cmsUInt64Number), sizeof(cmsUInt32Number));
        memcpy(&flags32, Data + sizeof(cmsUInt64Number) + sizeof(cmsUInt32Number), sizeof(cmsUInt32Number));
        memcpy(&model, Data + sizeof(cmsUInt64Number) + sizeof(cmsUInt32Number) * 2, sizeof(cmsUInt32Number));
        
        // Call target API functions
        cmsSetHeaderAttributes(hProfile, flags64);
        cmsSetEncodedICCversion(hProfile, version);
        cmsSetHeaderFlags(hProfile, flags32);
        cmsSetHeaderModel(hProfile, model);
        
        // Call getter functions
        cmsUInt32Number retrieved_version = cmsGetEncodedICCversion(hProfile);
        (void)retrieved_version;
        
        cmsUInt64Number retrieved_flags;
        cmsGetHeaderAttributes(hProfile, &retrieved_flags);
    }
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}