// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsCreateXYZProfile at cmsvirt.c:617:23 in lcms2.h
// cmsCreateBCHSWabstractProfile at cmsvirt.c:946:32 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsCreateTransform at cmsxform.c:1350:32 in lcms2.h
// cmsDeleteTransform at cmsxform.c:147:16 in lcms2.h
// cmsCreateTransformTHR at cmsxform.c:1333:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_profiles(cmsHPROFILE p1, cmsHPROFILE p2) {
    if (p1 != NULL) cmsCloseProfile(p1);
    if (p2 != NULL) cmsCloseProfile(p2);
}

static void cleanup_transform(cmsHTRANSFORM t) {
    if (t != NULL) cmsDeleteTransform(t);
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    cmsHPROFILE profile1 = NULL;
    cmsHPROFILE profile2 = NULL;
    cmsHTRANSFORM transform = NULL;
    cmsContext context = NULL;

    // Attempt to create a context
    context = cmsCreateContext(NULL, NULL);

    // Try to open a profile from memory if data is large enough to look like a header
    // ICC profiles are at least 128 bytes, but we try with smaller to test error handling
    if (Size >= 128) {
        profile1 = cmsOpenProfileFromMem(Data, Size);
        if (profile1 == NULL) {
            // If opening failed, try creating a synthetic profile
            profile1 = cmsCreateXYZProfile();
        }
    } else {
        profile1 = cmsCreateXYZProfile();
    }

    // Create a second profile using BCHSW abstract profile
    // Use fuzzed data for parameters if available, otherwise defaults
    cmsUInt32Number nLUTPoints = (Size > 0) ? Data[0] : 10;
    if (nLUTPoints == 0) nLUTPoints = 10;
    
    cmsFloat64Number Bright = (Size > 1) ? (cmsFloat64Number)Data[1] / 255.0 : 1.0;
    cmsFloat64Number Contrast = (Size > 2) ? (cmsFloat64Number)Data[2] / 255.0 : 1.0;
    cmsFloat64Number Hue = (Size > 3) ? (cmsFloat64Number)Data[3] / 255.0 : 0.0;
    cmsFloat64Number Saturation = (Size > 4) ? (cmsFloat64Number)Data[4] / 255.0 : 1.0;
    cmsUInt32Number TempSrc = (Size > 5) ? Data[5] : 6500;
    cmsUInt32Number TempDest = (Size > 6) ? Data[6] : 6500;

    if (TempSrc == 0) TempSrc = 6500;
    if (TempDest == 0) TempDest = 6500;

    profile2 = cmsCreateBCHSWabstractProfile(nLUTPoints, Bright, Contrast, Hue, Saturation, TempSrc, TempDest);

    if (profile1 == NULL) {
        // If we still don't have a profile, try to create one from memory with dummy data
        // This might fail, which is fine for fuzzing
        static uint8_t dummy_profile[128];
        memset(dummy_profile, 0, sizeof(dummy_profile));
        profile1 = cmsOpenProfileFromMem(dummy_profile, sizeof(dummy_profile));
    }

    // Test cmsDetectRGBProfileGamma
    if (profile1 != NULL) {
        cmsFloat64Number threshold = (Size > 7) ? (cmsFloat64Number)Data[7] / 255.0 : 0.01;
        cmsDetectRGBProfileGamma(profile1, threshold);
    }

    // Test cmsCreateTransform
    if (profile1 != NULL && profile2 != NULL) {
        cmsUInt32Number InputFormat = (Size > 8) ? Data[8] : 0;
        cmsUInt32Number OutputFormat = (Size > 9) ? Data[9] : 0;
        cmsUInt32Number Intent = (Size > 10) ? Data[10] : 0;
        cmsUInt32Number dwFlags = (Size > 11) ? Data[11] : 0;

        // Ensure formats are somewhat valid or test invalid ones
        // Common formats: 0 (undefined), 1 (RGB8), 2 (RGB16), etc.
        // We let the fuzzer explore invalid formats too.
        
        transform = cmsCreateTransform(profile1, InputFormat, profile2, OutputFormat, Intent, dwFlags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
            transform = NULL;
        }
    }

    // Test cmsCreateTransformTHR
    if (profile1 != NULL && profile2 != NULL && context != NULL) {
        cmsUInt32Number InputFormat = (Size > 12) ? Data[12] : 0;
        cmsUInt32Number OutputFormat = (Size > 13) ? Data[13] : 0;
        cmsUInt32Number Intent = (Size > 14) ? Data[14] : 0;
        cmsUInt32Number dwFlags = (Size > 15) ? Data[15] : 0;

        transform = cmsCreateTransformTHR(context, profile1, InputFormat, profile2, OutputFormat, Intent, dwFlags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
            transform = NULL;
        }
    }

    // Test with NULL output profile (single profile transform)
    if (profile1 != NULL && context != NULL) {
        cmsUInt32Number InputFormat = (Size > 16) ? Data[16] : 0;
        cmsUInt32Number Intent = (Size > 17) ? Data[17] : 0;
        cmsUInt32Number dwFlags = (Size > 18) ? Data[18] : 0;

        transform = cmsCreateTransformTHR(context, profile1, InputFormat, NULL, 0, Intent, dwFlags);
        if (transform != NULL) {
            cmsDeleteTransform(transform);
            transform = NULL;
        }
    }

    // Cleanup
    cleanup_profiles(profile1, profile2);
    if (context != NULL) cmsDeleteContext(context);

    return 0;
}