// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateProfilePlaceholder at cmsio0.c:526:23 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsSaveProfileToStream at cmsio0.c:1520:19 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsUInt32Number bytesNeeded = 0;
    void *memPtr = NULL;
    struct tm creationTime;
    cmsProfileClassSignature deviceClass;
    cmsInt32Number tagCount;
    cmsBool saveResult;
    FILE *stream = NULL;

    // Create a dummy profile using the input data if possible, or a minimal one
    // Since cmsCreateProfile requires specific parameters, we try to use the input
    // to influence the profile creation if it fits, otherwise create a basic one.
    // For fuzzing, we often create a profile from a dummy stream or just create a new one.
    // However, cmsCreateProfile is not in the target list, but it's needed to get a handle.
    // We will create a dummy profile using cmsCreateProfilePlaceholder or similar if available,
    // but the standard way is cmsCreateProfile. Let's assume we can create a profile.
    // Since we don't have cmsCreateProfile in the target list, we must rely on the fact that
    // the fuzzer might need to create a profile first. But the task says to fuzz the given APIs.
    // To get a valid hProfile, we must create one. The standard way is cmsCreateProfile.
    // We will use cmsCreateProfile with a dummy signature or a placeholder.
    // Actually, lcms2 provides cmsCreateProfilePlaceholder which is safe for testing.
    
    hProfile = cmsCreateProfilePlaceholder(NULL);
    if (hProfile == NULL) {
        return 0;
    }

    // 1. cmsSetDeviceClass
    // Use input data to determine the device class signature
    cmsProfileClassSignature sig = (cmsProfileClassSignature)(Data[0] & 0xFF);
    // Ensure it's a valid range if possible, but the function doesn't validate, so any value is fine for fuzzing
    cmsSetDeviceClass(hProfile, sig);

    // 2. cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);
    (void)tagCount; // Suppress unused variable warning

    // 3. cmsSaveProfileToStream
    // Create a dummy file
    stream = fopen("./dummy_file", "wb");
    if (stream != NULL) {
        saveResult = cmsSaveProfileToStream(hProfile, stream);
        (void)saveResult;
        fclose(stream);
    }

    // 4. cmsGetHeaderCreationDateTime
    memset(&creationTime, 0, sizeof(struct tm));
    cmsGetHeaderCreationDateTime(hProfile, &creationTime);

    // 5. cmsGetDeviceClass
    deviceClass = cmsGetDeviceClass(hProfile);
    (void)deviceClass;

    // 6. cmsSaveProfileToMem
    // First call to get required size
    if (!cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded)) {
        // If calculation fails, bytesNeeded might be 0, but we proceed to cleanup
    } else {
        if (bytesNeeded > 0) {
            memPtr = malloc(bytesNeeded);
            if (memPtr != NULL) {
                saveResult = cmsSaveProfileToMem(hProfile, memPtr, &bytesNeeded);
                (void)saveResult;
                free(memPtr);
            }
        }
    }

    // Cleanup
    cleanup_profile(hProfile);

    return 0;
}