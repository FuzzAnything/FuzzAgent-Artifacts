// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size < 128) return 0;

    cmsHPROFILE hProfile = NULL;
    void *tagData = NULL;
    cmsTagSignature tagSig;
    cmsBool res;
    cmsInt32Number tagCount;

    // Attempt to create a profile from the fuzzed data
    hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);

    if (hProfile == NULL) {
        // If opening fails, create a dummy RGB profile to test other APIs
        // Initialize dummy data for WhitePoint, Primaries, and TransferFunction
        cmsCIExyY whitePoint = {0.3127f, 0.3290f, 1.0f};
        cmsCIExyY primaries[3] = {
            {0.6400f, 0.3300f, 1.0f}, // Red
            {0.3000f, 0.6000f, 1.0f}, // Green
            {0.1500f, 0.0600f, 1.0f}  // Blue
        };
        cmsToneCurve *transferFunctions[3] = {NULL, NULL, NULL};

        hProfile = cmsCreateRGBProfile(&whitePoint, primaries, transferFunctions);
    }

    if (hProfile != NULL) {
        // 1. Test cmsMD5computeID
        cmsMD5computeID(hProfile);

        // 2. Test cmsGetTagCount
        tagCount = cmsGetTagCount(hProfile);
        (void)tagCount;

        // 3. Test cmsIsMatrixShaper
        cmsIsMatrixShaper(hProfile);

        // 4. Test cmsReadTag
        if (Size >= 4) {
            tagSig = *(cmsTagSignature*)(Data);
            tagData = cmsReadTag(hProfile, tagSig);
            if (tagData) {
                // Data is managed by lcms, do not free manually
            }
        }

        // 5. Test cmsWriteTag
        if (Size >= 4) {
            res = cmsWriteTag(hProfile, tagSig, Data);
            (void)res;
            
            // Try deleting the tag
            res = cmsWriteTag(hProfile, tagSig, NULL);
            (void)res;
        }

        // 6. Cleanup
        cmsCloseProfile(hProfile);
    }

    return 0;
}