#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>

namespace harness_007 {
// Fuzzing harness for Little CMS 2 (lcms) library - proofing transform operations
// Targets proofing transform APIs with 0% coverage per coverage guidance (419+ branches undiscovered each)
// Primary target APIs: cmsCreateProofingTransform, cmsCreateProofingTransformTHR
// Helper APIs: cmsOpenProfileFromMem, cmsCreateLab4Profile, cmsCreate_sRGBProfile, cmsDeleteTransform, cmsCloseProfile, cmsDoTransform
// This harness focuses on color proofing operations with gamut checking and soft proofing,
// differentiating from previous harnesses by targeting specialized proofing workflows.
// Invocation sequence: create profiles → CreateProofingTransform → CreateProofingTransformTHR → optionally DoTransform → cleanup


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for ICC profile and parameters
    if (size < 256) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input for multiple operations
    // Consume fixed-size data first for parameters
    bool use_thread_safe = fdp.ConsumeBool();  // Whether to use thread-safe context
    uint8_t intent_type = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 for intent types
    uint8_t proofing_intent_type = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 for proofing intent types
    uint8_t input_format_type = fdp.ConsumeIntegral<uint8_t>() % 6;  // 0-5 for input formats
    uint8_t output_format_type = fdp.ConsumeIntegral<uint8_t>() % 6;  // 0-5 for output formats
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();  // Various flag combinations
    
    // Map intent values to lcms constants
    cmsUInt32Number intent = INTENT_PERCEPTUAL;
    switch (intent_type) {
        case 0: intent = INTENT_PERCEPTUAL; break;
        case 1: intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: intent = INTENT_SATURATION; break;
        case 3: intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
    }
    
    cmsUInt32Number proofing_intent = INTENT_ABSOLUTE_COLORIMETRIC;
    switch (proofing_intent_type) {
        case 0: proofing_intent = INTENT_PERCEPTUAL; break;
        case 1: proofing_intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: proofing_intent = INTENT_SATURATION; break;
        case 3: proofing_intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
    }
    
    // Map format types to lcms constants
    cmsUInt32Number input_format = TYPE_RGB_8;
    switch (input_format_type) {
        case 0: input_format = TYPE_RGB_8; break;
        case 1: input_format = TYPE_RGB_FLT; break;
        case 2: input_format = TYPE_Lab_8; break;
        case 3: input_format = TYPE_Lab_DBL; break;
        case 4: input_format = TYPE_GRAY_8; break;
        case 5: input_format = TYPE_GRAY_FLT; break;
    }
    
    cmsUInt32Number output_format = TYPE_GRAY_8;
    switch (output_format_type) {
        case 0: output_format = TYPE_RGB_8; break;
        case 1: output_format = TYPE_RGB_FLT; break;
        case 2: output_format = TYPE_Lab_8; break;
        case 3: output_format = TYPE_Lab_DBL; break;
        case 4: output_format = TYPE_GRAY_8; break;
        case 5: output_format = TYPE_GRAY_FLT; break;
    }
    
    // Create thread-safe context if requested
    cmsContext context = NULL;
    if (use_thread_safe) {
        context = cmsCreateContext(NULL, NULL);
        if (context == NULL) {
            return 0;  // Context creation failed
        }
    }
    
    // Step 1: Create input profile from fuzzer data (ICC profile)
    // Consume a portion of remaining data for ICC profile
    size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(128, 4096);
    if (profile_size > fdp.remaining_bytes()) {
        profile_size = fdp.remaining_bytes();
    }
    
    std::vector<uint8_t> icc_profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
    if (icc_profile_data.size() < 128) {
        if (use_thread_safe) cmsDeleteContext(context);
        return 0;  // Not enough data for profile
    }
    
    cmsHPROFILE hInputProfile = NULL;
    if (use_thread_safe) {
        hInputProfile = cmsOpenProfileFromMemTHR(context, icc_profile_data.data(), 
                                                 static_cast<cmsUInt32Number>(icc_profile_data.size()));
    } else {
        hInputProfile = cmsOpenProfileFromMem(icc_profile_data.data(), 
                                              static_cast<cmsUInt32Number>(icc_profile_data.size()));
    }
    
    // If profile parsing fails, try using built-in profiles instead
    if (hInputProfile == NULL) {
        // Use Lab profile as input instead
        const cmsCIExyY d50_white_point = {0.3457, 0.3585, 1.0};  // D50 white point
        if (use_thread_safe) {
            hInputProfile = cmsCreateLab4ProfileTHR(context, &d50_white_point);
        } else {
            hInputProfile = cmsCreateLab4Profile(&d50_white_point);
        }
    }
    
    if (hInputProfile == NULL) {
        if (use_thread_safe) cmsDeleteContext(context);
        return 0;  // Failed to create input profile
    }
    
    // Step 2: Create output profile (sRGB)
    cmsHPROFILE hOutputProfile = NULL;
    if (use_thread_safe) {
        hOutputProfile = cmsCreate_sRGBProfileTHR(context);
    } else {
        hOutputProfile = cmsCreate_sRGBProfile();
    }
    
