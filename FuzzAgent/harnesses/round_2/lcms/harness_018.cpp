#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_018 {
// Fuzzing harness for LittleCMS2 library
// Targets main API entry points for ICC profile parsing and color transformation
// harness_018.cpp - Replica of harness_000.cpp for forced compilation


static void ReadAllTags(cmsHPROFILE h) {
    cmsInt32Number i, n;
    cmsTagSignature sig;
    uint32_t sig_u32;

    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        sig = cmsGetTagSignature(h, i);
        // Store signature in uint32_t to avoid AFL instrumentation issues with values > 0x7fffffff
        sig_u32 = (uint32_t)sig;
        (void)sig_u32; // Use the value to avoid unused variable warning
        if (cmsReadTag(h, sig) == NULL) return;
    }
}

// Function to read all raw tags from a profile (from existing fuzzer)
static void ReadAllRAWTags(cmsHPROFILE h) {
    cmsInt32Number i, n;
    cmsTagSignature sig;
    uint32_t sig_u32;
    cmsInt32Number len;

    n = cmsGetTagCount(h);
    for (i = 0; i < n; i++) {
        sig = cmsGetTagSignature(h, i);
        // Store signature in uint32_t to avoid AFL instrumentation issues with values > 0x7fffffff
        sig_u32 = (uint32_t)sig;
        (void)sig_u32; // Use the value to avoid unused variable warning
        len = cmsReadRawTag(h, sig, NULL, 0);
    }
}

