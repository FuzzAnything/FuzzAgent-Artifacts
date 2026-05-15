// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderRenderingIntent at cmsio0.c:984:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsGetPostScriptCSA at cmsps2.c:1579:27 in lcms2.h
// cmsIsIntentSupported at cmsio1.c:864:20 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
// cmsDetectBlackPoint at cmssamp.c:194:19 in lcms2.h
// cmsIsCLUT at cmsio1.c:830:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    
    // Try to open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    if (hProfile) {
        return hProfile;
    }
    
    // Fallback: create a simple sRGB profile
    return cmsCreate_sRGBProfile();
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    // Create a profile from fuzzer input
    cmsHPROFILE hProfile = create_profile_from_data(Data, Size);
    if (!hProfile) {
        return 0;
    }
    
    // Derive parameters from fuzzer input
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;
    cmsUInt32Number direction = 0;
    
    if (Size >= 4) {
        intent = *((cmsUInt32Number*)Data) % 4;  // 0-3 for standard intents
    }
    if (Size >= 8) {
        flags = *((cmsUInt32Number*)(Data + 4));
    }
    if (Size >= 12) {
        direction = *((cmsUInt32Number*)(Data + 8)) % 2;  // 0 or 1
    }
    
    // Test cmsGetHeaderRenderingIntent
    cmsUInt32Number headerIntent = cmsGetHeaderRenderingIntent(hProfile);
    (void)headerIntent;
    
    // Test cmsGetPostScriptCSA
    // First call to get required buffer size
    cmsUInt32Number bufferSize = cmsGetPostScriptCSA(NULL, hProfile, intent, flags, NULL, 0);
    
    if (bufferSize > 0 && bufferSize < 1024 * 1024) {  // Reasonable limit
        void *buffer = malloc(bufferSize);
        if (buffer) {
            cmsUInt32Number usedSize = cmsGetPostScriptCSA(NULL, hProfile, intent, flags, buffer, bufferSize);
            (void)usedSize;
            free(buffer);
        }
    }
    
    // Test cmsIsIntentSupported
    cmsBool intentSupported = cmsIsIntentSupported(hProfile, intent, direction);
    (void)intentSupported;
    
    // Test cmsDetectDestinationBlackPoint
    cmsCIEXYZ blackPoint1;
    cmsBool detected1 = cmsDetectDestinationBlackPoint(&blackPoint1, hProfile, intent, flags);
    (void)detected1;
    
    // Test cmsDetectBlackPoint
    cmsCIEXYZ blackPoint2;
    cmsBool detected2 = cmsDetectBlackPoint(&blackPoint2, hProfile, intent, flags);
    (void)detected2;
    
    // Test cmsIsCLUT
    cmsBool isClut = cmsIsCLUT(hProfile, intent, direction);
    (void)isClut;
    
    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}