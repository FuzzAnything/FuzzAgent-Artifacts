#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_013 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

// Socket callback for multi interface
static int socket_callback(CURL *easy,      /* easy handle */
                           curl_socket_t s, /* socket */
                           int what,        /* see above */
                           void *userp,     /* private callback pointer */
                           void *socketp) { /* private socket pointer */
    // Track socket state based on 'what' parameter
    // what values: CURL_POLL_IN, CURL_POLL_OUT, CURL_POLL_INOUT, CURL_POLL_REMOVE
    (void)easy;
    (void)s;
    (void)what;
    (void)userp;
    (void)socketp;
    
    // Always return 0 for success
    return 0;
}

// Timer callback for multi interface  
static int timer_callback(CURLM *multi,    /* multi handle */
                          long timeout_ms, /* timeout in milliseconds */
                          void *userp) {   /* private callback pointer */
    // Track timer state
    (void)multi;
    (void)timeout_ms;
    (void)userp;
    
    // Always return 0 for success
    return 0;
}

// Open socket callback
static curl_socket_t opensocket_callback(void *clientp, curlsocktype purpose, struct curl_sockaddr *address) {
    // Return invalid socket to prevent actual network operations
    (void)clientp;
    (void)purpose;
    (void)address;
    return CURL_SOCKET_BAD;
}

