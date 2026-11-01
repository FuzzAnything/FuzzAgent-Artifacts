// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsWhitePointFromTemp at cmswtpnt.c:48:20 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsTempFromWhitePoint at cmswtpnt.c:143:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsCreateLab4ProfileTHR at cmsvirt.c:524:23 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
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

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    cmsCIExyY whitePoint;
    cmsCIExyY whitePointOut;
    cmsFloat64Number tempK;
    cmsHPROFILE profile = NULL;
    cmsContext context = NULL;

    // Initialize whitePoint with data from input or defaults
    // Ensure we have at least 16 bytes (2 doubles) before reading x and y
    if (Size >= 16) {
        whitePoint.x = *(const cmsFloat64Number*)(Data);
        whitePoint.y = *(const cmsFloat64Number*)(Data + 8);
        whitePoint.Y = 1.0; // Fixed Y for simplicity
    } else {
        whitePoint.x = 0.3127; // D65 default
        whitePoint.y = 0.3290;
        whitePoint.Y = 1.0;
    }

    // Test 1: cmsTempFromWhitePoint
    if (Size > 0) {
        cmsBool res = cmsTempFromWhitePoint(&tempK, &whitePoint);
        (void)res; // Use result to avoid unused warning
    }

    // Test 2: cmsWhitePointFromTemp
    if (Size > 0) {
        // Derive temp from first byte, clamp to valid range 4000-25000
        cmsFloat64Number inputTemp = (cmsFloat64Number)Data[0];
        if (inputTemp < 4000) inputTemp = 4000;
        if (inputTemp > 25000) inputTemp = 25000;
        
        cmsBool res = cmsWhitePointFromTemp(&whitePointOut, inputTemp);
        (void)res;
        
        // If successful, try to convert back to temp
        if (res) {
            cmsTempFromWhitePoint(&tempK, &whitePointOut);
        }
    }

    // Test 3: cmsCreateLab4Profile
    profile = cmsCreateLab4Profile(&whitePoint);
    cleanup_profile(profile);

    // Test 4: cmsCreateLab2Profile
    profile = cmsCreateLab2Profile(&whitePoint);
    cleanup_profile(profile);

    // Test 5: cmsD50_xyY
    const cmsCIExyY *d50 = cmsD50_xyY();
    if (d50 != NULL) {
        // Use d50 to create profiles
        profile = cmsCreateLab4Profile(d50);
        cleanup_profile(profile);
        profile = cmsCreateLab2Profile(d50);
        cleanup_profile(profile);
        
        // Test conversion from D50
        cmsTempFromWhitePoint(&tempK, d50);
    }

    // Test 6: cmsCreateLab4ProfileTHR
    // Create a context if we have enough data to simulate variety
    if (Size >= 4) {
        context = cmsCreateContext(NULL, NULL);
        if (context != NULL) {
            profile = cmsCreateLab4ProfileTHR(context, &whitePoint);
            cleanup_profile(profile);
            
            profile = cmsCreateLab4ProfileTHR(context, d50);
            cleanup_profile(profile);
            
            // Test with NULL white point (should default to D50)
            profile = cmsCreateLab4ProfileTHR(context, NULL);
            cleanup_profile(profile);
            
            cmsDeleteContext(context);
        }
    }

    // Edge case: Test with potentially invalid white points (negative or > 1)
    if (Size > 0) {
        cmsCIExyY invalidWP;
        invalidWP.x = (cmsFloat64Number)Data[0] / 255.0 * 2.0 - 0.5; // Range -0.5 to 1.5
        invalidWP.y = (cmsFloat64Number)Data[0] / 255.0 * 2.0 - 0.5;
        invalidWP.Y = 1.0;
        
        cmsTempFromWhitePoint(&tempK, &invalidWP);
        cmsWhitePointFromTemp(&whitePointOut, 4000); // Valid temp
        profile = cmsCreateLab4Profile(&invalidWP);
        cleanup_profile(profile);
        profile = cmsCreateLab2Profile(&invalidWP);
        cleanup_profile(profile);
    }

    return 0;
}