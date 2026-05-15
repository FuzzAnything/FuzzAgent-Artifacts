#include <fuzzer/FuzzedDataProvider.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"  // Needed for low-level MD5 functions

namespace harness_011 {
// Fuzzing harness for Little CMS (lcms) library
// Target: MD5 checksum module functions (cmsmd5.c module)
// Focus: cmsMD5computeID and related MD5 functions with 0% coverage and 73 undiscovered branches
// Functions: cmsMD5alloc, cmsMD5add, cmsMD5finish, cmsMD5computeID
// Semantically different from:
//   harness_000.cpp - profile parsing and basic transformations
//   harness_001.cpp - device link creation and virtual profiles  
//   harness_002.cpp - PostScript generation
//   harness_003.cpp - CGATS parsing and manipulation
//   harness_004.cpp - Cube file device link creation
//   harness_005.cpp - CIECAM02 color appearance model
//   harness_006.cpp - Matrix operations and tone curves
//   harness_007.cpp - Named color profile APIs
//   harness_008.cpp - Alpha channel handling
//   harness_009.cpp - Date/time handling
//   harness_010.cpp - Stream-based profile I/O
// New coverage: MD5 checksum computation for ICC profile identification
// API sequence: MD5 initialization, update with profile data, final computation, ID generation


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for basic operations
    // We need at least some data for profile creation and MD5 testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for different test scenarios
    // 1. Consume operation type to determine test path
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // 2. Create context for all operations
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) return 0;
    
    // Operation 0: Test cmsMD5computeID on existing profiles
    if (operation == 0) {
        // Create test profiles from remaining input data
        std::vector<uint8_t> profile_data = fdp.ConsumeRemainingBytes<uint8_t>();
        
        if (!profile_data.empty()) {
            // Try to open the profile from memory
            cmsHPROFILE hProfile = cmsOpenProfileFromMemTHR(ctx, profile_data.data(), 
                                                           (cmsUInt32Number)profile_data.size());
            
            if (hProfile != NULL) {
                // Profile successfully parsed - compute MD5 checksum
                cmsBool md5_result = cmsMD5computeID(hProfile);
                
                // Get the computed profile ID if successful
                if (md5_result) {
                    cmsUInt8Number id_buffer[16];
                    cmsGetHeaderProfileID(hProfile, id_buffer);
                    
                    // Also get other header info that might be affected
                    cmsUInt32Number header_flags = cmsGetHeaderFlags(hProfile);
                    cmsUInt32Number rendering_intent = cmsGetHeaderRenderingIntent(hProfile);
                }
                
                // Test with profile modifications
                // Change header flags and recompute MD5
                if (fdp.ConsumeBool() && fdp.remaining_bytes() >= sizeof(cmsUInt32Number)) {
                    cmsUInt32Number new_flags = fdp.ConsumeIntegral<cmsUInt32Number>();
                    cmsSetHeaderFlags(hProfile, new_flags);
                    
                    // Recompute MD5 with modified flags
                    cmsBool md5_result2 = cmsMD5computeID(hProfile);
                }
                
                cmsCloseProfile(hProfile);
            } else {
                // Profile parsing failed - create a simple sRGB profile instead
                cmsHPROFILE hSrgbProfile = cmsCreate_sRGBProfileTHR(ctx);
                if (hSrgbProfile != NULL) {
                    // Compute MD5 for sRGB profile
                    cmsBool md5_result = cmsMD5computeID(hSrgbProfile);
                    
                    // Get the computed ID
                    cmsUInt8Number id_buffer[16];
                    cmsGetHeaderProfileID(hSrgbProfile, id_buffer);
                    
                    cmsCloseProfile(hSrgbProfile);
                }
            }
        }
    }
    
    // Operation 1: Test low-level MD5 functions directly
    else if (operation == 1) {
        // Test cmsMD5alloc, cmsMD5add, cmsMD5finish directly
        cmsHANDLE md5_handle = cmsMD5alloc(ctx);
        if (md5_handle != NULL) {
            // Add some data to the MD5 context
            size_t bytes_to_add = fdp.ConsumeIntegralInRange<size_t>(0, fdp.remaining_bytes());
            if (bytes_to_add > 0) {
                std::vector<uint8_t> md5_data = fdp.ConsumeBytes<uint8_t>(bytes_to_add);
                cmsMD5add(md5_handle, md5_data.data(), (cmsUInt32Number)md5_data.size());
            }
            
            // Add more data in chunks to test update functionality
            int num_chunks = fdp.ConsumeIntegralInRange<int>(0, 10);
            for (int i = 0; i < num_chunks && fdp.remaining_bytes() > 0; i++) {
                size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, fdp.remaining_bytes());
                if (chunk_size > 0) {
                    std::vector<uint8_t> chunk_data = fdp.ConsumeBytes<uint8_t>(chunk_size);
                    cmsMD5add(md5_handle, chunk_data.data(), (cmsUInt32Number)chunk_data.size());
                }
            }
            
            // Finalize and get the MD5 hash
            cmsProfileID profile_id;
            memset(&profile_id, 0, sizeof(profile_id));
            cmsMD5finish(&profile_id, md5_handle);
            
            // profile_id.ID8 now contains the 16-byte MD5 hash
            // We could verify it's not all zeros (unless input was empty)
        }
    }
    
    // Operation 2: Test MD5 on various profile types
    else if (operation == 2) {
        // Create different types of profiles and compute their MD5 checksums
        
        // Create sRGB profile
        cmsHPROFILE hSrgb = cmsCreate_sRGBProfileTHR(ctx);
        if (hSrgb != NULL) {
            cmsBool md5_result1 = cmsMD5computeID(hSrgb);
            cmsCloseProfile(hSrgb);
        }
        
        // Create Lab profile if we have enough data for white point
        if (fdp.remaining_bytes() >= 2 * sizeof(double)) {
            cmsCIExyY white_point;
            white_point.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
            white_point.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
            white_point.Y = 1.0;
            
            cmsHPROFILE hLab = cmsCreateLab2ProfileTHR(ctx, &white_point);
            if (hLab != NULL) {
                cmsBool md5_result2 = cmsMD5computeID(hLab);
                cmsCloseProfile(hLab);
            }
        }
        
        // Create gray profile if we have enough data
        if (fdp.remaining_bytes() >= 2 * sizeof(double)) {
            cmsCIExyY gray_white;
            gray_white.x = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
            gray_white.y = fdp.ConsumeFloatingPointInRange<double>(0.1, 0.9);
            gray_white.Y = 1.0;
            
            // Create a simple gamma tone curve
            cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<double>(0.5, 3.0);
            cmsToneCurve* gamma_curve = cmsBuildGamma(ctx, gamma);
            
            if (gamma_curve != NULL) {
                cmsHPROFILE hGray = cmsCreateGrayProfileTHR(ctx, &gray_white, gamma_curve);
                if (hGray != NULL) {
                    cmsBool md5_result3 = cmsMD5computeID(hGray);
                    cmsCloseProfile(hGray);
                }
                cmsFreeToneCurve(gamma_curve);
            }
        }
        
        // Test with profile that has been modified
        if (fdp.remaining_bytes() > 0) {
            cmsHPROFILE hProfile = cmsCreate_sRGBProfileTHR(ctx);
            if (hProfile != NULL) {
                // Modify profile before computing MD5
                cmsUInt32Number original_flags = cmsGetHeaderFlags(hProfile);
                cmsSetHeaderFlags(hProfile, original_flags ^ 0x1); // Toggle a flag
                
                cmsBool md5_result = cmsMD5computeID(hProfile);
                
                // Restore and recompute
                cmsSetHeaderFlags(hProfile, original_flags);
                cmsBool md5_result2 = cmsMD5computeID(hProfile);
                
                cmsCloseProfile(hProfile);
            }
        }
    }
    
    // Operation 3: Test edge cases and error conditions (EXCLUDING NULL profile test)
    else if (operation == 3) {
        // SKIP: Test 1: NULL profile for cmsMD5computeID - removed to avoid crashes
        // cmsMD5computeID expects non-NULL profile, assertion will fail with NULL
        
        // Test 2: Empty MD5 context
        cmsHANDLE empty_md5 = cmsMD5alloc(ctx);
        if (empty_md5 != NULL) {
            cmsProfileID empty_id;
            memset(&empty_id, 0, sizeof(empty_id));
            cmsMD5finish(&empty_id, empty_md5);
            // Should compute MD5 of empty input
        }
        
        // Test 3: Large input for MD5
        if (fdp.remaining_bytes() > 100) {
            cmsHANDLE large_md5 = cmsMD5alloc(ctx);
            if (large_md5 != NULL) {
                // Add data in various sized chunks
                while (fdp.remaining_bytes() > 0) {
                    size_t chunk_size = fdp.ConsumeIntegralInRange<size_t>(1, 
                        fdp.remaining_bytes() > 1024 ? 1024 : fdp.remaining_bytes());
                    std::vector<uint8_t> chunk = fdp.ConsumeBytes<uint8_t>(chunk_size);
                    cmsMD5add(large_md5, chunk.data(), (cmsUInt32Number)chunk.size());
                }
                
                cmsProfileID large_id;
                memset(&large_id, 0, sizeof(large_id));
                cmsMD5finish(&large_id, large_md5);
            }
        }
        
        // Test 4: Profile with zero size
        if (fdp.remaining_bytes() > 0) {
            std::vector<uint8_t> zero_data = fdp.ConsumeBytes<uint8_t>(0);
            cmsHPROFILE hZeroProfile = cmsOpenProfileFromMemTHR(ctx, zero_data.data(), 0);
            // Should be NULL, but test cmsMD5computeID won't be called
        }
        
        // Test 5: Repeated MD5 computation on same profile
        cmsHPROFILE hRepeatProfile = cmsCreate_sRGBProfileTHR(ctx);
        if (hRepeatProfile != NULL) {
            cmsBool md5_first = cmsMD5computeID(hRepeatProfile);
            cmsBool md5_second = cmsMD5computeID(hRepeatProfile); // Should be idempotent
            
            // Get ID after first and second computation
            cmsUInt8Number id1[16], id2[16];
            cmsGetHeaderProfileID(hRepeatProfile, id1);
            // Actually same buffer, but conceptually same ID
            
            cmsCloseProfile(hRepeatProfile);
        }
    }
    
    // Clean up context
    cmsDeleteContext(ctx);
    
    return 0;
}

} // namespace harness_011
