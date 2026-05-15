// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsGetToneCurveEstimatedTable at cmsgamma.c:774:34 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsEvalToneCurve16 at cmsgamma.c:1437:27 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
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

static cmsToneCurve* create_tone_curve_from_fuzz(const uint8_t* data, size_t size) {
    if (size < 2) {
        return NULL;
    }
    
    cmsUInt32Number nEntries = size / 2;
    if (nEntries == 0) {
        nEntries = 1;
    }
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)data;
    return cmsBuildTabulatedToneCurve16(NULL, nEntries, values);
}

static void test_tone_curve_operations(cmsToneCurve* curve) {
    if (!curve) {
        return;
    }
    
    cmsToneCurve* dup = cmsDupToneCurve(curve);
    if (dup) {
        const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(dup);
        if (table) {
            volatile cmsUInt16Number dummy = table[0];
            (void)dummy;
        }
        
        cmsToneCurve* rev = cmsReverseToneCurve(dup);
        if (rev) {
            for (int i = 0; i < 10; i++) {
                cmsUInt16Number val = cmsEvalToneCurve16(rev, i * 6553);
                (void)val;
            }
            
            cmsToneCurve* triple[3] = {dup, rev, NULL};
            cmsFreeToneCurveTriple(triple);
        } else {
            cmsToneCurve* single[3] = {dup, NULL, NULL};
            cmsFreeToneCurveTriple(single);
        }
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    cmsToneCurve* curve = create_tone_curve_from_fuzz(Data, Size);
    if (!curve) {
        return 0;
    }
    
    test_tone_curve_operations(curve);
    
    cmsToneCurve* to_free[3] = {curve, NULL, NULL};
    cmsFreeToneCurveTriple(to_free);
    
    return 0;
}