// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsEvalToneCurveFloat at cmsgamma.c:1418:28 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve_from_fuzz(const uint8_t* data, size_t size, cmsContext ctx) {
    if (size < sizeof(cmsFloat32Number)) return NULL;
    
    cmsUInt32Number nEntries = size / sizeof(cmsFloat32Number);
    if (nEntries > 256) nEntries = 256; // Limit to reasonable size
    
    cmsFloat32Number* values = (cmsFloat32Number*)data;
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

static cmsToneCurve* create_second_curve_from_fuzz(const uint8_t* data, size_t size, cmsContext ctx) {
    if (size < 2 * sizeof(cmsFloat32Number)) return NULL;
    
    // Use different part of data for second curve
    const uint8_t* alt_data = data + (size / 2);
    size_t alt_size = size - (size / 2);
    
    cmsUInt32Number nEntries = alt_size / sizeof(cmsFloat32Number);
    if (nEntries > 128) nEntries = 128;
    
    cmsFloat32Number* values = (cmsFloat32Number*)alt_data;
    return cmsBuildTabulatedToneCurveFloat(ctx, nEntries, values);
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(cmsFloat32Number)) return 0;
    
    cmsContext ctx = NULL; // Use default context
    
    // Test 1: cmsBuildTabulatedToneCurveFloat
    cmsToneCurve* curve1 = create_test_curve_from_fuzz(Data, Size, ctx);
    if (!curve1) return 0;
    
    // Test 2: cmsReverseToneCurve
    cmsToneCurve* reversed1 = cmsReverseToneCurve(curve1);
    if (reversed1) {
        // Test 5: cmsEvalToneCurveFloat on reversed curve
        for (int i = 0; i < 10; i++) {
            cmsFloat32Number test_val = (cmsFloat32Number)i / 10.0f;
            cmsEvalToneCurveFloat(reversed1, test_val);
        }
        
        // Test 6: cmsDupToneCurve
        cmsToneCurve* dup_reversed = cmsDupToneCurve(reversed1);
        if (dup_reversed) {
            cmsFreeToneCurve(dup_reversed);
        }
        
        cmsFreeToneCurve(reversed1);
    }
    
    // Test 3: cmsReverseToneCurveEx
    cmsUInt32Number sample_count = 256;
    if (Size >= sizeof(cmsUInt32Number)) {
        sample_count = *(cmsUInt32Number*)Data % 1024;
        if (sample_count < 2) sample_count = 2;
    }
    
    cmsToneCurve* reversed_ex = cmsReverseToneCurveEx(sample_count, curve1);
    if (reversed_ex) {
        cmsFreeToneCurve(reversed_ex);
    }
    
    // Test 4: cmsJoinToneCurve
    cmsToneCurve* curve2 = create_second_curve_from_fuzz(Data, Size, ctx);
    if (curve2) {
        cmsUInt32Number join_points = 256;
        if (Size >= 2 * sizeof(cmsUInt32Number)) {
            join_points = *((cmsUInt32Number*)Data + 1) % 1024;
            if (join_points < 2) join_points = 2;
        }
        
        cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, join_points);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
        
        cmsFreeToneCurve(curve2);
    }
    
    // Test 5: cmsEvalToneCurveFloat on original curve
    for (int i = 0; i < 10; i++) {
        cmsFloat32Number test_val = (cmsFloat32Number)i / 10.0f;
        cmsEvalToneCurveFloat(curve1, test_val);
    }
    
    // Test 6: cmsDupToneCurve on original
    cmsToneCurve* dup_curve = cmsDupToneCurve(curve1);
    if (dup_curve) {
        cmsFreeToneCurve(dup_curve);
    }
    
    cmsFreeToneCurve(curve1);
    
    return 0;
}