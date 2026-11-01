// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <wchar.h>
#include <time.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char*)Data, Size);
    if (hProfile == NULL) {
        // Try to create a dummy profile if opening fails
        cmsHPROFILE hProfileDummy = cmsCreate_sRGBProfile();
        if (hProfileDummy == NULL) return 0;
        hProfile = hProfileDummy;
    }

    // Test cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    (void)tagCount;

    // Test cmsSaveProfileToMem
    cmsUInt32Number bytesNeeded = 0;
    cmsBool saveResult = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    if (saveResult && bytesNeeded > 0) {
        unsigned char *buffer = (unsigned char *)malloc(bytesNeeded);
        if (buffer != NULL) {
            cmsSaveProfileToMem(hProfile, buffer, &bytesNeeded);
            free(buffer);
        }
    }

    // Test cmsMD5computeID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result;

    // Test cmsGetHeaderCreationDateTime
    struct tm creationTime;
    cmsBool dateTimeResult = cmsGetHeaderCreationDateTime(hProfile, &creationTime);
    (void)dateTimeResult;

    // Test cmsGetProfileInfo using correct enum value cmsInfoDescription
    wchar_t buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cmsUInt32Number infoResult = cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, 256);
    (void)infoResult;

    // Test cmsWriteTag with NULL data to attempt deletion
    cmsBool writeResult = cmsWriteTag(hProfile, cmsSigProfileDescriptionTag, NULL);
    (void)writeResult;

    cleanup_profile(hProfile);
    return 0;
}