// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsIsTag at cmsio0.c:708:19 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsWriteTag at cmsio0.c:1789:19 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <time.h>

static cmsHPROFILE create_dummy_profile_from_data(const uint8_t *data, size_t size) {
    if (size < 128) {
        return NULL;
    }
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(data, size);
    return hProfile;
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    struct tm tm_struct;
    FILE *fp = NULL;
    cmsUInt32Number bytesNeeded = 0;
    void *memBuffer = NULL;
    cmsBool res;
    cmsInt32Number tagCount;
    cmsTagSignature sig;

    // Initialize tm_struct to avoid uninitialized memory usage
    memset(&tm_struct, 0, sizeof(struct tm));

    // Try to create a profile from the input data
    hProfile = create_dummy_profile_from_data(Data, Size);

    // If profile creation failed, we do NOT call functions with NULL handle 
    // because lcms2 does not handle NULL gracefully for some functions (e.g., cmsGetHeaderCreationDateTime).
    // Instead, we only test with a valid profile or skip.
    if (hProfile == NULL) {
        // Cannot test with NULL safely for all APIs, so just return.
        // This avoids the SEGV observed in the sanitizer report.
        return 0;
    }

    // 1. Test cmsGetHeaderCreationDateTime
    res = cmsGetHeaderCreationDateTime(hProfile, &tm_struct);
    (void)res;

    // 2. Test cmsSaveProfileToStream
    fp = fopen("./dummy_file", "wb");
    if (fp) {
        res = cmsSaveProfileToStream(hProfile, fp);
        (void)res;
        fclose(fp);
    }

    // 3. Test cmsSaveProfileToMem
    // First call to get required size
    res = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    if (res && bytesNeeded > 0) {
        memBuffer = malloc(bytesNeeded);
        if (memBuffer) {
            res = cmsSaveProfileToMem(hProfile, memBuffer, &bytesNeeded);
            (void)res;
            free(memBuffer);
        }
    }

    // 4. Test cmsIsTag with various signatures
    if (Size >= 4) {
        memcpy(&sig, Data, sizeof(cmsTagSignature));
        res = cmsIsTag(hProfile, sig);
        (void)res;
    } else {
        sig = 0;
        res = cmsIsTag(hProfile, sig);
        (void)res;
    }

    // 5. Test cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);
    (void)tagCount;

    // 6. Test cmsWriteTag
    // Try to write a tag with NULL data (delete) and with some data
    res = cmsWriteTag(hProfile, sig, NULL);
    (void)res;

    if (Size >= 10) {
        res = cmsWriteTag(hProfile, sig, Data + 4);
        (void)res;
    }

    // Cleanup
    cmsCloseProfile(hProfile);

    return 0;
}