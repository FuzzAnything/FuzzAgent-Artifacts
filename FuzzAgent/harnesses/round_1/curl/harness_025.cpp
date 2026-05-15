#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>

namespace harness_025 {


// Comprehensive callback functions for testing curl_easy_setopt type-checking macros

// 1. WRITE callback - returns consumed bytes or special values for testing
static size_t write_callback(char* buffer, size_t size, size_t nmemb, void* userdata) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(userdata);
    if (!fdp || fdp->remaining_bytes() < 1) {
        return size * nmemb;  // Normal return
    }
    
    // Test different return values based on fuzzed input
    uint8_t return_type = fdp->ConsumeIntegral<uint8_t>() % 4;
    switch (return_type) {
        case 0:
            // Return less than requested (simulating partial write)
            return (size * nmemb) / 2;
        case 1:
            // Return 0 (CURL_WRITEFUNC_PAUSE)
            return CURL_WRITEFUNC_PAUSE;
        case 2:
            // Return error
            return CURL_WRITEFUNC_ERROR;
        default:
            // Normal return
            return size * nmemb;
    }
}

// 2. READ callback - provides data from fuzzer input
static size_t read_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    std::vector<uint8_t>* fuzz_data = static_cast<std::vector<uint8_t>*>(userdata);
    if (!fuzz_data || fuzz_data->empty()) {
        return 0;  // EOF
    }
    
    size_t buffer_size = size * nitems;
    size_t to_copy = std::min(buffer_size, fuzz_data->size());
    
    if (to_copy > 0) {
        memcpy(buffer, fuzz_data->data(), to_copy);
        fuzz_data->erase(fuzz_data->begin(), fuzz_data->begin() + to_copy);
    }
    
    return to_copy;
}

// 3. HEADER callback - processes headers
static size_t header_callback(char* buffer, size_t size, size_t nitems, void* userdata) {
    // Just consume headers, could analyze them in a real scenario
    return size * nitems;
}

// 4. PROGRESS callback (deprecated but still needs testing for coverage)
static int progress_callback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(clientp);
    if (!fdp || fdp->remaining_bytes() < 1) {
        return 0;  // Continue
    }
    
    // Test returning 1 to abort based on fuzzed input
    if (fdp->ConsumeBool()) {
        return 1;  // Abort transfer
    }
    return 0;  // Continue
}

// 5. XFERINFO callback (modern replacement for PROGRESS callback)
static int xferinfo_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(clientp);
    if (!fdp || fdp->remaining_bytes() < 1) {
        return 0;  // Continue
    }
    
    // Test returning non-zero to abort based on fuzzed input
    if (fdp->ConsumeBool()) {
        return 1;  // Abort transfer
    }
    return 0;  // Continue
}

// 6. DEBUG callback - handles debug information
static int debug_callback(CURL* handle, curl_infotype type, char* data, size_t size, void* userdata) {
    // Process debug information based on type
    switch (type) {
        case CURLINFO_TEXT:
            // Text data, could log it
            break;
        case CURLINFO_HEADER_IN:
        case CURLINFO_HEADER_OUT:
            // Header data
            break;
        case CURLINFO_DATA_IN:
        case CURLINFO_DATA_OUT:
            // Data being transferred
            break;
        case CURLINFO_SSL_DATA_IN:
        case CURLINFO_SSL_DATA_OUT:
            // SSL data
            break;
        default:
            break;
    }
    return 0;
}

// 7. SSL context callback for OpenSSL/wolfSSL
static CURLcode ssl_ctx_callback(CURL* curl, void* ssl_ctx, void* userptr) {
    // In a real scenario, this would configure SSL context
    return CURLE_OK;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Callback testing requires significant input for various combinations
    if (size < 64) {
        return 0;  // Need minimum input for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl
    CURLcode global_result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Prepare data for READ callback
    std::vector<uint8_t> upload_data;
    if (fdp.remaining_bytes() > 0) {
        size_t data_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min<size_t>(4096, fdp.remaining_bytes()));
        upload_data = fdp.ConsumeBytes<uint8_t>(data_size);
    }

    // Step 3: Test various callback combinations
    // Use fuzzed input to determine which callbacks to set and in what order
    
    // Consume configuration flags
    uint8_t config_flags = fdp.ConsumeIntegral<uint8_t>();
    
    // Set URL (required for some operations)
    if (fdp.remaining_bytes() > 8) {
        std::string url = "http://";
        url += fdp.ConsumeRandomLengthString(50);
        url += "/test";
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    }
    
    // Test WRITEFUNCTION callback (always set for basic operation)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fdp);
    
    // Test READFUNCTION callback if configured
    if (config_flags & 0x01) {
        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_data);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);  // Enable upload
    }
    
    // Test HEADERFUNCTION callback if configured
    if (config_flags & 0x02) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(curl, CURLOPT_HEADERDATA, nullptr);
    }
    
    // Test PROGRESSFUNCTION callback (deprecated but needs coverage)
    if (config_flags & 0x04) {
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_callback);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &fdp);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);  // Enable progress
    }
    
    // Test XFERINFOFUNCTION callback (modern)
    if (config_flags & 0x08) {
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &fdp);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);  // Enable progress
    }
    
    // Test DEBUGFUNCTION callback
    if (config_flags & 0x10) {
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debug_callback);
        curl_easy_setopt(curl, CURLOPT_DEBUGDATA, nullptr);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);  // Enable verbose for debug output
    }
    
    // Test SSL_CTX_FUNCTION callback
    if (config_flags & 0x20) {
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, ssl_ctx_callback);
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_DATA, nullptr);
    }
    
    // Set timeouts to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 10L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 10L);
    
    // Disable signal handling for fuzzing safety
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    // Step 4: Test curl_easy_perform with the configured callbacks
    // This will trigger the callbacks and test the type-checking macros
    CURLcode perform_result = curl_easy_perform(curl);
    
    // The result doesn't matter for fuzzing - we're testing the callback setup
    // and type-checking macros, not the network operation
    
    // Step 5: Test additional callback-related options
    
    // Test CURLOPT_SEEKFUNCTION if we have upload data
    if (!upload_data.empty() && fdp.remaining_bytes() > 1) {
        // Seek callback prototype
        auto seek_callback = [](void* userdata, curl_off_t offset, int origin) -> int {
            // Simple seek implementation for testing
            return CURL_SEEKFUNC_OK;
        };
        
        curl_easy_setopt(curl, CURLOPT_SEEKFUNCTION, seek_callback);
        curl_easy_setopt(curl, CURLOPT_SEEKDATA, nullptr);
    }
    
    // Test CURLOPT_SOCKOPTFUNCTION if configured
    if (config_flags & 0x40 && fdp.remaining_bytes() > 1) {
        auto sockopt_callback = [](void* clientp, curl_socket_t curlfd, curlsocktype purpose) -> int {
            // Simple sockopt implementation for testing
            return CURL_SOCKOPT_OK;
        };
        
        curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, sockopt_callback);
        curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, nullptr);
    }
    
    // Test CURLOPT_OPENSOCKETFUNCTION if configured
    if (config_flags & 0x80 && fdp.remaining_bytes() > 1) {
        auto opensocket_callback = [](void* clientp, curlsocktype purpose, struct curl_sockaddr* address) -> curl_socket_t {
            // Return invalid socket for testing
            return CURL_SOCKET_BAD;
        };
        
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, opensocket_callback);
        curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, nullptr);
    }
    
    // Step 6: Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_025
