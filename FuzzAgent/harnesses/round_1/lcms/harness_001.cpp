#include <cstdint>
#include <cstddef>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_001 {
// Fuzzing harness for Little CMS 2 library
// Target: Color transformation operations
// Strategy: Focus on core transformation APIs (cmsCreateTransform, cmsDoTransform, etc.)
//           using built-in profiles (sRGB, Lab, XYZ) and fuzzer input for transformation data
//           This complements harness_000.cpp which focuses on profile parsing


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for operation selection and pixel data
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation type from fuzzer input
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume transformation parameters
    uint32_t intent = fdp.ConsumeIntegral<uint32_t>() % 4;  // 0-3 corresponds to standard intents
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
    uint32_t pixel_count = fdp.ConsumeIntegralInRange<uint32_t>(1, 64);  // Reasonable pixel count
    
    // Map intent values to Little CMS intents
    cmsUInt32Number cms_intent;
    switch (intent) {
        case 0: cms_intent = INTENT_PERCEPTUAL; break;
        case 1: cms_intent = INTENT_RELATIVE_COLORIMETRIC; break;
        case 2: cms_intent = INTENT_SATURATION; break;
        case 3: cms_intent = INTENT_ABSOLUTE_COLORIMETRIC; break;
        default: cms_intent = INTENT_PERCEPTUAL; break;
    }
    
    // Create built-in profiles for testing
    cmsHPROFILE src_profile = NULL;
    cmsHPROFILE dst_profile = NULL;
    
    // Select profile combination based on operation type
    switch (operation_type) {
        case 0:
            // sRGB to Lab transformation
            src_profile = cmsCreate_sRGBProfile();
            dst_profile = cmsCreateLab2Profile(NULL);  // Use default D50 white point
            break;
        case 1:
            // Lab to XYZ transformation
            src_profile = cmsCreateLab2Profile(NULL);
            dst_profile = cmsCreateXYZProfile();
            break;
        case 2:
            // sRGB to XYZ transformation
            src_profile = cmsCreate_sRGBProfile();
            dst_profile = cmsCreateXYZProfile();
            break;
        case 3:
            // XYZ to sRGB transformation
            src_profile = cmsCreateXYZProfile();
            dst_profile = cmsCreate_sRGBProfile();
            break;
        default:
            return 0;
    }
    
    if (src_profile == NULL || dst_profile == NULL) {
        if (src_profile) cmsCloseProfile(src_profile);
        if (dst_profile) cmsCloseProfile(dst_profile);
        return 0;
    }
    
    // Create transform based on operation type
    cmsHTRANSFORM transform = NULL;
    cmsUInt32Number input_format, output_format;
    
    switch (operation_type) {
        case 0:  // sRGB to Lab
            input_format = TYPE_RGB_8;
            output_format = TYPE_Lab_8;
            break;
        case 1:  // Lab to XYZ
            input_format = TYPE_Lab_8;
            output_format = TYPE_XYZ_16;
            break;
        case 2:  // sRGB to XYZ
            input_format = TYPE_RGB_8;
            output_format = TYPE_XYZ_16;
            break;
        case 3:  // XYZ to sRGB
            input_format = TYPE_XYZ_16;
            output_format = TYPE_RGB_8;
            break;
        default:
            input_format = TYPE_RGB_8;
            output_format = TYPE_Lab_8;
            break;
    }
    
    // Create the transform with fuzzer-provided intent and flags
    transform = cmsCreateTransform(src_profile, input_format,
                                   dst_profile, output_format,
                                   cms_intent, flags);
    
    if (transform == NULL) {
        cmsCloseProfile(src_profile);
        cmsCloseProfile(dst_profile);
        return 0;
    }
    
    // Test diagnostic functions on the transform
    cmsContext context_id = cmsGetTransformContextID(transform);
    (void)context_id;  // Use context_id to avoid unused variable warning
    
    cmsUInt32Number actual_input_format = cmsGetTransformInputFormat(transform);
    cmsUInt32Number actual_output_format = cmsGetTransformOutputFormat(transform);
    (void)actual_input_format;  // Use to avoid unused variable warning
    (void)actual_output_format;
    
    // Consume pixel data for transformation
    size_t pixel_size = 0;
    switch (input_format) {
        case TYPE_RGB_8:
            pixel_size = 3;  // 3 bytes per pixel
            break;
        case TYPE_Lab_8:
            pixel_size = 3;  // 3 bytes per pixel
            break;
        case TYPE_XYZ_16:
            pixel_size = 6;  // 3 channels * 2 bytes per channel
            break;
        default:
            pixel_size = 3;
            break;
    }
    
    size_t input_buffer_size = pixel_count * pixel_size;
    
    // Check if we have enough data for input buffer
    if (fdp.remaining_bytes() < input_buffer_size) {
        cmsDeleteTransform(transform);
        cmsCloseProfile(src_profile);
        cmsCloseProfile(dst_profile);
        return 0;
    }
    
    // Consume input buffer data
    std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_buffer_size);
    
    // Calculate output buffer size
    size_t output_pixel_size = 0;
    switch (output_format) {
        case TYPE_RGB_8:
            output_pixel_size = 3;
            break;
        case TYPE_Lab_8:
            output_pixel_size = 3;
            break;
        case TYPE_XYZ_16:
            output_pixel_size = 6;
            break;
        default:
            output_pixel_size = 3;
            break;
    }
    
    size_t output_buffer_size = pixel_count * output_pixel_size;
    std::vector<uint8_t> output_buffer(output_buffer_size);
    
    // Perform the transformation
    cmsDoTransform(transform, input_buffer.data(), output_buffer.data(), pixel_count);
    
    // Alternative path: Test with cmsOpenProfileFromMem as fallback
    if (fdp.remaining_bytes() > 100) {
        std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(50, fdp.remaining_bytes()));
        
        cmsHPROFILE mem_profile = cmsOpenProfileFromMem(profile_data.data(), 
                                                      (cmsUInt32Number)profile_data.size());
        if (mem_profile != NULL) {
            // Test creating a transform with the memory-based profile
            cmsHTRANSFORM mem_transform = cmsCreateTransform(mem_profile, TYPE_RGB_8,
                                                            dst_profile, TYPE_Lab_8,
                                                            cms_intent, flags);
            if (mem_transform != NULL) {
                // Test with some pixel data if available
                if (fdp.remaining_bytes() >= 3) {
                    std::vector<uint8_t> test_input = fdp.ConsumeBytes<uint8_t>(3);
                    uint8_t test_output[3];
                    cmsDoTransform(mem_transform, test_input.data(), test_output, 1);
                }
                cmsDeleteTransform(mem_transform);
            }
            cmsCloseProfile(mem_profile);
        }
    }
    
    // Clean up
    cmsDeleteTransform(transform);
    cmsCloseProfile(src_profile);
    cmsCloseProfile(dst_profile);
    
    return 0;
}

} // namespace harness_001
