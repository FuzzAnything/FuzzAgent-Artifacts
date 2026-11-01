// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
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

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsBool closeResult;
    cmsFloat64Number tacResult;
    cmsFloat64Number versionResult;
    cmsInt32Number tagCount;

    // 1. Prepare environment: Open profile from memory
    // The input Data is treated as the ICC profile buffer.
    hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile != NULL) {
        // 2. Explore program states with valid profile
        
        // Test cmsSetProfileVersion with various inputs derived from fuzz data
        // We try to set version to 0.0, 4.2, or a value derived from data
        cmsFloat64Number testVersion;
        if (Size >= 8) {
            // Construct a float from the first 8 bytes if available
            memcpy(&testVersion, Data, 8);
        } else {
            testVersion = 4.2;
        }
        
        // Clamp or adjust version to reasonable range to avoid immediate crashes
        // though lcms should handle invalid versions gracefully
        if (testVersion < 0.0) testVersion = 0.0;
        if (testVersion > 100.0) testVersion = 100.0;
        
        cmsSetProfileVersion(hProfile, testVersion);

        // Test cmsGetProfileVersion
        versionResult = cmsGetProfileVersion(hProfile);

        // Test cmsGetTagCount
        tagCount = cmsGetTagCount(hProfile);

        // Test cmsDetectTAC
        // This function requires specific profile types (output profiles)
        // It will likely return 0 for random data, which is expected behavior
        tacResult = cmsDetectTAC(hProfile);

        // 3. Cleanup
        closeResult = cmsCloseProfile(hProfile);
        (void)closeResult; // Suppress unused variable warning
    }

    // Additional state exploration: Try to open a dummy file if needed, 
    // but cmsOpenProfileFromMem is the primary target here.
    // The logic above covers the main API interaction flow.

    return 0;
}