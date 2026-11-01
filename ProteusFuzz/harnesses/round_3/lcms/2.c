// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsInt32Number tagCount;
    cmsUInt32Number i;
    cmsTagSignature sig;
    void *tagData;

    // Attempt to create a profile from the input data
    // If the data is not a valid ICC profile, cmsOpenProfileFromMem will return NULL
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);

    if (hProfile == NULL) {
        // Fuzzing invalid inputs is also valuable; return early
        return 0;
    }

    // 1. cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // 2. cmsGetTagSignature
    // Iterate through all tags to explore different program states
    for (i = 0; i < (cmsUInt32Number)tagCount; i++) {
        sig = cmsGetTagSignature(hProfile, i);
        
        // If signature is 0, it indicates an invalid tag or index, skip reading
        if (sig == 0) {
            continue;
        }

        // 3. cmsReadTag
        tagData = cmsReadTag(hProfile, sig);
        
        // No specific action needed on tagData for fuzzing, 
        // but we ensure we don't leak memory if the API required freeing (it doesn't in lcms2).
        if (tagData != NULL) {
            // Accessing the data triggers internal decoding logic
            // We don't free tagData as lcms2 manages this internally.
            (void)tagData; 
        }
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}