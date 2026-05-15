#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_002 {
// Fuzzing harness for Little CMS 2 library
// Target: Proofing transform operations with gamut checking
// Strategy: Target proofing transform APIs with 537+ undiscovered branches
//           Implement color proofing workflow with gamut alarm codes
//           This complements harness_000.cpp (profile parsing) and harness_001.cpp (basic transforms)


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for profiles, alarm codes, and pixel data
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type from fuzzer input (0=THR version, 1=non-THR version)
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 2;
    
    // Consume transformation parameters
    uint32_t input_intent = fdp.ConsumeIntegral<uint32_t>() % 4;  // 0-3 corresponds to standard intents
    uint32_t proofing_intent = fdp.ConsumeIntegral<uint32_t>() % 4;
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
    uint32_t pixel_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 32);  // Reasonable pixel count
    
    // Map intent values to Little CMS intents
    cmsUInt32Number cms_input_intent, cms_proofing_intent;
    switch (input_intent) {
        case 0: cms_input_intent = INTENT_PERCEPTUAL; break;
        case 1: cms_input_intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: cms_input_intent = INTENT_SATURATION; break;
        case 3: cms_input_intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
        default: cms_input_intent = INTENT_PERCEPTUAL; break;
    }
    switch (proofing_intent) {
        case 0: cms_proofing_intent = INTENT_PERCEPTUAL; break;
        case 1: cms_proofing_intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: cms_proofing_intent = INTENT_SATURATION; break;
        case 3: cms_proofing_intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
        default: cms_proofing_intent = INTENT_PERCEPTUAL; break;
    }
    
    // Consume profile data for simulated profiles
    size_t profile1_size = fdp.ConsumeIntegralInRange<size_t>(128, 1024);
    size_t profile2_size = fdp.ConsumeIntegralInRange<size_t>(128, 1024);
    size_t proofing_profile_size = fdp.ConsumeIntegralInRange<size_t>(128, 1024);
    
    // Ensure we have enough data for all profiles
    if (fdp.remaining_bytes() < profile1_size + profile2_size + proofing_profile_size) {
        return 0;
    }
    
    std::vector<uint8_t> profile1_data = fdp.ConsumeBytes<uint8_t>(profile1_size);
    std::vector<uint8_t> profile2_data = fdp.ConsumeBytes<uint8_t>(profile2_size);
    std::vector<uint8_t> proofing_profile_data = fdp.ConsumeBytes<uint8_t>(proofing_profile_size);
    
    // Create profiles from fuzzer data
    cmsHPROFILE input_profile = NULL;
    cmsHPROFILE output_profile = NULL;
    cmsHPROFILE proofing_profile = NULL;
    
    if (operation_type == 0) {
        // Use THR versions with context
        cmsContext context = cmsCreateContext(NULL, NULL);
        
        input_profile = cmsOpenProfileFromMemTHR(context, profile1_data.data(), profile1_data.size());
        output_profile = cmsOpenProfileFromMemTHR(context, profile2_data.data(), profile2_data.size());
        proofing_profile = cmsOpenProfileFromMemTHR(context, proofing_profile_data.data(), proofing_profile_data.size());
        
        if (input_profile == NULL || output_profile == NULL || proofing_profile == NULL) {
            // Fallback to built-in profiles if fuzzer data doesn't create valid profiles
            if (input_profile == NULL) input_profile = cmsCreate_sRGBProfileTHR(context);
            if (output_profile == NULL) output_profile = cmsCreateLab2ProfileTHR(context, NULL);
            if (proofing_profile == NULL) proofing_profile = cmsCreateXYZProfileTHR(context);
        }
        
        // Set alarm codes for gamut checking
        cmsUInt16Number alarm_codes[cmsMAXCHANNELS];
        for (int i = 0; i < cmsMAXCHANNELS; i++) {
            alarm_codes[i] = fdp.ConsumeIntegral<uint16_t>();
        }
        cmsSetAlarmCodesTHR(context, alarm_codes);
        
        // Create proofing transform
        cmsHTRANSFORM transform = cmsCreateProofingTransformTHR(
            context,
            input_profile,
            TYPE_RGB_8,          // Input format
            output_profile,
            TYPE_CMYK_8,         // Output format (CMYK for proofing)
            proofing_profile,
            cms_input_intent,
            cms_proofing_intent,
            flags | cmsFLAGS_GAMUTCHECK
        );
        
        if (transform != NULL) {
            // Consume pixel data for transformation
            size_t pixel_data_size = pixel_count * 3; // RGB data (3 bytes per pixel)
            if (fdp.remaining_bytes() >= pixel_data_size) {
                std::vector<uint8_t> input_pixels = fdp.ConsumeBytes<uint8_t>(pixel_data_size);
                std::vector<uint8_t> output_pixels(pixel_count * 4); // CMYK output (4 bytes per pixel)
                
                cmsDoTransform(transform, input_pixels.data(), output_pixels.data(), pixel_count);
            }
            
            cmsDeleteTransform(transform);
        }
        
        // Cleanup
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        if (proofing_profile) cmsCloseProfile(proofing_profile);
        
        cmsDeleteContext(context);
        
    } else {
        // Use non-THR versions
        input_profile = cmsOpenProfileFromMem(profile1_data.data(), profile1_data.size());
        output_profile = cmsOpenProfileFromMem(profile2_data.data(), profile2_data.size());
        proofing_profile = cmsOpenProfileFromMem(proofing_profile_data.data(), proofing_profile_data.size());
        
        if (input_profile == NULL || output_profile == NULL || proofing_profile == NULL) {
            // Fallback to built-in profiles if fuzzer data doesn't create valid profiles
            if (input_profile == NULL) input_profile = cmsCreate_sRGBProfile();
            if (output_profile == NULL) output_profile = cmsCreateLab2Profile(NULL);
            if (proofing_profile == NULL) proofing_profile = cmsCreateXYZProfile();
        }
        
        // Set alarm codes for gamut checking
        cmsUInt16Number alarm_codes[cmsMAXCHANNELS];
        for (int i = 0; i < cmsMAXCHANNELS; i++) {
            alarm_codes[i] = fdp.ConsumeIntegral<uint16_t>();
        }
        cmsSetAlarmCodes(alarm_codes);
        
        // Create proofing transform
        cmsHTRANSFORM transform = cmsCreateProofingTransform(
            input_profile,
            TYPE_RGB_8,          // Input format
            output_profile,
            TYPE_CMYK_8,         // Output format (CMYK for proofing)
            proofing_profile,
            cms_input_intent,
            cms_proofing_intent,
            flags | cmsFLAGS_GAMUTCHECK
        );
        
        if (transform != NULL) {
            // Consume pixel data for transformation
            size_t pixel_data_size = pixel_count * 3; // RGB data (3 bytes per pixel)
            if (fdp.remaining_bytes() >= pixel_data_size) {
                std::vector<uint8_t> input_pixels = fdp.ConsumeBytes<uint8_t>(pixel_data_size);
                std::vector<uint8_t> output_pixels(pixel_count * 4); // CMYK output (4 bytes per pixel)
                
                cmsDoTransform(transform, input_pixels.data(), output_pixels.data(), pixel_count);
            }
            
            cmsDeleteTransform(transform);
        }
        
        // Cleanup
        if (input_profile) cmsCloseProfile(input_profile);
        if (output_profile) cmsCloseProfile(output_profile);
        if (proofing_profile) cmsCloseProfile(proofing_profile);
    }
    
    return 0;
}

} // namespace harness_002
