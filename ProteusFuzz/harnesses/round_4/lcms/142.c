// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <lcms2.h>

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;

    // Attempt to open a profile from the input memory buffer
    hProfile = cmsOpenProfileFromMem(Data, Size);

    if (hProfile != NULL) {
        // Test cmsGetTagCount
        cmsInt32Number tagCount = cmsGetTagCount(hProfile);
        (void)tagCount;

        // Test cmsGetProfileVersion
        cmsFloat64Number version = cmsGetProfileVersion(hProfile);
        (void)version;

        // Test cmsDetectTAC
        cmsFloat64Number tac = cmsDetectTAC(hProfile);
        (void)tac;

        // Test cmsSetProfileVersion with fuzzed value
        cmsFloat64Number fuzzedVersion = 0.0;
        if (Size >= sizeof(cmsFloat64Number)) {
            memcpy(&fuzzedVersion, Data, sizeof(cmsFloat64Number));
            cmsSetProfileVersion(hProfile, fuzzedVersion);
        } else if (Size > 0) {
            // Fallback for small inputs: derive a version from the first byte
            fuzzedVersion = (cmsFloat64Number)Data[0] / 10.0;
            cmsSetProfileVersion(hProfile, fuzzedVersion);
        }

        // Clean up the profile
        cmsCloseProfile(hProfile);
    }

    return 0;
}