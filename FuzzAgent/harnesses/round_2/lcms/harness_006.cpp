#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include "lcms2.h"
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>
#include <string>

namespace harness_006 {
// Fuzzing harness for LittleCMS2 library
// Targets cmsTransform2DeviceLink (409 undiscovered branches) and related profile I/O operations
// harness_006.cpp - Focus on device link creation and profile I/O functions
// Semantic diversity: harness_000 (basic profile parsing), harness_001 (proofing/PostScript), 
// harness_002 (virtual profiles/gamut), harness_003 (CGATS/IT8), harness_004 (CIECAM02), 
// harness_005 (TAC detection/sampling)
// This harness targets device link creation and profile I/O operations


// Helper function to create a temporary filename for testing file I/O
static std::string create_temp_filename(FuzzedDataProvider& fdp) {
    const char temp_prefix[] = "/tmp/lcms_fuzz_";
    std::string filename = temp_prefix;
    
    // Append random characters from fuzzer input
    if (fdp.remaining_bytes() > 0) {
        size_t name_len = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        std::vector<uint8_t> random_bytes = fdp.ConsumeBytes<uint8_t>(name_len);
        for (uint8_t byte : random_bytes) {
            // Convert to valid filename characters
            char c = 'a' + (byte % 26);
            filename.push_back(c);
        }
    } else {
        filename += "default";
    }
    filename += ".icc";
    return filename;
}

// Main fuzzer entry point for device link creation and profile I/O
int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 300) {
        return 0;  // Need minimum data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input into multiple parts for different test phases
    // Part 1: Profile data for input/output profiles
    size_t profile1_size = fdp.ConsumeIntegralInRange<size_t>(100, size / 3);
    std::vector<uint8_t> profile1_data = fdp.ConsumeBytes<uint8_t>(profile1_size);
    
    size_t profile2_size = fdp.ConsumeIntegralInRange<size_t>(100, fdp.remaining_bytes() / 2);
    std::vector<uint8_t> profile2_data = fdp.ConsumeBytes<uint8_t>(profile2_size);
    
    // Part 2: Transform parameters
    cmsUInt32Number input_format = fdp.ConsumeIntegral<uint32_t>();
    cmsUInt32Number output_format = fdp.ConsumeIntegral<uint32_t>();
    cmsUInt32Number intent = fdp.ConsumeIntegralInRange<cmsUInt32Number>(0, 15);  // 0-3 are standard ICC intents
    cmsUInt32Number transform_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // Part 3: Device link parameters
    cmsFloat64Number version = fdp.ConsumeFloatingPointInRange<cmsFloat64Number>(2.0, 4.4);
    cmsUInt32Number device_link_flags = fdp.ConsumeIntegral<uint32_t>();
    
    // TEST PHASE 1: Open or create profiles
    cmsHPROFILE hInput = NULL;
    cmsHPROFILE hOutput = NULL;
    
    // Try to open first profile from memory
    if (profile1_data.size() >= 128) {
        hInput = cmsOpenProfileFromMem(profile1_data.data(), (cmsUInt32Number)profile1_data.size());
    }
    
    // If we couldn't open a profile, create a built-in profile
    if (hInput == NULL) {
        hInput = cmsCreate_sRGBProfile();
    }
    
    // Try to open second profile from memory
    if (profile2_data.size() >= 128) {
        hOutput = cmsOpenProfileFromMem(profile2_data.data(), (cmsUInt32Number)profile2_data.size());
    }
    
    // If we couldn't open a second profile, create another built-in profile
    if (hOutput == NULL) {
        // Try different profile types based on fuzzed data
        uint8_t profile_type = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (profile_type) {
            case 0:
                hOutput = cmsCreateLab2Profile(NULL);
                break;
            case 1:
                hOutput = cmsCreateLab4Profile(NULL);
                break;
            case 2:
                hOutput = cmsCreateXYZProfile();
                break;
            case 3:
                hOutput = cmsCreate_sRGBProfile();  // Use sRGB again
                break;
            default:
                hOutput = cmsCreate_sRGBProfile();
                break;
        }
    }
    
    // Ensure we have valid profiles
    if (hInput == NULL || hOutput == NULL) {
        if (hInput) cmsCloseProfile(hInput);
        if (hOutput) cmsCloseProfile(hOutput);
        return 0;  // Can't proceed without profiles
    }
    
    // TEST PHASE 2: Create transform between profiles
    cmsHTRANSFORM hTransform = cmsCreateTransform(hInput, input_format, hOutput, output_format, intent, transform_flags);
    
    if (hTransform == NULL) {
        // Try with different flags if first attempt fails
        transform_flags = cmsFLAGS_NOCACHE | cmsFLAGS_NOOPTIMIZE;
        hTransform = cmsCreateTransform(hInput, input_format, hOutput, output_format, intent, transform_flags);
        
        if (hTransform == NULL) {
            cmsCloseProfile(hInput);
            cmsCloseProfile(hOutput);
            return 0;  // Can't create transform
        }
    }
    
    // TEST PHASE 3: PRIMARY TARGET - Convert transform to device link (409 undiscovered branches)
    cmsHPROFILE hDeviceLink = cmsTransform2DeviceLink(hTransform, version, device_link_flags);
    
    // TEST PHASE 4: Profile I/O operations with the device link
    
    // Test 4a: Save device link to memory
    if (hDeviceLink != NULL) {
        // First call to get required buffer size
        cmsUInt32Number bytes_needed = 0;
        cmsBool size_result = cmsSaveProfileToMem(hDeviceLink, NULL, &bytes_needed);
        
        // If we got the size, try to save with actual buffer
        if (size_result && bytes_needed > 0 && bytes_needed < 10 * 1024 * 1024) {  // Reasonable size limit
            std::vector<uint8_t> buffer(bytes_needed);
            cmsBool save_result = cmsSaveProfileToMem(hDeviceLink, buffer.data(), &bytes_needed);
            // Result can be success or failure, both are valid test cases
        }
        
        // Test 4b: Save device link to file
        std::string temp_filename = create_temp_filename(fdp);
        cmsBool file_result = cmsSaveProfileToFile(hDeviceLink, temp_filename.c_str());
        
        // If file was saved successfully, try to reopen it
        if (file_result) {
            cmsHPROFILE hReopened = cmsOpenProfileFromFile(temp_filename.c_str(), "r");
            if (hReopened != NULL) {
                // Quick validation of reopened profile
                cmsColorSpaceSignature cs = cmsGetColorSpace(hReopened);
                cmsProfileClassSignature cl = cmsGetDeviceClass(hReopened);
                cmsCloseProfile(hReopened);
            }
            
            // Clean up temporary file
            remove(temp_filename.c_str());
        }
        
        // Test 4c: Additional profile I/O tests with device link
        
        // Get profile info
        cmsUInt32Number profile_size = cmsGetProfileInfo(hDeviceLink, cmsInfoDescription, "en", "US", NULL, 0);
        if (profile_size > 0 && profile_size < 4096) {
            wchar_t* desc_buffer = (wchar_t*)malloc(profile_size);
            if (desc_buffer != NULL) {
                cmsGetProfileInfo(hDeviceLink, cmsInfoDescription, "en", "US", desc_buffer, profile_size);
                free(desc_buffer);
            }
        }
        
        // Get header info using available header functions
        cmsUInt32Number flags = cmsGetHeaderFlags(hDeviceLink);
        cmsUInt64Number attributes = 0;
        cmsGetHeaderAttributes(hDeviceLink, &attributes);
        cmsUInt32Number rendering_intent = cmsGetHeaderRenderingIntent(hDeviceLink);
        cmsUInt32Number manufacturer = cmsGetHeaderManufacturer(hDeviceLink);
        cmsUInt32Number creator = cmsGetHeaderCreator(hDeviceLink);
        cmsUInt32Number model = cmsGetHeaderModel(hDeviceLink);
        
        // Close device link profile
        cmsCloseProfile(hDeviceLink);
    }
    // Test actual color transformation if we have remaining input
    if (fdp.remaining_bytes() > 100 && hTransform != NULL) {
        size_t pixel_count = fdp.ConsumeIntegralInRange<size_t>(1, 16);
        size_t buffer_size = pixel_count * 16;  // Conservative estimate
        
        if (buffer_size <= fdp.remaining_bytes()) {
            std::vector<uint8_t> input_buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
            std::vector<uint8_t> output_buffer(buffer_size);
            
            // Perform transformation
            cmsDoTransform(hTransform, input_buffer.data(), output_buffer.data(), (cmsUInt32Number)pixel_count);
        }
    }
    
    // TEST PHASE 6: Test with different device link flags combinations
    
    // Try creating device link with specific flag combinations
    if (hTransform != NULL) {
        uint8_t flag_combo = fdp.ConsumeIntegral<uint8_t>() % 8;
        cmsUInt32Number test_flags = 0;
        
        switch (flag_combo) {
            case 0:
                test_flags = cmsFLAGS_8BITS_DEVICELINK;
                break;
            case 1:
                test_flags = cmsFLAGS_GUESSDEVICECLASS;
                break;
            case 2:
                test_flags = cmsFLAGS_KEEP_SEQUENCE;
                break;
            case 3:
                test_flags = cmsFLAGS_8BITS_DEVICELINK | cmsFLAGS_GUESSDEVICECLASS;
                break;
            case 4:
                test_flags = cmsFLAGS_8BITS_DEVICELINK | cmsFLAGS_KEEP_SEQUENCE;
                break;
            case 5:
                test_flags = cmsFLAGS_GUESSDEVICECLASS | cmsFLAGS_KEEP_SEQUENCE;
                break;
            case 6:
                test_flags = cmsFLAGS_8BITS_DEVICELINK | cmsFLAGS_GUESSDEVICECLASS | cmsFLAGS_KEEP_SEQUENCE;
                break;
            default:
                test_flags = 0;  // No special flags
                break;
        }
        
        cmsHPROFILE hDeviceLink2 = cmsTransform2DeviceLink(hTransform, version, test_flags);
        if (hDeviceLink2 != NULL) {
            cmsCloseProfile(hDeviceLink2);
        }
    }
    
    // Cleanup
    if (hTransform != NULL) {
        cmsDeleteTransform(hTransform);
    }
    cmsCloseProfile(hInput);
    cmsCloseProfile(hOutput);
    
    return 0;  // Always return 0
}

} // namespace harness_006
