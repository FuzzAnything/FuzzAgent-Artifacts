// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsIsMatrixShaper at cmsio1.c:806:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsReadTag at cmsio0.c:1636:17 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext *ctx = cmsCreateContext(0, 0);
    if (!ctx) return 0;

    // Write input to dummy file to attempt loading
    write_dummy_file(Data, Size);

    cmsHPROFILE hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    if (!hProfile) {
        cmsDeleteContext(ctx);
        return 0;
    }

    // Test cmsGetTagCount
    cmsInt32Number tagCount = cmsGetTagCount(hProfile);
    (void)tagCount; // Use to suppress unused warning

    // Test cmsIsMatrixShaper
    cmsBool isMatrix = cmsIsMatrixShaper(hProfile);
    (void)isMatrix;

    // Test cmsMD5computeID
    cmsBool md5Result = cmsMD5computeID(hProfile);
    (void)md5Result;

    // Test cmsReadTag with a common tag signature (e.g., "desc")
    cmsTagSignature sig = 'desc';
    void *tagData = cmsReadTag(hProfile, sig);
    if (tagData) {
        // Data is cached internally, do not free manually
    }

    // Test cmsWriteTag with NULL to attempt deletion
    cmsBool writeResult = cmsWriteTag(hProfile, sig, NULL);
    (void)writeResult;

    // Test cmsWriteTag with dummy data if available
    if (Size > 0) {
        cmsBool writeResult2 = cmsWriteTag(hProfile, 'test', Data);
        (void)writeResult2;
    }

    // Cleanup
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return 0;
}