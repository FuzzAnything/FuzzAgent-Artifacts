// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
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

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    // Create or open a profile to work with
    cmsHPROFILE hProfile = NULL;
    
    // First try to open the input as a profile
    if (Size > 0) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }
    
    // If that fails, create a dummy sRGB profile
    if (hProfile == NULL) {
        hProfile = cmsCreate_sRGBProfile();
    }
    
    if (hProfile == NULL) {
        return 0;
    }
    
    // Use the input data to generate parameters for the API functions
    // We'll use different parts of the input for different parameters
    
    // For tag signatures - use first 8 bytes (2 signatures)
    cmsTagSignature sig1 = 0, sig2 = 0;
    if (Size >= 8) {
        memcpy(&sig1, Data, 4);
        memcpy(&sig2, Data + 4, 4);
    } else if (Size >= 4) {
        memcpy(&sig1, Data, 4);
        sig2 = sig1; // Use same signature for both
    } else {
        // Use some default tag signatures if input is too small
        sig1 = cmsSigRedTRCTag;
        sig2 = cmsSigGreenTRCTag;
    }
    
    // For index - use next 4 bytes if available
    cmsUInt32Number index = 0;
    if (Size >= 12) {
        memcpy(&index, Data + 8, 4);
    }
    
    // Test cmsLinkTag
    cmsLinkTag(hProfile, sig1, sig2);
    
    // Test cmsGetTagSignature
    cmsGetTagSignature(hProfile, index);
    
    // Test cmsWriteTag
    // Use a small dummy data buffer for writing
    static uint8_t dummy_data[256];
    if (Size > 0) {
        size_t data_size = Size > sizeof(dummy_data) ? sizeof(dummy_data) : Size;
        memcpy(dummy_data, Data, data_size);
        cmsWriteTag(hProfile, sig1, dummy_data);
        
        // Also test deletion by passing NULL
        cmsWriteTag(hProfile, sig2, NULL);
    }
    
    // Test cmsReadRawTag
    // First query the size
    cmsUInt32Number needed_size = cmsReadRawTag(hProfile, sig1, NULL, 0);
    
    // Then try to read with a buffer if size is reasonable
    if (needed_size > 0 && needed_size < 1024 * 1024) { // Limit to 1MB
        void* buffer = malloc(needed_size);
        if (buffer) {
            cmsReadRawTag(hProfile, sig1, buffer, needed_size);
            free(buffer);
        }
    }
    
    // Test cmsTagLinkedTo
    cmsTagLinkedTo(hProfile, sig1);
    
    // Test cmsReadTag
    void* tag_data = cmsReadTag(hProfile, sig1);
    // Note: tag_data is owned by the profile, don't free it
    
    // Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}