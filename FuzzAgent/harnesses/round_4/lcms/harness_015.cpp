#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <algorithm>

namespace harness_015 {
/*
 * Harness 015 - Gamut Boundary Description (GDB) Operations for Little CMS 2
 * 
 * Target APIs:
 * - cmsGDBCompute (Target: 18 undiscovered branches)
 * - cmsGDBAddPoint (Target: 7 undiscovered branches)
 * - cmsGDBCheckPoint (Target: 5 undiscovered branches)
 * - cmsGBDAlloc (Required Helper: Initialization)
 * - cmsGBDFree (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Call cmsCreateContext to create a context
 * 2. Call cmsGBDAlloc to allocate a GDB structure
 * 3. Call cmsGDBAddPoint multiple times with different color points (L*a*b* coordinates)
 * 4. Call cmsGDBCompute to compute the gamut boundary from added points
 * 5. Call cmsGDBCheckPoint to test if specific points are within the computed gamut
 * 6. Call cmsGBDFree to clean up the GDB structure
 * 7. Call cmsDeleteContext to clean up context
 * 
 * These functions represent 40+ points of undiscovered complexity in the cmssm.c module
 * (currently at 0% file coverage). They form a logical lifecycle for gamut boundary testing
 * (Alloc → AddPoint → Compute → CheckPoint → Free).
 * 
 * Existing harnesses focus on profile I/O, tone curves, dictionaries, MD5, pipelines, and IT8,
 * but lack dedicated gamut boundary description testing, making this a semantically diverse target.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for GDB operations
    // We need: context flag (1 byte), number of points to add (1 byte), 
    // number of points to check (1 byte), and enough data for L*a*b* coordinates
    // Each L*a*b* point requires 3 doubles = 24 bytes
    const size_t MIN_POINTS_TO_ADD = 2;  // Need at least some points to compute gamut
    const size_t MIN_POINTS_TO_CHECK = 1;
    const size_t MIN_SIZE = sizeof(uint8_t) * 3 + 
                           (MIN_POINTS_TO_ADD + MIN_POINTS_TO_CHECK) * sizeof(double) * 3;
    
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t use_context = fdp.ConsumeBool();  // Whether to use thread-safe context
    uint8_t num_points_to_add = fdp.ConsumeIntegralInRange<uint8_t>(2, 16);  // 2-16 points
    uint8_t num_points_to_check = fdp.ConsumeIntegralInRange<uint8_t>(1, 8);  // 1-8 points
    
    // Create context if requested
    cmsContext context = use_context ? cmsCreateContext(NULL, NULL) : NULL;
    
    // 1. Allocate GDB structure
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    if (hGBD == NULL) {
        if (context) cmsDeleteContext(context);
        return 0;
    }
    
    // 2. Add points to the GDB
    std::vector<cmsCIELab> added_points;
    added_points.reserve(num_points_to_add);
    
    bool add_success = true;
    for (int i = 0; i < num_points_to_add && fdp.remaining_bytes() >= sizeof(double) * 3; i++) {
        cmsCIELab lab;
        lab.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);      // L*: 0-100
        lab.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);   // a*: -128 to 128
        lab.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);   // b*: -128 to 128
        
        added_points.push_back(lab);
        
        // Add the point to GDB
        if (!cmsGDBAddPoint(hGBD, &lab)) {
            add_success = false;
            // Continue anyway to test error paths
        }
    }
    
    // If we couldn't add enough points, clean up and return
    if (added_points.size() < 2) {
        cmsGBDFree(hGBD);
        if (context) cmsDeleteContext(context);
        return 0;
    }
    
    // 3. Compute the gamut boundary
    // Use different flag values to explore different code paths
    uint32_t compute_flags = fdp.ConsumeIntegral<uint32_t>();
    bool compute_success = cmsGDBCompute(hGBD, compute_flags);
    (void)compute_success;  // Result may be false, which is valid for fuzzing
    
    // 4. Check points against the computed gamut
    // Check some of the points we added (should be within gamut if computation succeeded)
    for (int i = 0; i < std::min(num_points_to_check, (uint8_t)added_points.size()); i++) {
        bool is_within = cmsGDBCheckPoint(hGBD, &added_points[i]);
        (void)is_within;  // Result may be true/false, both valid for fuzzing
    }
    
    // Also check some random points from fuzzer input
    for (int i = 0; i < num_points_to_check && fdp.remaining_bytes() >= sizeof(double) * 3; i++) {
        cmsCIELab random_lab;
        random_lab.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        random_lab.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        random_lab.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        
        bool is_within = cmsGDBCheckPoint(hGBD, &random_lab);
        (void)is_within;
    }
    
    // 5. Cleanup
    cmsGBDFree(hGBD);
    if (context) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_015