    if (hOutputProfile == NULL) {
        cmsCloseProfile(hInputProfile);
        if (use_thread_safe) cmsDeleteContext(context);
        return 0;  // Failed to create output profile
    }
    
    // Step 3: Create proofing profile (Lab4)
    cmsHPROFILE hProofingProfile = NULL;
    const cmsCIExyY proofing_white_point = {0.3127, 0.3290, 1.0};  // D65 white point
    if (use_thread_safe) {
        hProofingProfile = cmsCreateLab4ProfileTHR(context, &proofing_white_point);
    } else {
        hProofingProfile = cmsCreateLab4Profile(&proofing_white_point);
    }
    
    if (hProofingProfile == NULL) {
        cmsCloseProfile(hInputProfile);
        cmsCloseProfile(hOutputProfile);
        if (use_thread_safe) cmsDeleteContext(context);
        return 0;  // Failed to create proofing profile
    }
    
    // Step 4: Create proofing transform (non-thread-safe version)
    cmsHTRANSFORM hProofingTransform = NULL;
    hProofingTransform = cmsCreateProofingTransform(hInputProfile,
                                                   input_format,
                                                   hOutputProfile,
                                                   output_format,
                                                   hProofingProfile,
                                                   intent,
                                                   proofing_intent,
                                                   flags);
    
    // Step 5: Create proofing transform (thread-safe version if context exists)
    cmsHTRANSFORM hProofingTransformTHR = NULL;
    if (use_thread_safe) {
        hProofingTransformTHR = cmsCreateProofingTransformTHR(context,
                                                             hInputProfile,
                                                             input_format,
                                                             hOutputProfile,
                                                             output_format,
                                                             hProofingProfile,
                                                             intent,
                                                             proofing_intent,
                                                             flags);
    }
    
    // Step 6: Optionally exercise transforms with test data
    if (hProofingTransform != NULL) {
        // Prepare test data based on input format
        size_t pixel_size = 0;
        switch (input_format) {
            case TYPE_RGB_8:
            case TYPE_RGB_FLT:
                pixel_size = 3 * (input_format == TYPE_RGB_FLT ? sizeof(float) : 1);
                break;
            case TYPE_Lab_8:
            case TYPE_Lab_DBL:
                pixel_size = 3 * (input_format == TYPE_Lab_DBL ? sizeof(double) : 1);
                break;
            case TYPE_GRAY_8:
            case TYPE_GRAY_FLT:
                pixel_size = (input_format == TYPE_GRAY_FLT ? sizeof(float) : 1);
                break;
            default:
                pixel_size = 3;  // Default to RGB
        }
        
        // Consume test data from remaining input
        size_t test_data_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        if (test_data_size > fdp.remaining_bytes()) {
            test_data_size = fdp.remaining_bytes();
        }
        
        std::vector<uint8_t> test_input = fdp.ConsumeBytes<uint8_t>(test_data_size);
        
        // Only proceed if we have enough data for at least one pixel
        if (test_input.size() >= pixel_size) {
            // Create output buffer
            size_t output_pixel_size = 0;
            switch (output_format) {
                case TYPE_RGB_8:
                case TYPE_RGB_FLT:
                    output_pixel_size = 3 * (output_format == TYPE_RGB_FLT ? sizeof(float) : 1);
                    break;
                case TYPE_Lab_8:
                case TYPE_Lab_DBL:
                    output_pixel_size = 3 * (output_format == TYPE_Lab_DBL ? sizeof(double) : 1);
                    break;
                case TYPE_GRAY_8:
                case TYPE_GRAY_FLT:
                    output_pixel_size = (output_format == TYPE_GRAY_FLT ? sizeof(float) : 1);
                    break;
                default:
                    output_pixel_size = 1;  // Default to gray
            }
            
            // Allocate output buffer
            std::vector<uint8_t> test_output(output_pixel_size);
            
            // Perform transformation
            cmsDoTransform(hProofingTransform, test_input.data(), test_output.data(), 1);
            
            // Also test thread-safe transform if available
            if (hProofingTransformTHR != NULL) {
                std::vector<uint8_t> test_output_thr(output_pixel_size);
                cmsDoTransform(hProofingTransformTHR, test_input.data(), test_output_thr.data(), 1);
            }
        }
    }
    
    // Step 7: Clean up transforms
    if (hProofingTransform != NULL) {
        cmsDeleteTransform(hProofingTransform);
    }
    
    if (hProofingTransformTHR != NULL) {
        cmsDeleteTransform(hProofingTransformTHR);
    }
    
    // Step 8: Clean up profiles
    cmsCloseProfile(hInputProfile);
    cmsCloseProfile(hOutputProfile);
    cmsCloseProfile(hProofingProfile);
    
    // Clean up context if created
    if (use_thread_safe) {
        cmsDeleteContext(context);
    }
    
    return 0;
}

} // namespace harness_007
