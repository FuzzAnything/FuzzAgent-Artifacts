#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cmath>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_014 {
// Fuzzing harness for LittleCMS (lcms) Gamut Boundary Description (GBD) functions
// This harness specifically targets GBD functions identified as having 0% coverage in cmssm.c
// Target APIs: cmsGBDAlloc, cmsGDBAddPoint, cmsGDBCheckPoint, cmsGDBCompute, cmsGBDFree
// MODULE: cmssm.c - Gamut Boundary Description functions
// STRATEGY: Complete GBD workflow from allocation through point addition, checking, computation, to cleanup
// DIFFERENTIATION: Unlike existing harnesses, focus exclusively on GBD functions which have 0% coverage
// API SEQUENCE: Create context -> Allocate GBD -> Add multiple Lab points -> Check points -> Compute GBD -> Cleanup


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need sufficient data for meaningful GBD testing
    // Minimum: context, multiple Lab points (each Lab is 3 doubles = 24 bytes), operation flags
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // 1. Create a context for GBD operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        return 0; // Cannot proceed without context
    }
    
    // 2. Allocate GBD structure
    cmsHANDLE gbd_handle = cmsGBDAlloc(ctx);
    if (gbd_handle == NULL) {
        cmsDeleteContext(ctx);
        return 0; // Cannot proceed without GBD
    }
    
    // 3. Determine number of points to add (based on available input)
    // Each Lab point requires 3 doubles (L, a, b) = 24 bytes
    size_t max_points = fdp.remaining_bytes() / (3 * sizeof(double));
    if (max_points > 50) max_points = 50; // Cap for performance
    if (max_points < 1) max_points = 1;
    
    size_t num_points_to_add = fdp.ConsumeIntegralInRange<size_t>(1, max_points);
    
    // Vector to store points for later checking
    std::vector<cmsCIELab> added_points;
    
    // 4. Add multiple Lab points to GBD
    for (size_t i = 0; i < num_points_to_add; i++) {
        if (fdp.remaining_bytes() < 3 * sizeof(double)) {
            break; // Not enough data for another Lab point
        }
        
        cmsCIELab lab;
        // L* typically ranges 0-100, but GDB is centered at L=50
        // Allow wider range for testing edge cases
        lab.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        lab.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        lab.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        
        // Add point to GBD
        cmsBool add_result = cmsGDBAddPoint(gbd_handle, &lab);
        // Don't fail on add failure - some points may be invalid for the GBD structure
        
        // Store successful points for checking later
        if (add_result) {
            added_points.push_back(lab);
        }
    }
    
    // 5. Check some of the added points
    // Check random subset of added points
    if (!added_points.empty()) {
        size_t num_points_to_check = fdp.ConsumeIntegralInRange<size_t>(0, added_points.size());
        
        for (size_t i = 0; i < num_points_to_check && i < added_points.size(); i++) {
            // Use fuzzer to decide which point to check
            size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, added_points.size() - 1);
            cmsBool check_result = cmsGDBCheckPoint(gbd_handle, &added_points[idx]);
            // Result depends on whether point is inside gamut boundary
            // Don't fail on check result - it's a valid return
        }
    }
    
    // 6. Also check some random Lab points not necessarily added
    size_t num_random_checks = fdp.ConsumeIntegralInRange<size_t>(0, 10);
    for (size_t i = 0; i < num_random_checks; i++) {
        if (fdp.remaining_bytes() < 3 * sizeof(double)) {
            break;
        }
        
        cmsCIELab random_lab;
        random_lab.L = fdp.ConsumeFloatingPointInRange<double>(0.0, 100.0);
        random_lab.a = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        random_lab.b = fdp.ConsumeFloatingPointInRange<double>(-128.0, 128.0);
        
        cmsBool check_result = cmsGDBCheckPoint(gbd_handle, &random_lab);
        // Don't fail on check result
    }
    
    // 7. Compute GBD if we have enough points
    // Determine computation flags from fuzzer input
    cmsUInt32Number flags = 0;
    if (fdp.ConsumeBool()) {
        flags |= 0x01; // Use some flag value (actual flags depend on implementation)
    }
    if (fdp.ConsumeBool()) {
        flags |= 0x02;
    }
    
    // Only attempt computation if we have some points added
    if (!added_points.empty()) {
        cmsBool compute_result = cmsGDBCompute(gbd_handle, flags);
        // Don't fail on compute result - it may succeed or fail based on point configuration
    }
    
    // 8. Clean up - free GBD and context
    cmsGBDFree(gbd_handle);
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_014
