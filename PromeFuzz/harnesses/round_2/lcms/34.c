// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsSetLogErrorHandler at cmserr.c:503:16 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
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

static void dummy_cms_error_handler(cmsContext ContextID, cmsUInt32Number ErrorCode, const char *Text) {
    // Silently ignore errors during fuzzing
    (void)ContextID;
    (void)ErrorCode;
    (void)Text;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Set error handler to ignore errors
    cmsSetLogErrorHandler(dummy_cms_error_handler);
    
    // Create a simple sRGB profile to work with
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    
    if (!hProfile) {
        // If we can't create a profile, just return
        return 0;
    }
    
    // Extract tag signatures from input data
    cmsTagSignature sig1 = 0, sig2 = 0;
    if (Size >= 4) {
        memcpy(&sig1, Data, 4);
    }
    if (Size >= 8) {
        memcpy(&sig2, Data + 4, 4);
    }
    
    // Calculate remaining data for raw tag
    size_t remaining = Size > 8 ? Size - 8 : 0;
    const void* raw_data = remaining > 0 ? Data + 8 : NULL;
    
    // Test cmsWriteRawTag
    if (remaining > 0) {
        cmsWriteRawTag(hProfile, sig1, raw_data, (cmsUInt32Number)(remaining > UINT32_MAX ? UINT32_MAX : remaining));
    }
    
    // Test cmsWriteTag - try to write then delete
    if (remaining > 0) {
        // Try to write with the raw data (may not be valid tag data)
        cmsWriteTag(hProfile, sig1, raw_data);
        
        // Delete the tag
        cmsWriteTag(hProfile, sig1, NULL);
    }
    
    // Test cmsMD5computeID
    cmsMD5computeID(hProfile);
    
    // Test cmsIsTag
    cmsIsTag(hProfile, sig1);
    cmsIsTag(hProfile, sig2);
    
    // Test cmsSaveProfileToMem
    cmsUInt32Number bytesNeeded = 0;
    cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    
    if (bytesNeeded > 0 && bytesNeeded < 10 * 1024 * 1024) { // Limit to 10MB
        void* buffer = malloc(bytesNeeded);
        if (buffer) {
            cmsSaveProfileToMem(hProfile, buffer, &bytesNeeded);
            free(buffer);
        }
    }
    
    // Test cmsLinkTag
    cmsLinkTag(hProfile, sig1, sig2);
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}