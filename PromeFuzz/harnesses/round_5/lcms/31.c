// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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

static cmsToneCurve* create_test_curve_from_data(const uint8_t* data, size_t size, int curve_type) {
    // Create a simple tone curve from fuzzer data
    // Use different curve types based on input to explore more states
    if (size < 2) return NULL;
    
    // Create a tabulated curve with up to 256 entries
    cmsUInt32Number nEntries = 256;
    if (size/2 < nEntries) {
        nEntries = (cmsUInt32Number)(size/2);
    }
    if (nEntries < 2) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!table) return NULL;
    
    // Fill table with data from fuzzer input
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        if (i*2 + 1 < size) {
            table[i] = (data[i*2] << 8) | data[i*2 + 1];
        } else {
            table[i] = (cmsUInt16Number)((i * 65535) / (nEntries - 1));
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve_from_data(const uint8_t* data, size_t size) {
    // Create a parametric curve from fuzzer data
    if (size < 10) return NULL;
    
    cmsFloat64Number params[10];
    for (int i = 0; i < 10; i++) {
        params[i] = (cmsFloat64Number)data[i % size] / 255.0;
    }
    
    // Try different curve types
    int type = data[0] % 6;  // 0-5 for different parametric curve types
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create test tone curves from fuzzer data
    cmsToneCurve* curve1 = create_test_curve_from_data(Data, Size, 0);
    cmsToneCurve* curve2 = create_test_curve_from_data(Data + Size/2, Size/2, 1);
    cmsToneCurve* curve3 = create_parametric_curve_from_data(Data, Size);
    
    // Test cmsReverseToneCurve
    if (curve1) {
        cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
        if (reversed) {
            cmsFreeToneCurve(reversed);
        }
    }
    
    // Test cmsReverseToneCurveEx
    if (curve2) {
        cmsUInt32Number nSamples = 256;
        if (Size > 0) {
            nSamples = (cmsUInt32Number)(Data[0] * 4) + 2;  // 2-1022 samples
        }
        cmsToneCurve* reversed_ex = cmsReverseToneCurveEx(nSamples, curve2);
        if (reversed_ex) {
            cmsFreeToneCurve(reversed_ex);
        }
    }
    
    // Test cmsJoinToneCurve
    if (curve1 && curve2) {
        cmsUInt32Number nPoints = 256;
        if (Size > 1) {
            nPoints = (cmsUInt32Number)(Data[1] * 4) + 2;  // 2-1022 points
        }
        cmsToneCurve* joined = cmsJoinToneCurve(NULL, curve1, curve2, nPoints);
        if (joined) {
            cmsFreeToneCurve(joined);
        }
    }
    
    // Test cmsDupToneCurve
    cmsToneCurve* dup = NULL;
    if (curve1) {
        dup = cmsDupToneCurve(curve1);
    }
    
    // Test cmsFreeToneCurve on duplicated curve
    if (dup) {
        cmsFreeToneCurve(dup);
    }
    
    // Test cmsFreeToneCurveTriple - this will free curve1, curve2, curve3
    // and set the pointers in the array to NULL
    cmsToneCurve* triple[3] = {curve1, curve2, curve3};
    cmsFreeToneCurveTriple(triple);
    
    // After cmsFreeToneCurveTriple, the curves are already freed
    // so we don't need to free them again
    
    return 0;
}