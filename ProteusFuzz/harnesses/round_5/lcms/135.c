// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    cmsCIExyY whitePoint;
    cmsCIEXYZ xyzSource;
    cmsHPROFILE profile2 = NULL;
    cmsHPROFILE profile4 = NULL;
    cmsFloat64Number tempK = 0.0;
    cmsBool result;

    // Initialize whitePoint with fuzzed data or defaults
    if (Size >= sizeof(cmsCIExyY)) {
        memcpy(&whitePoint, Data, sizeof(cmsCIExyY));
    } else {
        whitePoint.x = 0.3127;
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }

    // Initialize xyzSource with fuzzed data or defaults
    if (Size >= sizeof(cmsCIExyY) + sizeof(cmsCIEXYZ)) {
        memcpy(&xyzSource, Data + sizeof(cmsCIExyY), sizeof(cmsCIEXYZ));
    } else {
        xyzSource.X = 0.9642;
        xyzSource.Y = 1.0000;
        xyzSource.Z = 0.8251;
    }

    // Fuzz cmsWhitePointFromTemp
    cmsFloat64Number tempFuzz = 0.0;
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&tempFuzz, Data, sizeof(cmsFloat64Number));
        // Clamp to reasonable range to avoid immediate failure, but allow edge cases
        if (tempFuzz < 0) tempFuzz = 4000;
        if (tempFuzz > 50000) tempFuzz = 25000;
    } else {
        tempFuzz = 6500;
    }
    result = cmsWhitePointFromTemp(&whitePoint, tempFuzz);
    (void)result; // Suppress unused warning

    // Fuzz cmsXYZ2xyY
    cmsCIExyY dest;
    cmsXYZ2xyY(&dest, &xyzSource);

    // Fuzz cmsD50_xyY
    const cmsCIExyY *d50 = cmsD50_xyY();
    if (d50 != NULL) {
        // Use the value to ensure it's processed
        (void)d50->x;
        (void)d50->y;
        (void)d50->Y;
    }

    // Fuzz cmsTempFromWhitePoint
    cmsFloat64Number tempOut = 0.0;
    result = cmsTempFromWhitePoint(&tempOut, &whitePoint);
    (void)result;

    // Fuzz cmsCreateLab2Profile
    profile2 = cmsCreateLab2Profile(&whitePoint);
    cleanup_profile(profile2);

    // Fuzz cmsCreateLab2Profile with NULL (should fail)
    profile2 = cmsCreateLab2Profile(NULL);
    cleanup_profile(profile2);

    // Fuzz cmsCreateLab4Profile
    profile4 = cmsCreateLab4Profile(&whitePoint);
    cleanup_profile(profile4);

    // Fuzz cmsCreateLab4Profile with NULL (undefined behavior, but we test it)
    profile4 = cmsCreateLab4Profile(NULL);
    cleanup_profile(profile4);

    // Additional state exploration: modify whitePoint slightly and retry
    whitePoint.x += 0.001;
    whitePoint.y -= 0.001;
    whitePoint.Y *= 0.5;

    profile2 = cmsCreateLab2Profile(&whitePoint);
    cleanup_profile(profile2);

    profile4 = cmsCreateLab4Profile(&whitePoint);
    cleanup_profile(profile4);

    return 0;
}