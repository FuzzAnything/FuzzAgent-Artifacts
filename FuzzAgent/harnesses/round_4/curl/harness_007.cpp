#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <fuzzer/FuzzedDataProvider.h>
#include "curl/curl.h"
#include "curl/websockets.h"

namespace harness_007 {
/*
 * Fuzzing harness for libcurl targeting WebSocket API family
 * Harness ID: 007
 * Targets: curl_ws_recv, curl_ws_send, curl_ws_start_frame, curl_ws_meta APIs
 * Focus: WebSocket protocol handling, frame types, and metadata
 * Differentiated from:
 * - harness_000.cpp (curl_url/curl_easy APIs)
 * - harness_001.cpp (MIME APIs) 
 * - harness_002.cpp (curl_multi APIs)
 * - harness_003.cpp (FTP protocol)
 * - harness_004.cpp (Form APIs)
 * - harness_005.cpp (HTTP Headers Management)
 * - harness_006.cpp (Share APIs)
 * This harness specifically targets WebSocket API coverage gap with 0% coverage
 * and 1000+ undiscovered branches across all fuzzing campaigns
 * Focuses on: WebSocket frame handling, message sending/receiving, metadata retrieval
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check - need enough data for WebSocket operations
    if (size < 32) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl globally
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // ==================== WebSocket Handle Creation ====================
    
    // Create an easy handle for WebSocket operations
    CURL* easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set CONNECT_ONLY option for WebSocket style connection
    // Note: We use a dummy value since we're fuzzing API calls, not establishing real connections
    curl_easy_setopt(easy_handle, CURLOPT_CONNECT_ONLY, 2L);
    
    // ==================== WebSocket API Testing ====================
    
    // Determine which WebSocket operations to test
    size_t num_operations = fdp.ConsumeIntegralInRange<size_t>(1, 10);
    
    for (size_t i = 0; i < num_operations && fdp.remaining_bytes() > 16; i++) {
        // Consume operation type (0-3 for different WebSocket APIs)
        int operation_type = fdp.ConsumeIntegralInRange<int>(0, 3);
        
        switch (operation_type) {
            case 0: {
                // Test curl_ws_send() - sending WebSocket frames
                
                // Consume frame flags from fuzzed data
                unsigned int flags = 0;
                if (fdp.ConsumeBool()) flags |= CURLWS_TEXT;
                if (fdp.ConsumeBool()) flags |= CURLWS_BINARY;
                if (fdp.ConsumeBool()) flags |= CURLWS_CONT;
                if (fdp.ConsumeBool()) flags |= CURLWS_CLOSE;
                if (fdp.ConsumeBool()) flags |= CURLWS_PING;
                if (fdp.ConsumeBool()) flags |= CURLWS_PONG;
                if (fdp.ConsumeBool()) flags |= CURLWS_OFFSET;
                
                // Consume fragsize (0 means send entire buffer at once)
                curl_off_t fragsize = fdp.ConsumeIntegral<curl_off_t>();
                
                // Consume payload data for sending
                size_t payload_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
                if (payload_size > fdp.remaining_bytes()) {
                    payload_size = fdp.remaining_bytes();
                }
                std::vector<uint8_t> payload = fdp.ConsumeBytes<uint8_t>(payload_size);
                
                size_t sent = 0;
                if (!payload.empty()) {
                    // Try sending the frame
                    CURLcode result = curl_ws_send(easy_handle, 
                                                  payload.data(), 
                                                  payload.size(), 
                                                  &sent, 
                                                  fragsize, 
                                                  flags);
                    
                    // Result can be CURLE_OK, CURLE_AGAIN, or various error codes
                    // All are valid outcomes for fuzzing
                    (void)result;
                }
                break;
            }
            
            case 1: {
                // Test curl_ws_recv() - receiving WebSocket frames
                
                // Allocate buffer for receiving
                size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 2048);
                std::vector<uint8_t> buffer(buffer_size);
                
                size_t recv = 0;
                const struct curl_ws_frame* meta = nullptr;
                
                if (!buffer.empty()) {
                    // Try receiving a frame
                    CURLcode result = curl_ws_recv(easy_handle, 
                                                  buffer.data(), 
                                                  buffer.size(), 
                                                  &recv, 
                                                  &meta);
                    
                    // Result can be CURLE_OK, CURLE_AGAIN, or various error codes
                    // All are valid outcomes for fuzzing
                    (void)result;
                    
                    // If metadata was returned, examine it
                    if (meta) {
                        // Access metadata fields to exercise the code
                        (void)meta->age;
                        (void)meta->flags;
                        (void)meta->offset;
                        (void)meta->bytesleft;
                        (void)meta->len;
                    }
                }
                break;
            }
            
            case 2: {
                // Test curl_ws_start_frame() - starting a new WebSocket frame
                
                // Consume frame flags
                unsigned int flags = 0;
                if (fdp.ConsumeBool()) flags |= CURLWS_TEXT;
                if (fdp.ConsumeBool()) flags |= CURLWS_BINARY;
                if (fdp.ConsumeBool()) flags |= CURLWS_CONT;
                if (fdp.ConsumeBool()) flags |= CURLWS_CLOSE;
                if (fdp.ConsumeBool()) flags |= CURLWS_PING;
                if (fdp.ConsumeBool()) flags |= CURLWS_PONG;
                if (fdp.ConsumeBool()) flags |= CURLWS_OFFSET;
                
                // Consume frame length
                curl_off_t frame_len = fdp.ConsumeIntegral<curl_off_t>();
                
                // Start a new frame
                CURLcode result = curl_ws_start_frame(easy_handle, flags, frame_len);
                
                // Result can be CURLE_OK or various error codes
                (void)result;
                break;
            }
            
            case 3: {
                // Test curl_ws_meta() - getting WebSocket frame metadata
                
                // Get metadata for current frame
                const struct curl_ws_frame* meta = curl_ws_meta(easy_handle);
                
                // If metadata was returned, examine it
                if (meta) {
                    // Access all metadata fields to exercise the code
                    (void)meta->age;
                    (void)meta->flags;
                    (void)meta->offset;
                    (void)meta->bytesleft;
                    (void)meta->len;
                    
                    // Test different flag combinations by examining them
                    if (meta->flags & CURLWS_TEXT) {
                        // TEXT frame flag is set
                    }
                    if (meta->flags & CURLWS_BINARY) {
                        // BINARY frame flag is set
                    }
                    if (meta->flags & CURLWS_CONT) {
                        // CONTinuation frame flag is set
                    }
                    if (meta->flags & CURLWS_CLOSE) {
                        // CLOSE frame flag is set
                    }
                    if (meta->flags & CURLWS_PING) {
                        // PING frame flag is set
                    }
                    if (meta->flags & CURLWS_PONG) {
                        // PONG frame flag is set
                    }
                }
                break;
            }
        }
    }
    
    // ==================== Additional WebSocket Option Testing ====================
    
    // Test CURLOPT_WS_OPTIONS if we have enough data
    if (fdp.remaining_bytes() > 4) {
        unsigned long ws_options = 0;
        if (fdp.ConsumeBool()) ws_options |= CURLWS_RAW_MODE;
        if (fdp.ConsumeBool()) ws_options |= CURLWS_NOAUTOPONG;
        
        curl_easy_setopt(easy_handle, CURLOPT_WS_OPTIONS, ws_options);
    }
    
    // ==================== Cleanup ====================
    
    // Clean up easy handle
    curl_easy_cleanup(easy_handle);
    
    // Clean up global state
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_007
