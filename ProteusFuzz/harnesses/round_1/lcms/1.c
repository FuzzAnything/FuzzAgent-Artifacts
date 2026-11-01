// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsInt32Number tagCount;
    cmsUInt32Number i;
    cmsTagSignature sig;
    uint8_t buffer[4096];
    cmsUInt32Number bytesRead;

    // Create a profile from the fuzz input
    hProfile = cmsOpenProfileFromMem((const uint8_t *)Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Step 1: Get the number of tags
    tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Step 2 & 3: Iterate over tags, get signature, and read raw data
    for (i = 0; i < (cmsUInt32Number)tagCount; i++) {
        // Get tag signature
        sig = cmsGetTagSignature(hProfile, i);
        if (sig == 0) {
            continue;
        }

        // Read raw tag data
        bytesRead = cmsReadRawTag(hProfile, sig, buffer, sizeof(buffer));
        (void)bytesRead; // Suppress unused variable warning
    }

    cleanup_profile(hProfile);
    return 0;
}