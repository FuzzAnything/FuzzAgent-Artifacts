// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsReadRawTag at cmsio0.c:1907:27 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsTagSignature sig = 0;
    cmsInt32Number tagCount = 0;
    cmsUInt32Number i;
    void *tagData = NULL;
    char buffer[4096];

    // Attempt to open profile from memory
    hProfile = cmsOpenProfileFromMem(Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    // Fuzz cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);
    if (tagCount < 0) {
        cleanup_profile(hProfile);
        return 0;
    }

    // Fuzz cmsGetTagSignature for each tag index
    for (i = 0; i < (size_t)tagCount; i++) {
        sig = cmsGetTagSignature(hProfile, i);
        if (sig == 0) {
            continue;
        }

        // Fuzz cmsTagLinkedTo
        cmsTagSignature linkedSig = cmsTagLinkedTo(hProfile, sig);
        (void)linkedSig; // Suppress unused variable warning

        // Fuzz cmsReadRawTag
        cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, sig, buffer, sizeof(buffer));
        (void)bytesRead; // Suppress unused variable warning

        // Fuzz cmsReadTag
        tagData = cmsReadTag(hProfile, sig);
        if (tagData != NULL) {
            // No specific action needed, just ensure we don't leak
            // The library manages memory for cmsReadTag, do not free manually
        }
    }

    // Fuzz with arbitrary signatures from input data if available
    if (Size >= 4) {
        cmsTagSignature arbitrarySig = *(const cmsTagSignature *)(Data);
        cmsTagSignature linkedSig = cmsTagLinkedTo(hProfile, arbitrarySig);
        (void)linkedSig;

        cmsUInt32Number bytesRead = cmsReadRawTag(hProfile, arbitrarySig, buffer, sizeof(buffer));
        (void)bytesRead;

        tagData = cmsReadTag(hProfile, arbitrarySig);
        if (tagData != NULL) {
            // Do nothing, memory managed by library
        }
    }

    cleanup_profile(hProfile);
    return 0;
}