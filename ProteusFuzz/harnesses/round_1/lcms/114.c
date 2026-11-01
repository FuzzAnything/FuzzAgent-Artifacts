// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    cmsHPROFILE profile = NULL;
    cmsCIEXYZ xyz;
    cmsCIExyY xyY;
    cmsFloat64Number tempK = 0.0;
    cmsBool result;

    // Extract basic values from input safely
    uint16_t val1 = Data[0] | (Data[1] << 8);
    uint16_t val2 = Data[2] | (Data[3] << 8);
    uint16_t val3 = Data[4] | (Data[5] << 8);

    // Initialize XYZ with fuzzed values, avoiding all zeros to prevent division by zero in cmsXYZ2xyY
    xyz.X = (cmsFloat64Number)(val1 & 0xFF) / 1000.0;
    xyz.Y = (cmsFloat64Number)((val1 >> 8) & 0xFF) / 1000.0;
    xyz.Z = (cmsFloat64Number)(val2 & 0xFF) / 1000.0;

    // Ensure sum is not zero
    if (xyz.X == 0.0 && xyz.Y == 0.0 && xyz.Z == 0.0) {
        xyz.X = 0.001;
    }

    // Test cmsXYZ2xyY
    cmsXYZ2xyY(&xyY, &xyz);

    // Test cmsTempFromWhitePoint with the computed xyY
    result = cmsTempFromWhitePoint(&tempK, &xyY);
    (void)result;

    // Test cmsWhitePointFromTemp with fuzzed temperature
    cmsFloat64Number fuzzTemp = (cmsFloat64Number)val3;
    if (fuzzTemp < 4000.0) fuzzTemp = 4000.0;
    if (fuzzTemp > 25000.0) fuzzTemp = 25000.0;

    cmsCIExyY whitePointFromTemp;
    result = cmsWhitePointFromTemp(&whitePointFromTemp, fuzzTemp);
    if (result) {
        // Use the generated white point to create profiles
        profile = cmsCreateLab4Profile(&whitePointFromTemp);
        cleanup_profile(profile);

        profile = cmsCreateLab2Profile(&whitePointFromTemp);
        cleanup_profile(profile);
    }

    // Test with D50 white point
    const cmsCIExyY *d50 = cmsD50_xyY();
    if (d50) {
        profile = cmsCreateLab4Profile(d50);
        cleanup_profile(profile);

        profile = cmsCreateLab2Profile(d50);
        cleanup_profile(profile);

        result = cmsTempFromWhitePoint(&tempK, d50);
        (void)result;
    }

    // Test with zero-sum XYZ (should handle gracefully in cmsXYZ2xyY if implemented safely)
    cmsCIEXYZ zeroXYZ = {0.0, 0.0, 0.0};
    cmsCIExyY zeroXY;
    cmsXYZ2xyY(&zeroXY, &zeroXYZ);

    // Test with NULL pointer for cmsCreateLab4Profile and cmsCreateLab2Profile (should return NULL)
    profile = cmsCreateLab4Profile(NULL);
    cleanup_profile(profile);

    profile = cmsCreateLab2Profile(NULL);
    cleanup_profile(profile);

    return 0;
}