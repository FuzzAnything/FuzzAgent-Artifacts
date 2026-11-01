// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char *)Data, Size);
    if (hProfile == NULL) {
        return 0;
    }

    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    
    // Define common tag signatures using their hex values
    cmsTagSignature signatures[] = {
        0x41544F42, // cmsSigAToBTag
        0x42544F41, // cmsSigBToATag
        0x4B525443, // cmsSigKRTCTag
        0x52445452, // cmsSigRedTRCTag
        0x47524543, // cmsSigGreenTRCTag
        0x424C5554, // cmsSigBlueTRCTag
        0x70646573, // cmsSigProfileDescriptionTag
        0x63707274  // cmsSigCopyrightTag
    };
    size_t numSignatures = sizeof(signatures) / sizeof(signatures[0]);

    for (size_t i = 0; i < numSignatures; i++) {
        cmsBool exists = cmsIsTag(hProfile, signatures[i]);
        if (exists) {
            void *tagData = cmsReadTag(hProfile, signatures[i]);
            if (tagData != NULL) {
                cmsWriteTag(hProfile, signatures[i], tagData);
            }
        }
    }

    cmsBool isMatrixShaper = cmsIsMatrixShaper(hProfile);
    (void)isMatrixShaper;

    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result;

    cmsTagSignature dummySig = 0x64756d6d; // 'dumm'
    const char *dummyData = "dummy";
    cmsWriteTag(hProfile, dummySig, dummyData);
    cmsWriteTag(hProfile, dummySig, NULL);

    cleanup_profile(hProfile);
    return 0;
}