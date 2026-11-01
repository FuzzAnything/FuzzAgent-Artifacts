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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;

    // Attempt to create a profile from the input data
    // cmsOpenProfileFromMem is a common way to load a profile from a buffer
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);

    if (hProfile == NULL) {
        // Profile loading failed, nothing to fuzz
        return 0;
    }

    // Step 1: Get the number of tags
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);

    // If tagCount is -1 or 0, we can't proceed with the next steps meaningfully
    // but we can still test edge cases if tagCount is negative (though docs say -1 for NULL handle)
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Step 2: Iterate through tags to fuzz cmsGetTagSignature and cmsReadTag
    // We use a loop to explore different states based on the number of tags found
    for (cmsUInt32Number i = 0; i < (cmsUInt32Number)tagCount; i++) {
        cmsTagSignature sig = cmsGetTagSignature(hProfile, i);

        // If signature is 0, it indicates an invalid index or missing tag
        if (sig == 0) {
            continue;
        }

        // Step 3: Read the tag data
        void *tagData = cmsReadTag(hProfile, sig);
        
        // tagData might be NULL if the tag is missing or invalid, which is a valid state to test
        // We don't need to free tagData manually as lcms manages the memory for read tags internally
        // or the caller is responsible depending on the specific tag type, but for fuzzing,
        // just invoking the API is the primary goal.
        (void)tagData; 
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}