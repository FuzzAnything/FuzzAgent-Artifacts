// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsOpenProfileFromMem at cmsio0.c:1295:23 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_tone_curve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static void cleanup_profile(cmsHPROFILE profile) {
    if (profile != NULL) {
        cmsCloseProfile(profile);
    }
}

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext Context = cmsCreateContext(NULL, NULL);
    if (Context == NULL) return 0;

    cmsToneCurve *curve = NULL;
    cmsToneCurve *dup_curve = NULL;
    cmsHPROFILE profile = NULL;
    cmsFloat64Number gamma_val, lambda_val, precision_val, threshold_val;
    cmsInt32Number param_type;
    cmsBool smooth_result;

    // Extract values from fuzz input
    if (Size >= sizeof(cmsFloat64Number)) {
        memcpy(&gamma_val, Data, sizeof(cmsFloat64Number));
        if (Size >= 2 * sizeof(cmsFloat64Number)) {
            memcpy(&lambda_val, Data + sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
            if (Size >= 3 * sizeof(cmsFloat64Number)) {
                memcpy(&precision_val, Data + 2 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
                if (Size >= 4 * sizeof(cmsFloat64Number)) {
                    memcpy(&threshold_val, Data + 3 * sizeof(cmsFloat64Number), sizeof(cmsFloat64Number));
                }
            }
        }
    } else {
        gamma_val = 1.0;
        lambda_val = 0.5;
        precision_val = 0.01;
        threshold_val = 0.1;
    }

    // Test cmsBuildGamma
    curve = cmsBuildGamma(Context, gamma_val);
    if (curve != NULL) {
        // Test cmsEstimateGamma
        cmsEstimateGamma(curve, precision_val);

        // Test cmsGetToneCurveParametricType
        param_type = cmsGetToneCurveParametricType(curve);
        (void)param_type;

        // Test cmsSmoothToneCurve
        smooth_result = cmsSmoothToneCurve(curve, lambda_val);
        (void)smooth_result;

        // Test cmsDupToneCurve
        dup_curve = cmsDupToneCurve(curve);
        if (dup_curve != NULL) {
            cmsEstimateGamma(dup_curve, precision_val);
            param_type = cmsGetToneCurveParametricType(dup_curve);
            (void)param_type;
            smooth_result = cmsSmoothToneCurve(dup_curve, lambda_val);
            (void)smooth_result;
            cleanup_tone_curve(dup_curve);
        }

        cleanup_tone_curve(curve);
    }

    // Test cmsDetectRGBProfileGamma with a dummy profile
    // Create a simple RGB profile for testing
    cmsUInt8Number rgb_data[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    cmsUInt32Number rgb_size = sizeof(rgb_data);
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(rgb_data, rgb_size);
    if (hProfile != NULL) {
        cmsFloat64Number detected_gamma = cmsDetectRGBProfileGamma(hProfile, threshold_val);
        (void)detected_gamma;
        cleanup_profile(hProfile);
    }

    // Cleanup context
    cmsDeleteContext(Context);

    return 0;
}