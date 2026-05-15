#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_002 {
/*
 * Fuzzing harness for libcurl targeting curl_multi API for concurrent transfers
 * Harness ID: 002
 * Targets: curl_multi_init, curl_multi_add_handle, curl_multi_perform, 
 *          curl_multi_info_read, curl_multi_remove_handle, curl_multi_cleanup
 * Differentiated from harness_000.cpp (curl_url/curl_easy APIs) and 
 * harness_001.cpp (MIME APIs)
 * Focus: Multi-interface for concurrent HTTP transfers with fuzzed URLs and options
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for multi handle operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Determine number of concurrent transfers (1-8 to avoid excessive resource usage)
    size_t num_transfers = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    
    // Create easy handles for transfers
    std::vector<CURL*> easy_handles;
    easy_handles.reserve(num_transfers);
    
    // Consume URLs and options for each transfer
    std::vector<std::string> urls;
    std::vector<int> http_methods; // 0: GET, 1: POST, 2: PUT, 3: HEAD, 4: DELETE
    std::vector<bool> verbose_flags;
    std::vector<bool> follow_location_flags;
    std::vector<int> timeout_values;
    std::vector<std::string> user_agents;
    std::vector<std::string> post_data;
    
    // Determine actual number of transfers we can create based on available input
    size_t actual_transfers = 0;
    for (size_t i = 0; i < num_transfers && fdp.remaining_bytes() > 32; i++) {
        // Consume URL for this transfer
        std::string url = fdp.ConsumeRandomLengthString(256);
        urls.push_back(url);
        
        // Consume HTTP method
        http_methods.push_back(fdp.ConsumeIntegralInRange<int>(0, 4));
        
        // Consume boolean flags
        verbose_flags.push_back(fdp.ConsumeBool());
        follow_location_flags.push_back(fdp.ConsumeBool());
        
        // Consume timeout value (0-30 seconds)
        timeout_values.push_back(fdp.ConsumeIntegralInRange<int>(0, 30));
        
        // Consume user agent string
        std::string user_agent = fdp.ConsumeRandomLengthString(128);
        user_agents.push_back(user_agent);
        
        // Consume POST data (if method is POST or PUT)
        std::string post = fdp.ConsumeRandomLengthString(1024);
        post_data.push_back(post);
        
        actual_transfers++;
    }
    
    // Update num_transfers to actual number created
    num_transfers = actual_transfers;
    
    // If no transfers could be created, cleanup and return
    if (num_transfers == 0) {
        curl_global_cleanup();
        return 0;
    }
    
    // Create easy handles
    for (size_t i = 0; i < num_transfers; i++) {
        CURL* easy = curl_easy_init();
        if (easy) {
            easy_handles.push_back(easy);
            
            // Set URL if available
            if (!urls[i].empty()) {
                curl_easy_setopt(easy, CURLOPT_URL, urls[i].c_str());
            } else {
                // Use a safe dummy URL to avoid crashes
                curl_easy_setopt(easy, CURLOPT_URL, "http://localhost/");
            }
            
            // Set HTTP method based on fuzzed value
            switch (http_methods[i]) {
                case 0: // GET (default)
                    break;
                case 1: // POST
                    curl_easy_setopt(easy, CURLOPT_POST, 1L);
                    if (!post_data[i].empty()) {
                        curl_easy_setopt(easy, CURLOPT_POSTFIELDS, post_data[i].c_str());
                        curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, post_data[i].size());
                    }
                    break;
                case 2: // PUT
                    curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
                    if (!post_data[i].empty()) {
                        curl_easy_setopt(easy, CURLOPT_READDATA, NULL);
                        // Note: For PUT, we'd need a read callback, but for fuzzing we skip actual data transfer
                    }
                    break;
                case 3: // HEAD
                    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);
                    break;
                case 4: // DELETE
                    curl_easy_setopt(easy, CURLOPT_CUSTOMREQUEST, "DELETE");
                    break;
            }
            
            // Set other options
            curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose_flags[i] ? 1L : 0L);
            curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, follow_location_flags[i] ? 1L : 0L);
            curl_easy_setopt(easy, CURLOPT_TIMEOUT, (long)timeout_values[i]);
            
            if (!user_agents[i].empty()) {
                curl_easy_setopt(easy, CURLOPT_USERAGENT, user_agents[i].c_str());
            }
            
            // Set a write callback that discards data (since we're not making actual network calls)
            curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, 
                            [](void* buffer, size_t size, size_t nmemb, void* userp) -> size_t {
                                return size * nmemb;  // Pretend we wrote everything
                            });
            
            // Disable actual network connections for fuzzing safety
            curl_easy_setopt(easy, CURLOPT_CONNECT_ONLY, 1L);
        }
    }
    
    // If no easy handles were created, cleanup and return
    if (easy_handles.empty()) {
        curl_global_cleanup();
        return 0;
    }
    
    // ==================== Multi API Testing ====================
    
    // Create multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        // Clean up easy handles
        for (CURL* easy : easy_handles) {
            curl_easy_cleanup(easy);
        }
        curl_global_cleanup();
        return 0;
    }
    
    // Add all easy handles to multi handle
    CURLMcode mcode;
    for (CURL* easy : easy_handles) {
        mcode = curl_multi_add_handle(multi_handle, easy);
        // Continue even if some adds fail
    }
    
    // Perform transfers in a loop (simulated, since we disabled network)
    int still_running = 0;
    int max_iterations = fdp.ConsumeIntegralInRange<int>(1, 10);
    
    for (int iter = 0; iter < max_iterations && fdp.remaining_bytes() > 8; iter++) {
        // Call curl_multi_perform
        mcode = curl_multi_perform(multi_handle, &still_running);
        
        // Check for messages after each perform
        int msgs_in_queue = 0;
        CURLMsg* msg;
        while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue)) != NULL) {
            if (msg->msg == CURLMSG_DONE) {
                // Transfer completed - could remove handle here, but we'll keep it for testing
                // Just record that a transfer would have completed
            }
        }
        
        // Simulate waiting/polling (without actual waiting)
        if (still_running > 0) {
            // In real code, we would call curl_multi_poll or curl_multi_wait here
            // For fuzzing, we just simulate the API calls
            int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 1000);
            int numfds = 0;
            
            // Test curl_multi_wait with dummy parameters
            struct curl_waitfd extra_fds[1];
            extra_fds[0].fd = 0;
            extra_fds[0].events = CURL_WAIT_POLLIN;
            extra_fds[0].revents = 0;
            
            mcode = curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &numfds);
            
            // Also test curl_multi_poll (newer API)
            mcode = curl_multi_poll(multi_handle, extra_fds, 1, timeout_ms, &numfds);
        }
        
        // Sometimes remove and re-add a handle to test that flow
        // FIX: Check if easy_handles is not empty before accessing
        if (fdp.ConsumeBool() && !easy_handles.empty() && fdp.remaining_bytes() > 4) {
            size_t idx = fdp.ConsumeIntegralInRange<size_t>(0, easy_handles.size() - 1);
            CURL* easy_to_remove = easy_handles[idx];
            
            mcode = curl_multi_remove_handle(multi_handle, easy_to_remove);
            
            // Re-add it if we removed it successfully
            if (mcode == CURLM_OK) {
                mcode = curl_multi_add_handle(multi_handle, easy_to_remove);
            }
        }
        
        // Break early if perform failed
        if (mcode != CURLM_OK && mcode != CURLM_CALL_MULTI_PERFORM) {
            break;
        }
    }
    
    // Test curl_multi_info_read more thoroughly after loop
    int msgs_in_queue = 0;
    CURLMsg* msg;
    while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue)) != NULL) {
        // Process message - in real code we would check msg->msg and msg->data.result
        // For fuzzing, we just exercise the API
        if (msg->msg == CURLMSG_DONE) {
            // Transfer completed message
        }
    }
    
    // Test removing all handles individually
    for (CURL* easy : easy_handles) {
        mcode = curl_multi_remove_handle(multi_handle, easy);
        // Continue even if some removes fail
    }
    
    // Test adding handles back in different order
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        for (CURL* easy : easy_handles) {
            mcode = curl_multi_add_handle(multi_handle, easy);
        }
        
        // Perform once more
        int temp_running = 0;
        mcode = curl_multi_perform(multi_handle, &temp_running);
        
        // Remove all handles again
        for (CURL* easy : easy_handles) {
            curl_multi_remove_handle(multi_handle, easy);
        }
    }
    
    // Clean up multi handle
    mcode = curl_multi_cleanup(multi_handle);
    
    // Clean up easy handles
    for (CURL* easy : easy_handles) {
        curl_easy_cleanup(easy);
    }
    
    // Test error handling with bad multi handle
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 4) {
        // Try to perform with NULL multi handle
        int dummy_running = 0;
        CURLMcode bad_code = curl_multi_perform(NULL, &dummy_running);
        
        // Try to add handle with NULL multi handle
        if (!easy_handles.empty()) {
            bad_code = curl_multi_add_handle(NULL, easy_handles[0]);
        }
        
        // Try to remove handle with NULL multi handle
        if (!easy_handles.empty()) {
            bad_code = curl_multi_remove_handle(NULL, easy_handles[0]);
        }
        
        // Try to cleanup NULL multi handle
        bad_code = curl_multi_cleanup(NULL);
        
        // Try to read info from NULL multi handle
        int dummy_msgs = 0;
        CURLMsg* dummy_msg = curl_multi_info_read(NULL, &dummy_msgs);
    }
    
    // Clean up global curl state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_002
