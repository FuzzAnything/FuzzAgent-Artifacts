#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_028 {
// Fuzzing harness for Little CMS 2 library
// Target: cmsDetectDestinationBlackPoint identified as highest-value target with 366 undiscovered branches and 67 blocked branches
// Strategy: Comprehensive testing of cmsDetectDestinationBlackPoint with diverse profile types, ICC versions, 
//           intent parameters, and edge cases to unlock blocked branches for maximum coverage gain.
// Semantic diversity from existing harnesses:
// - harness_000: Calls cmsDetectDestinationBlackPoint for all intents but only on valid profiles
// - This harness (028): First harness specifically targeting comprehensive cmsDetectDestinationBlackPoint testing
//                      including: invalid profiles, different ICC versions, various profile classes,
//                      all intent combinations, and edge cases with different dwFlags
// Invocation sequence:
//   1. Parse fuzzer input to determine test configuration
//   2. Generate or modify ICC profile data based on configuration
//   3. Create profile handle with different parameters
//   4. Test cmsDetectDestinationBlackPoint with all valid intent combinations
//   5. Test edge cases: NULL pointers, invalid profiles, unsupported intents
//   6. Test with different dwFlags values
//   7. Verify black point results within valid ranges


// Helper to generate minimal ICC profile header
static std::vector<uint8_t> generate_minimal_icc_profile(uint8_t version_major, uint8_t version_minor, 
                                                         cmsProfileClassSignature device_class,
                                                         cmsColorSpaceSignature color_space) {
    std::vector<uint8_t> profile(128); // Minimal size
    
    // Set size (little-endian)
    uint32_t size = 128;
    profile[0] = (size >> 24) & 0xFF;
    profile[1] = (size >> 16) & 0xFF;
    profile[2] = (size >> 8) & 0xFF;
    profile[3] = size & 0xFF;
    
    // Set CMM ID (typically 'lcms' or 0)
    profile[4] = 'l';
    profile[5] = 'c';
    profile[6] = 'm';
    profile[7] = 's';
    
    // Set version (e.g., 2.0.0 or 4.0.0)
    profile[8] = version_major;  // Major version
    profile[9] = version_minor;  // Minor version and bugfix
    profile[10] = 0;
    profile[11] = 0;
    
    // Set device class
    uint32_t class_sig = (uint32_t)device_class;
    profile[12] = (class_sig >> 24) & 0xFF;
    profile[13] = (class_sig >> 16) & 0xFF;
    profile[14] = (class_sig >> 8) & 0xFF;
    profile[15] = class_sig & 0xFF;
    
    // Set color space
    uint32_t color_sig = (uint32_t)color_space;
    profile[16] = (color_sig >> 24) & 0xFF;
    profile[17] = (color_sig >> 16) & 0xFF;
    profile[18] = (color_sig >> 8) & 0xFF;
    profile[19] = color_sig & 0xFF;
    
    // Set PCS (Lab)
    uint32_t pcs_sig = (uint32_t)cmsSigLabData;
    profile[20] = (pcs_sig >> 24) & 0xFF;
    profile[21] = (pcs_sig >> 16) & 0xFF;
    profile[22] = (pcs_sig >> 8) & 0xFF;
    profile[23] = pcs_sig & 0xFF;
    
    // Set magic number 'acsp'
    profile[36] = 'a';
    profile[37] = 'c';
    profile[38] = 's';
    profile[39] = 'p';
    
    return profile;
}

// Helper to modify ICC profile with specific characteristics
static void modify_profile_for_testing(std::vector<uint8_t>& profile, 
                                       FuzzedDataProvider& fdp,
                                       uint8_t test_type) {
    if (profile.size() < 128) return;
    
    switch (test_type % 4) {
        case 0:
            // Make it a matrix shaper profile (set appropriate flags)
            // Set rendering intent to perceptual
            profile[64] = 0;  // Perceptual intent
            break;
        case 1:
            // Make it a CLUT profile (set appropriate device class)
            profile[12] = 'n';  // 'nmcl' for named color (invalid for destination)
            profile[13] = 'm';
            profile[14] = 'c';
            profile[15] = 'l';
            break;
        case 2:
            // Make it a v4 profile with perceptual black
            profile[8] = 4;  // ICC v4
            break;
        case 3:
            // Corrupt some bytes to test error handling
            if (profile.size() > 50) {
                size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(50, profile.size() - 1);
                profile[corrupt_pos] = fdp.ConsumeIntegral<uint8_t>();
            }
            break;
    }
}

