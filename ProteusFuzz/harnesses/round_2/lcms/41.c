// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsGetToneCurveParametricType at cmsgamma.c:1409:27 in lcms2.h
// cmsSmoothToneCurve at cmsgamma.c:1213:20 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsEstimateGamma at cmsgamma.c:1465:28 in lcms2.h
// cmsCloseProfile at cmsio0.c:1582:20 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsOpenProfileFromFile at cmsio0.c:1231:23 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsDetectRGBProfileGamma at cmsgmt.c:605:28 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "lcms2.h"

static cmsContext CreateContextFromFuzzData(const uint8_t *Data, size_t Size) {
    if (Size < 1) return NULL;
    return cmsCreateContext(NULL, NULL);
}

static void CleanupContext(cmsContext ctx) {
    if (ctx != NULL) {
        cmsDeleteContext(ctx);
    }
}

static void CleanupToneCurve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsFloat64Number ReadGammaFromData(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + sizeof(double) > Size) {
        return 1.0;
    }
    cmsFloat64Number gamma;
    memcpy(&gamma, Data + offset, sizeof(double));
    return gamma;
}

static cmsFloat64Number ReadThresholdFromData(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + sizeof(double) > Size) {
        return 0.0;
    }
    cmsFloat64Number threshold;
    memcpy(&threshold, Data + offset, sizeof(double));
    return threshold;
}

static cmsFloat64Number ReadLambdaFromData(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + sizeof(double) > Size) {
        return 0.0;
    }
    cmsFloat64Number lambda;
    memcpy(&lambda, Data + offset, sizeof(double));
    return lambda;
}

static cmsFloat64Number ReadPrecisionFromData(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + sizeof(double) > Size) {
        return 0.0;
    }
    cmsFloat64Number precision;
    memcpy(&precision, Data + offset, sizeof(double));
    return precision;
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = CreateContextFromFuzzData(Data, Size);
    
    cmsHPROFILE hProfile = NULL;
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        hProfile = cmsOpenProfileFromFile("./dummy_file", "r");
    }
    
    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;

    // 1. cmsBuildGamma
    cmsFloat64Number gamma = ReadGammaFromData(Data, Size, 0);
    curve1 = cmsBuildGamma(ctx, gamma);
    
    if (Size > sizeof(double)) {
        cmsFloat64Number gamma2 = ReadGammaFromData(Data, Size, sizeof(double));
        cmsToneCurve *temp = cmsBuildGamma(ctx, gamma2);
        if (temp) CleanupToneCurve(temp);
    }

    // 2. cmsDupToneCurve
    if (curve1 != NULL) {
        curve2 = cmsDupToneCurve(curve1);
    }

    // 3. cmsDetectRGBProfileGamma
    // Only call if hProfile is valid to prevent SEGV in library internals
    cmsFloat64Number threshold = ReadThresholdFromData(Data, Size, 2 * sizeof(double));
    cmsFloat64Number detectedGamma = -1.0;
    if (hProfile != NULL) {
        detectedGamma = cmsDetectRGBProfileGamma(hProfile, threshold);
    }

    // 4. cmsGetToneCurveParametricType
    cmsInt32Number type1 = 0;
    cmsInt32Number type2 = 0;
    if (curve1 != NULL) {
        type1 = cmsGetToneCurveParametricType(curve1);
    }
    if (curve2 != NULL) {
        type2 = cmsGetToneCurveParametricType(curve2);
    }

    // 5. cmsSmoothToneCurve
    cmsFloat64Number lambda = ReadLambdaFromData(Data, Size, 3 * sizeof(double));
    cmsBool smoothResult = 0;
    if (curve1 != NULL) {
        smoothResult = cmsSmoothToneCurve(curve1, lambda);
    }

    // 6. cmsEstimateGamma
    cmsFloat64Number precision = ReadPrecisionFromData(Data, Size, 4 * sizeof(double));
    cmsFloat64Number estGamma1 = -1.0;
    cmsFloat64Number estGamma2 = -1.0;
    if (curve1 != NULL) {
        estGamma1 = cmsEstimateGamma(curve1, precision);
    }
    if (curve2 != NULL) {
        estGamma2 = cmsEstimateGamma(curve2, precision);
    }

    // Cleanup
    CleanupToneCurve(curve1);
    CleanupToneCurve(curve2);
    
    if (hProfile != NULL) {
        cmsCloseProfile(hProfile);
    }
    
    CleanupContext(ctx);

    remove("./dummy_file");

    return 0;
}