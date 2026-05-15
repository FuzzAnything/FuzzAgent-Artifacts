#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <curl/header.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_004 {

// Simple write callback that discards data
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the received data
    return size * nmemb;
}

// Header callback for collecting headers (optional)
static size_t header_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    // Just discard the headers
    return size * nmemb;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum size for basic operations
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize curl library - must be called once
    CURLcode global_init_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_init_result != CURLE_OK) {
        return 0;
    }
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume fuzzed data for various parameters
    std::string url = fdp.ConsumeRandomLengthString(256);
    bool follow_location = fdp.ConsumeBool();
    bool verbose = fdp.ConsumeBool();
    bool use_websocket = fdp.ConsumeBool();
    bool test_headers = fdp.ConsumeBool();
    
    // Set basic options
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow_location ? 1L : 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose ? 1L : 0L);
    
    // Set write callback to discard data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    
    // Set header callback if testing headers
    if (test_headers) {
        curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    }
    
    // Test 1: Websocket API functions (if enabled)
    if (use_websocket) {
        // Set CONNECT_ONLY mode for websocket
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
        
        // Try to perform connection (will likely fail in fuzzing, but that's OK)
        CURLcode perform_result = curl_easy_perform(curl);
        
        // Regardless of connection result, test websocket functions
        // to exercise their parameter validation and error paths
        
        // Test curl_ws_send with various flags and buffer sizes
        unsigned int ws_flags = 0;
        // Consume flags bitmask
        uint8_t flags_byte = fdp.ConsumeIntegral<uint8_t>();
        if (flags_byte & 0x01) ws_flags |= CURLWS_TEXT;
        if (flags_byte & 0x02) ws_flags |= CURLWS_BINARY;
        if (flags_byte & 0x04) ws_flags |= CURLWS_CONT;
        if (flags_byte & 0x08) ws_flags |= CURLWS_CLOSE;
        if (flags_byte & 0x10) ws_flags |= CURLWS_PING;
        if (flags_byte & 0x20) ws_flags |= CURLWS_PONG;
        if (flags_byte & 0x40) ws_flags |= CURLWS_OFFSET;
        
        // Consume buffer size for send
        size_t send_buf_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        std::vector<char> send_buffer(send_buf_size);
        if (send_buf_size > 0) {
            auto send_data = fdp.ConsumeBytes<uint8_t>(send_buf_size);
            std::copy(send_data.begin(), send_data.end(), send_buffer.begin());
        }
        
        size_t sent = 0;
        curl_off_t fragsize = fdp.ConsumeIntegral<curl_off_t>();
        
        // Call curl_ws_send
        curl_ws_send(curl, send_buffer.data(), send_buffer.size(), &sent, fragsize, ws_flags);
        
        // Test curl_ws_recv
        size_t recv_buf_size = fdp.ConsumeIntegralInRange<size_t>(0, 1024);
        std::vector<char> recv_buffer(recv_buf_size);
        size_t received = 0;
        const struct curl_ws_frame *meta = NULL;
        
        // Call curl_ws_recv
        curl_ws_recv(curl, recv_buffer.data(), recv_buffer.size(), &received, &meta);
        
        // Test curl_ws_start_frame
        unsigned int start_flags = 0;
        uint8_t start_flags_byte = fdp.ConsumeIntegral<uint8_t>();
        if (start_flags_byte & 0x01) start_flags |= CURLWS_TEXT;
        if (start_flags_byte & 0x02) start_flags |= CURLWS_BINARY;
        if (start_flags_byte & 0x04) start_flags |= CURLWS_CONT;
        if (start_flags_byte & 0x08) start_flags |= CURLWS_CLOSE;
        if (start_flags_byte & 0x10) start_flags |= CURLWS_PING;
        if (start_flags_byte & 0x20) start_flags |= CURLWS_OFFSET;
        
        curl_off_t frame_len = fdp.ConsumeIntegral<curl_off_t>();
        curl_ws_start_frame(curl, start_flags, frame_len);
        
        // Test curl_ws_meta
        const struct curl_ws_frame *ws_meta = curl_ws_meta(curl);
        (void)ws_meta; // Use to avoid unused variable warning
    }
    
    // Test 2: Header API functions (if enabled)
    if (test_headers) {
        // Try to perform a request to potentially get headers
        // (will likely fail in fuzzing, but that's OK)
        CURLcode perform_result = curl_easy_perform(curl);
        
        // Test curl_easy_header with various header names
        std::vector<std::string> header_names = {
            "Content-Type",
            "Server",
            "Date",
            "Cache-Control",
            "Content-Length",
            "Connection"
        };
        
        for (size_t i = 0; i < header_names.size() && fdp.remaining_bytes() > 0; i++) {
            // Consume which header name to test
            size_t name_index = fdp.ConsumeIntegralInRange<size_t>(0, header_names.size() - 1);
            std::string header_name = header_names[name_index];
            
            // Consume index and origin
            size_t header_index = fdp.ConsumeIntegralInRange<size_t>(0, 10);
            unsigned int origin = 0;
            uint8_t origin_byte = fdp.ConsumeIntegral<uint8_t>();
            if (origin_byte & 0x01) origin |= CURLH_HEADER;
            if (origin_byte & 0x02) origin |= CURLH_TRAILER;
            if (origin_byte & 0x04) origin |= CURLH_CONNECT;
            if (origin_byte & 0x08) origin |= CURLH_1XX;
            if (origin_byte & 0x10) origin |= CURLH_PSEUDO;
            
            int request = fdp.ConsumeIntegralInRange<int>(-1, 10);
            struct curl_header *header_out = NULL;
            
            // Call curl_easy_header
            CURLHcode header_result = curl_easy_header(curl, header_name.c_str(), 
                                                      header_index, origin, 
                                                      request, &header_out);
            (void)header_result; // Use to avoid unused variable warning
            
            // Test curl_easy_nextheader in a loop
            if (header_out) {
                struct curl_header *prev = header_out;
                int max_iterations = fdp.ConsumeIntegralInRange<int>(0, 20);
                for (int j = 0; j < max_iterations && prev; j++) {
                    struct curl_header *next = curl_easy_nextheader(curl, origin, request, prev);
                    if (!next) {
                        break;
                    }
                    prev = next;
                }
            } else {
                // Try with NULL prev to get first header
                struct curl_header *first = curl_easy_nextheader(curl, origin, request, NULL);
                (void)first; // Use to avoid unused variable warning
            }
        }
        
        // Also test with random header names from fuzzed data
        size_t random_header_count = fdp.ConsumeIntegralInRange<size_t>(0, 10);
        for (size_t i = 0; i < random_header_count && fdp.remaining_bytes() > 0; i++) {
            std::string random_header_name = fdp.ConsumeRandomLengthString(50);
            size_t header_index = fdp.ConsumeIntegralInRange<size_t>(0, 5);
            unsigned int origin = CURLH_HEADER;
            int request = -1;
            struct curl_header *header_out = NULL;
            
            curl_easy_header(curl, random_header_name.c_str(), header_index, 
                            origin, request, &header_out);
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_004
