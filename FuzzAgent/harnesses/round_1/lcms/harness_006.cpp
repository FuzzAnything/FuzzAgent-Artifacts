#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_006 {
// Fuzzing harness for Little CMS 2 library
// Target: Virtual profile creation functions with 892+ undiscovered branches in cmsvirt.c
// Strategy: Follow the exact workflow from coverage guidance:
//           1. Call cmsCreateContext to initialize context
//           2. Create multiple virtual profiles using target APIs (Lab4, NULL, RGB, Gray, OkLab variants)
//           3. Set profile metadata using cmsWriteTag for descriptive tags
//           4. Create color transforms using cmsCreateTransform with different profile combinations
//           5. Perform test color transformations with sample data
//           6. Clean up: cmsDeleteTransform, cmsCloseProfile for each profile, cmsDeleteContext
// Focus APIs: cmsCreateLab4Profile (143 undiscovered branches),
//             cmsCreateNULLProfileTHR (141), cmsCreateNULLProfile (141),
//             cmsCreate_OkLabProfile (141), cmsCreateRGBProfile (114),
//             cmsCreateGrayProfile (106), cmsCreateGrayProfileTHR (106)
// Helper APIs: cmsCreateContext, cmsCreateTransform (397 undiscovered),
//              cmsWriteTag, cmsCloseProfile, cmsDeleteTransform, cmsDeleteContext
// Semantic uniqueness: First harness to comprehensively target all virtual profile creation functions,
//                      implements full profile lifecycle from creation to transformation,
//                      uses both THR and non-THR API variants for maximum coverage


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for various parameters and operations
    // We need: operation selection, white point coordinates, profile parameters, etc.
    if (size < 128) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Create context for THR functions
    cmsContext context = cmsCreateContext(NULL, NULL);
    if (context == NULL) {
        return 0;
    }
    
    // Step 2: Create virtual profiles using target APIs
    
    // 2.1: Create cmsCIExyY white point structure from fuzzer input
    cmsCIExyY white_point;
    white_point.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    white_point.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    white_point.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    
    // 2.2: Create Lab4 profile (143 undiscovered branches)
    cmsHPROFILE lab4_profile = cmsCreateLab4Profile(&white_point);
    
    // 2.3: Create NULL profiles - both THR and non-THR versions
    cmsHPROFILE null_profile_thr = cmsCreateNULLProfileTHR(context);
    cmsHPROFILE null_profile = cmsCreateNULLProfile();
    
    // 2.4: Create OkLab profile (141 undiscovered branches)
    cmsHPROFILE oklab_profile = cmsCreate_OkLabProfile(context);
    
    // 2.5: Create RGB profile (114 undiscovered branches)
    // Need primaries and transfer functions for RGB profile
    cmsCIExyYTRIPLE primaries;
    primaries.Red.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Red.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Red.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Green.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Green.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Green.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Blue.x = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Blue.y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    primaries.Blue.Y = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.0, 1.0);
    
    // Create gamma curves for transfer functions
    cmsFloat64Number gamma = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(0.5, 5.0);
    cmsToneCurve* gamma_curve = cmsBuildGamma(context, gamma);
    cmsToneCurve* transfer_functions[3] = {gamma_curve, gamma_curve, gamma_curve};
    
    cmsHPROFILE rgb_profile = cmsCreateRGBProfile(&white_point, &primaries, transfer_functions);
    
    // 2.6: Create Gray profiles - both THR and non-THR versions
    cmsHPROFILE gray_profile_thr = cmsCreateGrayProfileTHR(context, &white_point, gamma_curve);
    cmsHPROFILE gray_profile = cmsCreateGrayProfile(&white_point, gamma_curve);
    
    // Clean up gamma curve after use
    if (gamma_curve != NULL) {
        cmsFreeToneCurve(gamma_curve);
    }
    
    // Step 3: Set profile metadata using cmsWriteTag
    // We'll create simple description tags for some profiles
    
    // Create MLU (Multi Localized Unicode) for description
    cmsMLU* description_mlu = cmsMLUalloc(context, 1);
    if (description_mlu != NULL) {
        // Set description for Lab4 profile
        cmsMLUsetWide(description_mlu, "en", "US", L"Lab4 Virtual Profile");
        if (lab4_profile != NULL) {
            cmsWriteTag(lab4_profile, cmsSigProfileDescriptionTag, description_mlu);
        }
        
        // Set description for RGB profile
        cmsMLUsetWide(description_mlu, "en", "US", L"RGB Virtual Profile");
        if (rgb_profile != NULL) {
            cmsWriteTag(rgb_profile, cmsSigProfileDescriptionTag, description_mlu);
        }
        
        cmsMLUfree(description_mlu);
    }
    
    // Step 4: Create color transforms using cmsCreateTransform (397 undiscovered branches)
    // We'll create transforms between different profile combinations
    
    // Consume transform parameters from fuzzer input
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 3);
    cmsUInt32Number flags = fdp.ConsumeIntegral<cmsUInt32Number>();
    
    // Map intent to Little CMS intents
    cmsUInt32Number cms_intent;
    switch (intent) {
        case 0: cms_intent = INTENT_PERCEPTUAL; break;
        case 1: cms_intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: cms_intent = INTENT_SATURATION; break;
        case 3: cms_intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
        default: cms_intent = INTENT_PERCEPTUAL; break;
    }
    
    // Create transforms between different virtual profile combinations
    cmsHTRANSFORM transform1 = NULL;
    cmsHTRANSFORM transform2 = NULL;
    cmsHTRANSFORM transform3 = NULL;
    
    // Transform 1: Lab4 to RGB (if both profiles exist)
    if (lab4_profile != NULL && rgb_profile != NULL) {
        transform1 = cmsCreateTransform(lab4_profile, TYPE_Lab_8,
                                        rgb_profile, TYPE_RGB_8,
                                        cms_intent, flags);
    }
    
    // Transform 2: RGB to Gray (if both profiles exist)
    if (rgb_profile != NULL && gray_profile != NULL) {
        transform2 = cmsCreateTransform(rgb_profile, TYPE_RGB_8,
                                        gray_profile, TYPE_GRAY_8,
                                        cms_intent, flags);
    }
    
    // Transform 3: OkLab to NULL (if both profiles exist)
    if (oklab_profile != NULL && null_profile != NULL) {
        transform3 = cmsCreateTransform(oklab_profile, TYPE_Lab_8,
                                        null_profile, TYPE_Lab_8,
                                        cms_intent, flags);
    }
    
    // Step 5: Perform test color transformations with sample data
    // Generate test pixel data from fuzzer input
    
    // Test with Lab4 to RGB transform
    if (transform1 != NULL) {
        // Consume pixel count
        uint32_t pixel_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);
        
        // Allocate input and output buffers
        std::vector<uint8_t> input_buffer(pixel_count * 3);  // 3 bytes per Lab pixel
        std::vector<uint8_t> output_buffer(pixel_count * 3); // 3 bytes per RGB pixel
        
        // Fill input buffer with fuzzer data
        if (fdp.remaining_bytes() >= input_buffer.size()) {
            auto input_data = fdp.ConsumeBytes<uint8_t>(input_buffer.size());
            std::copy(input_data.begin(), input_data.end(), input_buffer.begin());
            
            // Perform transformation
            cmsDoTransform(transform1, input_buffer.data(), output_buffer.data(), pixel_count);
        }
    }
    
    // Test with RGB to Gray transform
    if (transform2 != NULL) {
        uint32_t pixel_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);
        std::vector<uint8_t> input_buffer(pixel_count * 3);  // 3 bytes per RGB pixel
        std::vector<uint8_t> output_buffer(pixel_count * 1); // 1 byte per Gray pixel
        
        if (fdp.remaining_bytes() >= input_buffer.size()) {
            auto input_data = fdp.ConsumeBytes<uint8_t>(input_buffer.size());
            std::copy(input_data.begin(), input_data.end(), input_buffer.begin());
            
            cmsDoTransform(transform2, input_buffer.data(), output_buffer.data(), pixel_count);
        }
    }
    
    // Test with OkLab to NULL transform
    if (transform3 != NULL) {
        uint32_t pixel_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 16);
        std::vector<uint8_t> input_buffer(pixel_count * 3);  // 3 bytes per Lab pixel
        std::vector<uint8_t> output_buffer(pixel_count * 3); // 3 bytes per Lab pixel
        
        if (fdp.remaining_bytes() >= input_buffer.size()) {
            auto input_data = fdp.ConsumeBytes<uint8_t>(input_buffer.size());
            std::copy(input_data.begin(), input_data.end(), input_buffer.begin());
            
            cmsDoTransform(transform3, input_buffer.data(), output_buffer.data(), pixel_count);
        }
    }
    
    // Step 6: Clean up all resources
    // Delete transforms
    if (transform1 != NULL) cmsDeleteTransform(transform1);
    if (transform2 != NULL) cmsDeleteTransform(transform2);
    if (transform3 != NULL) cmsDeleteTransform(transform3);
    
    // Close profiles
    if (lab4_profile != NULL) cmsCloseProfile(lab4_profile);
    if (null_profile_thr != NULL) cmsCloseProfile(null_profile_thr);
    if (null_profile != NULL) cmsCloseProfile(null_profile);
    if (oklab_profile != NULL) cmsCloseProfile(oklab_profile);
    if (rgb_profile != NULL) cmsCloseProfile(rgb_profile);
    if (gray_profile_thr != NULL) cmsCloseProfile(gray_profile_thr);
    if (gray_profile != NULL) cmsCloseProfile(gray_profile);
    
    // Delete context
    cmsDeleteContext(context);
    
    return 0;
}

} // namespace harness_006