// Close socket callback
static int closesocket_callback(void *clientp, curl_socket_t item) {
    // Do nothing for socket close
    (void)clientp;
    (void)item;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for comprehensive multi interface testing
    // We need enough data for URL, various parameters, operation selection, and socket states
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // ============================================
    // PHASE 1: INITIALIZATION (as per guidance)
    // ============================================
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // 1. Call curl_multi_init() to create multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // 2. Call curl_easy_init() to create easy handle(s)
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // 3. Call curl_easy_setopt() to configure URL and options
    std::string url = fdp.ConsumeRandomLengthString(256);
    curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, nullptr);
    
    // Set additional options based on fuzzed data
    bool use_post = fdp.ConsumeBool();
    if (use_post) {
        std::string post_data = fdp.ConsumeRandomLengthString(1024);
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDS, post_data.c_str());
        curl_easy_setopt(easy_handle, CURLOPT_POSTFIELDSIZE, post_data.size());
    }
    
    bool follow_redirects = fdp.ConsumeBool();
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, follow_redirects ? 1L : 0L);
    
    // Set timeout values
    long timeout_sec = fdp.ConsumeIntegralInRange<long>(1, 10);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, timeout_sec);
    
    // Set connection timeout
    long connect_timeout_sec = fdp.ConsumeIntegralInRange<long>(1, 5);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, connect_timeout_sec);
    
    // Set socket callback for multi interface
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, socket_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, nullptr);
    
    // Set timer callback for multi interface
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, timer_callback);
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, nullptr);
    
    // Test various multi options based on fuzzed data
    uint8_t multi_options = fdp.ConsumeIntegral<uint8_t>();
    
    // Test CURLMOPT_MAX_TOTAL_CONNECTIONS option
    if (multi_options & 0x01) {
        long max_connections = fdp.ConsumeIntegralInRange<long>(1, 100);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_connections);
    }
    
    // Test CURLMOPT_MAXCONNECTS option
    if (multi_options & 0x02) {
        long max_connects = fdp.ConsumeIntegralInRange<long>(1, 100);
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
    }
    
    // Test CURLMOPT_MAX_HOST_CONNECTIONS option
    if (multi_options & 0x04) {
        long max_host_connects = fdp.ConsumeIntegralInRange<long>(1, 100);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_connects);
    }
    
    // ============================================
    // PHASE 2: CONNECTION SETUP
    // ============================================
    
    // 2. Call curl_multi_add_handle() to add easy handle to multi
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    if (add_result != CURLM_OK) {
        // Clean up and return
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }
    
    // ============================================
    // PHASE 3: EXECUTION (Primary API targets from guidance)
    // ============================================
    
    // 1. Call curl_multi_perform() to perform transfers
    int running_handles = 0;
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    // 2. Call curl_multi_socket_action() with various socket states
    // This is the modern API - guidance mentions curl_multi_socket (deprecated)
    // IMPORTANT: Ensure socket values are non-negative to avoid library bug in mev_sh_entry_hash
    // Negative socket descriptors cause undefined behavior in the hash function
    for (int i = 0; i < 3 && fdp.remaining_bytes() >= sizeof(curl_socket_t) + 1; i++) {
        // Generate socket event bitmask from fuzzed data
        int ev_bitmask = 0;
        bool pollin = fdp.ConsumeBool();
        bool pollout = fdp.ConsumeBool();
        bool pollpri = fdp.ConsumeBool();
        
        if (pollin) ev_bitmask |= CURL_WAIT_POLLIN;
        if (pollout) ev_bitmask |= CURL_WAIT_POLLOUT;
        if (pollpri) ev_bitmask |= CURL_WAIT_POLLPRI;
        
        // Use a dummy socket value from fuzzed data, ensure non-negative to avoid library bug
        curl_socket_t raw_socket = fdp.ConsumeIntegral<curl_socket_t>();
        curl_socket_t dummy_socket;
        if (raw_socket < 0) {
            // Avoid overflow when negating INT_MIN by casting to unsigned first
            dummy_socket = -static_cast<unsigned int>(raw_socket);
        } else {
            dummy_socket = raw_socket;
        }
        if (dummy_socket < 0) dummy_socket = 0; // Double-check for edge case (e.g., INT_MIN)
        // Call curl_multi_socket_action - PRIMARY TARGET (guidance: curl_multi_socket)
        CURLMcode socket_action_result = curl_multi_socket_action(multi_handle, 
                                                                  dummy_socket, 
                                                                  ev_bitmask, 
                                                                  &running_handles);
        (void)socket_action_result;
        
        // Call the deprecated curl_multi_socket macro for compatibility
        int running_handles_deprecated = 0;
        curl_socket_t raw_socket2 = fdp.ConsumeIntegral<curl_socket_t>();
        curl_socket_t dummy_socket2;
        if (raw_socket2 < 0) {
            // Avoid overflow when negating INT_MIN by casting to unsigned first
            dummy_socket2 = -static_cast<unsigned int>(raw_socket2);
        } else {
            dummy_socket2 = raw_socket2;
        }
        if (dummy_socket2 < 0) dummy_socket2 = 0; // Double-check for edge case (e.g., INT_MIN)
        CURLMcode socket_result = curl_multi_socket(multi_handle, dummy_socket2, &running_handles_deprecated);
        (void)socket_result;
        
        // Call curl_multi_perform() again after socket action
        CURLMcode perform_result2 = curl_multi_perform(multi_handle, &running_handles);
        (void)perform_result2;
    }
    
    // 3. Call curl_multi_timeout() to check/configure timeouts
    long timeout_ms = 0;
    CURLMcode timeout_result = curl_multi_timeout(multi_handle, &timeout_ms);
    (void)timeout_result;
    
    // 4. Call curl_multi_get_offt() for progress information - PRIMARY TARGET
    // This API is specifically mentioned in guidance and has 0% coverage
    curl_off_t off_t_value = 0;
    
    // Test all CURLMINFO_OFFT enum values
    for (CURLMinfo_offt info = CURLMINFO_XFERS_CURRENT; 
         info <= CURLMINFO_XFERS_ADDED && info != CURLMINFO_LASTENTRY; 
         info = static_cast<CURLMinfo_offt>(static_cast<int>(info) + 1)) {
        
        CURLMcode get_offt_result = curl_multi_get_offt(multi_handle, info, &off_t_value);
        (void)get_offt_result;
        
    }
    
    // ============================================
    // PHASE 4: ADDITIONAL ASYNCHRONOUS TRANSFER SCENARIOS
    // ============================================
    
    // Test curl_multi_waitfds() - modern API for polling
    unsigned int fd_count = 0;
    CURLMcode waitfds_count_result = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    
    if (waitfds_count_result == CURLM_OK && fd_count > 0 && fdp.remaining_bytes() > 0) {
        // Allocate array for file descriptors
        std::vector<struct curl_waitfd> waitfds(fd_count);
        
        // Get the actual file descriptors
        unsigned int actual_fd_count = fd_count;
        CURLMcode waitfds_result = curl_multi_waitfds(multi_handle, 
                                                     waitfds.data(), 
                                                     fd_count, 
                                                     &actual_fd_count);
        (void)waitfds_result;
    }
    
    // Test curl_multi_fdset() - legacy API
    fd_set read_fds, write_fds, exc_fds;
    int max_fd = 0;
    
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&exc_fds);
    
    CURLMcode fdset_result = curl_multi_fdset(multi_handle, 
                                              &read_fds, 
                                              &write_fds, 
                                              &exc_fds, 
                                              &max_fd);
    (void)fdset_result;
    
    // Check for completed transfers
    int msgs_in_queue = 0;
    CURLMsg *msg = NULL;
    while ((msg = curl_multi_info_read(multi_handle, &msgs_in_queue)) != NULL) {
        if (msg->msg == CURLMSG_DONE) {
            CURLcode result = msg->data.result;
            (void)result;
        }
    }
    
    // Test with multiple easy handles for complex scenarios
    if (fdp.remaining_bytes() > 64) {
        CURL *easy_handle2 = curl_easy_init();
        if (easy_handle2) {
            std::string url2 = fdp.ConsumeRandomLengthString(128);
            curl_easy_setopt(easy_handle2, CURLOPT_URL, url2.c_str());
            curl_easy_setopt(easy_handle2, CURLOPT_WRITEFUNCTION, write_callback);
            
            curl_multi_add_handle(multi_handle, easy_handle2);
            
            // Perform with multiple handles
            int running_handles2 = 0;
            CURLMcode perform_result3 = curl_multi_perform(multi_handle, &running_handles2);
            (void)perform_result3;
            
            // Remove the second handle
            curl_multi_remove_handle(multi_handle, easy_handle2);
            curl_easy_cleanup(easy_handle2);
        }
    }
    
    // ============================================
    // PHASE 5: CLEANUP (as per guidance)
    // ============================================
    
    // 1. Call curl_multi_remove_handle() to remove easy handle
    CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_result;
    
    // 2. Call curl_easy_cleanup() to destroy easy handle
    curl_easy_cleanup(easy_handle);
    
    // 3. Call curl_multi_cleanup() to destroy multi handle
    curl_multi_cleanup(multi_handle);
    
    // 4. Call curl_global_cleanup() to cleanup library
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_013
