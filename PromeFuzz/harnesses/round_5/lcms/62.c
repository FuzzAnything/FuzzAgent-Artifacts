// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve16(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    cmsUInt32Number nEntries = (cmsUInt32Number)(size / 2);
    if (nEntries > 65535) nEntries = 65535;
    
    cmsUInt16Number* values = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!values) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries && i * 2 < size; i++) {
        values[i] = (data[i * 2] << 8) | data[i * 2 + 1];
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
    free(values);
    return curve;
}

static cmsToneCurve* create_test_curve_float(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = (cmsUInt32Number)(size / 4);
    if (nEntries > 65535) nEntries = 65535;
    
    cmsFloat32Number* values = (cmsFloat32Number*)malloc(nEntries * sizeof(cmsFloat32Number));
    if (!values) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries && i * 4 < size; i++) {
        uint32_t temp = 0;
        for (int j = 0; j < 4 && (i * 4 + j) < size; j++) {
            temp = (temp << 8) | data[i * 4 + j];
        }
        values[i] = *(cmsFloat32Number*)&temp;
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
    free(values);
    return curve;
}

static cmsToneCurve* create_test_segmented_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsCurveSegment)) return NULL;
    
    cmsUInt32Number nSegments = 1;
    if (size >= sizeof(cmsCurveSegment) * 2) {
        nSegments = 2;
    }
    
    cmsCurveSegment* segments = (cmsCurveSegment*)malloc(nSegments * sizeof(cmsCurveSegment));
    if (!segments) return NULL;
    
    memset(segments, 0, nSegments * sizeof(cmsCurveSegment));
    
    for (cmsUInt32Number i = 0; i < nSegments; i++) {
        size_t offset = i * sizeof(cmsCurveSegment);
        if (offset + sizeof(cmsCurveSegment) <= size) {
            memcpy(&segments[i], data + offset, sizeof(cmsCurveSegment));
        }
    }
    
    cmsToneCurve* curve = cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
    free(segments);
    return curve;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = create_test_curve16(ctx, Data, Size);
    cmsToneCurve* curve2 = create_test_curve_float(ctx, Data, Size);
    
    if (curve1 && curve2) {
        cmsUInt32Number nPoints = 256;
        if (Size >= 4) {
            nPoints = (*(uint32_t*)Data) % 65536;
            if (nPoints < 2) nPoints = 2;
        }
        
        cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
        if (joined) {
            cmsUInt32Number nResultSamples = 256;
            if (Size >= 8) {
                nResultSamples = (*(uint32_t*)(Data + 4)) % 65536;
                if (nResultSamples < 2) nResultSamples = 2;
            }
            
            cmsToneCurve* reversed = cmsReverseToneCurveEx(nResultSamples, joined);
            if (reversed) {
                cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(reversed);
                (void)estimated;
                cmsFreeToneCurve(reversed);
            }
            cmsFreeToneCurve(joined);
        }
    }
    
    if (curve1) {
        cmsUInt32Number nResultSamples = 256;
        if (Size >= 4) {
            nResultSamples = (*(uint32_t*)Data) % 65536;
            if (nResultSamples < 2) nResultSamples = 2;
        }
        
        cmsToneCurve* reversed = cmsReverseToneCurveEx(nResultSamples, curve1);
        if (reversed) {
            cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(reversed);
            (void)estimated;
            cmsFreeToneCurve(reversed);
        }
        
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve1);
        (void)estimated;
        cmsFreeToneCurve(curve1);
    }
    
    if (curve2) {
        cmsUInt32Number nResultSamples = 256;
        if (Size >= 4) {
            nResultSamples = (*(uint32_t*)Data) % 65536;
            if (nResultSamples < 2) nResultSamples = 2;
        }
        
        cmsToneCurve* reversed = cmsReverseToneCurveEx(nResultSamples, curve2);
        if (reversed) {
            cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(reversed);
            (void)estimated;
            cmsFreeToneCurve(reversed);
        }
        
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve2);
        (void)estimated;
        cmsFreeToneCurve(curve2);
    }
    
    cmsToneCurve* segmented = create_test_segmented_curve(ctx, Data, Size);
    if (segmented) {
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(segmented);
        (void)estimated;
        cmsFreeToneCurve(segmented);
    }
    
    cmsDeleteContext(ctx);
    return 0;
}