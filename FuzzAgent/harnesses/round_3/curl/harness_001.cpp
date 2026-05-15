#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <unistd.h>

namespace harness_001 {
/*
 * Fuzzing harness for libcurl's multi interface
 * Targets: curl_multi_init, curl_multi_add_handle, curl_multi_perform, 
 *          curl_multi_remove_handle, curl_multi_cleanup, curl_multi_wait,
 *          curl_multi_info_read, curl_multi_setopt
 */


// Write callback for handling response data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    // Just discard the data for fuzzing purposes
    (void)contents;
    (void)userp;
    return realsize;
}

// Socket callback for multi interface
static int socket_callback(CURL *easy, curl_socket_t s, int what, void *cbp, void *sockp) {
    (void)easy;
    (void)s;
    (void)what;
    (void)cbp;
    (void)sockp;
    return 0;
}

// Timer callback for multi interface
static int timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Create multi handle
    CURLM *multi = curl_multi_init();
    if (!multi) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set multi options based on fuzzed input
    uint8_t pipelining = fdp.ConsumeIntegral<uint8_t>() % 4;
    if (pipelining == 0) {
        curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_NOTHING);
    } else if (pipelining == 1) {
        curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_HTTP1);
    } else if (pipelining == 2) {
        curl_multi_setopt(multi, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    }
    
    // Set max total connections
    long max_connects = fdp.ConsumeIntegralInRange<long>(1, 10);
    curl_multi_setopt(multi, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_connects);
    
    // Set max host connections
    long max_host_connects = fdp.ConsumeIntegralInRange<long>(1, 5);
    curl_multi_setopt(multi, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_connects);
    
    // Set max pipeline length
    long max_pipeline_length = fdp.ConsumeIntegralInRange<long>(1, 5);
    curl_multi_setopt(multi, CURLMOPT_MAX_PIPELINE_LENGTH, max_pipeline_length);
    
    // Set socket and timer callbacks
    curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, socket_callback);
    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, timer_callback);
    
    // Create multiple easy handles (between 1 and 5)
    int num_easy_handles = fdp.ConsumeIntegralInRange<int>(1, 5);
    std::vector<CURL*> easy_handles;
    
    for (int i = 0; i < num_easy_handles && fdp.remaining_bytes() > 10; i++) {
        CURL *easy = curl_easy_init();
        if (!easy) {
            continue;
        }
        
        // Set URL from fuzzed input
        std::string url = fdp.ConsumeRandomLengthString(128);
        if (!url.empty()) {
            curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
        }
        
        // Set write callback
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_callback);
        
        // Set various options based on remaining input
        if (fdp.remaining_bytes() > 5) {
            bool verbose = fdp.ConsumeBool();
            curl_easy_setopt(easy, CURLOPT_VERBOSE, verbose ? 1L : 0L);
        }
        
        if (fdp.remaining_bytes() > 5) {
            bool follow_redirects = fdp.ConsumeBool();
            curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1L : 0L);
        }
        
        // Add handle to multi
        CURLMcode mres = curl_multi_add_handle(multi, easy);
        if (mres == CURLM_OK) {
            easy_handles.push_back(easy);
        } else {
            curl_easy_cleanup(easy);
        }
    }
    
    if (easy_handles.empty()) {
        curl_multi_cleanup(multi);
        curl_global_cleanup();
        return 0;
    }
    
    // Perform multi operations for a few iterations
    int running_handles = 0;
    for (int iteration = 0; iteration < 3 && fdp.remaining_bytes() > 0; iteration++) {
        // Perform
        CURLMcode perform_res = curl_multi_perform(multi, &running_handles);
        
        // Wait with timeout
        long timeout_ms = fdp.ConsumeIntegralInRange<long>(1, 100);
        int numfds = 0;
        curl_multi_wait(multi, NULL, 0, timeout_ms, &numfds);
        
        // Poll
        curl_multi_poll(multi, NULL, 0, timeout_ms, &numfds);
        
        // Process info messages
        int msgs_in_queue = 0;
        CURLMsg *msg;
        while ((msg = curl_multi_info_read(multi, &msgs_in_queue))) {
            if (msg->msg == CURLMSG_DONE) {
                // Handle completed transfer
                CURL *easy = msg->easy_handle;
                // Could check result here: msg->data.result
            }
        }
        
        // Wakeup
        curl_multi_wakeup(multi);
    }
    
    // Get timeout value
    long timeout_val = 0;
    curl_multi_timeout(multi, &timeout_val);
    
    // Get socket actions (simulate with invalid socket)
    curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &running_handles);
    
    // Remove and cleanup easy handles
    for (CURL *easy : easy_handles) {
        curl_multi_remove_handle(multi, easy);
        curl_easy_cleanup(easy);
    }
    
    // Cleanup multi handle
    curl_multi_cleanup(multi);
    
    // Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_001
