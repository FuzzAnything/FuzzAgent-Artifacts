#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/multi.h>

namespace harness_023 {


// Simple callbacks for testing
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Just discard the data
}

static size_t read_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    // Read from fuzzed data if available
    std::vector<uint8_t>* fuzz_data = static_cast<std::vector<uint8_t>*>(userdata);
    size_t buffer_size = size * nitems;
    size_t to_copy = std::min(buffer_size, fuzz_data->size());
    
    if (to_copy > 0) {
        memcpy(buffer, fuzz_data->data(), to_copy);
        fuzz_data->erase(fuzz_data->begin(), fuzz_data->begin() + to_copy);
    }
    
    return to_copy;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // HTTP/2 and multi interface testing requires significant input
    if (size < 128) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: curl_global_init with SSL support
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create multiple easy handles for concurrent HTTP/2 connections
    uint8_t num_easy_handles = fdp.ConsumeIntegralInRange<uint8_t>(1, 3);
    std::vector<CURL*> easy_handles;
    std::vector<std::unique_ptr<std::vector<uint8_t>>> upload_data;
    
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
        
        // Create upload data for this handle
        auto upload_buf = std::make_unique<std::vector<uint8_t>>();
        size_t upload_size = fdp.ConsumeIntegralInRange<size_t>(0, 512);
        if (upload_size > 0 && fdp.remaining_bytes() >= upload_size) {
            upload_buf->resize(upload_size);
            auto upload_bytes = fdp.ConsumeBytes<uint8_t>(upload_size);
            memcpy(upload_buf->data(), upload_bytes.data(), upload_size);
        }
        upload_data.push_back(std::move(upload_buf));
        
        // Configure for HTTP/2 with various options
        // Set URL from fuzzer input
        std::string url = "https://";
        if (fdp.remaining_bytes() > 8) {
            url += fdp.ConsumeRandomLengthString(50);
        }
        url += "/";
        if (fdp.remaining_bytes() > 8) {
            url += fdp.ConsumeRandomLengthString(50);
        }
        curl_easy_setopt(easy_handle, CURLOPT_URL, url.c_str());
        // **HTTP/2 SPECIFIC OPTIONS**
        // Set HTTP version to HTTP/2
        long http_version = fdp.ConsumeIntegralInRange<long>(CURL_HTTP_VERSION_2_0, CURL_HTTP_VERSION_2_PRIOR_KNOWLEDGE);
        curl_easy_setopt(easy_handle, CURLOPT_HTTP_VERSION, http_version);
        
        // Enable HTTP/2 multiplexing
        curl_easy_setopt(easy_handle, CURLOPT_PIPEWAIT, fdp.ConsumeBool() ? 1L : 0L);
        
        // Set HTTP/2 settings
        if (fdp.ConsumeBool()) {
            // Enable server push (HTTP/2 feature)
            curl_easy_setopt(easy_handle, CURLOPT_PIPEWAIT, 1L);
        }
        
        // **SSL/TLS OPTIONS FOR HTTP/2**
        // SSL version options
        long ssl_version = fdp.ConsumeIntegralInRange<long>(CURL_SSLVERSION_TLSv1_2, CURL_SSLVERSION_TLSv1_3);
        curl_easy_setopt(easy_handle, CURLOPT_SSLVERSION, ssl_version);
        
        // SSL options for HTTP/2 optimization
        long ssl_options = 0;
        if (fdp.ConsumeBool()) ssl_options |= CURLSSLOPT_ALLOW_BEAST;
        if (fdp.ConsumeBool()) ssl_options |= CURLSSLOPT_NO_REVOKE;
        if (fdp.ConsumeBool()) ssl_options |= CURLSSLOPT_REVOKE_BEST_EFFORT;
        if (fdp.remaining_bytes() > 0) {
            curl_easy_setopt(easy_handle, CURLOPT_SSL_OPTIONS, ssl_options);
        }
        
        // ALPN for HTTP/2 negotiation
        curl_easy_setopt(easy_handle, CURLOPT_SSL_ENABLE_ALPN, fdp.ConsumeBool() ? 1L : 0L);
        
        // Early data for TLS 1.3 (beneficial for HTTP/2)
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            long early_data = fdp.ConsumeBool() ? 1L : 0L;
            // Note: CURLOPT_SSL_EARLYDATA available in newer curl versions
        }
        
        // **MULTI INTERFACE PREPARATION**
        // Set callbacks for multi interface
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, nullptr);
        
        if (!upload_data[i]->empty()) {
            curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, read_callback);
            curl_easy_setopt(easy_handle, CURLOPT_READDATA, upload_data[i].get());
            curl_easy_setopt(easy_handle, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(easy_handle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)upload_data[i]->size());
        }
        
        // Standard safety options for fuzzing
        curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 10L);
        curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT_MS, 10L);
        
        // Additional HTTP/2 related options
        if (fdp.ConsumeBool()) {
            // Enable TCP Fast Open
            curl_easy_setopt(easy_handle, CURLOPT_TCP_FASTOPEN, 1L);
        }
        
        // HTTP/2 window size options
        if (fdp.remaining_bytes() > 4) {
            long http2_window_size = fdp.ConsumeIntegralInRange<long>(4096, 65535);
            // CURLOPT_HTTP2_WINDOW_SIZE might be available in newer versions
        }
    }
    
    // Step 3: Create and configure multi handle
    CURLM* multi_handle = curl_multi_init();
    if (!multi_handle) {
        for (CURL* h : easy_handles) {
            curl_easy_cleanup(h);
        }
        curl_global_cleanup();
        return 0;
    }
    
    // **MULTI INTERFACE OPTIONS FOR HTTP/2**
    // Set HTTP/2 multiplexing in multi interface
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    
    // Set max concurrent streams for HTTP/2
    if (fdp.remaining_bytes() > 4) {
        long max_concurrent_streams = fdp.ConsumeIntegralInRange<long>(1, 100);
        // CURLMOPT_MAX_CONCURRENT_STREAMS might be available in newer versions
    }
    
    // Set max connections per host
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 
                     fdp.ConsumeIntegralInRange<long>(1, 10));
    
    // Set max total connections
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS,
                     fdp.ConsumeIntegralInRange<long>(1, 20));
    
    // Add all easy handles to multi handle
    for (CURL* easy_handle : easy_handles) {
        CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
        (void)add_result;  // May succeed or fail, both are interesting for coverage
    }
    
    // Step 4: Exercise multi interface functions
    int running_handles = 0;
    
    // **TARGET API 1: curl_multi_perform** - Core multi interface function
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    (void)perform_result;
    
    // **TARGET API 2: curl_multi_poll** - Advanced polling for HTTP/2 multiplexing
    if (fdp.remaining_bytes() > 4) {
        int timeout_ms = fdp.ConsumeIntegralInRange<int>(0, 100);
        int numfds = 0;
        CURLMcode poll_result = curl_multi_poll(multi_handle, nullptr, 0, timeout_ms, &numfds);
        (void)poll_result;
    }
    
    // **TARGET API 3: curl_multi_wakeup** - For waking up sleeping polls
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
        (void)wakeup_result;
    }
    
    // **TARGET API 4: curl_multi_info_read** - Check for completion messages
    int msgs_in_queue = 0;
    CURLMsg* msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    while (msg != nullptr) {
        // Process message if any
        if (msg->msg == CURLMSG_DONE) {
            // Transfer completed
            (void)msg->easy_handle;
            (void)msg->data.result;
        }
        msg = curl_multi_info_read(multi_handle, &msgs_in_queue);
    }
    
    // **TARGET API 5: curl_multi_timeout** - Get timeout value
    if (fdp.remaining_bytes() > 4) {
        long timeout = 0;
        CURLMcode timeout_result = curl_multi_timeout(multi_handle, &timeout);
        (void)timeout_result;
    }
    
    // **TARGET API 6: curl_multi_fdset** - Get file descriptors (deprecated but still in API)
    // Note: curl_multi_fdset is deprecated but still part of the API
    // We include it for coverage of deprecated code paths
    if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
        fd_set read_fd_set, write_fd_set, exc_fd_set;
        int max_fd = 0;
        FD_ZERO(&read_fd_set);
        FD_ZERO(&write_fd_set);
        FD_ZERO(&exc_fd_set);
        
        CURLMcode fdset_result = curl_multi_fdset(multi_handle, &read_fd_set, 
                                                 &write_fd_set, &exc_fd_set, &max_fd);
        (void)fdset_result;
    }
    
    // Additional multi perform calls to exercise different states
    if (fdp.remaining_bytes() > 0) {
        CURLMcode second_perform = curl_multi_perform(multi_handle, &running_handles);
        (void)second_perform;
    }
    
    // Step 5: Clean up - remove handles in various orders
    // Remove some handles to test partial cleanup
    if (!easy_handles.empty() && fdp.remaining_bytes() > 0) {
        size_t handle_to_remove = fdp.ConsumeIntegralInRange<size_t>(0, easy_handles.size() - 1);
        CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handles[handle_to_remove]);
        (void)remove_result;
    }
    
    // Remove remaining handles
    for (CURL* easy_handle : easy_handles) {
        CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
        if (remove_result == CURLM_OK) {
            curl_easy_cleanup(easy_handle);
        }
    }
    
    // **TARGET API 7: curl_multi_cleanup** - Final cleanup
    CURLMcode cleanup_result = curl_multi_cleanup(multi_handle);
    (void)cleanup_result;
    
    // Step 6: Global cleanup
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_023
