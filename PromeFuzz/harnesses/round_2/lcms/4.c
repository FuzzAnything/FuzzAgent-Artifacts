// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsMalloc at cmserr.c:265:17 in lcms2_plugin.h
// cmsGetPostScriptCRD at cmsps2.c:1552:27 in lcms2.h
// _cmsFree at cmserr.c:293:16 in lcms2_plugin.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile from fuzzer input
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If profile creation fails, create a simple sRGB profile as fallback
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            return 0;
        }
    }

    cmsContext context = NULL; // Use default context
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    
    // Use fuzzer data to influence parameters if we have enough data
    if (Size >= 8) {
        intent = *(cmsUInt32Number*)Data % 4; // Intent values are 0-3
        flags = *((cmsUInt32Number*)Data + 1) % 16; // Use some flags
    }

    // First call to cmsGetPostScriptCRD with NULL buffer to get required size
    cmsUInt32Number requiredSize = cmsGetPostScriptCRD(context, hProfile, intent, flags, NULL, 0);
    
    // Call _cmsMalloc to allocate buffer
    void* buffer = _cmsMalloc(context, requiredSize);
    
    if (buffer) {
        // Second call to cmsGetPostScriptCRD with allocated buffer
        cmsUInt32Number writtenSize = cmsGetPostScriptCRD(context, hProfile, intent, flags, buffer, requiredSize);
        
        // Clean up allocated buffer
        _cmsFree(context, buffer);
    }
    
    // Clean up profile
    cmsCloseProfile(hProfile);
    
    return 0;
}