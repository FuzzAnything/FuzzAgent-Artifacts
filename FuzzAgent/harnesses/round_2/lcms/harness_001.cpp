#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcms2.h"
#include "lcms2_plugin.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_001 {
// Fuzzing harness for LittleCMS2 library
// Targets proofing transform creation and PostScript resource generation APIs
// harness_001.cpp - Focus on cmsCreateProofingTransform, cmsGetPostScriptCSA, cmsGetPostScriptCRD

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

// Helper function to read all raw tags from a profile (from existing fuzzer)
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

// Helper function to fetch profile info (from existing fuzzer)
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

// Generate PostScript Color Space Array (CSA)
static void GenerateCSA(cmsHPROFILE hProfile, cmsUInt32Number intent, cmsUInt32Number flags) {
    cmsUInt32Number n;
    char* Buffer;

    n = cmsGetPostScriptCSA(0, hProfile, intent, flags, NULL, 0);
    if (n == 0) return;

    Buffer = (char*)_cmsMalloc(0, n);
    if (Buffer != NULL) {
        cmsGetPostScriptCSA(0, hProfile, intent, flags, Buffer, n);
        _cmsFree(0, Buffer);
    }
}

// Generate PostScript Color Rendering Dictionary (CRD)
static void GenerateCRD(cmsHPROFILE hProfile, cmsUInt32Number intent, cmsUInt32Number flags) {
    cmsUInt32Number n;
    char* Buffer;

    n = cmsGetPostScriptCRD(0, hProfile, intent, flags, NULL, 0);
    if (n == 0) return;

    Buffer = (char*)_cmsMalloc(0, n);
    if (Buffer != NULL) {
        cmsGetPostScriptCRD(0, hProfile, intent, flags, Buffer, n);
        _cmsFree(0, Buffer);
    }
}

// Main fuzzer entry point
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // We need at least: 128 bytes for profile1 + 128 bytes for profile2 + 32 bytes for testing = 288 bytes
    if (size < 288) {
        return 0;  // Insufficient input for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into three parts: input profile, proofing profile, and transform/testing data
    // Ensure min <= max for ConsumeIntegralInRange calls
    size_t max_profile1_size = size / 3;
    // Ensure max_profile1_size >= 128, otherwise adjust min or return
    if (max_profile1_size < 128) {
        // Not enough data for profile1 with minimum 128 bytes
        return 0;
    }
    size_t profile1_size = fdp.ConsumeIntegralInRange<size_t>(128, max_profile1_size);
    std::vector<uint8_t> profile1_data = fdp.ConsumeBytes<uint8_t>(profile1_size);
    
    size_t remaining_after_profile1 = fdp.remaining_bytes();
    size_t max_profile2_size = remaining_after_profile1 / 2;
    // Ensure max_profile2_size >= 128, otherwise return (hInput not created yet)
    if (max_profile2_size < 128) {
        // Not enough remaining data for profile2 with minimum 128 bytes
        return 0;
    }
    size_t profile2_size = fdp.ConsumeIntegralInRange<size_t>(128, max_profile2_size);
    std::vector<uint8_t> profile2_data = fdp.ConsumeBytes<uint8_t>(profile2_size);
    cmsHPROFILE hInput = cmsOpenProfileFromMem(profile1_data.data(), (cmsUInt32Number)profile1_data.size());
    if (hInput == NULL) {
        return 0;  // Invalid profile, nothing to test
    }
    
    // Try to open second profile from memory (for proofing)
    cmsHPROFILE hProofing = cmsOpenProfileFromMem(profile2_data.data(), (cmsUInt32Number)profile2_data.size());
    if (hProofing == NULL) {
        cmsCloseProfile(hInput);
        return 0;  // Invalid proofing profile
    }
    
    // Create sRGB profile for output
    cmsHPROFILE hSRGB = cmsCreate_sRGBProfile();
    if (hSRGB == NULL) {
        cmsCloseProfile(hProofing);
        cmsCloseProfile(hInput);
        return 0;
    }
    
    // Test profile operations on all profiles
    ReadAllTags(hInput);
    ReadAllRAWTags(hInput);
    FetchAllInfos(hInput);
    
    ReadAllTags(hProofing);
    ReadAllRAWTags(hProofing);
    FetchAllInfos(hProofing);
    
    // Get remaining bytes for transform testing and parameters
    size_t remaining = fdp.remaining_bytes();
    if (remaining < 32) {
        cmsCloseProfile(hSRGB);
        cmsCloseProfile(hProofing);
        cmsCloseProfile(hInput);
        return 0;
    }
    
    // Consume parameters for proofing transform creation
    uint8_t intent = fdp.ConsumeIntegral<uint8_t>() % 4;  // 0-3 for different intents
    uint8_t proofing_intent = fdp.ConsumeIntegral<uint8_t>() % 4;
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
    
    // Create proofing transform
    cmsHTRANSFORM proofing_transform = cmsCreateProofingTransform(hInput, input_format,
                                                                 hSRGB, output_format,
                                                                 hProofing,
                                                                 intent, proofing_intent,
                                                                 flags);
    
    if (proofing_transform != NULL) {
        // Test transform with random pixel data
        // Ensure we have at least 1 byte for pixel_count
        if (fdp.remaining_bytes() >= 1) {
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
                cmsDoTransform(proofing_transform, input_buffer.data(), output_buffer.data(), pixel_count);
            }
        }
        
        cmsDeleteTransform(proofing_transform);
    }
    // Test PostScript resource generation with different intents and flags
    // We need: 4 intents * 4 bytes for CSA + 4 intents * 4 bytes for CRD + 4 bytes for no_default = 36 bytes
    if (fdp.remaining_bytes() >= 36) {
        // Test CSA generation
        for (int i = 0; i < 4; i++) {  // Test with different intents
            uint32_t csa_flags = fdp.ConsumeIntegral<uint32_t>();
            GenerateCSA(hInput, i, csa_flags);
            GenerateCSA(hProofing, i, csa_flags);
            GenerateCSA(hSRGB, i, csa_flags);
        }
        
        // Test CRD generation
        for (int i = 0; i < 4; i++) {  // Test with different intents
            uint32_t crd_flags = fdp.ConsumeIntegral<uint32_t>();
            GenerateCRD(hInput, i, crd_flags);
            GenerateCRD(hProofing, i, crd_flags);
            GenerateCRD(hSRGB, i, crd_flags);
        }
        
        // Also test with cmsFLAGS_NODEFAULTRESOURCEDEF flag
        uint32_t no_default_flags = fdp.ConsumeIntegral<uint32_t>() | cmsFLAGS_NODEFAULTRESOURCEDEF;
        GenerateCSA(hInput, intent % 4, no_default_flags);
        GenerateCRD(hInput, intent % 4, no_default_flags);
    }
    // Clean up
    cmsCloseProfile(hSRGB);
    cmsCloseProfile(hProofing);
    cmsCloseProfile(hInput);
    
    return 0;
}

} // namespace harness_001
