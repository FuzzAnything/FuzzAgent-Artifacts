// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void safe_close_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for profile loading
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Load the profile from the temporary file
    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    if (hProfile == NULL) {
        // If the file is not a valid ICC profile, cmsOpenProfileFromFile returns NULL.
        remove(filename);
        return 0;
    }

    // Fuzz cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    (void)tagCount;

    // Fuzz cmsIsMatrixShaper
    cmsBool isMatrix = cmsIsMatrixShaper(hProfile);
    (void)isMatrix;

    // Fuzz cmsMD5computeID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result;

    // Fuzz cmsReadTag with various signatures
    cmsTagSignature commonTags[] = {
        cmsSigProfileDescriptionTag,
        cmsSigCopyrightTag,
        cmsSigDeviceMfgDescTag,
        cmsSigDeviceModelDescTag,
        cmsSigMediaWhitePointTag,
        cmsSigMediaBlackPointTag,
        cmsSigChromaticAdaptationTag,
        cmsSigRedTRCTag,
        cmsSigGreenTRCTag,
        cmsSigBlueTRCTag
    };
    size_t numCommonTags = sizeof(commonTags) / sizeof(commonTags[0]);

    for (size_t i = 0; i < numCommonTags; i++) {
        void *tagData = cmsReadTag(hProfile, commonTags[i]);
        if (tagData != NULL) {
            // Tag data is managed by the profile, we shouldn't free it manually.
        }
    }

    // Try reading with signatures derived from the input data if available
    if (Size >= 4) {
        cmsTagSignature randomSig = *(const cmsTagSignature *)(Data);
        void *randomTagData = cmsReadTag(hProfile, randomSig);
        (void)randomTagData;
    }

    // Fuzz cmsWriteTag
    // Prepare some dummy data for writing
    uint8_t dummyData[64];
    size_t dummySize = Size < 64 ? Size : 64;
    memcpy(dummyData, Data, dummySize);

    // Try writing with common tags
    for (size_t i = 0; i < numCommonTags; i++) {
        cmsBool writeResult = cmsWriteTag(hProfile, commonTags[i], dummyData);
        (void)writeResult;
        
        // Try deleting the tag
        cmsBool deleteResult = cmsWriteTag(hProfile, commonTags[i], NULL);
        (void)deleteResult;
    }

    // Try writing with random signature
    if (Size >= 4) {
        cmsTagSignature randomSig = *(const cmsTagSignature *)(Data);
        cmsBool randomWriteResult = cmsWriteTag(hProfile, randomSig, dummyData);
        (void)randomWriteResult;
        
        cmsBool randomDeleteResult = cmsWriteTag(hProfile, randomSig, NULL);
        (void)randomDeleteResult;
    }

    // Clean up
    safe_close_profile(hProfile);
    
    // Remove the temporary file
    remove(filename);

    return 0;
}