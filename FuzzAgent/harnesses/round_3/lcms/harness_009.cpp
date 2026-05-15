#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_009 {
// Fuzzing harness for LittleCMS (lcms) tone curve manipulation functions
// This harness specifically targets tone curve manipulation functions from cmsgamma.c module
// that are currently 0% covered according to coverage analysis
// Target APIs: cmsSmoothToneCurve, cmsJoinToneCurve, cmsGetToneCurveSegment,
// cmsGetToneCurveParametricType, cmsGetToneCurveEstimatedTableEntries
// MODULE: cmsgamma.c [current coverage: low]
// STRATEGY: Target tone curve manipulation and inspection operations lifecycle
// DIFFERENTIATION: Unlike existing harnesses focused on transforms, PostScript, virtual profiles, I/O operations, IT8 handling, dictionary management, CIECAM02, and MD5, target fundamental tone curve operations used throughout color management
// API SEQUENCE: Create tone curves -> Apply smoothing -> Join curves -> Inspect segments -> Determine curve type -> Estimate table entries -> Cleanup


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful tone curve operations
    // Minimum: context + gamma values + lambda + nPoints + segment index
    // Let's require at least 64 bytes
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create Context: Create a working context for tone curve operations
    // Use 0 for default context as per common practice
    cmsContext contextID = 0;
    
    // 2. Create initial tone curves using different methods
    
    // 2a. Create a simple gamma curve
    cmsFloat64Number gamma1 = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
    cmsToneCurve* curve1 = cmsBuildGamma(contextID, gamma1);
    if (curve1 == NULL) {
        return 0; // Cannot create first tone curve
    }
    
    // 2b. Create another gamma curve with different gamma
    cmsFloat64Number gamma2 = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.1, 5.0);
    cmsToneCurve* curve2 = cmsBuildGamma(contextID, gamma2);
    if (curve2 == NULL) {
        cmsFreeToneCurve(curve1);
        return 0; // Cannot create second tone curve
    }
    
    // 2c. Create a parametric tone curve if we have enough data
    cmsToneCurve* parametricCurve = NULL;
    if (fdp.remaining_bytes() >= sizeof(cmsFloat64Number) * 7) {
        // Type 1-8 are parametric curve types, let's pick one
        cmsInt32Number paramType = fdp.ConsumeIntegralInRange<cmsInt32Number>(1, 8);
        
        // Create parameter array based on type
        cmsFloat64Number params[10]; // Maximum parameters for any type
        
        // Consume appropriate number of parameters
        int paramCount = 0;
        switch(paramType) {
            case 1: paramCount = 1; break; // Y = X^g
            case 2: paramCount = 3; break; // Y = (aX + b)^g + c
            case 3: paramCount = 4; break; // Y = (aX + b)^g + c | X >= -b/a
            case 4: paramCount = 5; break; // Y = (aX + b)^g + c | X <= d
            case 5: paramCount = 7; break; // Y = (aX + b)^g + c | X >= d X <= e
            case 6: paramCount = 4; break; // Y = a * log(b * X^g + c) + d
            case 7: paramCount = 5; break; // Y = a * log(b * X^g + c) + d
            case 8: paramCount = 5; break; // Y = (aX + b) / (cX + d) | X >= -d/c
        }
        
        if (fdp.remaining_bytes() >= sizeof(cmsFloat64Number) * paramCount) {
            for (int i = 0; i < paramCount; i++) {
                params[i] = fdp.ConsumeFloatingPoint<cmsFloat64Number>();
            }
            parametricCurve = cmsBuildParametricToneCurve(contextID, paramType, params);
        }
    }
    
    // 3. Apply smoothing to curve1
    cmsFloat64Number lambda = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    cmsBool smoothResult = cmsSmoothToneCurve(curve1, lambda);
    // Don't check result - fuzzer should explore both success and failure paths
    
    // 4. Join curve1 and curve2
    cmsUInt32Number nPoints = fdp.ConsumeIntegralInRange<cmsUInt32Number>(2, 4096); // Reasonable range
    cmsToneCurve* joinedCurve = cmsJoinToneCurve(contextID, curve1, curve2, nPoints);
    
    // 5. Inspect curve segments if we have a curve to inspect
    if (joinedCurve != NULL) {
        // Try to get segments by calling cmsGetToneCurveSegment in a loop
        // until it returns NULL
        for (cmsInt32Number i = 0; i < 10; i++) { // Reasonable limit
            const cmsCurveSegment* segment = cmsGetToneCurveSegment(i, joinedCurve);
            if (segment == NULL) {
                break;
            }
            // Access segment fields to trigger coverage
            // segment->Type, segment->Params, etc.
            // Just accessing is enough for coverage
        }
        
        // 6. Determine parametric type of the curve
        cmsInt32Number parametricType = cmsGetToneCurveParametricType(joinedCurve);
        // parametricType could be -1 for non-parametric curves
        
        // 7. Estimate table entries for the curve
        cmsUInt32Number estimatedEntries = cmsGetToneCurveEstimatedTableEntries(joinedCurve);
        // estimatedEntries could be 0 for invalid curves
    }
    
    // 8. Also inspect original curves if parametricCurve exists
    if (parametricCurve != NULL) {
        // Get parametric type
        cmsInt32Number paramType = cmsGetToneCurveParametricType(parametricCurve);
        
        // Estimate table entries
        cmsUInt32Number estimatedEntries = cmsGetToneCurveEstimatedTableEntries(parametricCurve);
        
        // Get segments - try a few indices
        for (cmsInt32Number i = 0; i < 5; i++) {
            const cmsCurveSegment* segment = cmsGetToneCurveSegment(i, parametricCurve);
            if (segment == NULL) {
                break;
            }
        }
    }
    
    // 9. Clean up all tone curves
    if (joinedCurve != NULL) {
        cmsFreeToneCurve(joinedCurve);
    }
    if (parametricCurve != NULL) {
        cmsFreeToneCurve(parametricCurve);
    }
    cmsFreeToneCurve(curve2);
    cmsFreeToneCurve(curve1);
    
    return 0;
}

} // namespace harness_009
