// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_tone_curve_from_fuzz(const uint8_t *data, size_t size, cmsContext ctx) {
    if (size < 4) return NULL;
    
    cmsUInt32Number nEntries = *(const cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 65536) return NULL;
    
    size_t needed_bytes = nEntries * sizeof(cmsUInt16Number);
    if (size < 4 + needed_bytes) return NULL;
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + 4);
    return cmsBuildTabulatedToneCurve16(ctx, nEntries, values);
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    cmsContext context = NULL;
    
    cmsToneCurve* curve1 = create_tone_curve_from_fuzz(Data, Size, context);
    if (!curve1) return 0;
    
    cmsToneCurve* curve2 = cmsDupToneCurve(curve1);
    
    cmsToneCurve* curve3 = NULL;
    if (curve2) {
        curve3 = cmsReverseToneCurve(curve2);
    }
    
    if (curve1) {
        for (int i = 0; i < 10; i++) {
            cmsUInt16Number v = (i * 65535) / 9;
            cmsUInt16Number result = cmsEvalToneCurve16(curve1, v);
            (void)result;
        }
        
        const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(curve1);
        (void)table;
    }
    
    cmsToneCurve* triple[3] = {curve1, curve2, curve3};
    cmsFreeToneCurveTriple(triple);
    
    return 0;
}