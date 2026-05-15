// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_cleanup(void) {
    // Cleanup function for any resources if needed
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    // Create a dummy ICC profile from input data
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(Data, Size);
    if (!hProfile) {
        // If we can't create from input, create a minimal sRGB profile
        hProfile = cmsCreate_sRGBProfile();
        if (!hProfile) {
            return 0;
        }
    }

    // Use fuzz data to generate tag signatures
    cmsTagSignature sig1 = 0, sig2 = 0;
    if (Size >= 8) {
        memcpy(&sig1, Data, 4);
        memcpy(&sig2, Data + 4, 4);
    } else if (Size >= 4) {
        memcpy(&sig1, Data, 4);
        sig2 = cmsSigMediaWhitePointTag;  // Use a known tag
    } else {
        // Use some default tags if not enough data
        sig1 = cmsSigRedTRCTag;
        sig2 = cmsSigGreenTRCTag;
    }

    // Test cmsIsTag
    (void)cmsIsTag(hProfile, sig1);
    (void)cmsIsTag(hProfile, sig2);

    // Test cmsReadTag
    void* tagData = cmsReadTag(hProfile, sig1);
    if (tagData) {
        // Don't use the data, just check it's non-NULL
    }

    // Test cmsLinkTag
    (void)cmsLinkTag(hProfile, sig1, sig2);

    // Test cmsTagLinkedTo
    (void)cmsTagLinkedTo(hProfile, sig1);
    (void)cmsTagLinkedTo(hProfile, sig2);

    // Test cmsGetTagSignature
    cmsUInt32Number tagCount = cmsGetTagCount(hProfile);
    for (cmsUInt32Number i = 0; i < tagCount && i < 10; i++) {
        (void)cmsGetTagSignature(hProfile, i);
    }

    // Test cmsReadRawTag
    if (Size > 8) {
        cmsUInt32Number bufferSize = Size - 8;
        if (bufferSize > 1024) bufferSize = 1024;  // Limit buffer size
        
        void* buffer = malloc(bufferSize);
        if (buffer) {
            (void)cmsReadRawTag(hProfile, sig1, NULL, 0);  // Query size
            (void)cmsReadRawTag(hProfile, sig1, buffer, bufferSize);
            free(buffer);
        }
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    dummy_cleanup();
    
    return 0;
}