// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCreateLab2ProfileTHR at cmsvirt.c:473:23 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsxyY2XYZ at cmspcs.c:102:16 in lcms2.h
// cmsXYZ2xyY at cmspcs.c:91:16 in lcms2.h
// cmsD50_xyY at cmswtpnt.c:38:28 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab2Profile at cmsvirt.c:517:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
// cmsCreateLab4Profile at cmsvirt.c:570:23 in lcms2.h
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

static void cleanup_context(cmsContext context) {
    if (context != NULL) {
        cmsDeleteContext(context);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    // Extract data for cmsCIExyY (x, y, Y are typically 16-bit or 32-bit, 
    // but we'll interpret bytes to create varied inputs)
    cmsCIExyY xyY;
    cmsCIEXYZ xyz;
    
    // Initialize with some data from the input to ensure diversity
    xyY.x = (Data[0] | (Data[1] << 8)) / 65535.0;
    xyY.y = (Data[2] | (Data[3] << 8)) / 65535.0;
    xyY.Y = (Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
    
    // Ensure Y is non-negative
    if (xyY.Y < 0) xyY.Y = -xyY.Y;

    // Initialize XYZ for reverse conversion
    xyz.X = (Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24));
    xyz.Y = xyY.Y; // Keep Y consistent for testing round-trip
    xyz.Z = (Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24));
    if (xyz.X < 0) xyz.X = -xyz.X;
    if (xyz.Z < 0) xyz.Z = -xyz.Z;

    // Test cmsxyY2XYZ
    // Avoid division by zero if y is 0
    if (xyY.y != 0.0) {
        cmsCIEXYZ dest_xyz;
        cmsxyY2XYZ(&dest_xyz, &xyY);
        // Use the result to prevent unused variable warnings
        (void)dest_xyz.X;
        (void)dest_xyz.Y;
        (void)dest_xyz.Z;
    }

    // Test cmsXYZ2xyY
    // Avoid division by zero if sum is 0
    double sum = xyz.X + xyz.Y + xyz.Z;
    if (sum != 0.0) {
        cmsCIExyY dest_xyY;
        cmsXYZ2xyY(&dest_xyY, &xyz);
        (void)dest_xyY.x;
        (void)dest_xyY.y;
        (void)dest_xyY.Y;
    }

    // Test cmsD50_xyY
    const cmsCIExyY *d50 = cmsD50_xyY();
    if (d50 != NULL) {
        (void)d50->x;
        (void)d50->y;
        (void)d50->Y;
    }

    // Test cmsCreateLab2Profile
    cmsHPROFILE lab2_profile = NULL;
    if (xyY.y != 0.0) { // Avoid potential issues with invalid white points if library checks
        lab2_profile = cmsCreateLab2Profile(&xyY);
        cleanup_profile(lab2_profile);
    }
    // Also test with NULL to ensure robustness
    lab2_profile = cmsCreateLab2Profile(NULL);
    cleanup_profile(lab2_profile);

    // Test cmsCreateLab4Profile
    cmsHPROFILE lab4_profile = NULL;
    if (xyY.y != 0.0) {
        lab4_profile = cmsCreateLab4Profile(&xyY);
        cleanup_profile(lab4_profile);
    }
    // Test with NULL (undefined behavior per spec, but fuzzing should handle it gracefully or crash)
    // We skip NULL for Lab4 if the spec says undefined behavior to avoid immediate crashes in the fuzzer,
    // but typically fuzzers want to see the crash. However, the prompt says "NULL causes undefined behavior".
    // Let's try it anyway as fuzzing aims to find crashes.
    lab4_profile = cmsCreateLab4Profile(NULL);
    cleanup_profile(lab4_profile);

    // Test cmsCreateLab2ProfileTHR
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx != NULL) {
        cmsHPROFILE lab2_thr_profile = NULL;
        if (xyY.y != 0.0) {
            lab2_thr_profile = cmsCreateLab2ProfileTHR(ctx, &xyY);
            cleanup_profile(lab2_thr_profile);
        }
        // Test with NULL white point
        lab2_thr_profile = cmsCreateLab2ProfileTHR(ctx, NULL);
        cleanup_profile(lab2_thr_profile);
        
        cleanup_context(ctx);
    }

    return 0;
}