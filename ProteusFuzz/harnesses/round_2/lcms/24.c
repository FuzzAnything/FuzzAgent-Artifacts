// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreate_sRGBProfile at cmsvirt.c:680:23 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsGetProfileInfo at cmsio1.c:1016:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <time.h>

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hProfile2 = NULL;
    cmsUInt32Number BytesNeeded = 0;
    void *MemPtr = NULL;
    struct tm Dest = {0};
    cmsInt32Number tagCount;
    cmsBool res;
    cmsUInt32Number infoLen;
    wchar_t Buffer[256] = {0};
    const char LanguageCode[3] = "en";
    const char CountryCode[3] = "US";

    // Try to load a profile from the input data if it looks like an ICC profile
    // ICC profiles start with a size field (4 bytes) followed by signature "acsp"
    if (Size >= 12) {
        uint32_t sizeField = 0;
        memcpy(&sizeField, Data, 4);
        // Basic sanity check: size should be reasonable and signature should be "acsp"
        if (sizeField > 0 && sizeField <= 100 * 1024 * 1024 && 
            Data[4] == 'a' && Data[5] == 'c' && Data[6] == 's' && Data[7] == 'p') {
            hProfile = cmsOpenProfileFromMem(Data, Size);
        }
    }

    // If we couldn't load a profile, try to create a dummy one or skip
    if (hProfile == NULL) {
        // Attempt to create a simple profile for testing if input is not valid ICC
        // We'll try to create a basic sRGB-like profile for testing purposes
        cmsHPROFILE hRGB = cmsCreate_sRGBProfile();
        if (hRGB != NULL) {
            hProfile = hRGB;
        }
    }

    if (hProfile == NULL) {
        // If still null, we can't test much, but we can test NULL handling
        cmsGetHeaderCreationDateTime(NULL, &Dest);
        cmsMD5computeID(NULL);
        cmsSaveProfileToMem(NULL, NULL, &BytesNeeded);
        cmsGetTagCount(NULL);
        cmsWriteTag(NULL, 0, NULL);
        cmsGetProfileInfo(NULL, 0, LanguageCode, CountryCode, Buffer, sizeof(Buffer)/sizeof(wchar_t));
        return 0;
    }

    // Test cmsGetHeaderCreationDateTime
    cmsGetHeaderCreationDateTime(hProfile, &Dest);

    // Test cmsMD5computeID
    res = cmsMD5computeID(hProfile);
    (void)res;

    // Test cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);
    if (tagCount > 0) {
        // Try to get tag info if tags exist
        // Use cmsGetTagSignature to get individual tags
        cmsTagSignature sig = cmsGetTagSignature(hProfile, 0);
        if (sig != 0) {
            // Try to write a tag (using the first tag signature)
            // We'll pass NULL to delete it, or a dummy data to write
            res = cmsWriteTag(hProfile, sig, NULL);
            (void)res;
        }
    }

    // Test cmsSaveProfileToMem
    // First call to get size
    if (!cmsSaveProfileToMem(hProfile, NULL, &BytesNeeded)) {
        // If size calculation fails, skip writing
    } else if (BytesNeeded > 0) {
        MemPtr = malloc(BytesNeeded);
        if (MemPtr != NULL) {
            // Second call to actually write
            if (!cmsSaveProfileToMem(hProfile, MemPtr, &BytesNeeded)) {
                // Write failed
            }
            free(MemPtr);
        }
    }

    // Test cmsGetProfileInfo
    infoLen = cmsGetProfileInfo(hProfile, 0, LanguageCode, CountryCode, Buffer, sizeof(Buffer)/sizeof(wchar_t));
    (void)infoLen;

    // Try to create a second profile for more state exploration
    if (Size >= 12) {
        uint32_t sizeField2 = 0;
        memcpy(&sizeField2, Data + 4, 4);
        if (sizeField2 > 0 && sizeField2 <= 100 * 1024 * 1024 && 
            Data[8] == 'a' && Data[9] == 'c' && Data[10] == 's' && Data[11] == 'p') {
            hProfile2 = cmsOpenProfileFromMem(Data + 4, Size - 4);
        }
    }

    if (hProfile2 == NULL) {
        hProfile2 = cmsCreate_sRGBProfile();
    }

    if (hProfile2 != NULL) {
        // Test with second profile
        cmsGetHeaderCreationDateTime(hProfile2, &Dest);
        cmsMD5computeID(hProfile2);
        tagCount = cmsGetTagCount(hProfile2);
        if (tagCount > 0) {
            cmsTagSignature sig2 = cmsGetTagSignature(hProfile2, 0);
            if (sig2 != 0) {
                cmsWriteTag(hProfile2, sig2, NULL);
            }
        }
        cmsSaveProfileToMem(hProfile2, NULL, &BytesNeeded);
        if (BytesNeeded > 0) {
            MemPtr = malloc(BytesNeeded);
            if (MemPtr != NULL) {
                cmsSaveProfileToMem(hProfile2, MemPtr, &BytesNeeded);
                free(MemPtr);
            }
        }
        cmsGetProfileInfo(hProfile2, 0, LanguageCode, CountryCode, Buffer, sizeof(Buffer)/sizeof(wchar_t));
        cmsCloseProfile(hProfile2);
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    return 0;
}