// Function to fetch all profile info (from existing fuzzer)
static void FetchAllInfos(cmsHPROFILE h) {
    wchar_t* text;
    cmsInt32Number len;
    cmsContext id = 0;

    // Get description
    len = cmsGetProfileInfo(h, cmsInfoDescription, "en", "US", NULL, 0);
    if (len > 0) {
        text = (wchar_t*)_cmsMalloc(id, len);
        if (text != NULL) {
            cmsGetProfileInfo(h, cmsInfoDescription, "en", "US", text, len);
            _cmsFree(id, text);
        }
    }
    
    // Get manufacturer
    len = cmsGetProfileInfo(h, cmsInfoManufacturer, "en", "US", NULL, 0);
    if (len > 0) {
        text = (wchar_t*)_cmsMalloc(id, len);
        if (text != NULL) {
            cmsGetProfileInfo(h, cmsInfoManufacturer, "en", "US", text, len);
            _cmsFree(id, text);
        }
    }
    
    // Get model
    len = cmsGetProfileInfo(h, cmsInfoModel, "en", "US", NULL, 0);
    if (len > 0) {
        text = (wchar_t*)_cmsMalloc(id, len);
        if (text != NULL) {
            cmsGetProfileInfo(h, cmsInfoModel, "en", "US", text, len);
            _cmsFree(id, text);
        }
    }
    
    // Get copyright
    len = cmsGetProfileInfo(h, cmsInfoCopyright, "en", "US", NULL, 0);
    if (len > 0) {
        text = (wchar_t*)_cmsMalloc(id, len);
        if (text != NULL) {
            cmsGetProfileInfo(h, cmsInfoCopyright, "en", "US", text, len);
            _cmsFree(id, text);
        }
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 128) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for profile data, second part for transform testing
    size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(128, size - 64);
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
    
    // Try to open profile from memory
    cmsHPROFILE hInput = cmsOpenProfileFromMem(profile_data.data(), (cmsUInt32Number)profile_data.size());
    if (hInput == NULL) {
        return 0;  // Invalid profile, nothing to test
    }
    
    // Test profile operations (from existing fuzzer)
    ReadAllTags(hInput);
    ReadAllRAWTags(hInput);
    FetchAllInfos(hInput);
    
    // Create sRGB profile for output
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB == NULL) {
        cmsCloseProfile(hInput);
        return 0;
    }
    
    // Get remaining bytes for transform testing
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 32) {
        cmsCloseProfile(hSRGB);
        cmsCloseProfile(hInput);
        return 0;
    }
    
    // Consume parameters for transform creation
    uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 for different intents
    uint8_t input_format_sel = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint8_t output_format_sel = fdp.ConsumeIntegral<uint8_t>() % 4;
    uint32_t flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Map selections to actual format types
    cmsUInt32Number input_format, output_format;
    switch (input_format_sel % 4) {
        case 0: input_format = TYPE_RGB_8; break;
        case 1: input_format = TYPE_RGB_16; break;
        case 2: input_format = TYPE_CMYK_8; break;
        case 3: input_format = TYPE_GRAY_8; break;
        default: input_format = TYPE_RGB_8; break;
    }
    
    switch (output_format_sel % 4) {
        case 0: output_format = TYPE_RGB_8; break;
        case 1: output_format = TYPE_RGB_16; break;
        case 2: output_format = TYPE_CMYK_8; break;
        case 3: output_format = TYPE_GRAY_8; break;
        default: output_format = TYPE_RGB_8; break;
    }
    
    // Create transform
    cmsHTRANSFORM transform = cmsCreateTransform(hInput, input_format,
                                                 hSRGB, output_format,
                                                 intent, flags);
    
    if (transform != NULL) {
        // Test transform with random pixel data
        size_t pixel_count = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        size_t input_buffer_size = 0;
        size_t output_buffer_size = 0;
        
        // Calculate buffer sizes based on format
        switch (input_format) {
            case TYPE_RGB_8: input_buffer_size = pixel_count * 3; break;
            case TYPE_RGB_16: input_buffer_size = pixel_count * 3 * 2; break;
            case TYPE_CMYK_8: input_buffer_size = pixel_count * 4; break;
            case TYPE_GRAY_8: input_buffer_size = pixel_count; break;
            default: input_buffer_size = pixel_count * 3; break;
        }
        
        switch (output_format) {
            case TYPE_RGB_8: output_buffer_size = pixel_count * 3; break;
            case TYPE_RGB_16: output_buffer_size = pixel_count * 3 * 2; break;
            case TYPE_CMYK_8: output_buffer_size = pixel_count * 4; break;
            case TYPE_GRAY_8: output_buffer_size = pixel_count; break;
            default: output_buffer_size = pixel_count * 3; break;
        }
        
        // Check if we have enough data for buffers
        if (fdp.remaining_bytes() >= input_buffer_size) {
            std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(input_buffer_size);
            std::vector<uint8_t> output_buffer(output_buffer_size);
            
            // Perform transformation
            cmsDoTransform(transform, input_buffer.data(), output_buffer.data(), pixel_count);
        }
        
        // Test with stride if there's more data
        if (fdp.remaining_bytes() >= input_buffer_size + 4) {
            uint32_t stride = fdp.ConsumeIntegral<uint32_t>();
            std::vector<uint8_t> input_buffer2 = fdp.ConsumeBytes<uint8_t>(input_buffer_size);
            std::vector<uint8_t> output_buffer2(output_buffer_size);
            
            // Use stride version if available
            cmsDoTransformStride(transform, input_buffer2.data(), output_buffer2.data(), 
                                 pixel_count, stride);
        }
        
        cmsDeleteTransform(transform);
    }
    
    // Also test multiprofile transform
    cmsHPROFILE profiles[2] = { hInput, hSRGB };
    cmsHTRANSFORM multi_transform = cmsCreateMultiprofileTransform(profiles, 2,
                                                                  TYPE_RGB_8, TYPE_RGB_8,
                                                                  intent, flags);
    if (multi_transform != NULL) {
        // Simple test with a few pixels if we have data
        if (fdp.remaining_bytes() >= 12) {
            uint8_t rgb_input[12];
            uint8_t rgb_output[12];
            memcpy(rgb_input, fdp.ConsumeBytes<uint8_t>(12).data(), 12);
            cmsDoTransform(multi_transform, rgb_input, rgb_output, 4); // 4 RGB pixels
        }
        cmsDeleteTransform(multi_transform);
    }
    
    // Clean up
    cmsCloseProfile(hSRGB);
    cmsCloseProfile(hInput);
    
    return 0;
}

} // namespace harness_018
