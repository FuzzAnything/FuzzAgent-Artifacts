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
#include <lcms2.h>

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
    cmsInt32Number tagCount;
    cmsUInt32Number i;
    cmsTagSignature sig;
    void *tagData = NULL;

    // Attempt to create a profile from the fuzzed data
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Step 1: Get the number of tags
    tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Step 2 & 3: Iterate through tags to get signature and read tag data
    for (i = 0; i < (cmsUInt32Number)tagCount; i++) {
        // Get tag signature
        sig = cmsGetTagSignature(hProfile, i);
        if (sig == 0) {
            continue; // Skip invalid tags
        }

        // Read tag data
        tagData = cmsReadTag(hProfile, sig);
        if (tagData != NULL) {
            // Optionally, we could do something with tagData here,
            // but for fuzzing, just ensuring it doesn't crash is key.
            // The data is owned by the profile, so we don't free it.
            (void)tagData; // Suppress unused variable warning
        }
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}