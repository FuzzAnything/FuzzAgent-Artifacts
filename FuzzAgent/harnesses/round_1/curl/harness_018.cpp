#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_018 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size: need enough for SSL backend ID and name testing
    if (size < 16) {
        return 0; // Need minimum input for meaningful SSL backend testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: curl_global_init (required setup) - as per guidance
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Test curl_global_sslset with various configurations - primary target
    // Consume SSL backend ID from fuzzer input
    uint32_t backend_id_raw = fdp.ConsumeIntegral<uint32_t>();
    // Map to valid curl_sslbackend enum range (0-14 based on enum values)
    curl_sslbackend backend_id = static_cast<curl_sslbackend>(backend_id_raw % 15);
    
    // Consume SSL backend name from fuzzer input (could be NULL or string)
    std::string backend_name;
    const char* name_ptr = nullptr;
    
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        // Use a string for the backend name
        backend_name = fdp.ConsumeRandomLengthString(64);
        name_ptr = backend_name.c_str();
    }
    // else: name_ptr remains NULL (testing NULL parameter case)
    // Pointer to receive available backends list (third parameter is pointer to pointer)
    const curl_ssl_backend*** avail_list_ptr = nullptr;
    if (fdp.ConsumeBool()) {
        // Test with non-NULL avail pointer to get list of available backends
        static const curl_ssl_backend** static_avail_ptr = nullptr;
        avail_list_ptr = &static_avail_ptr;
    }
    // else: avail_list_ptr remains NULL (testing NULL parameter case)

    // Call the target API: curl_global_sslset - primary focus with 0% coverage
    CURLsslset sslset_result = curl_global_sslset(backend_id, name_ptr, avail_list_ptr);
    
    // Handle the result and potentially examine available backends
    switch (sslset_result) {
        case CURLSSLSET_OK:
            // SSL backend successfully set
            break;
        case CURLSSLSET_UNKNOWN_BACKEND:
            // Backend not available - check avail list if we provided pointer
            if (avail_list_ptr && *avail_list_ptr) {
                // Iterate through available backends (just for coverage)
                const curl_ssl_backend** backends = *avail_list_ptr;
                if (backends) {
                    for (int i = 0; backends[i] != nullptr; i++) {
                        // Access fields for coverage
                        (void)backends[i]->id;
                        (void)backends[i]->name;
                    }
                }
            }
            break;
        case CURLSSLSET_TOO_LATE:
            // SSL backend already set (can happen on repeated calls)
            break;
        case CURLSSLSET_NO_BACKENDS:
            // libcurl built without SSL support
            break;
        default:
            // Unknown return value
            break;
    }

    // Step 3: Test multiple calls with different parameters for more coverage
    // Try a second call with different parameters (should likely return CURLSSLSET_TOO_LATE)
    if (fdp.remaining_bytes() > 8) {
        uint32_t second_backend_id_raw = fdp.ConsumeIntegral<uint32_t>();
        curl_sslbackend second_backend_id = static_cast<curl_sslbackend>(second_backend_id_raw % 15);
        
        std::string second_name;
        const char* second_name_ptr = nullptr;
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
            second_name = fdp.ConsumeRandomLengthString(32);
            second_name_ptr = second_name.c_str();
        }
        
        // Second call - should typically return TOO_LATE but exercises error paths
        CURLsslset second_result = curl_global_sslset(second_backend_id, second_name_ptr, nullptr);
        (void)second_result;
    }

    // Step 4: Test curl_version_info to get SSL backend info (optional helper)
    // Use remaining bytes to determine version stamp
    if (fdp.remaining_bytes() > 4) {
        CURLversion version_stamp = static_cast<CURLversion>(fdp.ConsumeIntegral<uint32_t>() % 5);
        curl_version_info_data* version_info = curl_version_info(version_stamp);
        
        // Check SSL features in version info for coverage
        if (version_info) {
            (void)version_info->ssl_version;
            (void)version_info->ssl_version_num;
            (void)version_info->features;
        }
    }

    // Step 5: curl_global_cleanup (required cleanup) - as per guidance
    curl_global_cleanup();

    return 0;
}

} // namespace harness_018
