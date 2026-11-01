// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE *profile) {
    if (profile && *profile) {
        cmsCloseProfile(*profile);
        *profile = NULL;
    }
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsCIExyY whitePoint = {0.0, 0.0, 0.0};
    cmsCIEXYZ xyzSource = {0.0, 0.0, 0.0};
    cmsCIExyY xyYDest = {0.0, 0.0, 0.0};
    cmsFloat64Number tempK = 0.0;
    cmsBool result;

    // Extract data from input
    uint16_t tempSeed = Data[0] | (Data[1] << 8);
    cmsFloat64Number temp = 4000.0 + (tempSeed % 21001); // Range 4000K - 25000K

    // Test cmsD50_xyY
    const cmsCIExyY *d50 = cmsD50_xyY();
    if (d50) {
        // Use d50 to create a profile
        profile1 = cmsCreateLab2Profile(d50);
        cleanup_profile(&profile1);

        profile2 = cmsCreateLab4Profile(d50);
        cleanup_profile(&profile2);

        // Test cmsTempFromWhitePoint with D50
        cmsTempFromWhitePoint(&tempK, d50);
    }

    // Test cmsWhitePointFromTemp
    if (Size >= 4) {
        temp = 4000.0 + (Data[2] | (Data[3] << 8)) % 21001;
        result = cmsWhitePointFromTemp(&whitePoint, temp);
        if (result) {
            // Use generated white point
            profile1 = cmsCreateLab2Profile(&whitePoint);
            cleanup_profile(&profile1);

            profile2 = cmsCreateLab4Profile(&whitePoint);
            cleanup_profile(&profile2);

            // Test cmsTempFromWhitePoint with generated white point
            cmsTempFromWhitePoint(&tempK, &whitePoint);
        }
    }

    // Test cmsXYZ2xyY
    if (Size >= 12) {
        xyzSource.X = (Data[4] | (Data[5] << 8)) / 256.0;
        xyzSource.Y = (Data[6] | (Data[7] << 8)) / 256.0;
        xyzSource.Z = (Data[8] | (Data[9] << 8)) / 256.0;
        
        // Avoid division by zero in cmsXYZ2xyY
        if (xyzSource.X + xyzSource.Y + xyzSource.Z > 0.0) {
            cmsXYZ2xyY(&xyYDest, &xyzSource);
            
            // Use converted xyY
            profile1 = cmsCreateLab2Profile(&xyYDest);
            cleanup_profile(&profile1);

            profile2 = cmsCreateLab4Profile(&xyYDest);
            cleanup_profile(&profile2);

            cmsTempFromWhitePoint(&tempK, &xyYDest);
        }
    }

    // Test edge cases with invalid inputs
    // cmsCreateLab2Profile with NULL
    profile1 = cmsCreateLab2Profile(NULL);
    if (profile1) cleanup_profile(&profile1);

    // cmsCreateLab4Profile with NULL
    profile2 = cmsCreateLab4Profile(NULL);
    if (profile2) cleanup_profile(&profile2);

    // cmsTempFromWhitePoint with NULL
    cmsTempFromWhitePoint(NULL, &whitePoint);
    cmsTempFromWhitePoint(&tempK, NULL);

    // cmsWhitePointFromTemp with out of range
    cmsWhitePointFromTemp(&whitePoint, 100.0); // Too low
    cmsWhitePointFromTemp(&whitePoint, 30000.0); // Too high

    return 0;
}