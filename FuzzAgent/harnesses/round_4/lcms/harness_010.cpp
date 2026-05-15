#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2_plugin.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

namespace harness_010 {
/*
 * Harness 010 - MD5 Computation Fuzzer for Little CMS 2
 * 
 * Target APIs:
 * - cmsMD5computeID (59 undiscovered branches, 0% coverage)
 * - cmsMD5add (8 undiscovered branches)
 * - cmsMD5alloc (Required Helper: Allocation) - 3 undiscovered branches
 * - cmsMD5finish (Required Helper: Cleanup) - 3 undiscovered branches
 * 
 * Helper APIs:
 * - cmsOpenProfileFromMem (Required Helper: Profile source)
 * - cmsCloseProfile (Required Helper: Cleanup)
 * 
 * Invocation sequence:
 * 1. Call cmsMD5alloc to create MD5 context
 * 2. Call cmsOpenProfileFromMem to get a profile handle (or create dummy data)
 * 3. Call cmsMD5add repeatedly with different data chunks
 * 4. Call cmsMD5computeID to compute MD5 hash for profile
 * 5. Call cmsMD5finish to finalize and get the hash result
 * 6. Call cmsCloseProfile to clean up profile resources
 * 7. Call appropriate free functions if needed
 * 
 * This harness specifically targets the MD5 computation subsystem
 * which has complete zero coverage across all metrics.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    if (size < 128) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint32_t num_chunks = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);
    uint32_t chunk_size_policy = fdp.ConsumeIntegral<uint32_t>() % 4;
    
    // Consume profile data (first part of input)
    size_t profile_data_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (profile_data_size > fdp.remaining_bytes()) {
        profile_data_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_data_size);
    
    // Test 1: Open profile from memory
    cmsHPROFILE hProfile = cmsOpenProfileFromMem(profile_data.data(), 
                                                 static_cast<cmsUInt32Number>(profile_data.size()));
    if (hProfile == NULL) {
        // If we can't open a profile, we can still test MD5 functions with raw data
        // Create a simple in-memory profile for testing
        cmsHPROFILE hSimpleProfile = cmsCreate_sRGBProfile();
        if (hSimpleProfile == NULL) {
            return 0;
        }
        hProfile = hSimpleProfile;
    }
    
    // Test 2: Allocate MD5 context
    cmsHANDLE hMD5 = cmsMD5alloc(NULL);  // Use default context
    if (hMD5 == NULL) {
        cmsCloseProfile(hProfile);
        return 0;
    }
    
    // Test 3: Add data chunks to MD5
    for (uint32_t i = 0; i < num_chunks && fdp.remaining_bytes() > 0; i++) {
        // Determine chunk size based on policy
        size_t chunk_size = 0;
        switch (chunk_size_policy) {
            case 0:
                chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 64);
                break;
            case 1:
                chunk_size = fdp.ConsumeIntegralInRange<size_t>(65, 512);
                break;
            case 2:
                chunk_size = fdp.ConsumeIntegralInRange<size_t>(513, 2048);
                break;
            case 3:
                // Variable size based on remaining bytes
                chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                break;
        }
        
        if (chunk_size > fdp.remaining_bytes()) {
            chunk_size = fdp.remaining_bytes();
        }
        
        if (chunk_size > 0) {
            std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_size);
            cmsMD5add(hMD5, chunk_data.data(), static_cast<cmsUInt32Number>(chunk_size));
        }
    }
    
    // Test 4: Compute MD5 ID for the profile
    // First, try to compute MD5 for the opened profile
    cmsBool compute_result = cmsMD5computeID(hProfile);
    
    // Test 5: Finish MD5 computation and get the hash
    cmsProfileID profile_id;
    memset(&profile_id, 0, sizeof(profile_id));
    cmsMD5finish(&profile_id, hMD5);
    
    // Test 6: Get profile ID from header (if compute was successful)
    if (compute_result) {
        cmsProfileID header_id;
        cmsGetHeaderProfileID(hProfile, header_id.ID8);
    }
    
    // Test 7: Clean up resources
    cmsCloseProfile(hProfile);
    // Note: cmsMD5finish should handle freeing the MD5 context
    
    return 0;
}

} // namespace harness_010
