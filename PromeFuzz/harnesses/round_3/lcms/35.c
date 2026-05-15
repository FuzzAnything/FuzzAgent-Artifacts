// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve_from_data(const uint8_t* data, size_t size, cmsContext context) {
    if (size < 4) return NULL;
    
    // Use first 2 bytes for nEntries (limit to reasonable size)
    cmsUInt32Number nEntries = (data[0] << 8) | data[1];
    if (nEntries == 0) nEntries = 1;
    if (nEntries > 256) nEntries = 256; // Limit to avoid excessive memory
    
    // Check if we have enough data for the table
    size_t needed_bytes = nEntries * 2;
    if (size < 2 + needed_bytes) {
        nEntries = (size - 2) / 2;
        if (nEntries == 0) return NULL;
    }
    
    cmsUInt16Number* values = (cmsUInt16Number*)malloc(nEntries * sizeof(cmsUInt16Number));
    if (!values) return NULL;
    
    // Fill table with data from fuzzer input
    for (cmsUInt32Number i = 0; i < nEntries; i++) {
        size_t offset = 2 + i * 2;
        if (offset + 1 < size) {
            values[i] = (data[offset] << 8) | data[offset + 1];
        } else {
            values[i] = i * 256; // Default pattern if data runs out
        }
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(context, nEntries, values);
    free(values);
    return curve;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Use default context
    cmsContext context = NULL;
    
    // Create test curves from fuzzer data
    cmsToneCurve* curve1 = create_test_curve_from_data(Data, Size, context);
    if (!curve1) return 0;
    
    // Test cmsEvalToneCurve16 with various inputs
    if (Size >= 4) {
        cmsUInt16Number v = (Data[0] << 8) | Data[1];
        cmsEvalToneCurve16(curve1, v);
        
        // Test with more values from the input
        for (size_t i = 2; i + 1 < Size && i < 10; i += 2) {
            v = (Data[i] << 8) | Data[i + 1];
            cmsEvalToneCurve16(curve1, v);
        }
    }
    
    // Test cmsReverseToneCurve
    cmsToneCurve* reversed = cmsReverseToneCurve(curve1);
    
    // Test cmsDupToneCurve
    cmsToneCurve* duplicated = cmsDupToneCurve(curve1);
    
    // Test cmsGetToneCurveEstimatedTable
    if (curve1) {
        const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(curve1);
        (void)table; // Use table to avoid unused variable warning
    }
    
    if (reversed) {
        const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(reversed);
        (void)table;
    }
    
    if (duplicated) {
        const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(duplicated);
        (void)table;
    }
    
    // Test cmsFreeToneCurveTriple
    cmsToneCurve* triple[3];
    triple[0] = curve1;
    triple[1] = reversed;
    triple[2] = duplicated;
    cmsFreeToneCurveTriple(triple);
    
    // Create another curve with different data offset to explore more states
    if (Size > 50) {
        const uint8_t* second_data = Data + 25;
        size_t second_size = Size - 25;
        cmsToneCurve* curve2 = create_test_curve_from_data(second_data, second_size, context);
        if (curve2) {
            // Test evaluation with this curve
            if (second_size >= 2) {
                cmsUInt16Number v = (second_data[0] << 8) | second_data[1];
                cmsEvalToneCurve16(curve2, v);
            }
            
            // Test reverse and duplicate
            cmsToneCurve* reversed2 = cmsReverseToneCurve(curve2);
            cmsToneCurve* dup2 = cmsDupToneCurve(curve2);
            
            // Free using triple
            cmsToneCurve* triple2[3] = {curve2, reversed2, dup2};
            cmsFreeToneCurveTriple(triple2);
        }
    }
    
    return 0;
}