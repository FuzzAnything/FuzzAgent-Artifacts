// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve_float(cmsContext context) {
    // Create a simple linear tone curve with 256 entries
    cmsFloat32Number values[256];
    for (int i = 0; i < 256; i++) {
        values[i] = (cmsFloat32Number)i / 255.0f;
    }
    return cmsBuildTabulatedToneCurveFloat(context, 256, values);
}

static cmsToneCurve* create_test_tone_curve_16(cmsContext context) {
    // Create a simple linear tone curve with 256 entries
    cmsUInt16Number values[256];
    for (int i = 0; i < 256; i++) {
        values[i] = (cmsUInt16Number)(i * 65535 / 255);
    }
    return cmsBuildTabulatedToneCurve16(context, 256, values);
}

static cmsToneCurve* create_test_segmented_curve(cmsContext context) {
    // Create a simple segmented curve with one linear segment
    cmsCurveSegment segment;
    segment.x0 = 0.0;
    segment.x1 = 1.0;
    segment.Type = 1;  // Linear segment type
    segment.Params[0] = 1.0;  // Gamma = 1.0 (linear)
    
    return cmsBuildSegmentedToneCurve(context, 1, &segment);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Test 1: cmsGetToneCurveEstimatedTableEntries
    cmsToneCurve* curve1 = create_test_tone_curve_float(context);
    if (curve1) {
        cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve1);
        (void)entries;  // Use result to avoid compiler warnings
        cmsFreeToneCurve(curve1);
    }
    
    // Test 2: cmsBuildTabulatedToneCurveFloat
    if (Size >= sizeof(cmsFloat32Number)) {
        size_t max_entries = Size / sizeof(cmsFloat32Number);
        if (max_entries > 0) {
            size_t nEntries = max_entries > 4096 ? 4096 : max_entries;
            cmsFloat32Number* float_values = (cmsFloat32Number*)Data;
            
            cmsToneCurve* curve2 = cmsBuildTabulatedToneCurveFloat(context, 
                (cmsUInt32Number)nEntries, float_values);
            if (curve2) {
                cmsFreeToneCurve(curve2);
            }
        }
    }
    
    // Test 3: cmsBuildTabulatedToneCurve16
    if (Size >= sizeof(cmsUInt16Number)) {
        size_t max_entries = Size / sizeof(cmsUInt16Number);
        if (max_entries > 0) {
            size_t nEntries = max_entries > 4096 ? 4096 : max_entries;
            cmsUInt16Number* int_values = (cmsUInt16Number*)Data;
            
            cmsToneCurve* curve3 = cmsBuildTabulatedToneCurve16(context, 
                (cmsUInt32Number)nEntries, int_values);
            if (curve3) {
                cmsFreeToneCurve(curve3);
            }
        }
    }
    
    // Test 4: cmsBuildSegmentedToneCurve
    if (Size >= sizeof(cmsCurveSegment)) {
        size_t max_segments = Size / sizeof(cmsCurveSegment);
        if (max_segments > 0) {
            size_t nSegments = max_segments > 10 ? 10 : max_segments;
            cmsCurveSegment* segments = (cmsCurveSegment*)Data;
            
            cmsToneCurve* curve4 = cmsBuildSegmentedToneCurve(context, 
                (cmsUInt32Number)nSegments, segments);
            if (curve4) {
                cmsFreeToneCurve(curve4);
            }
        }
    }
    
    // Test 5: cmsReverseToneCurveEx
    cmsToneCurve* curve5 = create_test_tone_curve_16(context);
    if (curve5) {
        cmsUInt32Number nSamples = 256;
        if (Size >= sizeof(cmsUInt32Number)) {
            nSamples = *(cmsUInt32Number*)Data;
            nSamples = nSamples > 65536 ? 65536 : nSamples;
            nSamples = nSamples < 2 ? 2 : nSamples;
        }
        
        cmsToneCurve* reversed = cmsReverseToneCurveEx(nSamples, curve5);
        if (reversed) {
            cmsFreeToneCurve(reversed);
        }
        cmsFreeToneCurve(curve5);
    }
    
    // Test 6: cmsJoinToneCurve
    cmsToneCurve* curve6a = create_test_tone_curve_float(context);
    cmsToneCurve* curve6b = create_test_tone_curve_16(context);
    
    if (curve6a && curve6b) {
        cmsUInt32Number nPoints = 256;
        if (Size >= sizeof(cmsUInt32Number)) {
            nPoints = *(cmsUInt32Number*)Data;
            nPoints = nPoints > 65536 ? 65536 : nPoints;
            nPoints = nPoints < 2 ? 2 : nPoints;
        }
        
        cmsToneCurve* joined = cmsJoinToneCurve(context, curve6a, curve6b, nPoints);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
    }
    
    if (curve6a) cmsFreeToneCurve(curve6a);
    if (curve6b) cmsFreeToneCurve(curve6b);
    
    // Clean up
    cmsDeleteContext(context);
    
    return 0;
}