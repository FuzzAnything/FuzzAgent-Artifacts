#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>
#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"

namespace harness_016 {
// Fuzzing harness for LittleCMS (lcms) profile information retrieval functions
// This harness specifically targets cmsGetProfileInfoUTF8 and cmsGetProfileInfoASCII
// which have 0% coverage according to the coverage analysis report


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed: some profile data + parameters for info functions
    if (size < 64) {
        return 0;  // Need sufficient data for meaningful testing
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Split input: first part for profile data, rest for fuzzing parameters
    // Reserve at least 32 bytes for parameters (info type, language codes, buffer size)
    size_t min_param_size = 32;
    if (size < min_param_size) return 0;
    
    size_t max_profile_size = size - min_param_size;
    size_t profile_size = fdp.ConsumeIntegralInRange<size_t>(0, max_profile_size);
    std::vector<uint8_t> profile_data = fdp.ConsumeBytes<uint8_t>(profile_size);
    
    cmsHPROFILE hProfile = NULL;
    
    // Try to open the input as an ICC profile from memory
    if (profile_size > 0) {
        hProfile = cmsOpenProfileFromMem(profile_data.data(), 
                                         static_cast<cmsUInt32Number>(profile_data.size()));
    }
    
    // If memory-based profile opening failed, try creating built-in profiles
    if (hProfile == NULL) {
        // Use alternative initialization as suggested in the guidance
        // Try different profile creation functions
        int profile_choice = fdp.ConsumeIntegralInRange<int>(0, 2);
        switch (profile_choice) {
            case 0:
                hProfile = cmsCreate_sRGBProfile();
                break;
            case 1:
                hProfile = cmsCreateXYZProfile();
                break;
            case 2:
                hProfile = cmsCreateNULLProfile();
                break;
            default:
                hProfile = cmsCreate_sRGBProfile();
                break;
        }
        
        // If all creation attempts fail, return early
        if (hProfile == NULL) {
            return 0;
        }
    }
    
    // Now test the profile information retrieval functions
    
    // Consume parameters for info functions
    // Info type: 0-3 (cmsInfoDescription, cmsInfoManufacturer, cmsInfoModel, cmsInfoCopyright)
    cmsInfoType info_type = static_cast<cmsInfoType>(
        fdp.ConsumeIntegralInRange<int>(0, 3));
    
    // Language code (2 characters + null terminator)
    char language_code[3] = {0};
    if (fdp.remaining_bytes() >= 2) {
        language_code[0] = fdp.ConsumeIntegralInRange<char>('a', 'z');
        language_code[1] = fdp.ConsumeIntegralInRange<char>('a', 'z');
    } else {
        // Default to "en"
        language_code[0] = 'e';
        language_code[1] = 'n';
    }
    
    // Country code (2 characters + null terminator)
    char country_code[3] = {0};
    if (fdp.remaining_bytes() >= 2) {
        country_code[0] = fdp.ConsumeIntegralInRange<char>('A', 'Z');
        country_code[1] = fdp.ConsumeIntegralInRange<char>('A', 'Z');
    } else {
        // Default to "US"
        country_code[0] = 'U';
        country_code[1] = 'S';
    }
    
    // Determine buffer size for info retrieval
    // Use remaining bytes to determine buffer size, but limit to reasonable size
    size_t max_buffer_size = fdp.remaining_bytes();
    if (max_buffer_size > 4096) max_buffer_size = 4096;  // Reasonable limit
    
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, max_buffer_size);
    
    // Allocate buffers for both UTF8 and ASCII versions
    char* utf8_buffer = new char[buffer_size];
    char* ascii_buffer = new char[buffer_size];
    
    if (utf8_buffer == NULL || ascii_buffer == NULL) {
        // Cleanup if allocation failed
        delete[] utf8_buffer;
        delete[] ascii_buffer;
        cmsCloseProfile(hProfile);
        return 0;
    }
    
    // Initialize buffers
    memset(utf8_buffer, 0, buffer_size);
    memset(ascii_buffer, 0, buffer_size);
    
    // Test cmsGetProfileInfoUTF8
    cmsUInt32Number utf8_result = cmsGetProfileInfoUTF8(
        hProfile, 
        info_type,
        language_code,
        country_code,
        utf8_buffer,
        static_cast<cmsUInt32Number>(buffer_size));
    
    // Test cmsGetProfileInfoASCII  
    cmsUInt32Number ascii_result = cmsGetProfileInfoASCII(
        hProfile,
        info_type,
        language_code,
        country_code,
        ascii_buffer,
        static_cast<cmsUInt32Number>(buffer_size));
    
    // Also test with different buffer sizes to explore edge cases
    if (buffer_size > 1) {
        // Test with smaller buffer
        char small_buffer[1] = {0};
        cmsUInt32Number small_result = cmsGetProfileInfoUTF8(
            hProfile,
            info_type,
            language_code,
            country_code,
            small_buffer,
            1);
    }
    
    // Test with empty language/country codes (NULL pointers are allowed per documentation)
    char empty_lang[3] = {0};
    char empty_country[3] = {0};
    
    cmsUInt32Number empty_result = cmsGetProfileInfoASCII(
        hProfile,
        info_type,
        empty_lang,
        empty_country,
        ascii_buffer,
        static_cast<cmsUInt32Number>(buffer_size));
    
    // Clean up
    delete[] utf8_buffer;
    delete[] ascii_buffer;
    cmsCloseProfile(hProfile);
    
    return 0;
}

} // namespace harness_016
