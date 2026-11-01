// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsToneCurve* create_curve_from_fuzz_data(const uint8_t *Data, size_t Size, cmsContext ContextID) {
    if (Size < 8) return NULL;

    // Try to build a gamma curve
    cmsFloat64Number gamma = *(const cmsFloat64Number *)(Data);
    if (gamma <= 0.0) gamma = 1.0; // Ensure positive gamma
    cmsToneCurve *curve = cmsBuildGamma(ContextID, gamma);
    if (curve != NULL) return curve;

    // Try to build a parametric curve
    if (Size >= 24) {
        cmsInt32Number type = *(const cmsInt32Number *)(Data + 8);
        const cmsFloat64Number *params = (const cmsFloat64Number *)(Data + 12);
        // Limit type to reasonable range to avoid invalid types
        if (type >= 0 && type <= 10) {
            curve = cmsBuildParametricToneCurve(ContextID, type, params);
            if (curve != NULL) return curve;
        }
    }

    // Fallback: create a simple gamma curve with default value
    return cmsBuildGamma(ContextID, 2.2);
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    cmsContext ContextID = cmsCreateContext(NULL, NULL);
    if (ContextID == NULL) return 0;

    cmsToneCurve *curve1 = create_curve_from_fuzz_data(Data, Size, ContextID);
    cmsToneCurve *curve2 = create_curve_from_fuzz_data(Data, Size, ContextID);
    
    if (curve1 == NULL && curve2 == NULL) {
        cmsDeleteContext(ContextID);
        return 0;
    }

    // Test cmsEstimateGamma
    if (curve1 != NULL) {
        cmsFloat64Number precision = (Size > 0) ? (cmsFloat64Number)(Data[0] / 255.0) : 0.01;
        cmsFloat64Number gamma = cmsEstimateGamma(curve1, precision);
        (void)gamma; // Suppress unused variable warning
    }

    // Test cmsReverseToneCurve
    cmsToneCurve *reversed = NULL;
    if (curve1 != NULL) {
        reversed = cmsReverseToneCurve(curve1);
    }

    // Test cmsDupToneCurve
    cmsToneCurve *duplicated = NULL;
    if (curve2 != NULL) {
        duplicated = cmsDupToneCurve(curve2);
    }

    // Test cmsJoinToneCurve
    cmsToneCurve *joined = NULL;
    if (curve1 != NULL && curve2 != NULL) {
        cmsUInt32Number nPoints = (Size > 0) ? (cmsUInt32Number)(Data[0] % 100) + 10 : 100;
        joined = cmsJoinToneCurve(ContextID, curve1, curve2, nPoints);
    }

    // Cleanup
    cleanup_curve(curve1);
    cleanup_curve(curve2);
    cleanup_curve(reversed);
    cleanup_curve(duplicated);
    cleanup_curve(joined);
    cmsDeleteContext(ContextID);

    return 0;
}