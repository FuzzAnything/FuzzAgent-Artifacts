#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>

namespace harness_015 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for configuration and multiple handles
    if (size < 128) {
        return 0; // Need minimum input for meaningful multi interface testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally (as per guidance sequence step 1)
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create multiple easy handles (variation: test multiple handles as suggested)
    // Determine number of easy handles from fuzzer input (1-5 for fuzzing safety)
    uint8_t num_easy_handles = fdp.ConsumeIntegralInRange<uint8_t>(1, 5);
    std::vector<CURL*> easy_handles;
    
    for (uint8_t i = 0; i < num_easy_handles; i++) {
        CURL* easy_handle = curl_easy_init();
        if (!easy_handle) {
            // Clean up any already created handles
            for (CURL* h : easy_handles) {
                curl_easy_cleanup(h);
            }
            curl_global_cleanup();
            return 0;
        }
        easy_handles.push_back(easy_handle);
        
        // Step 3: Configure each easy handle with options from fuzzer input
        // Consume URL string for this handle
        std::string url = fdp.ConsumeRandomLengthString(256);
        if (url.empty()) {
            // Use different dummy URLs for variety
            const char* dummy_urls[] = {
                "http://localhost/test",
                "https://example.com/data",
                "ftp://test.server/file.txt"
            };
            url = dummy_urls[i % 3];
        }
        
        // Set URL (CURLOPT_URL)
        curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
        
        // Consume and set various options to exercise setopt.c (low coverage)
        bool verbose = fdp.ConsumeBool();
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        
        bool follow_location = fdp.ConsumeBool();
        curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
        
        // Set timeout to prevent hanging
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 10L);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 10L);
        
        // Disable signal handling
        curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
        
        // Consume and set additional options based on fuzzer input
        uint8_t option_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        switch (option_choice) {
            case 0:
                curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, 
                                 fdp.ConsumeIntegralInRange<long>(0, 10));
                break;
            case 1:
                curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, 
                                 fdp.ConsumeRandomLengthString(64).c_str());
                break;
            case 2:
                curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, 
                                 fdp.ConsumeIntegralInRange<long>(CURL_HTTP_VERSION_NONE, 
                                                                 CURL_HTTP_VERSION_2_0));
                break;
            case 3:
                curl_easy_setopt(easy_handle, CURLOPT_FAILONERROR, 
                                 fdp.ConsumeBool() ? 1L : 0L);
                break;
        }
    }

    // Step 4: Initialize multi handle (guidance step 4)
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        for (CURL* h : easy_handles) {
            curl_easy_cleanup(h);
        }
        curl_global_cleanup();
        return 0;
    }

    // Step 5: Add easy handles to multi handle (guidance step 5)
    std::vector<CURLMcode> add_results;
    for (CURL* easy_handle : easy_handles) {
        CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
        add_results.push_back(add_result);
        // Continue even if some adds fail
    }

    // Step 6: Perform multi operations (guidance step 6)
    int running_handles = 0;
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    
    // Step 7: Wait for activity (guidance step 7)
    // Consume timeout value from fuzzer input
    int timeout_ms = fdp.ConsumeIntegralInRange<int>(-1, 100);
    int num_fds = 0;
    CURLMcode wait_result = curl_multi_wait(multi_handle, NULL, 0, timeout_ms, &num_fds);

    // Step 8: PRIMARY TARGET - curl_multi_get_handles (0% coverage, 10 undiscovered branches)
    // This is the main target API from the guidance
    CURL** handle_array = curl_multi_get_handles(multi_handle);
    
    if (handle_array) {
        // Process the array - it's NULL-terminated
        for (CURL** ptr = handle_array; *ptr != NULL; ptr++) {
            // We could do something with each handle, but for fuzzing we just verify
            // that we can access them
            (void)ptr; // Mark as used
        }
        
        // The array is allocated by curl, we need to free it
        // According to documentation, it's allocated and should be freed
        curl_free(handle_array);
    }
    
    // Additional testing: call get_handles multiple times with different states
    if (fdp.ConsumeBool()) {
        // Call get_handles again
        CURL** handle_array2 = curl_multi_get_handles(multi_handle);
        if (handle_array2) {
            curl_free(handle_array2);
        }
    }

    // Step 9: Remove handles from multi (guidance step 9)
    for (size_t i = 0; i < easy_handles.size(); i++) {
        if (add_results[i] == CURLM_OK) {
            CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handles[i]);
            (void)remove_result; // Result not critical for fuzzing
        }
    }

    // Step 10: Get info from easy handles (guidance step 10 - target getinfo.c low coverage)
    for (CURL* easy_handle : easy_handles) {
        // Try to get different types of info based on fuzzer input
        uint8_t info_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
        
        switch (info_choice) {
            case 0: {
                // Get response code
                long response_code = 0;
                curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
                break;
            }
            case 1: {
                // Get total time
                double total_time = 0.0;
                curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);
                break;
            }
            case 2: {
                // Get content type
                char* content_type = NULL;
                curl_easy_getinfo(easy_handle, CURLINFO_CONTENT_TYPE, &content_type);
                break;
            }
            case 3: {
                // Get effective URL
                char* effective_url = NULL;
                curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
                break;
            }
        }
    }

    // Step 11: Clean up easy handles (guidance step 11)
    for (CURL* h : easy_handles) {
        curl_easy_cleanup(h);
    }

    // Step 12: Clean up multi handle (guidance step 12)
    curl_multi_cleanup(multi_handle);

    // Step 13: Global cleanup (guidance step 13)
    curl_global_cleanup();

    return 0;
}

} // namespace harness_015
