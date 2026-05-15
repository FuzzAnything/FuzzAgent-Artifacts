#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <cmath>

namespace harness_011 {
/*
 * Harness 011 - Tone Curve Manipulation Fuzzer for Little CMS 2
 * 
 * Target APIs:
 * - cmsSmoothToneCurve (56 accumulated branches, 50 undiscovered)
 * - cmsJoinToneCurve (90 accumulated branches, 43 undiscovered)
 * - cmsGetToneCurveSegment (8 accumulated branches, 8 undiscovered)
 * - cmsGetToneCurveParametricType (6 accumulated branches, 6 undiscovered)
 * - cmsIsToneCurveMultisegment (4 accumulated branches, 4 undiscovered)
 * - cmsGetToneCurveEstimatedTableEntries (4 accumulated branches, 4 undiscovered)
 * - cmsGetToneCurveEstimatedTable (4 accumulated branches, 4 undiscovered)
 * 
 * Required Helper APIs (Creation):
 * - cmsBuildGamma
 * - cmsBuildParametricToneCurve
 * - cmsBuildSegmentedToneCurve
 * - cmsBuildTabulatedToneCurve16
 * - cmsBuildTabulatedToneCurveFloat
 * 
 * Required Helper APIs (Cleanup):
 * - cmsFreeToneCurve
 * - cmsFreeToneCurveTriple
 * 
 * Invocation sequence:
 * 1. Create various tone curves using different creation functions
 * 2. Apply smoothing operation using cmsSmoothToneCurve on created curves
 * 3. Join multiple tone curves using cmsJoinToneCurve
 * 4. Query curve properties using cmsGetToneCurveSegment, cmsGetToneCurveParametricType, etc.
 * 5. Check curve characteristics using cmsIsToneCurveMultisegment
 * 6. Get estimated table data using cmsGetToneCurveEstimatedTableEntries and cmsGetToneCurveEstimatedTable
 * 7. Clean up all allocated tone curves
 * 
 * These functions represent 180+ points of undiscovered complexity in the cmsgamma.c module.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multiple tone curve operations
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint32_t curve_type = fdp.ConsumeIntegral<uint32_t>() % 5;  // 0-4 for different curve types
    uint32_t num_segments = fdp.ConsumeIntegralInRange<uint32_t>(1, 5);
    double lambda = fdp.ConsumeFloatingPointInRange<double>(0.0, 10.0);
    uint32_t join_points = fdp.ConsumeIntegralInRange<uint32_t>(10, 1000);
    bool use_context = fdp.ConsumeBool();
    
    // Create context if requested
    cmsContext context = use_context ? cmsCreateContext(NULL, NULL) : NULL;
    
    // Array to store created curves for cleanup
    std::vector<cmsToneCurve*> curves;
    
    // Test 1: Create various types of tone curves
    for (uint32_t i = 0; i < 3; i++) {
        cmsToneCurve* curve = NULL;
        
        switch ((curve_type + i) % 5) {
            case 0: {
                // Gamma curve
                double gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
                curve = cmsBuildGamma(context, gamma);
                break;
            }
            case 1: {
                // Parametric tone curve
                int parametric_type = fdp.ConsumeIntegralInRange<int>(1, 5);  // Common parametric types
                cmsFloat64Number params[10];
                for (int j = 0; j < 10; j++) {
                    params[j] = fdp.ConsumeFloatingPoint<double>();
                }
                curve = cmsBuildParametricToneCurve(context, parametric_type, params);
                break;
            }
            case 2: {
                // Segmented tone curve
                std::vector<cmsCurveSegment> segments(num_segments);
                for (uint32_t seg = 0; seg < num_segments; seg++) {
                    segments[seg].x0 = fdp.ConsumeFloatingPoint<float>();
                    segments[seg].x1 = fdp.ConsumeFloatingPoint<float>();
                    segments[seg].Type = fdp.ConsumeIntegral<int32_t>() % 5;
                    
                    for (int p = 0; p < 10; p++) {
                        segments[seg].Params[p] = fdp.ConsumeFloatingPoint<double>();
                    }
                    
                    segments[seg].nGridPoints = fdp.ConsumeIntegralInRange<uint32_t>(2, 100);
                    if (segments[seg].nGridPoints > 0) {
                        // Allocate and fill sampled points
                        segments[seg].SampledPoints = (cmsFloat32Number*)malloc(
                            segments[seg].nGridPoints * sizeof(cmsFloat32Number));
                        if (segments[seg].SampledPoints) {
                            for (uint32_t p = 0; p < segments[seg].nGridPoints; p++) {
                                segments[seg].SampledPoints[p] = fdp.ConsumeFloatingPoint<float>();
                            }
                        }
                    } else {
                        segments[seg].SampledPoints = NULL;
                    }
                }
                
                curve = cmsBuildSegmentedToneCurve(context, num_segments, segments.data());
                
                // Clean up allocated sampled points
                for (uint32_t seg = 0; seg < num_segments; seg++) {
                    if (segments[seg].SampledPoints) {
                        free(segments[seg].SampledPoints);
                    }
                }
                break;
            }
            case 3: {
                // Tabulated 16-bit tone curve
                uint32_t n_entries = fdp.ConsumeIntegralInRange<uint32_t>(2, 1000);
                std::vector<cmsUInt16Number> values(n_entries);
                for (uint32_t j = 0; j < n_entries; j++) {
                    values[j] = fdp.ConsumeIntegral<uint16_t>();
                }
                curve = cmsBuildTabulatedToneCurve16(context, n_entries, values.data());
                break;
            }
            case 4: {
                // Tabulated float tone curve
                uint32_t n_entries = fdp.ConsumeIntegralInRange<uint32_t>(2, 1000);
                std::vector<cmsFloat32Number> values(n_entries);
                for (uint32_t j = 0; j < n_entries; j++) {
                    values[j] = fdp.ConsumeFloatingPoint<float>();
                }
                curve = cmsBuildTabulatedToneCurveFloat(context, n_entries, values.data());
                break;
            }
        }
        
        if (curve) {
            curves.push_back(curve);
        }
    }
    
    // Need at least 2 curves for join operation
    if (curves.size() < 2) {
        for (auto curve : curves) {
            cmsFreeToneCurve(curve);
        }
        if (context) cmsDeleteContext(context);
        return 0;
    }
    
    // Test 2: Apply smoothing on curves
    for (auto curve : curves) {
        cmsSmoothToneCurve(curve, lambda);
    }
    
    // Test 3: Join curves
    cmsToneCurve* joined_curve = cmsJoinToneCurve(context, curves[0], curves[1], join_points);
    if (joined_curve) {
        curves.push_back(joined_curve);
        
        // Test 4: Query curve properties
        cmsIsToneCurveMultisegment(joined_curve);
        cmsGetToneCurveParametricType(joined_curve);
        
        // Test 5: Get curve segments
        int segment_index = 0;
        while (true) {
            const cmsCurveSegment* segment = cmsGetToneCurveSegment(segment_index, joined_curve);
            if (!segment) break;
            segment_index++;
        }
        
        // Test 6: Get estimated table data
        cmsUInt32Number table_entries = cmsGetToneCurveEstimatedTableEntries(joined_curve);
        if (table_entries > 0) {
            const cmsUInt16Number* table = cmsGetToneCurveEstimatedTable(joined_curve);
            if (table) {
                // Optionally use table data
                (void)table;
            }
        }
    }
    
    // Test 7: Create tone curve triple for testing cmsFreeToneCurveTriple
    if (curves.size() >= 3) {
        cmsToneCurve* triple[3] = {curves[0], curves[1], curves[2]};
        cmsFreeToneCurveTriple(triple);
        // Remove from curves vector to avoid double free
        curves[0] = curves[1] = curves[2] = NULL;
    }
    
    // Clean up remaining curves
    for (auto curve : curves) {
        if (curve) {
            cmsFreeToneCurve(curve);
        }
    }
    
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_011
