// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsCreateLinearizationDeviceLinkTHR at cmsvirt.c:288:23 in lcms2.h
// cmsCreateLinearizationDeviceLink at cmsvirt.c:340:23 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve **curves, int count) {
    for (int i = 0; i < count; i++) {
        if (curves[i] != NULL) {
            cmsFreeToneCurve(curves[i]);
            curves[i] = NULL;
        }
    }
}

static void cleanup_profiles(cmsHPROFILE **profiles, int count) {
    for (int i = 0; i < count; i++) {
        if (profiles[i] != NULL) {
            cmsCloseProfile(profiles[i]);
            profiles[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    // cmsCreateContext requires two arguments: Plugin and UserData
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    // Extract parameters from input data
    uint32_t color_space = 0;
    uint32_t n_points = 0;
    uint32_t n_result_samples = 0;

    if (Size >= 4) color_space = *(const uint32_t *)Data;
    if (Size >= 8) n_points = *(const uint32_t *)(Data + 4);
    if (Size >= 12) n_result_samples = *(const uint32_t *)(Data + 8);

    // Limit values to reasonable ranges to prevent crashes
    if (n_points > 10000) n_points = 10000;
    if (n_result_samples > 10000) n_result_samples = 10000;

    // Create initial tone curves using cmsBuildToneCurve with correct signature
    // cmsBuildToneCurve(cmsContext ContextID, int nTones, const cmsFloat64Number* Tones)
    // We need to construct a valid tone curve. Let's use a simple gamma curve or identity.
    // Since we don't have a full array of doubles, we'll create a simple identity curve manually
    // or use cmsBuildGamma which is simpler.
    
    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;

    // Use cmsBuildGamma to create simple curves based on input data if available
    cmsFloat64Number gamma_val = 1.0;
    if (Size > 0) {
        gamma_val = (cmsFloat64Number)Data[0] / 255.0;
        if (gamma_val <= 0.0) gamma_val = 1.0;
    }

    curve1 = cmsBuildGamma(ContextID, gamma_val);
    curve2 = cmsBuildGamma(ContextID, gamma_val + 0.1);
    curve3 = cmsBuildGamma(ContextID, gamma_val + 0.2);

    if (curve1 == NULL || curve2 == NULL || curve3 == NULL) {
        // Cleanup partial creation
        if (curve1) cmsFreeToneCurve(curve1);
        if (curve2) cmsFreeToneCurve(curve2);
        if (curve3) cmsFreeToneCurve(curve3);
        cmsDeleteContext(ContextID);
        return 0;
    }

    cmsToneCurve *curves[3] = {curve1, curve2, curve3};
    cmsHPROFILE *profiles[2] = {NULL, NULL};

    // Test cmsDupToneCurve
    cmsToneCurve *dup_curve = NULL;
    if (curve1 != NULL) {
        dup_curve = cmsDupToneCurve(curve1);
    }

    // Test cmsReverseToneCurve
    cmsToneCurve *rev_curve = NULL;
    if (curve1 != NULL) {
        rev_curve = cmsReverseToneCurve(curve1);
    }

    // Test cmsReverseToneCurveEx
    cmsToneCurve *rev_curve_ex = NULL;
    if (curve1 != NULL) {
        rev_curve_ex = cmsReverseToneCurveEx(n_result_samples, curve1);
    }

    // Test cmsJoinToneCurve
    cmsToneCurve *joined_curve = NULL;
    if (curve1 != NULL && curve2 != NULL) {
        joined_curve = cmsJoinToneCurve(ContextID, curve1, curve2, n_points);
    }

    // Test cmsCreateLinearizationDeviceLinkTHR
    cmsHPROFILE profile_thr = NULL;
    if (curve1 != NULL) {
        cmsToneCurve *transfer_functions[] = {curve1, curve2, curve3};
        profile_thr = cmsCreateLinearizationDeviceLinkTHR(ContextID, (cmsColorSpaceSignature)color_space, transfer_functions);
    }

    // Test cmsCreateLinearizationDeviceLink
    cmsHPROFILE profile = NULL;
    if (curve1 != NULL) {
        cmsToneCurve *transfer_functions[] = {curve1, curve2, curve3};
        profile = cmsCreateLinearizationDeviceLink((cmsColorSpaceSignature)color_space, transfer_functions);
    }

    profiles[0] = profile;
    profiles[1] = profile_thr;

    // Cleanup
    cleanup_curves(curves, 3);
    if (dup_curve != NULL) cmsFreeToneCurve(dup_curve);
    if (rev_curve != NULL) cmsFreeToneCurve(rev_curve);
    if (rev_curve_ex != NULL) cmsFreeToneCurve(rev_curve_ex);
    if (joined_curve != NULL) cmsFreeToneCurve(joined_curve);
    cleanup_profiles(profiles, 2);
    cmsDeleteContext(ContextID);

    return 0;
}