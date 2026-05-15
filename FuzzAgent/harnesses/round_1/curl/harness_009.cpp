#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/websockets.h>

namespace harness_009 {


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // WebSocket operations require significant input for meaningful testing
    if (size < 64) {
        return 0; // Need minimum input for WebSocket operations
    }

    FuzzedDataProvider fdp(data, size);

    // Step 1: Initialize curl globally
    CURLcode global_result = curl_global_init(CURL_GLOBAL_ALL);
    if (global_result != CURLE_OK) {
        return 0;
    }

    // Step 2: Create curl easy handle
    CURL* curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }

    // Step 3: Configure for WebSocket mode
    // Use CURLOPT_CONNECT_ONLY = 2L for WebSocket style (as shown in example)
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    // Consume a WebSocket URL from fuzzer input
    std::string ws_url = fdp.ConsumeRandomLengthString(128);
    if (ws_url.empty()) {
        // Fallback to a minimal URL if no string was consumed
        ws_url = "ws://localhost/";
    }
    curl_easy_setopt(curl, CURLOPT_URL, ws_url.c_str());

    // Step 4: Attempt to establish connection
    // Note: This will likely fail in fuzzing context (no network),
    // but we still want to test the WebSocket API calls
    CURLcode perform_result = curl_easy_perform(curl);
    
    // Even if connection fails, we can still test some WebSocket APIs
    // as they might have initialization/parameter validation code paths
    
    // Step 5: Test curl_ws_send with various frame types
    if (fdp.remaining_bytes() > 32) {
        // Consume data for send buffer
        size_t send_buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
        std::vector<uint8_t> send_buffer = fdp.ConsumeBytes<uint8_t>(send_buf_size);
        
        if (!send_buffer.empty()) {
            size_t sent_bytes = 0;
            curl_off_t fragsize = fdp.ConsumeIntegral<curl_off_t>();
            
            // Determine frame type from fuzzer input
            uint8_t frame_type_choice = fdp.ConsumeIntegral<uint8_t>() % 3;
            unsigned int flags = 0;
            
            switch (frame_type_choice) {
                case 0:
                    flags = CURLWS_TEXT;
                    break;
                case 1:
                    flags = CURLWS_BINARY;
                    break;
                case 2:
                    flags = CURLWS_PING;
                    break;
            }
            
            // Try to send WebSocket frame
            CURLcode send_result = curl_ws_send(curl, 
                                               send_buffer.data(), 
                                               send_buffer.size(), 
                                               &sent_bytes, 
                                               fragsize, 
                                               flags);
            // Result is ignored intentionally - we want to explore error paths too
            (void)send_result;
        }
    }

    // Step 6: Test curl_ws_recv
    if (fdp.remaining_bytes() > 16) {
        // Allocate receive buffer
        size_t recv_buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 512);
        std::vector<uint8_t> recv_buffer(recv_buf_size);
        
        size_t received_bytes = 0;
        const struct curl_ws_frame* meta = nullptr;
        
        // Try to receive WebSocket data
        CURLcode recv_result = curl_ws_recv(curl, 
                                           recv_buffer.data(), 
                                           recv_buffer.size(), 
                                           &received_bytes, 
                                           &meta);
        // Result is ignored intentionally - we want to explore error paths too
        (void)recv_result;
        
        // Step 7: Test curl_ws_meta
        if (meta != nullptr) {
            // Access metadata fields to ensure they're read
            (void)meta->age;
            (void)meta->flags;
            (void)meta->offset;
            (void)meta->bytesleft;
            (void)meta->len;
        }
        
        // Also test curl_ws_meta API directly
        const struct curl_ws_frame* direct_meta = curl_ws_meta(curl);
        if (direct_meta != nullptr) {
            (void)direct_meta->age;
            (void)direct_meta->flags;
            (void)direct_meta->offset;
            (void)direct_meta->bytesleft;
            (void)direct_meta->len;
        }
    }

    // Step 8: Test curl_ws_start_frame
    if (fdp.remaining_bytes() > 8) {
        // Determine frame type for start_frame
        uint8_t start_frame_type = fdp.ConsumeIntegral<uint8_t>() % 2;
        unsigned int start_flags = 0;
        
        if (start_frame_type == 0) {
            start_flags = CURLWS_PING;
        } else {
            start_flags = CURLWS_TEXT;
        }
        
        curl_off_t frame_len = fdp.ConsumeIntegral<curl_off_t>();
        
        // Try to start a WebSocket frame
        CURLcode start_result = curl_ws_start_frame(curl, start_flags, frame_len);
        // Result is ignored intentionally - we want to explore error paths too
        (void)start_result;
    }

    // Step 9: Test additional send with PONG flag (response to PING)
    if (fdp.remaining_bytes() > 16) {
        size_t pong_buf_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
        std::vector<uint8_t> pong_buffer = fdp.ConsumeBytes<uint8_t>(pong_buf_size);
        
        if (!pong_buffer.empty()) {
            size_t pong_sent = 0;
            curl_off_t pong_fragsize = fdp.ConsumeIntegral<curl_off_t>();
            
            // Send PONG frame (response to PING)
            CURLcode pong_result = curl_ws_send(curl, 
                                               pong_buffer.data(), 
                                               pong_buffer.size(), 
                                               &pong_sent, 
                                               pong_fragsize, 
                                               CURLWS_PONG);
            (void)pong_result;
        }
    }

    // Step 10: Test send with CLOSE flag
    if (fdp.remaining_bytes() > 8) {
        size_t close_sent = 0;
        std::string close_reason = fdp.ConsumeRandomLengthString(64);
        
        // Send CLOSE frame
        CURLcode close_result = curl_ws_send(curl, 
                                            close_reason.c_str(), 
                                            close_reason.length(), 
                                            &close_sent, 
                                            0, 
                                            CURLWS_CLOSE);
        (void)close_result;
    }

    // Step 11: Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

} // namespace harness_009
