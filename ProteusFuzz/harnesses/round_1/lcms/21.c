// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
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

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsTagSignature sig = 0;
    cmsTagSignature destSig = 0;
    cmsBool result = FALSE;
    void *tagData = NULL;

    // Create a dummy profile for testing
    hProfile = cmsCreateProfilePlaceholder(NULL);
    if (hProfile == NULL) {
        return 0;
    }

    // Extract tag signatures from input data if possible
    if (Size >= 4) {
        sig = *(const cmsTagSignature *)(Data);
    } else {
        sig = 0x61637370; // 'acsp' as a fallback
    }

    if (Size >= 8) {
        destSig = *(const cmsTagSignature *)(Data + 4);
    } else {
        destSig = 0x637274; // 'crt' as a fallback
    }

    // Test cmsIsTag
    result = cmsIsTag(hProfile, sig);

    // Test cmsWriteRawTag
    if (Size > 8) {
        cmsUInt32Number dataSize = Size - 8;
        result = cmsWriteRawTag(hProfile, sig, Data + 8, dataSize);
    }

    // Test cmsWriteTag
    if (Size > 4) {
        result = cmsWriteTag(hProfile, sig, Data + 4);
    }

    // Test cmsLinkTag
    result = cmsLinkTag(hProfile, sig, destSig);

    // Test cmsReadTag
    tagData = cmsReadTag(hProfile, sig);
    if (tagData != NULL) {
        // Do nothing with tagData to avoid memory leaks
    }

    // Test cmsTagLinkedTo
    cmsTagSignature linkedSig = cmsTagLinkedTo(hProfile, sig);
    (void)linkedSig;

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}