// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_XYZ at cmswtpnt.c:31:28 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsFloat2XYZEncoded at cmspcs.c:374:16 in lcms2.h
// cmsXYZEncoded2Float at cmspcs.c:429:16 in lcms2.h
// cmsLab2XYZ at cmspcs.c:161:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsDetectDestinationBlackPoint at cmssamp.c:355:19 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    cmsHPROFILE hProfile = NULL;
    cmsCIEXYZ blackPoint = {0, 0, 0};
    cmsCIEXYZ xyzDest = {0, 0, 0};
    cmsCIEXYZ labXyz = {0, 0, 0};
    cmsCIExyY xyY = {0, 0, 0};
    cmsCIELab lab = {0, 0, 0};
    cmsUInt16Number xyzEncoded[3] = {0, 0, 0};
    cmsCIEXYZ xyzFloat = {0, 0, 0};
    cmsUInt32Number intent = 0;
    cmsUInt32Number flags = 0;

    // Parse input to diversify states
    if (Size >= 1) intent = Data[0] % 4; // 0 to 3
    if (Size >= 2) flags = Data[1];
    if (Size >= 4) {
        xyY.x = Data[2] / 255.0;
        xyY.y = Data[3] / 255.0;
        xyY.Y = (Size >= 5) ? (double)Data[4] / 255.0 : 0.5;
    }
    if (Size >= 7) {
        lab.L = (double)Data[5] / 255.0 * 100.0;
        lab.a = (double)Data[6] / 255.0 * 200.0 - 100.0;
        lab.b = (Size >= 7) ? ((double)Data[7] / 255.0 * 200.0 - 100.0) : 0.0;
    }
    if (Size >= 10) {
        xyzFloat.X = (double)Data[8] / 255.0;
        xyzFloat.Y = (double)Data[9] / 255.0;
        xyzFloat.Z = (Size >= 10) ? ((double)Data[10] / 255.0) : 0.0;
    }
    if (Size >= 13) {
        xyzEncoded[0] = Data[11];
        xyzEncoded[1] = Data[12];
        xyzEncoded[2] = (Size >= 13) ? Data[13] : 0;
    }

    // Fuzz cmsD50_XYZ (no args, safe)
    const cmsCIEXYZ *d50 = cmsD50_XYZ();
    if (d50 != NULL) {
        // Use d50 for Lab2XYZ
        cmsLab2XYZ(d50, &labXyz, &lab);
    }

    // Fuzz cmsxyY2XYZ (avoid division by zero)
    if (xyY.y > 0.0001) {
        cmsxyY2XYZ(&xyzDest, &xyY);
    } else {
        // Fuzz with zero y to trigger undefined behavior (safely in fuzzing context)
        cmsxyY2XYZ(&xyzDest, &xyY);
    }

    // Fuzz cmsFloat2XYZEncoded
    cmsFloat2XYZEncoded(xyzEncoded, &xyzFloat);

    // Fuzz cmsXYZEncoded2Float
    cmsXYZEncoded2Float(&xyzFloat, xyzEncoded);

    // Fuzz cmsLab2XYZ with NULL white point (uses D50)
    cmsLab2XYZ(NULL, &labXyz, &lab);

    // Fuzz cmsDetectDestinationBlackPoint
    // We need a valid profile for this. Try to create one from input if possible.
    // Since we don't have a full profile generator here, we simulate with a dummy file.
    // Write dummy data to file
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
        hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    }

    cmsBool result = cmsDetectDestinationBlackPoint(&blackPoint, hProfile, intent, flags);
    (void)result; // Suppress unused warning

    cleanup_profile(hProfile);

    // Cleanup dummy file
    remove("./dummy_file");

    return 0;
}