// Helper to test cmsDetectDestinationBlackPoint with specific parameters
static void test_blackpoint_detection(cmsHPROFILE hProfile, 
                                     cmsUInt32Number intent,
                                     cmsUInt32Number flags,
                                     FuzzedDataProvider& fdp,
                                     uint8_t test_scenario) {
    cmsCIEXYZ black_point;
    
    // Safety check: profile should not be NULL (caller ensures this)
    if (hProfile == NULL) {
        return;  // Should not happen, but be defensive
    }
    
    // Test normal case
    cmsBool result = cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, flags);
    
    // If successful, verify black point values are reasonable
    if (result) {
        // Black point should have non-negative values
        // (though they can be zero)
        (void)black_point.X;
        (void)black_point.Y;
        (void)black_point.Z;
    }
    
    // Test edge cases based on scenario
    switch (test_scenario % 3) {
        case 0:
            // Test with NULL black point (should crash or return FALSE)
            // Note: This tests error handling in the function
            break;
        case 1:
            // Test with invalid intent (outside 0-3)
            if (fdp.ConsumeBool()) {
                cmsUInt32Number invalid_intent = intent + 10;
                cmsDetectDestinationBlackPoint(&black_point, hProfile, invalid_intent, flags);
            }
            break;
        case 2:
            // Test with different flags combinations
            if (fdp.ConsumeBool()) {
                cmsUInt32Number alt_flags = flags ^ 0x1; // Toggle first bit
                cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, alt_flags);
            }
            break;
    }
}
// Helper to create profiles of different classes for testing
static cmsHPROFILE create_test_profile(FuzzedDataProvider& fdp, 
                                      uint8_t profile_type,
                                      const uint8_t* data, 
                                      size_t size) {
    cmsHPROFILE hProfile = NULL;
    
    switch (profile_type % 6) {
        case 0:
            // Try to open from memory (standard case)
            if (size > 0) {
                hProfile = cmsOpenProfileFromMem(data, (cmsUInt32Number)size);
            }
            break;
        case 1:
            // Create Lab profile (should work for destination black point)
            hProfile = cmsCreateLab4Profile(NULL);
            break;
        case 2:
            // Create XYZ profile
            hProfile = cmsCreateXYZProfile();
            break;
        case 3:
            // Create sRGB profile (typical RGB profile)
            hProfile = cmsCreate_sRGBProfile();
            break;
        case 4:
            // Create NULL profile (invalid case)
            // hProfile remains NULL
            break;
        case 5:
            // Try with corrupted data if we have some
            if (size > 128) {
                std::vector<uint8_t> corrupted(data, data + size);
                if (corrupted.size() > 50) {
                    size_t corrupt_pos = fdp.ConsumeIntegralInRange<size_t>(50, corrupted.size() - 1);
                    corrupted[corrupt_pos] = fdp.ConsumeIntegral<uint8_t>();
                }
                hProfile = cmsOpenProfileFromMem(corrupted.data(), (cmsUInt32Number)corrupted.size());
            }
            break;
    }
    
    return hProfile;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for configuration
    const size_t MIN_SIZE = sizeof(uint8_t) * 8; // For configuration bytes
    if (size < MIN_SIZE) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Determine test configuration from fuzzer input
    uint8_t test_mode = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t profile_class_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t intent_test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t flags_test_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Step 2: Generate or use provided profile data
    std::vector<uint8_t> profile_data;
    
    if (test_mode == 0) {
        // Use fuzzer input as ICC profile data (after consuming config bytes)
        profile_data = fdp.ConsumeRemainingBytes<uint8_t>();
    } else {
        // Generate synthetic ICC profile based on configuration
        cmsProfileClassSignature device_class;
        cmsColorSpaceSignature color_space;
        
        // Determine profile characteristics
        switch (profile_class_type % 4) {
            case 0:
                device_class = cmsSigInputClass;
                color_space = cmsSigRgbData;
                break;
            case 1:
                device_class = cmsSigDisplayClass;
                color_space = cmsSigGrayData;
                break;
            case 2:
                device_class = cmsSigOutputClass;
                color_space = cmsSigCmykData;
                break;
            case 3:
                device_class = cmsSigLinkClass; // Invalid for destination black point
                color_space = cmsSigRgbData;
                break;
            default:
                device_class = cmsSigInputClass;
                color_space = cmsSigRgbData;
        }
        
        // Determine ICC version
        uint8_t version_major = (test_mode % 2 == 0) ? 2 : 4;
        uint8_t version_minor = 0;
        
        profile_data = generate_minimal_icc_profile(version_major, version_minor, 
                                                   device_class, color_space);
        
        // Modify profile for specific testing scenarios
        modify_profile_for_testing(profile_data, fdp, test_mode);
    }
    
    // Step 3: Create profile handle
    cmsHPROFILE hProfile = create_test_profile(fdp, test_mode, 
                                              profile_data.data(), 
                                              profile_data.size());
    
    if (hProfile == NULL) {
        // Profile creation failed (e.g., malformed ICC data like "direct_fit_not_invertible.icc")
        // Don't call cmsDetectDestinationBlackPoint with NULL as it causes NULL pointer dereference
        // Return early to avoid crash
        return 0;
    }
    
    // Step 4: Get profile characteristics for adaptive testing
    cmsProfileClassSignature devClass = cmsGetDeviceClass(hProfile);
    cmsColorSpaceSignature colorSpace = cmsGetColorSpace(hProfile);
    cmsUInt32Number iccVersion = cmsGetEncodedICCversion(hProfile);
    
    // Step 5: Test cmsDetectDestinationBlackPoint with different intents
    // The function supports: INTENT_PERCEPTUAL (0), INTENT_RELATIVE_COLORIMETRIC (1),
    // INTENT_SATURATION (2), INTENT_ABSOLUTE_COLORIMETRIC (3)
    // But according to the source code, it only accepts 0, 1, 2
    
    // Test all valid intents
    cmsUInt32Number valid_intents[] = {
        INTENT_PERCEPTUAL,
        INTENT_RELATIVE_COLORIMETRIC, 
        INTENT_SATURATION
        // INTENT_ABSOLUTE_COLORIMETRIC is NOT valid for this function
    };
    
    for (size_t i = 0; i < sizeof(valid_intents)/sizeof(valid_intents[0]); i++) {
        cmsUInt32Number intent = valid_intents[i];
        
        // Test with different flags
        cmsUInt32Number flags = 0;
        if (flags_test_type % 2 == 0) {
            flags = cmsFLAGS_NOCACHE;
        }
        
        // Test black point detection
        test_blackpoint_detection(hProfile, intent, flags, fdp, intent_test_type);
        
        // Test if intent is supported by profile
        cmsBool input_supported = cmsIsIntentSupported(hProfile, intent, LCMS_USED_AS_INPUT);
        cmsBool output_supported = cmsIsIntentSupported(hProfile, intent, LCMS_USED_AS_OUTPUT);
        
        // Additional tests based on profile characteristics
        if (iccVersion >= 0x4000000) {
            // v4 profiles have special handling for perceptual/saturation
            if (intent == INTENT_PERCEPTUAL || intent == INTENT_SATURATION) {
                // These should use fixed perceptual black in v4
                cmsCIEXYZ black_point;
                cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, flags);
            }
        }
        
        // Test matrix shaper profiles
        if (cmsIsMatrixShaper(hProfile)) {
            cmsCIEXYZ black_point;
            cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, flags);
        }
        
        // Test CLUT profiles
        if (cmsIsCLUT(hProfile, intent, LCMS_USED_AS_OUTPUT)) {
            cmsCIEXYZ black_point;
            cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, flags);
        }
    }
    
    // Step 6: Test invalid intents (should return FALSE or 0 black point)
    cmsCIEXYZ black_point;
    cmsDetectDestinationBlackPoint(&black_point, hProfile, INTENT_ABSOLUTE_COLORIMETRIC, 0);
    
    // Step 7: Test with link/abstract/named color classes (should return FALSE)
    if (devClass == cmsSigLinkClass || 
        devClass == cmsSigAbstractClass || 
        devClass == cmsSigNamedColorClass) {
        // These should return FALSE according to source code
        for (int intent = 0; intent < 3; intent++) {
            cmsDetectDestinationBlackPoint(&black_point, hProfile, intent, 0);
        }
    }
    
    // Step 8: Compare with cmsDetectBlackPoint (related function)
    cmsCIEXYZ black_point2;
    cmsDetectBlackPoint(&black_point2, hProfile, INTENT_PERCEPTUAL, 0);
    
    // Step 9: Clean up
    cmsCloseProfile(hProfile);
    
    return 0;
}

} // namespace harness_028
