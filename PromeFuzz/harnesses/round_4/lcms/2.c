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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Use default context for simplicity
    cmsContext context = NULL;
    
    // Create a dummy profile from fuzz data
    cmsHPROFILE hProfile = NULL;
    if (Size > 0) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }
    
    // If profile creation failed, create a default sRGB profile
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
        if (hProfile == NULL) {
            return 0; // Cannot proceed without a profile
        }
    }
    
    // First call to cmsGetPostScriptCRD to get required size
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    if (Size > 0) {
        intent = Data[0] % 4; // Intent values are 0-3
        if (Size > 1) {
            flags = Data[1];
        }
    }
    
    cmsUInt32Number requiredSize = cmsGetPostScriptCRD(context, hProfile, intent, flags, NULL, 0);
    
    // Call _cmsMalloc to allocate memory
    void* allocatedMemory = NULL;
    if (requiredSize > 0 && requiredSize < 1024*1024) { // Limit allocation to 1MB
        allocatedMemory = _cmsMalloc(context, requiredSize);
    }
    
    // Second call to cmsGetPostScriptCRD to write CRD data
    if (allocatedMemory != NULL) {
        cmsUInt32Number writtenSize = cmsGetPostScriptCRD(context, hProfile, intent, flags, allocatedMemory, requiredSize);
        (void)writtenSize; // Use result to avoid unused variable warning
    }
    
    // Call _cmsFree to free allocated memory
    if (allocatedMemory != NULL) {
        _cmsFree(context, allocatedMemory);
    }
    
    // Clean up profile
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    return 0;
}