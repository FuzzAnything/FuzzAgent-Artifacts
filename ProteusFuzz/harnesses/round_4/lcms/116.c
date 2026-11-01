// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsCreateRGBProfileTHR at cmsvirt.c:101:23 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsCreateRGBProfile at cmsvirt.c:217:23 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_profile(cmsHPROFILE hProfile) {
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
}

static void cleanup_tone_curve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static void cleanup_tone_curves_array(cmsToneCurve *curves[3]) {
    for (int i = 0; i < 3; i++) {
        cleanup_tone_curve(curves[i]);
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Prepare basic color data structures
    cmsCIExyY WhitePoint = {0.3127, 0.3290, 1.0};
    cmsCIExyYTRIPLE Primaries = {{{0.64, 0.33, 1.0}, {0.30, 0.60, 1.0}, {0.15, 0.06, 1.0}}};

    cmsToneCurve *TransferFunction[3] = {NULL, NULL, NULL};
    cmsToneCurve *DupCurve = NULL;
    cmsHPROFILE hProfile = NULL;
    cmsFloat64Number GammaVal = 1.0;
    cmsFloat64Number Threshold = 0.01;
    cmsFloat64Number Precision = 0.01;

    // Parse input to derive parameters for fuzzing
    if (Size > 0) {
        GammaVal = (double)(Data[0] / 255.0) * 3.0 + 0.1; // Range [0.1, 3.1]
        if (Size > 1) {
            Threshold = (double)(Data[1] / 255.0) * 0.1; // Range [0.0, 0.1]
        }
        if (Size > 2) {
            Precision = (double)(Data[2] / 255.0) * 0.1; // Range [0.0, 0.1]
        }
    }

    // Test cmsBuildGamma
    cmsToneCurve *Curve1 = cmsBuildGamma(ContextID, GammaVal);
    if (Curve1 != NULL) {
        // Test cmsDupToneCurve
        DupCurve = cmsDupToneCurve(Curve1);
        
        // Test cmsEstimateGamma on original and duplicate
        cmsEstimateGamma(Curve1, Precision);
        if (DupCurve != NULL) {
            cmsEstimateGamma(DupCurve, Precision);
        }

        // Prepare transfer functions for profile creation
        TransferFunction[0] = Curve1;
        TransferFunction[1] = DupCurve ? DupCurve : Curve1;
        TransferFunction[2] = DupCurve ? DupCurve : Curve1;

        // Test cmsCreateRGBProfileTHR
        hProfile = cmsCreateRGBProfileTHR(ContextID, &WhitePoint, &Primaries, TransferFunction);
        if (hProfile != NULL) {
            // Test cmsDetectRGBProfileGamma
            cmsDetectRGBProfileGamma(hProfile, Threshold);
        }

        // Test cmsCreateRGBProfile (non-threaded)
        cmsHPROFILE hProfile2 = cmsCreateRGBProfile(&WhitePoint, &Primaries, TransferFunction);
        if (hProfile2 != NULL) {
            cmsDetectRGBProfileGamma(hProfile2, Threshold);
            cleanup_profile(hProfile2);
        }
    }

    // Cleanup
    cleanup_tone_curves_array(TransferFunction);
    cleanup_tone_curve(DupCurve);
    cleanup_profile(hProfile);
    cmsDeleteContext(ContextID);

    return 0;
}