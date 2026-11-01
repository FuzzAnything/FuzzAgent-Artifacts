// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsSetDeviceClass at cmsio0.c:1099:16 in lcms2.h
// cmsGetDeviceClass at cmsio0.c:1093:36 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetHeaderCreationDateTime at cmsio0.c:1062:20 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsSaveProfileToMem at cmsio0.c:1536:19 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsMD5computeID at cmsmd5.c:257:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsHPROFILE hOutput = NULL;
    cmsUInt32Number bytesNeeded = 0;
    void *memBuffer = NULL;
    struct tm creationTime;
    cmsProfileClassSignature deviceClass;
    cmsInt32Number tagCount;
    cmsBool result;

    // Attempt to open profile from input memory
    if (Size > 128) {
        hProfile = cmsOpenProfileFromMem(Data, Size);
    }
    
    if (hProfile == NULL) {
        // Fallback: Create a minimal profile from a dummy file
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            // Write minimal valid ICC header (128 bytes)
            unsigned char dummyHeader[128] = {0};
            // Set size to 128 (little endian)
            dummyHeader[0] = 0; dummyHeader[1] = 0; dummyHeader[2] = 0; dummyHeader[3] = 128;
            // Magic number for ICC
            dummyHeader[36] = 'a'; dummyHeader[37] = 'c'; dummyHeader[38] = 's'; dummyHeader[39] = 'p';
            fwrite(dummyHeader, 1, 128, f);
            fclose(f);
            
            hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
            if (hProfile == NULL) {
                return 0;
            }
        } else {
            return 0;
        }
    }

    // Test cmsGetDeviceClass
    deviceClass = cmsGetDeviceClass(hProfile);
    
    // Test cmsSetDeviceClass with various values from input
    if (Size > 0) {
        cmsProfileClassSignature sig = (cmsProfileClassSignature)Data[0];
        cmsSetDeviceClass(hProfile, sig);
    }
    
    // Verify change
    deviceClass = cmsGetDeviceClass(hProfile);

    // Test cmsGetTagCount
    tagCount = cmsGetTagCount(hProfile);

    // Test cmsGetHeaderCreationDateTime
    memset(&creationTime, 0, sizeof(struct tm));
    result = cmsGetHeaderCreationDateTime(hProfile, &creationTime);

    // Test cmsSaveProfileToMem
    // First call to get size
    result = cmsSaveProfileToMem(hProfile, NULL, &bytesNeeded);
    
    if (result && bytesNeeded > 0) {
        memBuffer = malloc(bytesNeeded);
        if (memBuffer != NULL) {
            result = cmsSaveProfileToMem(hProfile, memBuffer, &bytesNeeded);
            if (result) {
                // Optionally verify by creating a new profile from memory
                hOutput = cmsOpenProfileFromMem(memBuffer, bytesNeeded);
                if (hOutput != NULL) {
                    cmsCloseProfile(hOutput);
                    hOutput = NULL;
                }
            }
            free(memBuffer);
            memBuffer = NULL;
        }
    }

    // Test cmsMD5computeID
    result = cmsMD5computeID(hProfile);

    // Cleanup
    cleanup_profile(hProfile);
    if (hOutput != NULL) {
        cmsCloseProfile(hOutput);
    }
    if (memBuffer != NULL) {
        free(memBuffer);
    }

    return 0;
}