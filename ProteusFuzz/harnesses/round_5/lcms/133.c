// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsWriteRawTag at cmsio0.c:2034:19 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsSaveProfileToFile at cmsio0.c:1502:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <time.h>
#include <unistd.h>

static cmsHPROFILE create_test_profile(void) {
    // cmsCreateProfilePlaceholder requires a cmsContext argument.
    // NULL is used to create a profile in the default context.
    cmsHPROFILE hProfile = cmsCreateProfilePlaceholder(NULL);
    if (hProfile == NULL) return NULL;
    return hProfile;
}

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsHPROFILE hProfile = create_test_profile();
    if (hProfile == NULL) return 0;

    // Extract some data from input to use as tag data
    const void *tagData = Data;
    cmsUInt32Number tagSize = (Size > 0x100000) ? 0x100000 : Size;

    // 1. Set Device Class
    cmsSetDeviceClass(hProfile, cmsSigInputClass);

    // 2. Write Raw Tag
    // Using a dummy signature "TEST" (0x54455354)
    cmsTagSignature sig = (cmsTagSignature)0x54455354; 
    cmsWriteRawTag(hProfile, sig, tagData, tagSize);

    // 3. Compute MD5 ID
    cmsMD5computeID(hProfile);

    // 4. Get Device Class
    cmsProfileClassSignature deviceClass = cmsGetDeviceClass(hProfile);
    (void)deviceClass; // Suppress unused variable warning

    // 5. Get Creation DateTime
    struct tm creationTime;
    cmsGetHeaderCreationDateTime(hProfile, &creationTime);
    (void)creationTime; // Suppress unused variable warning

    // 6. Save Profile to File
    const char *filename = "./dummy_file";
    cmsSaveProfileToFile(hProfile, filename);

    // Cleanup
    cleanup_profile(hProfile);

    // Remove dummy file if it exists
    unlink(filename);

    return 0;
}