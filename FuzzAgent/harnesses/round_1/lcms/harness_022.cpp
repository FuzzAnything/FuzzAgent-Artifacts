#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include "lcms2_plugin.h"

namespace harness_022 {
// Fuzzing harness for Little CMS 2 library
// Target: Intent discovery and querying functions
// Strategy: Target cmsGetSupportedIntentsTHR and cmsGetSupportedIntents (both 0% coverage, 17 undiscovered branches each)
//           with 34 undiscovered branches total. These are fundamental APIs for querying available rendering intents.
// Target APIs:
//   - cmsGetSupportedIntentsTHR (Primary Target - 17 undiscovered branches)
//   - cmsGetSupportedIntents (Primary Target - 17 undiscovered branches)
// Required Helper APIs:
//   - cmsCreateContext (Required Helper: Context initialization for thread-safe operations)
//   - cmsDeleteContext (Required Helper: Resource cleanup)
// Invocation sequence:
//   1. Call cmsCreateContext to establish context for thread-safe operations
//   2. Call cmsGetSupportedIntentsTHR with created context to query available intents
//   3. Call cmsGetSupportedIntents (wrapper function) without context for backward compatibility
//   4. Call cmsDeleteContext to clean up resources
// Testing strategy:
//   - Test both functions with NULL parameters to get intent count (as per API documentation)
//   - Test with various nMax values to exercise different code paths
//   - Test retrieval of actual intent codes and descriptions
//   - Test edge cases: nMax = 0, nMax < actual count, nMax > actual count
//   - Ensure proper memory management for returned description strings
// Error handling scenarios included:
//   - Handling NULL context creation
//   - Testing with insufficient buffer sizes
//   - Proper cleanup of allocated arrays
// Note: Uses FuzzedDataProvider for input-driven testing of nMax values and operation selection
// Semantic diversity from harnesses 000-021: This harness focuses exclusively on intent discovery
// functions, which are not covered in any previous harnesses. Previous harnesses target:
//   - Profile operations (000-004, 006-009, 011-014, 016-020)
//   - Context management (021 - focuses on duplication, not intent discovery)
//   - Color transforms (005, 015)
//   - DeltaE calculations (007)
//   - IT8 operations (008)
//   - Dictionary operations (010)
//   - Plugin operations (017)
//   - Error logging (018)
// Intent discovery is a unique functional area not covered elsewhere.


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for operation selection and nMax values
    // We need: operation type + nMax value + cleanup flag
    const size_t MIN_SIZE = sizeof(uint8_t) + sizeof(uint32_t) + 1;
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Determine operation mode from fuzzer input
    // 0: Test with NULL parameters only (get count)
    // 1: Test with small nMax (less than actual count)
    // 2: Test with exact nMax (equal to actual count)
    // 3: Test with large nMax (greater than actual count)
    // 4: Test with nMax = 0
    uint8_t operation_mode = fdp.ConsumeIntegral<uint8_t>() % 5;
    
