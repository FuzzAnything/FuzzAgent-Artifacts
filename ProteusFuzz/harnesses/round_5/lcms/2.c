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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
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

    if (tagCount > 0) {
        cmsUInt32Number idx = 0;
        if (Size > 0) {
            idx = Data[0] % (tagCount + 1);
        }

        cmsTagSignature sig = cmsGetTagSignature(hProfile, idx);
        if (sig != 0) {
            uint8_t buffer[4096];
            cmsUInt32Number bufferSize = sizeof(buffer);
            if (Size > 1) {
                bufferSize = (Data[1] % 4096) + 1;
            }
            cmsReadRawTag(hProfile, sig, buffer, bufferSize);
        }
    }

    cleanup_profile(hProfile);
    return 0;
}