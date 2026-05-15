// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        return 0;
    }

    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount > 0) {
        for (cmsInt32Number i = 0; i < tagCount && i < 10; ++i) {
            cmsTagSignature sig = cmsGetTagSignature(hProfile, i);
            if (sig == 0) {
                continue;
            }

            cmsUInt32Number requiredSize = cmsReadRawTag(hProfile, sig, NULL, 0);
            if (requiredSize > 0 && requiredSize < 1024 * 1024) {
                void *buffer = malloc(requiredSize);
                if (buffer) {
                    cmsReadRawTag(hProfile, sig, buffer, requiredSize);
                    free(buffer);
                }
            }
        }
    }

    cmsCloseProfile(hProfile);
    return 0;
}