// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void destroy_test_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsToneCurve* create_test_curve_float(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    cmsUInt32Number nEntries;
    memcpy(&nEntries, data, sizeof(cmsUInt32Number));
    nEntries = nEntries % 1024 + 1;
    
    size_t values_needed = nEntries * sizeof(cmsFloat32Number);
    if (values_needed > size - sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    const cmsFloat32Number* values = (const cmsFloat32Number*)(data + sizeof(cmsUInt32Number));
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

static cmsToneCurve* create_test_curve_16(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    cmsUInt32Number nEntries;
    memcpy(&nEntries, data, sizeof(cmsUInt32Number));
    nEntries = nEntries % 1024 + 1;
    
    size_t values_needed = nEntries * sizeof(cmsUInt16Number);
    if (values_needed > size - sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + sizeof(cmsUInt32Number));
    return cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
}

static cmsToneCurve* create_test_segmented_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    cmsUInt32Number nSegments;
    memcpy(&nSegments, data, sizeof(cmsUInt32Number));
    nSegments = nSegments % 10 + 1;
    
    size_t segment_size = sizeof(cmsCurveSegment);
    size_t segments_needed = nSegments * segment_size;
    
    if (segments_needed > size - sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    const cmsCurveSegment* segments = (const cmsCurveSegment*)(data + sizeof(cmsUInt32Number));
    return cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    
    size_t offset = 0;
    
    if (Size - offset >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number curve_type;
        memcpy(&curve_type, Data + offset, sizeof(cmsUInt32Number));
        offset += sizeof(cmsUInt32Number);
        curve_type = curve_type % 3;
        
        switch (curve_type) {
            case 0:
                if (Size - offset >= sizeof(cmsUInt32Number)) {
                    curve1 = create_test_curve_float(ctx, Data + offset, Size - offset);
                }
                break;
            case 1:
                if (Size - offset >= sizeof(cmsUInt32Number)) {
                    curve1 = create_test_curve_16(ctx, Data + offset, Size - offset);
                }
                break;
            case 2:
                if (Size - offset >= sizeof(cmsUInt32Number)) {
                    curve1 = create_test_segmented_curve(ctx, Data + offset, Size - offset);
                }
                break;
        }
    }
    
    if (curve1) {
        cmsUInt32Number table_entries = cmsGetToneCurveEstimatedTableEntries(curve1);
        
        cmsUInt32Number nResultSamples = 256;
        if (Size - offset >= sizeof(cmsUInt32Number)) {
            memcpy(&nResultSamples, Data + offset, sizeof(cmsUInt32Number));
            offset += sizeof(cmsUInt32Number);
            nResultSamples = nResultSamples % 1024 + 1;
        }
        
        cmsToneCurve* reversed = cmsReverseToneCurveEx(nResultSamples, curve1);
        if (reversed) {
            cmsFreeToneCurve(reversed);
        }
        
        if (Size - offset >= sizeof(cmsUInt32Number)) {
            cmsUInt32Number second_curve_type;
            memcpy(&second_curve_type, Data + offset, sizeof(cmsUInt32Number));
            offset += sizeof(cmsUInt32Number);
            second_curve_type = second_curve_type % 3;
            
            switch (second_curve_type) {
                case 0:
                    if (Size - offset >= sizeof(cmsUInt32Number)) {
                        curve2 = create_test_curve_float(ctx, Data + offset, Size - offset);
                    }
                    break;
                case 1:
                    if (Size - offset >= sizeof(cmsUInt32Number)) {
                        curve2 = create_test_curve_16(ctx, Data + offset, Size - offset);
                    }
                    break;
                case 2:
                    if (Size - offset >= sizeof(cmsUInt32Number)) {
                        curve2 = create_test_segmented_curve(ctx, Data + offset, Size - offset);
                    }
                    break;
            }
            
            if (curve2) {
                cmsUInt32Number nPoints = 256;
                if (Size - offset >= sizeof(cmsUInt32Number)) {
                    memcpy(&nPoints, Data + offset, sizeof(cmsUInt32Number));
                    nPoints = nPoints % 1024 + 1;
                }
                
                cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
                if (joined) {
                    cmsFreeToneCurve(joined);
                }
                
                cmsFreeToneCurve(curve2);
            }
        }
        
        cmsFreeToneCurve(curve1);
    }
    
    destroy_test_context(ctx);
    return 0;
}