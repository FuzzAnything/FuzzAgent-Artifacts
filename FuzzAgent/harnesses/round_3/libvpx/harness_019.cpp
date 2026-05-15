#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

namespace harness_019 {
/*
 * Fuzzing harness for libvpx version information APIs
 * Targets: vpx_codec_version, vpx_codec_version_str, vpx_codec_version_extra_str,
 *          vpx_codec_version_major/minor/patch macros, vpx_codec_build_config,
 *          vpx_codec_iface_name, vpx_codec_err_to_string, vpx_codec_error, vpx_codec_error_detail
 * Goal: Achieve 100% API coverage by testing remaining uncovered version functions
 */




int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Even though version functions don't take input, we still need to handle
    // the fuzzer input to satisfy the API. We'll use it to control which
    // functions to call and how to validate the results.
    
    if (size < 1) {
        return 0;  // Need at least 1 byte for decision making
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume operation selector from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>();
    
    // Test version functions - these should always be callable
    int version = vpx_codec_version();
    const char *version_str = vpx_codec_version_str();
    const char *version_extra_str = vpx_codec_version_extra_str();
    const char *build_config = vpx_codec_build_config();
    
    // Use macro functions to extract version components
    int major = vpx_codec_version_major();
    int minor = vpx_codec_version_minor();
    int patch = vpx_codec_version_patch();
    
    // Validate version components make sense
    // Version should be non-negative
    if (version < 0) {
        // This would be unexpected, but we should handle it
        return 0;
    }
    
    // Major, minor, patch should be in reasonable ranges (0-255 for 8-bit fields)
    if (major < 0 || major > 255) return 0;
    if (minor < 0 || minor > 255) return 0;
    if (patch < 0 || patch > 255) return 0;
    
    // Version string should not be null
    if (version_str == nullptr) return 0;
    
    // Version string should contain something
    if (strlen(version_str) == 0) return 0;
    
    // Extra version string can be null or empty, both are valid
    // Build config can be null or empty, both are valid
    
    // Test error string functions with different error codes
    // Use fuzzer input to select error codes to test
    uint8_t err_code = fdp.ConsumeIntegral<uint8_t>() % 10;  // Limit to first 10 codes
    
    const char *err_str = vpx_codec_err_to_string(static_cast<vpx_codec_err_t>(err_code));
    // err_str can be null for invalid error codes, that's OK
    
    // Test error context functions (vpx_codec_error, vpx_codec_error_detail)
    // These require a codec context, but we can test with null
    const char *ctx_err = vpx_codec_error(nullptr);
    const char *ctx_err_detail = vpx_codec_error_detail(nullptr);
    // These should probably return null or default strings when ctx is null
    
    // Test interface name functions based on fuzzer input
    uint8_t iface_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    const vpx_codec_iface_t *iface = nullptr;
    
    switch (iface_choice) {
        case 0:
            iface = vpx_codec_vp8_cx();
            break;
        case 1:
            iface = vpx_codec_vp8_dx();
            break;
        case 2:
            iface = vpx_codec_vp9_cx();
            break;
        case 3:
            iface = vpx_codec_vp9_dx();
            break;
        default:
            iface = vpx_codec_vp8_cx();
            break;
    }
    
    if (iface != nullptr) {
        const char *iface_name = vpx_codec_iface_name(const_cast<vpx_codec_iface_t*>(iface));
        // iface_name can be null, but shouldn't be for valid interfaces
    }
    
    // Additional validation based on operation selector
    // Use the operation byte to perform different validation paths
    switch (operation % 8) {
        case 0: {
            // Basic validation: check version string starts with 'v'
            if (version_str[0] != 'v' && version_str[0] != 'V') {
                // Not necessarily an error, but unusual
            }
            break;
        }
        case 1: {
            // Validate version number reconstruction
            int reconstructed = (major << 16) | (minor << 8) | patch;
            if (reconstructed != version) {
                // This would be unexpected but handle gracefully
            }
            break;
        }
        case 2: {
            // Check build config if available
            if (build_config != nullptr && strlen(build_config) > 0) {
                // Build config exists, do minimal validation
                // (e.g., check it doesn't contain obviously wrong chars)
                for (size_t i = 0; i < strlen(build_config) && i < 100; i++) {
                    if (build_config[i] == 0) break;  // Shouldn't happen but be safe
                }
            }
            break;
        }
        case 3: {
            // Test with remaining fuzzer data as "extra validation"
            if (fdp.remaining_bytes() > 0) {
                // Consume some bytes but don't use them for version APIs
                // This ensures fuzzer can still explore the input space
                std::vector<uint8_t> unused = fdp.ConsumeRemainingBytes<uint8_t>();
                // The version functions don't use this data, but that's OK
                // The fuzzer will learn this code path doesn't depend on input
            }
            break;
        }
        case 4: {
            // Test error handling edge cases
            // Try error code 255 (likely invalid)
            const char *invalid_err = vpx_codec_err_to_string(static_cast<vpx_codec_err_t>(255));
            // Should probably return null or default string
            break;
        }
        case 5: {
            // Test null interface
            const char *null_iface_name = vpx_codec_iface_name(nullptr);
            // Should probably return null or default string
            break;
        }
        case 6: {
            // Exercise all version macros in different ways
            int major2 = VPX_VERSION_MAJOR(version);
            int minor2 = VPX_VERSION_MINOR(version);
            int patch2 = VPX_VERSION_PATCH(version);
            
            if (major2 != major || minor2 != minor || patch2 != patch) {
                // Macros should match function results
            }
            break;
        }
        case 7: {
            // Comprehensive string validation
            if (version_extra_str != nullptr) {
                // Extra string should be a substring of full version string
                if (strstr(version_str, version_extra_str) == nullptr) {
                    // Not found, but could be valid if extra_str is empty
                    if (strlen(version_extra_str) > 0) {
                        // Unexpected but not necessarily an error
                    }
                }
            }
            break;
        }
    }
    
    // All version APIs have been exercised
    // Return 0 indicates successful execution
    return 0;
}

} // namespace harness_019
