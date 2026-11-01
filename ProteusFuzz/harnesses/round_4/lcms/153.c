// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#include "lcms2.h"

static cmsHPROFILE load_profile_from_data(const uint8_t *data, size_t size) {
    if (size == 0 || data == NULL) return NULL;
    cmsHPROFILE hProfile = cmsOpenProfileFromMem((const unsigned char *)data, size);
    return hProfile;
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = load_profile_from_data(Data, Size);
    if (hProfile == NULL) return 0;

    // Test cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    (void)tagCount;

    // Test cmsMD5computeID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result;

    // Test cmsSaveProfileToMem
    cmsUInt32Number bytesNeeded = 0;
    cmsBool saveResult1 = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    if (saveResult1 && bytesNeeded > 0) {
        unsigned char *buffer = (unsigned char *)malloc(bytesNeeded);
        if (buffer != NULL) {
            cmsBool saveResult2 = cmsSaveProfileToMem(hProfile, buffer, &bytesNeeded);
            (void)saveResult2;
            free(buffer);
        }
    }

    // Test cmsWriteTag with dummy data using correct tag signature
    cmsTagSignature sig = cmsSigMediaWhitePointTag;
    cmsBool writeResult = cmsWriteTag(hProfile, sig, NULL);
    (void)writeResult;

    // Test cmsGetProfileInfo using correct info type
    wchar_t buffer[256] = {0};
    cmsUInt32Number infoCount = cmsGetProfileInfo(hProfile, cmsInfoDescription, "en", "US", buffer, sizeof(buffer) / sizeof(wchar_t));
    (void)infoCount;

    // Test cmsGetHeaderCreationDateTime
    struct tm tmStruct = {0};
    cmsBool dateTimeResult = cmsGetHeaderCreationDateTime(hProfile, &tmStruct);
    (void)dateTimeResult;

    // Cleanup
    cmsCloseProfile(hProfile);

    return 0;
}