#include <fuzzer/FuzzedDataProvider.h>
#include "lcms2.h"
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>

namespace harness_013 {
/*
 * Harness 013 - Profile File I/O Operations Fuzzer for Little CMS 2
 * 
 * Target APIs:
 * - cmsOpenProfileFromFile (Target: File-based profile opening, 49+ undiscovered branches, 0% coverage)
 * - cmsOpenProfileFromStream (Target: Stream-based profile opening, 50+ undiscovered branches, 0% coverage)
 * - cmsOpenProfileFromIOhandlerTHR (Target: I/O handler-based profile opening, 48+ undiscovered branches, 0% coverage)
 * - cmsFormatterForPCSOfProfile (Target: Profile color space formatter, 80+ undiscovered branches, 0% coverage)
 * - cmsGetProfileInfoUTF8 (Required Helper: UTF8 profile metadata, 31+ undiscovered branches)
 * - cmsGetProfileInfoASCII (Required Helper: ASCII profile metadata, 26+ undiscovered branches)
 * - cmsSaveProfileToFile (Required Helper: Profile file saving, 40+ undiscovered branches, 0 hits)
 * - cmsCloseProfile (Required Helper: Profile cleanup, 42+ undiscovered branches)
 * - cmsCreateXYZProfile (Required Helper: XYZ profile creation, has coverage)
 * - cmsCreateRGBProfile (Required Helper: RGB profile creation, has coverage)
 * - cmsCreateGrayProfile (Required Helper: Gray profile creation, has coverage)
 * - cmsCreateProfilePlaceholder (Required Helper: Profile setup, has coverage)
 * 
 * Invocation sequence:
 * 1. Call cmsCreateXYZProfile, cmsCreateRGBProfile, or cmsCreateGrayProfile to create test profiles
 * 2. Call cmsSaveProfileToFile to write profiles to temporary files (exercising this 0-hit API)
 * 3. Call cmsOpenProfileFromFile to open profiles from the filesystem
 * 4. Call cmsOpenProfileFromStream to open profiles from file streams
 * 5. Call cmsGetProfileInfoUTF8 and cmsGetProfileInfoASCII to retrieve profile metadata
 * 6. Call cmsFormatterForPCSOfProfile to get formatter for profile color space
 * 7. Call cmsCloseProfile to clean up opened profiles
 * 8. Repeat with different profile types (XYZ, RGB, Gray) and access modes
 * 
 * These functions represent 284+ points of undiscovered branch complexity and form a logical workflow
 * for profile-based color management. Currently 0% covered despite being fundamental operations,
 * they block deeper coverage of color transformation pipelines.
 * The harness uses temporary files and streams to safely test filesystem operations without side effects.
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for profile operations
    // Need at least: profile_type + access_mode + buffer for file operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size data first
    uint8_t profile_type = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0=XYZ, 1=RGB, 2=Gray
    uint8_t access_mode = fdp.ConsumeIntegral<uint8_t>() % 2;   // 0="r", 1="w"
    uint8_t use_context = fdp.ConsumeBool();
    uint8_t info_type = fdp.ConsumeIntegral<uint8_t>() % 4;     // 0-3 for cmsInfoType enum
    uint8_t nbytes = fdp.ConsumeIntegral<uint8_t>() % 2 + 1;    // 1 or 2 bytes for formatter
    uint8_t lIsFloat = fdp.ConsumeBool();
    
    // Consume language and country codes (2 bytes each + null terminator)
    char language_code[3] = {0};
    char country_code[3] = {0};
    if (fdp.remaining_bytes() >= 4) {
        language_code[0] = fdp.ConsumeIntegral<uint8_t>() % 26 + 'a';
        language_code[1] = fdp.ConsumeIntegral<uint8_t>() % 26 + 'a';
        country_code[0] = fdp.ConsumeIntegral<uint8_t>() % 26 + 'A';
        country_code[1] = fdp.ConsumeIntegral<uint8_t>() % 26 + 'A';
    } else {
        // Default values if not enough data
        strcpy(language_code, "en");
        strcpy(country_code, "US");
    }
    
    // Create context if requested
    cmsContext context = use_context ? cmsCreateContext(NULL, NULL) : NULL;
    
    // Create profile based on type
    cmsHPROFILE hProfile = NULL;
    const char* profile_name = "";
    
    switch (profile_type) {
        case 0:  // XYZ profile
            if (context) {
                hProfile = cmsCreateXYZProfileTHR(context);
            } else {
                hProfile = cmsCreateXYZProfile();
            }
            profile_name = "XYZ";
            break;
            
        case 1:  // RGB profile (simplified sRGB)
            if (context) {
                hProfile = cmsCreate_sRGBProfileTHR(context);
            } else {
                hProfile = cmsCreate_sRGBProfile();
            }
            profile_name = "RGB";
            break;
            
        case 2:  // Gray profile
            if (context) {
                // Create a simple gamma 2.2 curve for gray profile
                cmsToneCurve* gamma22 = cmsBuildGamma(context, 2.2);
                if (gamma22) {
                    hProfile = cmsCreateGrayProfileTHR(context, NULL, gamma22);
                    cmsFreeToneCurve(gamma22);
                }
            } else {
                cmsToneCurve* gamma22 = cmsBuildGamma(NULL, 2.2);
                if (gamma22) {
                    hProfile = cmsCreateGrayProfile(NULL, gamma22);
                    cmsFreeToneCurve(gamma22);
                }
            }
            profile_name = "Gray";
            break;
    }
    
    if (hProfile == NULL) {
        if (context) {
            cmsDeleteContext(context);
        }
        return 0;
    }
    
    // Generate a unique temporary filename using fuzzer input
    char temp_filename[] = "/tmp/lcms_fuzz_XXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        cmsCloseProfile(hProfile);
        if (context) {
            cmsDeleteContext(context);
        }
        return 0;
    }
    close(fd);
    
    const char* mode = (access_mode == 0) ? "r" : "w";
    
    // Test 1: Save profile to file (cmsSaveProfileToFile - 0 hits)
    bool save_success = false;
    if (access_mode == 1) {  // Write mode
        save_success = cmsSaveProfileToFile(hProfile, temp_filename);
        // Don't fail if save fails - it's part of the testing
    }
    
    // Test 2: Open profile from file (cmsOpenProfileFromFile - 49+ branches)
    cmsHPROFILE hFileProfile = NULL;
    if (context) {
        hFileProfile = cmsOpenProfileFromFileTHR(context, temp_filename, mode);
    } else {
        hFileProfile = cmsOpenProfileFromFile(temp_filename, mode);
    }
    
    // Test 3: Open profile from stream (cmsOpenProfileFromStream - 50+ branches)
    cmsHPROFILE hStreamProfile = NULL;
    if (save_success && access_mode == 1) {
        FILE* stream = fopen(temp_filename, "rb");
        if (stream != NULL) {
            if (context) {
                hStreamProfile = cmsOpenProfileFromStreamTHR(context, stream, "r");
            } else {
                hStreamProfile = cmsOpenProfileFromStream(stream, "r");
            }
            // DO NOT call fclose(stream) here - library owns it now
        }
    }
    
    // Test 4: Open profile from I/O handler (cmsOpenProfileFromIOhandlerTHR - 48+ branches)
    cmsHPROFILE hIOProfile = NULL;
    if (context && save_success && access_mode == 1) {
        cmsIOHANDLER* io = cmsOpenIOhandlerFromFile(context, temp_filename, "r");
        if (io != NULL) {
            hIOProfile = cmsOpenProfileFromIOhandlerTHR(context, io);
            // DO NOT call cmsCloseIOhandler(io) here - library owns it now
        }
    }
    
    // Test 5: Get profile info in UTF8 and ASCII (cmsGetProfileInfoUTF8/ASCII - 31+/26+ branches)
    // Use a buffer for profile info retrieval
    char info_buffer[256] = {0};
    
    cmsInfoType lcms_info_type = cmsInfoDescription;
    switch (info_type) {
        case 0: lcms_info_type = cmsInfoDescription; break;
        case 1: lcms_info_type = cmsInfoManufacturer; break;
        case 2: lcms_info_type = cmsInfoModel; break;
        case 3: lcms_info_type = cmsInfoCopyright; break;
    }
    
    cmsUInt32Number utf8_size = cmsGetProfileInfoUTF8(hProfile, lcms_info_type,
                                                     language_code, country_code,
                                                     info_buffer, sizeof(info_buffer));
    (void)utf8_size;  // Suppress unused variable warning
    // Also test ASCII version
    cmsUInt32Number ascii_size = cmsGetProfileInfoASCII(hProfile, lcms_info_type,
                                                       language_code, country_code,
                                                       info_buffer, sizeof(info_buffer));
    (void)ascii_size;  // Suppress unused variable warning
    
    // Test with file-opened profile if available
    if (hFileProfile != NULL) {
        cmsGetProfileInfoUTF8(hFileProfile, lcms_info_type,
                             language_code, country_code,
                             info_buffer, sizeof(info_buffer));
    }
    
    // Test 6: Get formatter for profile color space (cmsFormatterForPCSOfProfile - 80+ branches)
    // Test with original profile
    cmsUInt32Number formatter1 = cmsFormatterForPCSOfProfile(hProfile, nbytes, lIsFloat);
    (void)formatter1;
    
    // Test with file-opened profile if available
    if (hFileProfile != NULL) {
        cmsUInt32Number formatter2 = cmsFormatterForPCSOfProfile(hFileProfile, nbytes, lIsFloat);
        (void)formatter2;
    }
    
    // Test with stream-opened profile if available
    if (hStreamProfile != NULL) {
        cmsUInt32Number formatter3 = cmsFormatterForPCSOfProfile(hStreamProfile, nbytes, lIsFloat);
        (void)formatter3;
    }
    
    // Test with I/O handler-opened profile if available
    if (hIOProfile != NULL) {
        cmsUInt32Number formatter4 = cmsFormatterForPCSOfProfile(hIOProfile, nbytes, lIsFloat);
        (void)formatter4;
    }
    
    // Test 7: Cleanup all opened profiles (cmsCloseProfile - 42+ branches)
    if (hFileProfile != NULL) {
        cmsCloseProfile(hFileProfile);
    }
    
    if (hStreamProfile != NULL) {
        cmsCloseProfile(hStreamProfile);
    }
    
    if (hIOProfile != NULL) {
        cmsCloseProfile(hIOProfile);
    }
    
    // Also test with cmsCreateProfilePlaceholder if we have remaining data
    if (fdp.remaining_bytes() > 10) {
        // Create a placeholder profile for additional testing
        uint32_t placeholder_color_space = fdp.ConsumeIntegral<uint32_t>();
        cmsHPROFILE hPlaceholder = cmsCreateProfilePlaceholder(context);
        if (hPlaceholder != NULL) {
            // Try to save and open the placeholder
            if (access_mode == 1) {
                char placeholder_filename[] = "/tmp/lcms_placeholder_XXXXXX";
                int placeholder_fd = mkstemp(placeholder_filename);
                if (placeholder_fd != -1) {
                    close(placeholder_fd);
                    cmsSaveProfileToFile(hPlaceholder, placeholder_filename);
                    
                    // Open and test placeholder
                    cmsHPROFILE hPlaceholderFile = NULL;
                    if (context) {
                        hPlaceholderFile = cmsOpenProfileFromFileTHR(context, placeholder_filename, "r");
                    } else {
                        hPlaceholderFile = cmsOpenProfileFromFile(placeholder_filename, "r");
                    }
                    
                    if (hPlaceholderFile != NULL) {
                        // Test formatter on placeholder
                        cmsUInt32Number placeholder_formatter = cmsFormatterForPCSOfProfile(
                            hPlaceholderFile, nbytes, lIsFloat);
                        (void)placeholder_formatter;
                        
                        cmsCloseProfile(hPlaceholderFile);
                    }
                    
                    unlink(placeholder_filename);
                }
            }
            
            cmsCloseProfile(hPlaceholder);
        }
    }
    
    // Cleanup main profile
    cmsCloseProfile(hProfile);
    
    // Cleanup context if created
    if (context) {
        cmsDeleteContext(context);
    }
    
    // Cleanup temporary file
    if (access_mode == 1) {
        unlink(temp_filename);
    }
    
    return 0;
}

} // namespace harness_013
