// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void write_dummy_profile(const uint8_t *Data, size_t Size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 128) {
        return 0;
    }

    const char *filename = "./dummy_file";
    write_dummy_profile(Data, Size, filename);

    cmsHPROFILE hProfile = cmsOpenProfileFromFile(filename, "r");
    if (!hProfile) {
        return 0;
    }

    // Test cmsIsMatrixShaper
    cmsBool isMatrix = cmsIsMatrixShaper(hProfile);
    (void)isMatrix;

    // Test cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    if (tagCount > 0) {
        // Try to read a tag if we know the signature, but we don't, so skip specific tag reads
        // We can try to read a common tag like 'rTRC' if we cast, but safer to just test API flow
        // Let's try to read a tag with a dummy signature to trigger error handling
        cmsTagSignature dummySig = 0x72545243; // 'rTRC'
        void *tagData = cmsReadTag(hProfile, dummySig);
        (void)tagData;
    }

    // Test cmsMD5computeID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result;

    // Test cmsWriteTag with NULL data to attempt deletion of a dummy tag
    cmsTagSignature dummySig2 = 0x64756d6d; // 'dumm'
    cmsBool writeResult = cmsWriteTag(hProfile, dummySig2, NULL);
    (void)writeResult;

    // Test cmsWriteTag with some dummy data to attempt addition
    uint8_t dummyData[16] = {0};
    cmsBool writeResult2 = cmsWriteTag(hProfile, dummySig2, dummyData);
    (void)writeResult2;

    // Cleanup
    cmsCloseProfile(hProfile);

    return 0;
}