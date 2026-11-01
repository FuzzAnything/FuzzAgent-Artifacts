// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsTagLinkedTo at cmsio0.c:2092:28 in lcms2.h
// cmsLinkTag at cmsio0.c:2065:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsHPROFILE create_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    return cmsOpenProfileFromMem(data, size);
}

int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsTagSignature sig = 0;
    cmsTagSignature destSig = 0;
    cmsTagSignature linkedSig = 0;
    void *tagData = NULL;
    cmsBool exists = FALSE;
    cmsBool writeResult = FALSE;

    // Extract signatures from input if possible
    if (Size >= 4) {
        memcpy(&sig, Data, 4);
        if (Size >= 8) {
            memcpy(&destSig, Data + 4, 4);
        }
    }

    // Try to open a profile from the input data
    hProfile = create_profile_from_data(Data, Size);

    if (hProfile) {
        // Test cmsIsTag
        exists = cmsIsTag(hProfile, sig);
        (void)exists;

        // Test cmsReadTag
        tagData = cmsReadTag(hProfile, sig);
        if (tagData) {
            // Data is managed by the profile, do not free manually
        }

        // Test cmsTagLinkedTo
        linkedSig = cmsTagLinkedTo(hProfile, sig);
        (void)linkedSig;

        // Test cmsLinkTag (create an alias)
        writeResult = cmsLinkTag(hProfile, sig, destSig);
        (void)writeResult;

        // Test cmsWriteTag (try to write some dummy data)
        uint8_t dummyData[16] = {0};
        writeResult = cmsWriteTag(hProfile, sig, dummyData);
        (void)writeResult;

        // Test cmsWriteRawTag
        writeResult = cmsWriteRawTag(hProfile, sig, dummyData, sizeof(dummyData));
        (void)writeResult;

        // Test cmsWriteTag with NULL to delete
        writeResult = cmsWriteTag(hProfile, sig, NULL);
        (void)writeResult;

        // Cleanup profile
        cmsCloseProfile(hProfile);
    }

    return 0;
}