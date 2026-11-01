// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsStageAllocToneCurves at cmslut.c:248:21 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsStageFree at cmslut.c:1209:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <lcms2.h>

static void cleanup_curves(cmsToneCurve *c1, cmsToneCurve *c2, cmsToneCurve *c3, cmsToneCurve *c4, cmsToneCurve *c5) {
    if (c1) cmsFreeToneCurve(c1);
    if (c2) cmsFreeToneCurve(c2);
    if (c3) cmsFreeToneCurve(c3);
    if (c4) cmsFreeToneCurve(c4);
    if (c5) cmsFreeToneCurve(c5);
}

static void cleanup_stage(cmsStage *s) {
    if (s) cmsStageFree(s);
}

static cmsContext createContext(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return NULL;
    return ctx;
}

// Helper to create a tone curve using cmsBuildParametricToneCurve.
static cmsToneCurve* createTestCurve(cmsContext ctx, const uint8_t *Data, size_t Size) {
    if (Size < 2) return NULL;
    
    cmsFloat64Number params[5] = {0};

    // Try Gamma Curve (Type 1)
    params[0] = 1.0; 
    params[1] = (double)(Data[0] + 1) / 10.0; 
    params[2] = 0.0;
    params[3] = 0.0;
    params[4] = 0.0;
    cmsToneCurve *curve = cmsBuildParametricToneCurve(ctx, 1, params);
    if (curve) return curve;

    // Try Power Curve (Type 2)
    params[0] = (double)(Data[1] + 1) / 10.0; 
    params[1] = 0.0;
    params[2] = 0.0;
    params[3] = 0.0;
    params[4] = 0.0;
    curve = cmsBuildParametricToneCurve(ctx, 2, params);
    if (curve) return curve;

    // Try Linear Curve (Type 3)
    params[0] = (double)(Data[0] + 1) / 10.0; 
    params[1] = (double)(Data[1]) / 100.0; 
    params[2] = 0.0;
    params[3] = 0.0;
    params[4] = 0.0;
    curve = cmsBuildParametricToneCurve(ctx, 3, params);
    if (curve) return curve;

    return NULL;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    cmsContext ctx = createContext(Data, Size);
    if (!ctx) {
        ctx = NULL; 
    }

    cmsToneCurve *curve1 = NULL;
    cmsToneCurve *curve2 = NULL;
    cmsToneCurve *curve3 = NULL;
    cmsToneCurve *curve4 = NULL;
    cmsToneCurve *curve5 = NULL;
    cmsStage *stage = NULL;

    // 1. Create a base curve
    curve1 = createTestCurve(ctx, Data, Size);

    // If curve1 failed, try with different data offset
    if (!curve1 && Size >= 2) {
        curve1 = createTestCurve(ctx, Data + 1, Size - 1);
    }

    // 2. cmsDupToneCurve
    if (curve1) {
        curve2 = cmsDupToneCurve(curve1);
    }

    // 3. cmsReverseToneCurve
    if (curve1) {
        curve3 = cmsReverseToneCurve(curve1);
    } else if (curve2) {
        curve3 = cmsReverseToneCurve(curve2);
    }

    // 4. cmsReverseToneCurveEx
    cmsUInt32Number nSamples = 0;
    if (Size > 0) nSamples = Data[0] % 4096;
    if (nSamples == 0) nSamples = 256; 
    
    if (curve1) {
        curve4 = cmsReverseToneCurveEx(nSamples, curve1);
    } else if (curve2) {
        curve4 = cmsReverseToneCurveEx(nSamples, curve2);
    } else if (curve3) {
        curve4 = cmsReverseToneCurveEx(nSamples, curve3);
    }

    // 5. cmsJoinToneCurve
    cmsToneCurve *X = curve1 ? curve1 : curve2;
    cmsToneCurve *Y = curve3 ? curve3 : curve4;
    
    if (X && Y) {
        cmsUInt32Number nPoints = 0;
        if (Size > 1) nPoints = Data[1] % 4096;
        if (nPoints == 0) nPoints = 256;
        curve5 = cmsJoinToneCurve(ctx, X, Y, nPoints);
    }

    // 6. cmsStageAllocToneCurves
    // Fixed: Ensure we do not write beyond the array bounds.
    // The array size is 4, so we can only store up to 4 curves.
    // We prioritize the first 4 valid curves found.
    if (curve1 || curve2 || curve3 || curve4 || curve5) {
        cmsToneCurve *curves[4] = {NULL, NULL, NULL, NULL};
        int count = 0;
        
        if (curve1) { curves[count++] = curve1; }
        if (curve2 && count < 4) { curves[count++] = curve2; }
        if (curve3 && count < 4) { curves[count++] = curve3; }
        if (curve4 && count < 4) { curves[count++] = curve4; }
        // curve5 is ignored if we already have 4 curves to prevent overflow
        
        if (count > 0) {
            stage = cmsStageAllocToneCurves(ctx, count, curves);
        }
    }

    // Cleanup
    cleanup_curves(curve1, curve2, curve3, curve4, curve5);
    cleanup_stage(stage);
    if (ctx) cmsDeleteContext(ctx);

    return 0;
}