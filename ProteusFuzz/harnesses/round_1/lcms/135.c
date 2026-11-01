// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDetectTAC at cmsgmt.c:462:28 in lcms2.h
// cmsGetTagCount at cmsio0.c:581:26 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsGetTagSignature at cmsio0.c:590:27 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsSetProfileVersion at cmsio0.c:1138:17 in lcms2.h
// cmsGetProfileVersion at cmsio0.c:1147:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    cmsHPROFILE hProfile = NULL;
    cmsFloat64Number tacValue;
    cmsInt32Number tagCount;
    cmsFloat64Number version;
    cmsTagSignature tagSig;
    cmsUInt32Number i;

    // Fuzz cmsOpenProfileFromMem
    hProfile = cmsOpenProfileFromMem((const void *)Data, (cmsUInt32Number)Size);

    if (hProfile != NULL) {
        // Fuzz cmsDetectTAC
        tacValue = cmsDetectTAC(hProfile);
        (void)tacValue;

        // Fuzz cmsGetTagCount
        tagCount = cmsGetTagCount(hProfile);
        if (tagCount > 0) {
            // Fuzz cmsGetTagSignature with valid indices
            for (i = 0; i < (cmsUInt32Number)tagCount; i++) {
                tagSig = cmsGetTagSignature(hProfile, i);
                (void)tagSig;
            }
            // Fuzz cmsGetTagSignature with out-of-bounds indices
            tagSig = cmsGetTagSignature(hProfile, (cmsUInt32Number)tagCount);
            tagSig = cmsGetTagSignature(hProfile, (cmsUInt32Number)tagCount + 100);
            (void)tagSig;
        }

        // Fuzz cmsGetProfileVersion
        version = cmsGetProfileVersion(hProfile);
        (void)version;

        // Fuzz cmsSetProfileVersion with various inputs
        cmsSetProfileVersion(hProfile, 0.0);
        cmsSetProfileVersion(hProfile, 4.2);
        cmsSetProfileVersion(hProfile, -1.5);
        cmsSetProfileVersion(hProfile, 100.0);
        
        // Verify version after setting
        version = cmsGetProfileVersion(hProfile);
        (void)version;

        // Cleanup
        cleanup_profile(hProfile);
    }

    // Removed NULL handle tests to prevent crashes in library functions 
    // that do not perform internal NULL checks (e.g., cmsDetectTAC, cmsGetDeviceClass).
    // The library expects valid handles for these operations.

    return 0;
}