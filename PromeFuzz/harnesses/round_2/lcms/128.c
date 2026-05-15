// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

static cmsToneCurve* create_tone_curve_from_fuzzer(const uint8_t* data, size_t size, cmsContext context) {
    if (size < 4) return NULL;
    
    // Use first 4 bytes for nEntries (but limit to reasonable size)
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data;
    if (nEntries > 1024) nEntries = 1024; // Prevent excessive memory usage
    
    // Ensure we have enough data for the table
    size_t needed_bytes = nEntries * sizeof(cmsUInt16Number);
    if (size < 4 + needed_bytes) return NULL;
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + 4);
    return cmsBuildTabulatedToneCurve16(context, nEntries, values);
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    // Use default context
    cmsContext context = NULL;
    
    // Create initial tone curve from fuzzer data
    cmsToneCurve* curve = create_tone_curve_from_fuzzer(Data, Size, context);
    if (!curve) return 0;
    
    // Test cmsDupToneCurve
    cmsToneCurve* dup_curve = cmsDupToneCurve(curve);
    
    // Test cmsReverseToneCurve
    cmsToneCurve* rev_curve = cmsReverseToneCurve(curve);
    
    // Test cmsEvalToneCurve16 with various inputs
    if (Size > 0) {
        // Use first byte as input value (0-255), convert to 16-bit
        cmsUInt16Number input_val = (cmsUInt16Number)(Data[0] * 257); // Scale to 16-bit
        cmsUInt16Number result = cmsEvalToneCurve16(curve, input_val);
        
        // Also test with duplicate and reverse curves if they exist
        if (dup_curve) {
            cmsUInt16Number dup_result = cmsEvalToneCurve16(dup_curve, input_val);
        }
        if (rev_curve) {
            cmsUInt16Number rev_result = cmsEvalToneCurve16(rev_curve, input_val);
        }
    }
    
    // Test cmsGetToneCurveEstimatedTable
    const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(curve);
    if (table) {
        // We don't know the exact size, but we can safely read a few values
        // to ensure the function works without accessing internal struct
        for (int i = 0; i < 10; i++) {
            cmsUInt16Number val = table[i];
        }
    }
    
    // Test cmsFreeToneCurveTriple
    cmsToneCurve* triple[3] = {curve, dup_curve, rev_curve};
    cmsFreeToneCurveTriple(triple);
    
    // Create another test with different data pattern
    if (Size > 100) {
        // Use middle portion of data for second curve
        const uint8_t* second_data = Data + (Size / 2);
        size_t second_size = Size / 2;
        
        cmsToneCurve* curve2 = create_tone_curve_from_fuzzer(second_data, second_size, context);
        if (curve2) {
            cmsToneCurve* curve2_dup = cmsDupToneCurve(curve2);
            cmsToneCurve* curve2_rev = cmsReverseToneCurve(curve2);
            
            // Test evaluation with different input
            if (second_size > 0) {
                cmsUInt16Number input_val2 = (cmsUInt16Number)(second_data[0] * 257);
                cmsUInt16Number result2 = cmsEvalToneCurve16(curve2, input_val2);
            }
            
            // Test cmsGetToneCurveEstimatedTable on second curve
            const cmsUInt16Number* table2 = cmsGetToneCurveEstimatedTable(curve2);
            if (table2) {
                for (int i = 0; i < 5; i++) {
                    cmsUInt16Number val = table2[i];
                }
            }
            
            // Free using triple
            cmsToneCurve* triple2[3] = {curve2, curve2_dup, curve2_rev};
            cmsFreeToneCurveTriple(triple2);
        }
    }
    
    return 0;
}