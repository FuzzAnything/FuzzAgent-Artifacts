// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsToneCurve* create_test_curve_float(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 1024) {
        nEntries = 256;
    }
    
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
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 1024) {
        nEntries = 256;
    }
    
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
    
    cmsUInt32Number nSegments = *(const cmsUInt32Number*)data;
    if (nSegments == 0 || nSegments > 16) {
        nSegments = 1;
    }
    
    size_t segments_needed = nSegments * sizeof(cmsCurveSegment);
    if (segments_needed > size - sizeof(cmsUInt32Number)) {
        return NULL;
    }
    
    const cmsCurveSegment* segments = (const cmsCurveSegment*)(data + sizeof(cmsUInt32Number));
    return cmsBuildSegmentedToneCurve(ctx, nSegments, segments);
}

int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }
    
    size_t offset = 0;
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* curve3 = NULL;
    
    if (Size - offset >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number curve_type = *(const cmsUInt32Number*)(Data + offset);
        offset += sizeof(cmsUInt32Number);
        
        size_t remaining = Size - offset;
        if (remaining > 0) {
            switch (curve_type % 3) {
                case 0:
                    curve1 = create_test_curve_float(ctx, Data + offset, remaining);
                    break;
                case 1:
                    curve1 = create_test_curve_16(ctx, Data + offset, remaining);
                    break;
                case 2:
                    curve1 = create_test_segmented_curve(ctx, Data + offset, remaining);
                    break;
            }
        }
    }
    
    if (curve1) {
        cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve1);
        (void)estimated;
        
        if (Size - offset >= sizeof(cmsUInt32Number)) {
            cmsUInt32Number nResultSamples = *(const cmsUInt32Number*)(Data + offset);
            offset += sizeof(cmsUInt32Number);
            
            if (nResultSamples > 0 && nResultSamples < 65536) {
                cmsToneCurve* reversed = cmsReverseToneCurveEx(nResultSamples, curve1);
                if (reversed) {
                    cmsFreeToneCurve(reversed);
                }
            }
        }
        
        if (Size - offset >= sizeof(cmsUInt32Number)) {
            cmsUInt32Number curve_type2 = *(const cmsUInt32Number*)(Data + offset);
            offset += sizeof(cmsUInt32Number);
            
            size_t remaining = Size - offset;
            if (remaining > 0) {
                switch (curve_type2 % 3) {
                    case 0:
                        curve2 = create_test_curve_float(ctx, Data + offset, remaining);
                        break;
                    case 1:
                        curve2 = create_test_curve_16(ctx, Data + offset, remaining);
                        break;
                    case 2:
                        curve2 = create_test_segmented_curve(ctx, Data + offset, remaining);
                        break;
                }
            }
        }
        
        if (curve2) {
            if (Size - offset >= sizeof(cmsUInt32Number)) {
                cmsUInt32Number nPoints = *(const cmsUInt32Number*)(Data + offset);
                offset += sizeof(cmsUInt32Number);
                
                if (nPoints > 0 && nPoints < 65536) {
                    cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, nPoints);
                    if (joined) {
                        cmsFreeToneCurve(joined);
                    }
                }
            }
            
            cmsFreeToneCurve(curve2);
        }
        
        if (Size - offset >= sizeof(cmsUInt32Number)) {
            cmsUInt32Number curve_type3 = *(const cmsUInt32Number*)(Data + offset);
            offset += sizeof(cmsUInt32Number);
            
            size_t remaining = Size - offset;
            if (remaining > 0) {
                switch (curve_type3 % 3) {
                    case 0:
                        curve3 = create_test_curve_float(ctx, Data + offset, remaining);
                        break;
                    case 1:
                        curve3 = create_test_curve_16(ctx, Data + offset, remaining);
                        break;
                    case 2:
                        curve3 = create_test_segmented_curve(ctx, Data + offset, remaining);
                        break;
                }
            }
        }
        
        if (curve3) {
            if (Size - offset >= sizeof(cmsUInt32Number)) {
                cmsUInt32Number nPoints = *(const cmsUInt32Number*)(Data + offset);
                offset += sizeof(cmsUInt32Number);
                
                if (nPoints > 0 && nPoints < 65536) {
                    cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve3, nPoints);
                    if (joined) {
                        cmsFreeToneCurve(joined);
                    }
                }
            }
            
            cmsFreeToneCurve(curve3);
        }
        
        cmsFreeToneCurve(curve1);
    }
    
    cleanup_context(ctx);
    return 0;
}