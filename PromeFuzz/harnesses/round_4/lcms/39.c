// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcms2.h"

static cmsToneCurve* create_test_curve(const uint8_t* data, size_t size) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = 2 + (data[0] % 256); // Ensure at least 2 entries
    if (nEntries > 256) nEntries = 256; // Limit to reasonable size
    
    size_t needed_bytes = nEntries * sizeof(cmsUInt16Number);
    if (needed_bytes > size - 1) {
        nEntries = (size - 1) / sizeof(cmsUInt16Number);
        if (nEntries < 2) return NULL;
    }
    
    cmsUInt16Number* values = (cmsUInt16Number*)malloc(needed_bytes);
    if (!values) return NULL;
    
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        if (i * 2 + 1 < size) {
            values[i] = (data[i * 2] << 8) | data[i * 2 + 1];
        } else {
            values[i] = i * 1000;
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, values);
    free(values);
    return curve;
}

static void test_triple_free(cmsToneCurve* curve) {
    cmsToneCurve* triple[3] = {NULL, NULL, NULL};
    
    if (curve) {
        triple[0] = cmsDupToneCurve(curve);
        triple[1] = cmsReverseToneCurve(curve);
        triple[2] = cmsDupToneCurve(triple[1]);
    }
    
    cmsFreeToneCurveTriple(triple);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    cmsToneCurve* curve = create_test_curve(Data, Size);
    if (!curve) return 0;
    
    // Test cmsReverseToneCurve
    cmsToneCurve* reversed = cmsReverseToneCurve(curve);
    if (reversed) {
        // Test cmsEvalToneCurve16 with various inputs
        for (int i = 0; i < 10; i++) {
            cmsUInt16Number input = (Data[i % Size] << 8) | Data[(i + 1) % Size];
            cmsUInt16Number result1 = cmsEvalToneCurve16(curve, input);
            cmsUInt16Number result2 = cmsEvalToneCurve16(reversed, result1);
            (void)result1;
            (void)result2;
        }
        
        // Test cmsGetToneCurveEstimatedTable
        const cmsUInt16Number* table1 = cmsGetToneCurveEstimatedTable(curve);
        const cmsUInt16Number* table2 = cmsGetToneCurveEstimatedTable(reversed);
        (void)table1;
        (void)table2;
        
        // Test cmsDupToneCurve
        cmsToneCurve* dup = cmsDupToneCurve(curve);
        if (dup) {
            cmsUInt16Number test_val = 0x8000;
            cmsUInt16Number dup_result = cmsEvalToneCurve16(dup, test_val);
            (void)dup_result;
            cmsFreeToneCurve(dup);
        }
        
        cmsFreeToneCurve(reversed);
    }
    
    // Test triple free functionality
    test_triple_free(curve);
    
    cmsFreeToneCurve(curve);
    
    return 0;
}