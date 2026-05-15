// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_dummy_tone_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data % 1024;
    if (nEntries == 0) nEntries = 1;
    
    size_t values_needed = nEntries * sizeof(cmsUInt16Number);
    if (size < 4 + values_needed) return NULL;
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + 4);
    return cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
}

static cmsToneCurve* create_dummy_tone_curve_float(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data % 1024;
    if (nEntries == 0) nEntries = 1;
    
    size_t values_needed = nEntries * sizeof(cmsFloat32Number);
    if (size < 4 + values_needed) return NULL;
    
    const cmsFloat32Number* values = (const cmsFloat32Number*)(data + 4);
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

static void test_reverse_tone_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsToneCurve* curve = create_dummy_tone_curve(ctx, data, size);
    if (!curve) return;
    
    cmsUInt32Number nSamples = 256;
    if (size >= 8) {
        nSamples = *(const cmsUInt32Number*)(data + 4) % 65536;
        if (nSamples == 0) nSamples = 256;
    }
    
    cmsToneCurve* reversed = cmsReverseToneCurveEx(nSamples, curve);
    if (reversed) {
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(reversed);
        (void)estimated;
        cmsFreeToneCurve(reversed);
    }
    
    cmsFreeToneCurve(curve);
}

static void test_build_segmented(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 4) return;
    
    cmsUInt32Number nSegments = *(const cmsUInt32Number*)data % 8;
    if (nSegments == 0) nSegments = 1;
    
    size_t needed = 4 + nSegments * sizeof(cmsCurveSegment);
    if (size < needed) return;
    
    const cmsCurveSegment* segments = (const cmsCurveSegment*)(data + 4);
    cmsToneCurve* curve = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
    if (curve) {
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve);
        (void)estimated;
        cmsFreeToneCurve(curve);
    }
}

static void test_build_tabulated(cmsContext ctx, const uint8_t* data, size_t size) {
    cmsToneCurve* curve16 = create_dummy_tone_curve(ctx, data, size);
    if (curve16) {
        cmsUInt32Number estimated16 = cmsGetToneCurveEstimatedTableEntries(curve16);
        (void)estimated16;
        cmsFreeToneCurve(curve16);
    }
    
    cmsToneCurve* curveFloat = create_dummy_tone_curve_float(ctx, data, size);
    if (curveFloat) {
        cmsUInt32Number estimatedFloat = cmsGetToneCurveEstimatedTableEntries(curveFloat);
        (void)estimatedFloat;
        cmsFreeToneCurve(curveFloat);
    }
}

static void test_join_curves(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 8) return;
    
    size_t half = size / 2;
    cmsToneCurve* curve1 = create_dummy_tone_curve(ctx, data, half);
    cmsToneCurve* curve2 = create_dummy_tone_curve(ctx, data + half, size - half);
    
    if (curve1 && curve2) {
        cmsUInt32Number nPoints = 256;
        if (size >= 12) {
            nPoints = *(const cmsUInt32Number*)(data + 8) % 65536;
            if (nPoints == 0) nPoints = 256;
        }
        
        cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
        if (joined) {
            cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(joined);
            (void)estimated;
            cmsFreeToneCurve(joined);
        }
    }
    
    if (curve1) cmsFreeToneCurve(curve1);
    if (curve2) cmsFreeToneCurve(curve2);
}

int LLVMFuzzerTestOneInput_75(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    uint8_t selector = Data[0] % 4;
    
    switch (selector) {
        case 0:
            test_reverse_tone_curve(ctx, Data, Size);
            break;
        case 1:
            test_build_segmented(ctx, Data, Size);
            break;
        case 2:
            test_build_tabulated(ctx, Data, Size);
            break;
        case 3:
            test_join_curves(ctx, Data, Size);
            break;
    }
    
    cmsDeleteContext(ctx);
    return 0;
}