// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurveTriple at cmsgamma.c:954:16 in lcms2.h
// cmsDupToneCurve at cmsgamma.c:968:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildParametricToneCurve at cmsgamma.c:880:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurve at cmsgamma.c:1137:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
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

static cmsToneCurve* create_test_tone_curve(const uint8_t* data, size_t size) {
    if (size < 2) return NULL;
    
    size_t nEntries = (data[0] << 8) | data[1];
    if (nEntries == 0) nEntries = 256;
    if (nEntries > 65535) nEntries = 65535;
    
    size_t needed_bytes = nEntries * sizeof(cmsUInt16Number);
    if (size < needed_bytes + 2) return NULL;
    
    cmsUInt16Number* table = (cmsUInt16Number*)malloc(needed_bytes);
    if (!table) return NULL;
    
    for (size_t i = 0; i < nEntries; i++) {
        table[i] = (data[i % (size - 2) + 2] << 8) | data[(i + 1) % (size - 2) + 2];
    }
    
    cmsToneCurve* curve = cmsBuildTabulatedToneCurve16(NULL, nEntries, table);
    free(table);
    return curve;
}

static cmsToneCurve* create_parametric_curve(const uint8_t* data, size_t size) {
    if (size < 12) return NULL;
    
    int type = data[0] % 10;
    cmsFloat64Number params[10];
    
    for (int i = 0; i < 10; i++) {
        params[i] = (cmsFloat64Number)data[i % size] / 255.0 * 10.0;
    }
    
    return cmsBuildParametricToneCurve(NULL, type, params);
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    switch (selector % 6) {
        case 0: {
            cmsToneCurve* curve = create_test_tone_curve(remaining_data, remaining_size);
            cmsFreeToneCurve(curve);
            break;
        }
        
        case 1: {
            cmsToneCurve* curve = create_test_tone_curve(remaining_data, remaining_size);
            if (curve) {
                cmsToneCurve* reversed = cmsReverseToneCurve(curve);
                cmsFreeToneCurve(reversed);
                cmsFreeToneCurve(curve);
            }
            break;
        }
        
        case 2: {
            cmsToneCurve* curve = create_test_tone_curve(remaining_data, remaining_size);
            if (curve) {
                cmsUInt32Number nSamples = (remaining_data[0] << 8) | remaining_data[1];
                if (nSamples == 0) nSamples = 256;
                if (nSamples > 65535) nSamples = 65535;
                
                cmsToneCurve* reversed = cmsReverseToneCurveEx(nSamples, curve);
                cmsFreeToneCurve(reversed);
                cmsFreeToneCurve(curve);
            }
            break;
        }
        
        case 3: {
            if (remaining_size < 4) break;
            
            size_t split = remaining_size / 2;
            cmsToneCurve* curve1 = create_test_tone_curve(remaining_data, split);
            cmsToneCurve* curve2 = create_test_tone_curve(remaining_data + split, remaining_size - split);
            
            if (curve1 && curve2) {
                cmsUInt32Number nPoints = (remaining_data[0] << 8) | remaining_data[1];
                if (nPoints == 0) nPoints = 256;
                if (nPoints > 65535) nPoints = 65535;
                
                cmsToneCurve* joined = cmsJoinToneCurve(NULL, curve1, curve2, nPoints);
                cmsFreeToneCurve(joined);
            }
            
            cmsFreeToneCurve(curve1);
            cmsFreeToneCurve(curve2);
            break;
        }
        
        case 4: {
            cmsToneCurve* triple[3] = {NULL, NULL, NULL};
            
            if (remaining_size > 0) {
                size_t part_size = remaining_size / 3;
                triple[0] = create_test_tone_curve(remaining_data, part_size);
                if (remaining_size > part_size) {
                    triple[1] = create_test_tone_curve(remaining_data + part_size, part_size);
                }
                if (remaining_size > 2 * part_size) {
                    triple[2] = create_test_tone_curve(remaining_data + 2 * part_size, remaining_size - 2 * part_size);
                }
            }
            
            cmsFreeToneCurveTriple(triple);
            break;
        }
        
        case 5: {
            cmsToneCurve* curve = create_parametric_curve(remaining_data, remaining_size);
            if (!curve) {
                curve = create_test_tone_curve(remaining_data, remaining_size);
            }
            
            if (curve) {
                cmsToneCurve* dup = cmsDupToneCurve(curve);
                cmsFreeToneCurve(dup);
                cmsFreeToneCurve(curve);
            }
            break;
        }
    }
    
    return 0;
}