// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curve(cmsToneCurve *curve) {
    if (curve != NULL) {
        cmsFreeToneCurve(curve);
    }
}

static cmsContext create_context(const uint8_t *Data, size_t Size) {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return NULL;
    }
    return ctx;
}

static cmsToneCurve* create_curve_from_fuzz(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 4) return NULL;
    
    // Interpret first 4 bytes as number of entries
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)(Data);
    
    // Sanity check to prevent huge allocations
    if (nEntries == 0 || nEntries > 100000) {
        return NULL;
    }
    
    // Ensure we have enough data for the array
    size_t required_size = sizeof(cmsFloat32Number) * nEntries;
    if (Size < 4 + required_size) {
        return NULL;
    }
    
    const cmsFloat32Number *values = (const cmsFloat32Number*)(Data + 4);
    
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = create_context(Data, Size);
    if (ctx == NULL) return 0;

    // Create a base curve from fuzz data
    cmsToneCurve *curve1 = create_curve_from_fuzz(ctx, Data, Size);
    
    // Fuzz cmsDupToneCurve
    cmsToneCurve *dupCurve = NULL;
    if (curve1 != NULL) {
        dupCurve = cmsDupToneCurve(curve1);
    }

    // Fuzz cmsEvalToneCurveFloat
    cmsFloat32Number evalResult = 0.0f;
    if (curve1 != NULL && Size >= 8) {
        // Use next 4 bytes as float value (offset 4 to 8)
        cmsFloat32Number v = *(const cmsFloat32Number*)(Data + 4);
        evalResult = cmsEvalToneCurveFloat(curve1, v);
    }

    // Fuzz cmsReverseToneCurve
    cmsToneCurve *revCurve = NULL;
    if (curve1 != NULL) {
        revCurve = cmsReverseToneCurve(curve1);
    }

    // Fuzz cmsReverseToneCurveEx
    cmsToneCurve *revCurveEx = NULL;
    if (curve1 != NULL && Size >= 8) {
        cmsUInt32Number nSamples = *(const cmsUInt32Number*)(Data + 4);
        if (nSamples > 0 && nSamples < 100000) {
            revCurveEx = cmsReverseToneCurveEx(nSamples, curve1);
        }
    }

    // Fuzz cmsJoinToneCurve
    cmsToneCurve *joinCurve = NULL;
    if (curve1 != NULL && dupCurve != NULL && Size >= 12) {
        cmsUInt32Number nPoints = *(const cmsUInt32Number*)(Data + 8);
        if (nPoints > 0 && nPoints < 100000) {
            joinCurve = cmsJoinToneCurve(ctx, curve1, dupCurve, nPoints);
        }
    }

    // Cleanup
    cleanup_curve(curve1);
    cleanup_curve(dupCurve);
    cleanup_curve(revCurve);
    cleanup_curve(revCurveEx);
    cleanup_curve(joinCurve);
    cmsDeleteContext(ctx);

    return 0;
}