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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    if (tagCount == 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    cmsUInt32Number maxIndex = (tagCount > 100) ? 100 : (cmsUInt32Number)tagCount;
    
    for (cmsUInt32Number i = 0; i < maxIndex; i++) {
        cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
        if (sig == 0) {
            continue;
        }

        void *tagData = cmsReadTag(hProfile, sig);
        if (tagData != NULL) {
            free(tagData);
        }
    }

    cleanup_profile(hProfile);
    return 0;
}