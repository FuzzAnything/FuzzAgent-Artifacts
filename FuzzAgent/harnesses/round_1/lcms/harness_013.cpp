#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_013 {
// Fuzzing harness for Little CMS 2 library
// Target: Grid-Based Data (GDB) operations in cmssm.c module
// Coverage: 0% coverage with 36+ undiscovered branches
// Target APIs: cmsGDBCompute (18 branches), cmsGDBAddPoint (7 branches), 
//              cmsGDBCheckPoint (5 branches), cmsGBDAlloc (3 branches), 
//              cmsGBDFree (3 branches)
// Invocation sequence:
//   1. cmsGBDAlloc allocate grid-based data structure
//   2. cmsGDBAddPoint multiple times to populate grid with sample data points
//   3. cmsGDBCheckPoint validate added points
//   4. cmsGDBCompute perform grid-based computations
//   5. cmsGBDFree cleanup allocated structure
// Strategy: Test various grid sizes and data point configurations using fuzzed Lab color data


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for context and multiple Lab points
    // Each Lab point requires 3 doubles (24 bytes) plus overhead
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context for GBD allocation
    // Use NULL context (default) or create one if needed
    cmsContext context = NULL;
    
    // Consume some data to decide whether to use default context
    bool use_default_context = fdp.ConsumeBool();
    if (!use_default_context) {
        // Could create a context here, but for fuzzing we'll use NULL
        context = NULL;
    }
    
    // Step 2: Allocate GBD structure using cmsGBDAlloc
    cmsHANDLE hGBD = cmsGBDAlloc(context);
    if (hGBD == NULL) {
        // If allocation fails, return early
        return 0;
    }
    
    // Step 3: Add multiple points to the grid using cmsGDBAddPoint
    // Determine number of points to add (1-50 for reasonable fuzzing)
    size_t num_points = fdp.ConsumeIntegralInRange<size_t>(1, 50);
    
    // Track successful additions for later checking
    std::vector<cmsCIELab> added_points;
    
    for (size_t i = 0; i < num_points && fdp.remaining_bytes() >= 24; i++) {
        cmsCIELab lab_point;
        
        // Consume Lab values from fuzzer input
        // L* ranges typically 0-100, a* and b* can be negative/positive
        lab_point.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        lab_point.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        lab_point.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        
        // Add the point to the GBD
        cmsBool add_result = cmsGDBAddPoint(hGBD, &lab_point);
        
        if (add_result) {
            // Store successful additions for checking later
            added_points.push_back(lab_point);
        }
        
        // Also test with some edge cases
        if (i % 5 == 0) {
            // Test with extreme values
            cmsCIELab extreme_lab;
            extreme_lab.L = fdp.ConsumeFloatingPoint<double>();
            extreme_lab.a = fdp.ConsumeFloatingPoint<double>();
            extreme_lab.b = fdp.ConsumeFloatingPoint<double>();
            
            cmsGDBAddPoint(hGBD, &extreme_lab);
        }
    }
    
    // Step 4: Check some of the added points using cmsGDBCheckPoint
    // Check a subset of successfully added points
    for (size_t i = 0; i < added_points.size() && i < 10; i++) {
        cmsBool check_result = cmsGDBCheckPoint(hGBD, &added_points[i]);
        // Result can be TRUE or FALSE depending on point validity
        (void)check_result; // Use result to avoid unused variable warning
    }
    
    // Also check some random Lab points from fuzzer input
    size_t random_checks = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < random_checks && fdp.remaining_bytes() >= 24; i++) {
        cmsCIELab random_lab;
        random_lab.L = fdp.ConsumeFloatingPoint<double>();
        random_lab.a = fdp.ConsumeFloatingPoint<double>();
        random_lab.b = fdp.ConsumeFloatingPoint<double>();
        
        cmsGDBCheckPoint(hGBD, &random_lab);
    }
    
    // Step 5: Perform grid computations using cmsGDBCompute
    // Consume flags parameter from fuzzer input
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // Call compute with the flags
    cmsBool compute_result = cmsGDBCompute(hGBD, flags);
    (void)compute_result; // Use result to avoid unused variable warning
    
    // Try compute again with different flag combinations
    if (fdp.remaining_bytes() >= 4) {
        cmsUInt32Number alt_flags = fdp.ConsumeIntegral<cmsUInt32Number>();
        cmsGDBCompute(hGBD, alt_flags);
    }
    
    // Step 6: Test adding more points after computation
    // This tests if GBD structure can accept more points after compute
    if (fdp.remaining_bytes() >= 24) {
        cmsCIELab post_compute_lab;
        post_compute_lab.L = fdp.ConsumeFloatingPoint<double>();
        post_compute_lab.a = fdp.ConsumeFloatingPoint<double>();
        post_compute_lab.b = fdp.ConsumeFloatingPoint<double>();
        
        cmsGDBAddPoint(hGBD, &post_compute_lab);
        
        // Check the newly added point
        cmsGDBCheckPoint(hGBD, &post_compute_lab);
        
        // Compute again with the new point
        cmsGDBCompute(hGBD, 0);
    }
    
    // Step 7: Test with Lab values that might trigger edge cases
    // Zero values
    if (fdp.remaining_bytes() >= 1) {
        cmsCIELab zero_lab = {0.0, 0.0, 0.0};
        cmsGDBAddPoint(hGBD, &zero_lab);
        cmsGDBCheckPoint(hGBD, &zero_lab);
    }
    
    // NaN/Infinity handling (though API expects doubles)
    if (fdp.remaining_bytes() >= 1) {
        // Use some special floating point values
        double special_val = fdp.ConsumeFloatingPoint<double>();
        cmsCIELab special_lab = {special_val, special_val, special_val};
        cmsGDBAddPoint(hGBD, &special_lab);
    }
    
    // Step 8: Clean up allocated structure using cmsGBDFree
    cmsGBDFree(hGBD);
    
    return 0;
}

} // namespace harness_013
