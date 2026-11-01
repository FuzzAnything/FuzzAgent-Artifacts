// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
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

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsTagSignature sig = 0;
    cmsTagSignature dest = 0;
    void *tagData = NULL;
    cmsBool result;
    cmsTagSignature linkedSig;

    // Create a dummy profile from input data if possible
    // We try to parse the input as a profile; if it fails, we create a minimal one
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    
    if (hProfile == NULL) {
        // If parsing fails, create a minimal profile for testing tag operations
        cmsHPROFILE hProfileDummy = cmsCreate_sRGBProfile();
        if (hProfileDummy != NULL) {
            hProfile = hProfileDummy;
        } else {
            return 0;
        }
    }

    // Extract tag signatures from input data if available
    if (Size >= 4) {
        memcpy(&sig, Data, 4);
        if (Size >= 8) {
            memcpy(&dest, Data + 4, 4);
        }
    }

    // Test cmsIsTag
    result = cmsIsTag(hProfile, sig);
    (void)result;

    // Test cmsReadTag
    tagData = cmsReadTag(hProfile, sig);
    if (tagData != NULL) {
        // Data is managed by lcms, do not free
    }

    // Test cmsTagLinkedTo
    linkedSig = cmsTagLinkedTo(hProfile, sig);
    (void)linkedSig;

    // Test cmsLinkTag
    result = cmsLinkTag(hProfile, sig, dest);
    (void)result;

    // Test cmsWriteTag (with NULL to delete, or with data)
    if (Size >= 4) {
        result = cmsWriteTag(hProfile, sig, (const void *)(Data + 4));
        (void)result;
    } else {
        result = cmsWriteTag(hProfile, sig, NULL);
        (void)result;
    }

    // Test cmsWriteRawTag
    cmsUInt32Number dataSize = 0;
    if (Size >= 4) {
        memcpy(&dataSize, Data, 4);
        // Ensure dataSize is reasonable to avoid allocation issues
        if (dataSize > 0 && dataSize < Size - 4) {
            result = cmsWriteRawTag(hProfile, sig, (const void *)(Data + 4), dataSize);
            (void)result;
        }
    }

    // Cleanup
    cleanup_profile(hProfile);
    cleanup_profile(hProfile2);

    return 0;
}