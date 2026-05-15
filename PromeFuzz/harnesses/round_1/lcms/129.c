// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve_float(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 1024) nEntries = 16;
    
    size_t needed = nEntries * sizeof(cmsFloat32Number);
    if (size < sizeof(cmsUInt32Number) + needed) return NULL;
    
    const cmsFloat32Number* values = (const cmsFloat32Number*)(data + sizeof(cmsUInt32Number));
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

static cmsToneCurve* create_test_curve_16(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 1024) nEntries = 16;
    
    size_t needed = nEntries * sizeof(cmsUInt16Number);
    if (size < sizeof(cmsUInt32Number) + needed) return NULL;
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + sizeof(cmsUInt32Number));
    return cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
}

static cmsToneCurve* create_test_segmented_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return NULL;
    
    cmsUInt32Number nSegments = *(const cmsUInt32Number*)data;
    if (nSegments == 0 || nSegments > 4) nSegments = 1;
    
    size_t needed = nSegments * sizeof(cmsCurveSegment);
    if (size < sizeof(cmsUInt32Number) + needed) return NULL;
    
    const cmsCurveSegment* segments = (const cmsCurveSegment*)(data + sizeof(cmsUInt32Number));
    return cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (!ctx) return 0;
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* result = NULL;
    
    uint32_t selector = *(const uint32_t*)Data;
    const uint8_t* remaining_data = Data + 4;
    size_t remaining_size = Size - 4;
    
    switch (selector % 6) {
        case 0:
            curve1 = create_test_curve_float(ctx, remaining_data, remaining_size);
            if (curve1) {
                cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve1);
                (void)entries;
            }
            break;
            
        case 1:
            curve1 = create_test_curve_float(ctx, remaining_data, remaining_size);
            break;
            
        case 2:
            curve1 = create_test_curve_16(ctx, remaining_data, remaining_size);
            break;
            
        case 3:
            curve1 = create_test_curve_float(ctx, remaining_data, remaining_size);
            if (curve1) {
                cmsUInt32Number nSamples = 256;
                if (remaining_size >= 8) {
                    nSamples = *(const cmsUInt32Number*)(remaining_data + 4);
                    if (nSamples == 0 || nSamples > 65536) nSamples = 256;
                }
                result = cmsReverseToneCurveEx(nSamples, curve1);
            }
            break;
            
        case 4:
            if (remaining_size >= 8) {
                size_t half = remaining_size / 2;
                curve1 = create_test_curve_float(ctx, remaining_data, half);
                curve2 = create_test_curve_float(ctx, remaining_data + half, remaining_size - half);
                
                if (curve1 && curve2) {
                    cmsUInt32Number nPoints = 256;
                    if (remaining_size >= 12) {
                        nPoints = *(const cmsUInt32Number*)(remaining_data + 8);
                        if (nPoints == 0 || nPoints > 65536) nPoints = 256;
                    }
                    result = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
                }
            }
            break;
            
        case 5:
            curve1 = create_test_segmented_curve(ctx, remaining_data, remaining_size);
            break;
    }
    
    if (result) cmsFreeToneCurve(result);
    if (curve1) cmsFreeToneCurve(curve1);
    if (curve2) cmsFreeToneCurve(curve2);
    
    cmsDeleteContext(ctx);
    return 0;
}