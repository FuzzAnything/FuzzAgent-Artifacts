// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_70(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    // Extract data for cmsCIExyY (x, y, Y are typically doubles or floats, 
    // but we will interpret raw bytes carefully to avoid immediate crashes on invalid floats)
    // cmsCIExyY structure typically contains: double x, y, Y;
    // We need at least 24 bytes for 3 doubles to be safe, but let's try to parse what we have.
    
    cmsCIExyY whitePoint;
    cmsCIExyY whitePoint2;
    cmsCIEXYZ xyz;
    cmsCIEXYZ xyz2;
    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsHPROFILE profile3 = NULL;
    cmsContext context = NULL;

    // Initialize with safe defaults
    whitePoint.x = 0.0;
    whitePoint.y = 0.0;
    whitePoint.Y = 0.0;
    
    whitePoint2.x = 0.0;
    whitePoint2.y = 0.0;
    whitePoint2.Y = 0.0;

    xyz.X = 0.0;
    xyz.Y = 0.0;
    xyz.Z = 0.0;

    // Try to parse input as doubles for whitePoint if enough data
    if (Size >= 24) {
        memcpy(&whitePoint.x, Data, sizeof(double));
        memcpy(&whitePoint.y, Data + 8, sizeof(double));
        memcpy(&whitePoint.Y, Data + 16, sizeof(double));
    } else if (Size >= 12) {
        // Fallback to floats if only 12 bytes
        float fx, fy, fY;
        memcpy(&fx, Data, sizeof(float));
        memcpy(&fy, Data + 4, sizeof(float));
        memcpy(&fY, Data + 8, sizeof(float));
        whitePoint.x = fx;
        whitePoint.y = fy;
        whitePoint.Y = fY;
    }

    // Parse second white point from remaining data or reuse
    if (Size >= 36) {
        memcpy(&whitePoint2.x, Data + 24, sizeof(double));
        memcpy(&whitePoint2.y, Data + 32, sizeof(double));
        memcpy(&whitePoint2.Y, Data + 40, sizeof(double));
    } else {
        // Use D50 as fallback or randomize slightly
        const cmsCIExyY *d50 = cmsD50_xyY();
        if (d50) {
            whitePoint2 = *d50;
        }
    }

    // Parse XYZ data if available
    if (Size >= 48) {
        memcpy(&xyz.X, Data + 36, sizeof(double));
        memcpy(&xyz.Y, Data + 44, sizeof(double));
        memcpy(&xyz.Z, Data + 52, sizeof(double));
    } else {
        // Generate some dummy XYZ
        xyz.X = whitePoint.x;
        xyz.Y = whitePoint.y;
        xyz.Z = whitePoint.Y;
    }

    // 1. Test cmsCreateLab4Profile
    // Ensure whitePoint is not NULL (it's a local struct, so it's valid memory)
    profile1 = cmsCreateLab4Profile(&whitePoint);
    cleanup_profile(profile1);

    // 2. Test cmsCreateLab2Profile
    profile2 = cmsCreateLab2Profile(&whitePoint);
    cleanup_profile(profile2);

    // 3. Test cmsXYZ2xyY
    cmsCIExyY dest_xyY;
    dest_xyY.x = 0; dest_xyY.y = 0; dest_xyY.Y = 0;
    cmsXYZ2xyY(&dest_xyY, &xyz);

    // 4. Test cmsxyY2XYZ
    // Avoid division by zero if y is 0
    if (whitePoint.y != 0.0) {
        cmsCIEXYZ dest_XYZ;
        cmsxyY2XYZ(&dest_XYZ, &whitePoint);
    } else {
        // Force a non-zero y for the test to avoid undefined behavior crash in the library
        cmsCIExyY safeWhitePoint = whitePoint;
        safeWhitePoint.y = 0.001; 
        cmsCIEXYZ dest_XYZ_safe;
        cmsxyY2XYZ(&dest_XYZ_safe, &safeWhitePoint);
    }

    // 5. Test cmsCreateLab2ProfileTHR
    // Create a context first
    context = cmsCreateContext(NULL, NULL);
    if (context) {
        profile3 = cmsCreateLab2ProfileTHR(context, &whitePoint2);
        cleanup_profile(profile3);
        
        // Test with NULL white point (should use default D50)
        profile3 = cmsCreateLab2ProfileTHR(context, NULL);
        cleanup_profile(profile3);
        
        cmsDeleteContext(context);
    }

    // 6. Test cmsD50_xyY
    const cmsCIExyY *d50_ptr = cmsD50_xyY();
    if (d50_ptr) {
        // Just access to ensure it's valid
        volatile double dummy = d50_ptr->x;
        (void)dummy;
    }

    return 0;
}