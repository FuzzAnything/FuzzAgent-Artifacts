// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildGamma at cmsgamma.c:909:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
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

static cmsContext create_test_context(void) {
    return cmsCreateContext(NULL, NULL);
}

static void cleanup_context(cmsContext ctx) {
    if (ctx) {
        cmsDeleteContext(ctx);
    }
}

static cmsToneCurve* create_test_float_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsFloat32Number) * 2) {
        return NULL;
    }
    
    size_t max_entries = size / sizeof(cmsFloat32Number);
    if (max_entries > 1024) {
        max_entries = 1024;
    }
    
    cmsFloat32Number* values = (cmsFloat32Number*)data;
    return cmsBuildTabulatedToneCurveFloat(ctx, (cmsUInt32Number)max_entries, values);
}

static cmsToneCurve* create_test_16bit_curve(cmsContext ctx, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt16Number) * 2) {
        return NULL;
    }
    
    size_t max_entries = size / sizeof(cmsUInt16Number);
    if (max_entries > 1024) {
        max_entries = 1024;
    }
    
    cmsUInt16Number* values = (cmsUInt16Number*)data;
    return cmsBuildTabulatedToneCurve16(ctx, (cmsUInt32Number)max_entries, values);
}

static cmsToneCurve* create_simple_gamma_curve(cmsContext ctx, cmsFloat64Number gamma) {
    cmsToneCurve* curve = cmsBuildGamma(ctx, gamma);
    return curve;
}

static void test_curve_operations(cmsContext ctx, cmsToneCurve* curve1, cmsToneCurve* curve2, const uint8_t* data, size_t size) {
    if (!curve1) return;
    
    // Test cmsGetToneCurveEstimatedTableEntries
    cmsUInt32Number entries = cmsGetToneCurveEstimatedTableEntries(curve1);
    (void)entries;
    
    // Test cmsReverseToneCurveEx with various sample counts
    cmsUInt32Number sample_count = 256;
    if (size >= sizeof(cmsUInt32Number)) {
        sample_count = *(const cmsUInt32Number*)data;
        if (sample_count == 0) sample_count = 256;
        if (sample_count > 65536) sample_count = 65536;
    }
    
    cmsToneCurve* reversed = cmsReverseToneCurveEx(sample_count, curve1);
    if (reversed) {
        cmsFreeToneCurve(reversed);
    }
    
    // Test cmsJoinToneCurve if we have two curves
    if (curve2) {
        cmsUInt32Number join_points = 256;
        if (size >= sizeof(cmsUInt32Number) * 2) {
            join_points = *((const cmsUInt32Number*)data + 1);
            if (join_points == 0) join_points = 256;
            if (join_points > 65536) join_points = 65536;
        }
        
        cmsToneCurve* joined = cmsJoinToneCurve(ctx, curve1, curve2, join_points);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t* data, size_t size) {
    if (size < 4) {
        return 0;
    }
    
    cmsContext ctx = create_test_context();
    if (!ctx) {
        return 0;
    }
    
    // Create various types of curves for testing
    cmsToneCurve* float_curve = create_test_float_curve(ctx, data, size);
    cmsToneCurve* bit16_curve = create_test_16bit_curve(ctx, data, size);
    
    // Create a simple gamma curve for additional testing
    cmsFloat64Number gamma = 2.2;
    if (size >= sizeof(cmsFloat64Number)) {
        gamma = *(const cmsFloat64Number*)data;
        if (gamma < 0.1) gamma = 0.1;
        if (gamma > 10.0) gamma = 10.0;
    }
    cmsToneCurve* gamma_curve = create_simple_gamma_curve(ctx, gamma);
    
    // Test operations with different curve combinations
    if (float_curve) {
        test_curve_operations(ctx, float_curve, gamma_curve, data, size);
    }
    
    if (bit16_curve) {
        test_curve_operations(ctx, bit16_curve, float_curve ? float_curve : gamma_curve, data, size);
    }
    
    if (gamma_curve) {
        test_curve_operations(ctx, gamma_curve, bit16_curve ? bit16_curve : float_curve, data, size);
    }
    
    // Cleanup
    if (float_curve) {
        cmsFreeToneCurve(float_curve);
    }
    if (bit16_curve) {
        cmsFreeToneCurve(bit16_curve);
    }
    if (gamma_curve) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    cleanup_context(ctx);
    return 0;
}