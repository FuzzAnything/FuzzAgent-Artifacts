// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Early exit if not enough data for basic operations
    if (Size < 4) return 0;

    // Use default context
    cmsContext contextID = NULL;

    // Create tone curve from fuzzer data
    cmsUInt32Number nEntries = 256; // Fixed reasonable size
    if (Size / 2 < nEntries) nEntries = Size / 2;
    
    cmsUInt16Number* values = (cmsUInt16Number*)Data;
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(contextID, nEntries, values);
    if (!curve) return 0;

    // Test cmsDupToneCurve
    cmsToneCurve* dupCurve = cmsDupToneCurve(curve);
    
    // Test cmsReverseToneCurve
    cmsToneCurve* revCurve = cmsReverseToneCurve(curve);
    
    // Test cmsGetToneCurveEstimatedTable
    const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(curve);
    if (table && nEntries > 0) {
        // Test cmsEvalToneCurve16 with some values
        for (int i = 0; i < 10 && i < nEntries; i++) {
            cmsUInt16Number result = cmsEvalToneCurve16(curve, values[i]);
            (void)result; // Suppress unused warning
        }
    }

    // Test cmsFreeToneCurveTriple
    cmsToneCurve* triple[3] = {curve, dupCurve, revCurve};
    cmsFreeToneCurveTriple(triple);

    return 0;
}