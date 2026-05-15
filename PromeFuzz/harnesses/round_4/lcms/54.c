// This fuzz driver is generated for library lcms, aiming to fuzz the following functions:
// cmsReverseToneCurveEx at cmsgamma.c:1070:25 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsFreeToneCurve at cmsgamma.c:916:16 in lcms2.h
// cmsDeleteContext at cmsplugin.c:963:16 in lcms2.h
// cmsBuildTabulatedToneCurveFloat at cmsgamma.c:832:25 in lcms2.h
// cmsBuildTabulatedToneCurve16 at cmsgamma.c:783:25 in lcms2.h
// cmsBuildSegmentedToneCurve at cmsgamma.c:797:25 in lcms2.h
// cmsCreateContext at cmsplugin.c:824:22 in lcms2.h
// cmsJoinToneCurve at cmsgamma.c:980:25 in lcms2.h
// cmsGetToneCurveEstimatedTableEntries at cmsgamma.c:768:27 in lcms2.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcms2.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static cmsToneCurve* create_test_curve_float(cmsContext context, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return NULL;
    
    cmsUInt32Number nEntries = *(cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 1024) nEntries = 256; // Sanity check
    
    size_t values_needed = nEntries * sizeof(cmsFloat32Number);
    if (size < sizeof(cmsUInt32Number) + values_needed) return NULL;
    
    const cmsFloat32Number* values = (const cmsFloat32Number*)(data + sizeof(cmsUInt32Number));
    return cmsBuildTabulatedToneCurveFloat(context, nEntries, values);
}

static cmsToneCurve* create_test_curve_16(cmsContext context, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return NULL;
    
    cmsUInt32Number nEntries = *(cmsUInt32Number*)data;
    if (nEntries == 0 || nEntries > 1024) nEntries = 256; // Sanity check
    
    size_t values_needed = nEntries * sizeof(cmsUInt16Number);
    if (size < sizeof(cmsUInt32Number) + values_needed) return NULL;
    
    const cmsUInt16Number* values = (const cmsUInt16Number*)(data + sizeof(cmsUInt32Number));
    return cmsBuildTabulatedToneCurve16(context, nEntries, values);
}

static cmsToneCurve* create_test_segmented_curve(cmsContext context, const uint8_t* data, size_t size) {
    if (size < sizeof(cmsUInt32Number)) return NULL;
    
    cmsUInt32Number nSegments = *(cmsUInt32Number*)data;
    if (nSegments == 0 || nSegments > 16) nSegments = 1; // Sanity check
    
    size_t segments_needed = nSegments * sizeof(cmsCurveSegment);
    if (size < sizeof(cmsUInt32Number) + segments_needed) return NULL;
    
    const cmsCurveSegment* segments = (const cmsCurveSegment*)(data + sizeof(cmsUInt32Number));
    return cmsBuildSegmentedToneCurve(context, nSegments, segments);
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (!context) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t* fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    cmsToneCurve* curve1 = NULL;
    cmsToneCurve* curve2 = NULL;
    cmsToneCurve* result_curve = NULL;
    
    switch (selector) {
        case 0: // cmsBuildTabulatedToneCurveFloat
            curve1 = create_test_curve_float(context, fuzz_data, fuzz_size);
            break;
            
        case 1: // cmsBuildSegmentedToneCurve
            curve1 = create_test_segmented_curve(context, fuzz_data, fuzz_size);
            break;
            
        case 2: // cmsBuildTabulatedToneCurve16
            curve1 = create_test_curve_16(context, fuzz_data, fuzz_size);
            break;
            
        case 3: // cmsJoinToneCurve
            if (fuzz_size >= 2) {
                // Split fuzz data for two curves
                size_t half_size = fuzz_size / 2;
                curve1 = create_test_curve_float(context, fuzz_data, half_size);
                curve2 = create_test_curve_16(context, fuzz_data + half_size, fuzz_size - half_size);
                
                if (curve1 && curve2) {
                    cmsUInt32Number nPoints = 256; // Default value
                    if (fuzz_size >= sizeof(cmsUInt32Number)) {
                        nPoints = *(cmsUInt32Number*)fuzz_data % 4096;
                    }
                    result_curve = cmsJoinToneCurve(context, curve1, curve2, nPoints);
                }
            }
            break;
            
        case 4: // cmsGetToneCurveEstimatedTableEntries
            curve1 = create_test_curve_float(context, fuzz_data, fuzz_size);
            if (curve1) {
                cmsUInt32Number estimated = cmsGetToneCurveEstimatedTableEntries(curve1);
                (void)estimated; // Avoid unused variable warning
            }
            break;
            
        case 5: // cmsReverseToneCurveEx
            curve1 = create_test_curve_float(context, fuzz_data, fuzz_size);
            if (curve1) {
                cmsUInt32Number nResultSamples = 256; // Default value
                if (fuzz_size >= sizeof(cmsUInt32Number)) {
                    nResultSamples = *(cmsUInt32Number*)fuzz_data % 4096;
                }
                result_curve = cmsReverseToneCurveEx(nResultSamples, curve1);
            }
            break;
    }
    
    // Cleanup
    if (curve1) cmsFreeToneCurve(curve1);
    if (curve2) cmsFreeToneCurve(curve2);
    if (result_curve) cmsFreeToneCurve(result_curve);
    
    cmsDeleteContext(context);
    
    return 0;
}