    // Step 2: Create context for thread-safe operations
    // Use NULL plugin and NULL user data for simplest case
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        // Context creation failed, can't proceed with thread-safe version
        // But we can still test the non-thread-safe version
        // However, for consistency with the intended sequence, we'll return early
        return 0;
    }
    
    // Step 3: First, get intent count using NULL parameters (as per API documentation)
    // This tests the basic "get count" functionality of both APIs
    cmsUInt32Number intent_count_thr = cmsGetSupportedIntentsTHR(ctx, 0, NULL, NULL);
    cmsUInt32Number intent_count = cmsGetSupportedIntents(0, NULL, NULL);
    
    // Both counts should be the same (thread-safe and non-thread-safe versions)
    // We'll use the thread-safe count for buffer allocation
    
    // Step 4: Determine nMax value based on operation mode
    cmsUInt32Number nMax = 0;
    
    switch (operation_mode) {
        case 0: // NULL parameters only (already handled above)
            // We've already tested with NULL parameters, now clean up and exit
            cmsDeleteContext(ctx);
            return 0;
            
        case 1: // Small nMax (less than actual count)
            if (intent_count_thr > 1) {
                nMax = fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, intent_count_thr - 1);
            } else {
                nMax = 1; // If only 0 or 1 intents, can't test "less than"
            }
            break;
            
        case 2: // Exact nMax (equal to actual count)
            nMax = intent_count_thr;
            break;
            
        case 3: // Large nMax (greater than actual count)
            nMax = intent_count_thr + fdp.ConsumeIntegralInRange<cmsUInt32Number>(1, 10);
            break;
            
        case 4: // nMax = 0
            nMax = 0;
            break;
    }
    
    // Step 5: Allocate buffers for intent codes and descriptions
    cmsUInt32Number* intent_codes = NULL;
    char** intent_descriptions = NULL;
    
    if (nMax > 0) {
        intent_codes = (cmsUInt32Number*)malloc(nMax * sizeof(cmsUInt32Number));
        intent_descriptions = (char**)malloc(nMax * sizeof(char*));
        
        if (intent_codes == NULL || intent_descriptions == NULL) {
            // Allocation failed, clean up and exit
            free(intent_codes);
            free(intent_descriptions);
            cmsDeleteContext(ctx);
            return 0;
        }
        
        // Initialize description pointers to NULL
        for (cmsUInt32Number i = 0; i < nMax; i++) {
            intent_descriptions[i] = NULL;
        }
    }
    
    // Step 6: Test thread-safe version (cmsGetSupportedIntentsTHR)
    cmsUInt32Number retrieved_count_thr = cmsGetSupportedIntentsTHR(ctx, nMax, intent_codes, intent_descriptions);
    
    // Step 7: Process retrieved data from thread-safe version
    // Note: intent_descriptions[i] points to static library strings (DefaultIntents)
    // Do NOT free these strings - they are static library data
    // The harness should only free the intent_descriptions array itself (allocated at line 118)
    
    // Step 8: Reinitialize buffers for non-thread-safe version test
    if (nMax > 0) {
        // Reset codes array (optional, but clean)
        memset(intent_codes, 0, nMax * sizeof(cmsUInt32Number));
        
        // Reinitialize description pointers to NULL
        for (cmsUInt32Number i = 0; i < nMax; i++) {
            intent_descriptions[i] = NULL;
        }
    }
    
    // Step 9: Test non-thread-safe version (cmsGetSupportedIntents)
    cmsUInt32Number retrieved_count = cmsGetSupportedIntents(nMax, intent_codes, intent_descriptions);
    
    // Step 10: Process retrieved data from non-thread-safe version
    // Note: intent_descriptions[i] points to static library strings (DefaultIntents)
    // Do NOT free these strings - they are static library data
    // The harness should only free the intent_descriptions array itself (allocated at line 118)
    // Step 11: Additional test - exercise the functions with remaining fuzzer input
    // Use different nMax values from remaining fuzzer data
    while (fdp.remaining_bytes() >= sizeof(cmsUInt32Number)) {
        cmsUInt32Number extra_nMax = fdp.ConsumeIntegral<cmsUInt32Number>();
        
        // Limit nMax to a reasonable value to avoid excessive allocation
        if (extra_nMax > 100) {
            extra_nMax = 100;
        }
        
        // Test with this nMax value (just get count, don't allocate buffers)
        (void)cmsGetSupportedIntentsTHR(ctx, extra_nMax, NULL, NULL);
        (void)cmsGetSupportedIntents(extra_nMax, NULL, NULL);
        
        // If we have enough remaining bytes, test with actual buffers
        if (fdp.remaining_bytes() >= extra_nMax * (sizeof(cmsUInt32Number) + sizeof(char*))) {
            cmsUInt32Number* extra_codes = (cmsUInt32Number*)malloc(extra_nMax * sizeof(cmsUInt32Number));
            char** extra_descriptions = (char**)malloc(extra_nMax * sizeof(char*));
            
            if (extra_codes && extra_descriptions) {
                for (cmsUInt32Number i = 0; i < extra_nMax; i++) {
                    extra_descriptions[i] = NULL;
                }
                
                cmsUInt32Number extra_retrieved_thr = cmsGetSupportedIntentsTHR(ctx, extra_nMax, extra_codes, extra_descriptions);
                
                // Note: extra_descriptions[i] points to static library strings (DefaultIntents)
                // Do NOT free these strings - they are static library data
                // The harness should only free the extra_descriptions array itself (allocated at line 177)
                
                free(extra_codes);
                free(extra_descriptions);
            }
        }
        
        // Break if we're running low on input
        if (fdp.remaining_bytes() < sizeof(cmsUInt32Number)) {
            break;
        }
    }
    
    // Step 12: Clean up
    if (intent_codes != NULL) {
        free(intent_codes);
    }
    if (intent_descriptions != NULL) {
        free(intent_descriptions);
    }
    
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_022
