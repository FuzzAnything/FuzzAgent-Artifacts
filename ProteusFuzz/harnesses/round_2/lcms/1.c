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

    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Explore different tag indices
    for (int i = 0; i < tagCount && i < 100; i++) {
        cmsTagSignature sig = cmsGetTagSignature(hProfile, (cmsUInt32Number)i);
        if (sig == 0) {
            continue;
        }

        // Allocate buffer for raw tag data
        // Try different buffer sizes to explore edge cases
        size_t bufferSize = 0;
        if (Size > 0) {
            bufferSize = Size;
        } else {
            bufferSize = 1024;
        }

        void *buffer = malloc(bufferSize);
        if (buffer == NULL) {
            continue;
        }

        cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, sig, buffer, bufferSize);
        (void)bytesRead; // Use the return value to suppress unused warning

        free(buffer);
    }

    cleanup_profile(hProfile);
    return 0;
}