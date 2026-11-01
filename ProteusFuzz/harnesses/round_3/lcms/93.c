// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static cmsHPROFILE create_dummy_profile(const uint8_t *Data, size_t Size) {
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(NULL);
    if (hProfile == NULL) return NULL;
    
    // Attempt to write some raw data to make the profile non-empty
    if (Size > 0) {
        cmsWriteRawTag(hProfile, 0x74657374, Data, Size);
    }
    return hProfile;
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = create_dummy_profile(Data, Size);
    if (hProfile == NULL) return 0;

    // Extract tag signatures from input data if possible, otherwise use fixed ones
    cmsTagSignature sig1 = 0;
    cmsTagSignature sig2 = 0;
    
    if (Size >= 4) {
        memcpy(&sig1, Data, 4);
    }
    if (Size >= 8) {
        memcpy(&sig2, Data + 4, 4);
    }
    
    // Fallback signatures if input is too short or invalid
    if (sig1 == 0) sig1 = 0x61637370; // 'acsp'
    if (sig2 == 0) sig2 = 0x63686164; // 'chad'

    // 1. cmsIsTag: Check if tags exist
    cmsBool exists1 = cmsIsTag(hProfile, sig1);
    cmsBool exists2 = cmsIsTag(hProfile, sig2);
    (void)exists1;
    (void)exists2;

    // 2. cmsReadTag: Try to read tags
    void *tagData = cmsReadTag(hProfile, sig1);
    if (tagData != NULL) {
        // Data is allocated by lcms, but we don't own it in this context usually,
        // so we just check it exists. Do not free unless documentation says so.
        // lcms docs say cmsReadTag returns a pointer to internal data, do not free.
    }
    tagData = cmsReadTag(hProfile, sig2);
    (void)tagData;

    // 3. cmsWriteTag: Write/Update/Delete tags
    // Write some data
    cmsBool writeRes = cmsWriteTag(hProfile, sig1, Data);
    (void)writeRes;
    
    // Try to write NULL (delete)
    cmsBool delRes = cmsWriteTag(hProfile, sig2, NULL);
    (void)delRes;

    // 4. cmsWriteRawTag: Write raw binary data
    cmsBool rawRes = cmsWriteRawTag(hProfile, sig1, Data, Size);
    (void)rawRes;
    
    // Write raw with size 0
    cmsBool rawResZero = cmsWriteRawTag(hProfile, sig2, NULL, 0);
    (void)rawResZero;

    // 5. cmsLinkTag: Create tag alias
    cmsBool linkRes = cmsLinkTag(hProfile, sig1, sig2);
    (void)linkRes;
    
    // Link to self or invalid
    cmsBool linkResSelf = cmsLinkTag(hProfile, sig1, sig1);
    (void)linkResSelf;

    // 6. cmsTagLinkedTo: Check links
    cmsTagSignature linked = cmsTagLinkedTo(hProfile, sig1);
    cmsTagSignature linked2 = cmsTagLinkedTo(hProfile, sig2);
    (void)linked;
    (void)linked2;

    // Cleanup
    cmsCloseProfile(hProfile);
    
    return 0;
}