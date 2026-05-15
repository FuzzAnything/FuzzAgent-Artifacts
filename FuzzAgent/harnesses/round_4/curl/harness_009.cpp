#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"

namespace harness_009 {
/*
 * Fuzzing harness for libcurl targeting advanced cURL Easy and Multi interfaces
 * Harness ID: 009
 * Targets: Advanced curl_easy and curl_multi APIs with complex option setting,
 *          error handling, state transitions, and sophisticated multi-handle operations
 * Focus: Deep coverage of core cURL Easy and Multi interfaces identified as most critical
 *        coverage gap with only 0.59% API coverage overall
 * Differentiated from:
 * - harness_000.cpp (basic curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs)
 * - harness_002.cpp (basic curl_multi APIs for concurrent transfers)
 * - harness_003.cpp (FTP protocol)
 * - harness_004.cpp (Form APIs)
 * This harness targets advanced, complex usage patterns not covered by previous harnesses
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for complex operations
    if (size < 64) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== Advanced Easy API Testing ====================
    
    // Test 1: Complex curl_easy API patterns with advanced options
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume URL for testing
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        url = "http://localhost/";
    }
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    
    // Test various advanced options based on fuzzed data
    int option_set = fdp.ConsumeIntegralInRange<int>(0, 7);
    
    switch (option_set) {
        case 0:  // HTTP/2 and protocol options
            curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
            curl_easy_setopt(easy_handle, CURLOPT_PIPEWAIT, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_TCP_FASTOPEN, 1L);
            break;
            
        case 1:  // SSL/TLS security options
            curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYPEER, 0L);  // For testing
            curl_easy_setopt(easy_handle, CURLOPT_SSL_VERIFYHOST, 0L);
            curl_easy_setopt(easy_handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_2);
            curl_easy_setopt(easy_handle, CURLOPT_SSL_SESSIONID_CACHE, 0L);
            break;
            
        case 2:  // Connection and timeout options
            curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 5L);
            curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 10L);
            curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_LIMIT, 1024L);
            curl_easy_setopt(easy_handle, CURLOPT_LOW_SPEED_TIME, 5L);
            curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPALIVE, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPIDLE, 60L);
            curl_easy_setopt(easy_handle, CURLOPT_TCP_KEEPINTVL, 10L);
            break;
            
        case 3: { // HTTP header and cookie options
            // Create and set custom headers
            struct curl_slist* headers = NULL;
            if (fdp.ConsumeBool()) {
                std::string custom_header = fdp.ConsumeRandomLengthString(128);
                if (!custom_header.empty()) {
                    headers = curl_slist_append(headers, custom_header.c_str());
                }
            }
            if (fdp.ConsumeBool()) {
                std::string another_header = "User-Agent: " + fdp.ConsumeRandomLengthString(64);
                headers = curl_slist_append(headers, another_header.c_str());
            }
            if (headers) {
                curl_easy_setopt(easy_handle, CURLOPT_HTTPHEADER, headers);
            }
            
            // Set cookies
            if (fdp.ConsumeBool()) {
                std::string cookie_data = fdp.ConsumeRandomLengthString(256);
                if (!cookie_data.empty()) {
                    curl_easy_setopt(easy_handle, CURLOPT_COOKIE, cookie_data.c_str());
                }
            }
            break;
        }
            
        case 4:  // Proxy and network options
            if (fdp.ConsumeBool()) {
                std::string proxy_url = fdp.ConsumeRandomLengthString(128);
                if (!proxy_url.empty()) {
                    curl_easy_setopt(easy_handle, CURLOPT_PROXY, proxy_url.c_str());
                }
            }
            curl_easy_setopt(easy_handle, CURLOPT_NOPROXY, "localhost,127.0.0.1");
            curl_easy_setopt(easy_handle, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);
            break;
            
        case 5:  // Authentication options
            if (fdp.ConsumeBool()) {
                std::string username = fdp.ConsumeRandomLengthString(64);
                std::string password = fdp.ConsumeRandomLengthString(64);
                curl_easy_setopt(easy_handle, CURLOPT_USERNAME, username.c_str());
                curl_easy_setopt(easy_handle, CURLOPT_PASSWORD, password.c_str());
                curl_easy_setopt(easy_handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC | CURLAUTH_DIGEST);
            }
            break;
            
        case 6:  // Redirection and follow options
            curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, fdp.ConsumeIntegralInRange<long>(0, 10));
            curl_easy_setopt(easy_handle, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
            break;
            
        case 7:  // Verbose and debug options
            curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_HEADER, 1L);
            if (fdp.ConsumeBool()) {
                // Would normally set debug function callback here
            }
            break;
    }
    
    // Test curl_easy_getinfo for various info types
    if (fdp.ConsumeBool()) {
        long response_code;
        curl_easy_getinfo(easy_handle, CURLINFO_RESPONSE_CODE, &response_code);
        
        double total_time;
        curl_easy_getinfo(easy_handle, CURLINFO_TOTAL_TIME, &total_time);
        
        char* effective_url = NULL;
        curl_easy_getinfo(easy_handle, CURLINFO_EFFECTIVE_URL, &effective_url);
        
        long http_connectcode;
        curl_easy_getinfo(easy_handle, CURLINFO_HTTP_CONNECTCODE, &http_connectcode);
    }
    
    // Test curl_easy_duphandle and curl_easy_reset
    if (fdp.ConsumeBool()) {
        CURL* dup_handle = curl_easy_duphandle(easy_handle);
        if (dup_handle) {
            // Set a different option on duplicate handle
            std::string dup_url = fdp.ConsumeRandomLengthString(128);
            if (!dup_url.empty()) {
                curl_easy_setopt(dup_handle, CURLOPT_URL, dup_url.c_str());
            }
            curl_easy_cleanup(dup_handle);
        }
    }
    
    if (fdp.ConsumeBool()) {
        curl_easy_reset(easy_handle);
        // After reset, set a simple option to verify handle is still usable
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/");
    }
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // ==================== Advanced Multi API Testing ====================
    
    // Test 2: Sophisticated curl_multi API patterns
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set multi options
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_HTTP1 | CURLPIPE_MULTIPLEX);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, fdp.ConsumeIntegralInRange<long>(1, 10));
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, fdp.ConsumeIntegralInRange<long>(1, 10));
    
    // Create multiple easy handles for multi interface
    size_t num_easy_handles = fdp.ConsumeIntegralInRange<size_t>(1, 5);
    std::vector<CURL*> easy_handles;
    easy_handles.reserve(num_easy_handles);
    
    for (size_t i = 0; i < num_easy_handles && fdp.remaining_bytes() > 32; i++) {
        CURL* easy = curl_easy_init();
        if (easy) {
            easy_handles.push_back(easy);
            
            // Set URL for this easy handle
            std::string easy_url = fdp.ConsumeRandomLengthString(128);
            if (easy_url.empty()) {
                easy_url = "http://localhost/path" + std::to_string(i);
            }
            curl_easy_setopt(easy, CURLOPT_URL, easy_url.c_str());
            
            // Add to multi handle
            curl_multi_add_handle(multi_handle, easy);
        }
    }
    
    // Test curl_multi_perform with running handles tracking
    int still_running = 0;
    if (!easy_handles.empty()) {
        curl_multi_perform(multi_handle, &still_running);
        
        // Test curl_multi_info_read for completed transfers
        int msgs_in_queue = 0;
        CURLMsg* msg = NULL;
        do {
            msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
            if (msg && msg->msg == CURLMSG_DONE) {
                // Handle completed transfer
                CURL* easy_done = msg->easy_handle;
                CURLcode result = msg->data.result;
                // Could log result or take action based on result
                (void)result;
                (void)easy_done;
            }
        } while (msg);
        
        // Test curl_multi_timeout
        long timeout = 0;
        curl_multi_timeout(multi_handle, &timeout);
        
        // Test curl_multi_fdset (legacy but still supported)
        fd_set read_fd, write_fd, exc_fd;
        int max_fd = 0;
        FD_ZERO(&read_fd);
        FD_ZERO(&write_fd);
        FD_ZERO(&exc_fd);
        curl_multi_fdset(multi_handle, &read_fd, &write_fd, &exc_fd, &max_fd);
        
        // Test curl_multi_wait (modern alternative)
        int numfds = 0;
        curl_multi_wait(multi_handle, NULL, 0, 100, &numfds);
        
        // Test curl_multi_poll (newest API)
        curl_multi_poll(multi_handle, NULL, 0, 100, &numfds);
        
        // Test curl_multi_wakeup
        curl_multi_wakeup(multi_handle);
    }
    
    // Test curl_multi_assign (for socket action interface)
    if (fdp.ConsumeBool() && !easy_handles.empty()) {
        // In real usage, would assign socket-specific data
        void* sockptr = NULL;
        curl_multi_assign(multi_handle, 0, sockptr);
    }
    
    // Test curl_multi_socket_action (advanced socket interface)
    if (fdp.ConsumeBool()) {
        // Would normally be called with actual socket events
        int running = 0;
        curl_multi_socket_action(multi_handle, CURL_SOCKET_TIMEOUT, 0, &running);
    }
    
    // Test curl_multi_get_handles
    CURL** handle_array = curl_multi_get_handles(multi_handle);
    if (handle_array) {
        // Array is NULL-terminated
        for (int i = 0; handle_array[i] != NULL; i++) {
            // Could inspect each handle
            (void)handle_array[i];
        }
    }
    
    // Remove and clean up easy handles
    for (CURL* easy : easy_handles) {
        curl_multi_remove_handle(multi_handle, easy);
        curl_easy_cleanup(easy);
    }
    
    // Test curl_multi_cleanup
    curl_multi_cleanup(multi_handle);
    
    // ==================== Error Handling and State Testing ====================
    
    // Test 3: Error scenarios and edge cases
    if (fdp.ConsumeBool()) {
        // Test with NULL handles
        CURL* null_easy = NULL;
        // These should handle NULL gracefully
        curl_easy_setopt(null_easy, CURLOPT_URL, "http://localhost/");
        curl_easy_perform(null_easy);
        curl_easy_cleanup(null_easy);
        
        CURLM* null_multi = NULL;
        curl_multi_perform(null_multi, &still_running);
        curl_multi_cleanup(null_multi);
    }
    // Test curl_easy_strerror and curl_multi_strerror
    if (fdp.ConsumeBool()) {
        // Test a few specific error codes instead of iterating through all
        const char* ok_str = curl_easy_strerror(CURLE_OK);
        const char* url_str = curl_easy_strerror(CURLE_URL_MALFORMAT);
        const char* conn_str = curl_easy_strerror(CURLE_COULDNT_CONNECT);
        const char* mem_str = curl_easy_strerror(CURLE_OUT_OF_MEMORY);
        const char* timeout_str = curl_easy_strerror(CURLE_OPERATION_TIMEDOUT);
        (void)ok_str; (void)url_str; (void)conn_str; (void)mem_str; (void)timeout_str;
        
        // Test multi error strings
        const char* multi_ok_str = curl_multi_strerror(CURLM_OK);
        const char* multi_bad_handle_str = curl_multi_strerror(CURLM_BAD_HANDLE);
        const char* multi_mem_str = curl_multi_strerror(CURLM_OUT_OF_MEMORY);
        (void)multi_ok_str; (void)multi_bad_handle_str; (void)multi_mem_str;
    }

    
    // Test invalid option values
    if (fdp.ConsumeBool()) {
        CURL* test_easy = curl_easy_init();
        if (test_easy) {
            // Try setting invalid option values
            curl_easy_setopt(test_easy, CURLOPT_URL, NULL);
            curl_easy_setopt(test_easy, CURLOPT_VERBOSE, 999L);
            curl_easy_setopt(test_easy, (CURLoption)99999, 0L);
            curl_easy_cleanup(test_easy);
        }
    }
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_009
