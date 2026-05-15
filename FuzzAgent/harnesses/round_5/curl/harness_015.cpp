#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/websockets.h>

namespace harness_015 {
/*
 * Advanced fuzzing harness for libcurl's WebSocket protocol operations
 * Coverage analysis identified WebSocket protocol as critical coverage gap with 67 undiscovered branches.
 * 
 * Primary targets:
 *   - Comprehensive WebSocket protocol lifecycle testing
 *   - Connection establishment and handshake simulation
 *   - Advanced frame operations: fragmentation, continuation frames, control frames
 *   - Protocol state management and error handling paths
 *   - WebSocket-specific curl_easy_setopt options and combinations
 *   - Multiple WebSocket API call sequences and interaction patterns
 * 
 * Semantic differentiation from harness_004.cpp:
 *   - harness_004: Basic WebSocket frame operations (TEXT, BINARY, PING, PONG, CLOSE)
 *   - harness_015: Advanced protocol testing including:
 *     * Frame fragmentation and continuation sequences
 *     * Protocol state transitions and error recovery
 *     * Combined flag usage (CURLWS_CONT with other flags)
 *     * Edge case testing with invalid frame combinations
 *     * Connection lifecycle with various timeout and buffer scenarios
 *     * Raw mode vs auto-pong mode comprehensive testing
 * 
 * Key coverage objectives for 67 undiscovered branches:
 *   - WebSocket frame header parsing with various payload lengths
 *   - Masking/unmasking operations in different modes
 *   - Protocol compliance checking and validation
 *   - Buffer management for fragmented frames
 *   - State machine transitions during connection lifecycle
 *   - Error handling for malformed frames and protocol violations
 * 
 * Invocation sequence:
 *   1. curl_global_init(CURL_GLOBAL_ALL)
 *   2. curl_easy_init()
 *   3. curl_easy_setopt with WebSocket-specific options
 *   4. curl_easy_perform() for connection establishment
 *   5. Multiple WebSocket API sequences with various frame types and fragmentation
 *   6. Protocol state testing with different connection scenarios
 *   7. curl_easy_cleanup() and curl_global_cleanup()
 */


// Simple write callback (not used for WebSockets but needed for curl_easy_perform)
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

// WebSocket protocol test scenarios
enum ws_test_scenario {
    WS_SCENARIO_BASIC_FRAMES = 0,
    WS_SCENARIO_FRAGMENTATION = 1,
    WS_SCENARIO_CONTROL_SEQUENCE = 2,
    WS_SCENARIO_STATE_TRANSITIONS = 3,
    WS_SCENARIO_ERROR_PATHS = 4,
    WS_SCENARIO_PROTOCOL_EDGES = 5,
    WS_SCENARIO_MAX
};

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for comprehensive WebSocket protocol fuzzing
    // Need enough for all protocol configuration, frame data, and test parameters
    if (size < 256) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t test_scenario = fdp.ConsumeIntegral<uint8_t>() % WS_SCENARIO_MAX;
    uint8_t use_raw_mode = fdp.ConsumeBool();
    uint8_t use_no_auto_pong = fdp.ConsumeBool();
    uint8_t test_fragmentation = fdp.ConsumeBool();
    uint8_t test_continuation = fdp.ConsumeBool();
    uint8_t test_control_frames = fdp.ConsumeBool();
    uint8_t test_error_recovery = fdp.ConsumeBool();
    uint8_t connection_timeout = fdp.ConsumeIntegralInRange<uint8_t>(1, 30);
    
    // Protocol-specific parameters
    uint16_t max_frame_size = fdp.ConsumeIntegralInRange<uint16_t>(64, 65535);
    uint8_t num_fragments = fdp.ConsumeIntegralInRange<uint8_t>(1, 16);
    uint8_t num_frames_to_test = fdp.ConsumeIntegralInRange<uint8_t>(1, 32);
    
    // String length parameters
    uint16_t url_length = fdp.ConsumeIntegralInRange<uint16_t>(8, 256);
    uint16_t subprotocol_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 128);
    uint16_t origin_length = fdp.ConsumeIntegralInRange<uint16_t>(0, 128);
    
    // Consume string data
    std::string url = fdp.ConsumeBytesAsString(url_length);
    std::string subprotocol = fdp.ConsumeBytesAsString(subprotocol_length);
    std::string origin = fdp.ConsumeBytesAsString(origin_length);
    
    // Consume frame data for sending
    size_t total_frame_data = fdp.ConsumeIntegralInRange<size_t>(128, 8192);
    std::vector<uint8_t> frame_data = fdp.ConsumeBytes<uint8_t>(total_frame_data);
    
    // Consume remaining data for receive buffers and additional testing
    std::vector<uint8_t> remaining_data = fdp.ConsumeRemainingBytes<uint8_t>();
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    CURLcode res;
    
    // Set URL - ensure it has WebSocket protocol prefix
    if (url.find("ws://") != 0 && url.find("wss://") != 0 && 
        url.find("http://") != 0 && url.find("https://") != 0) {
        url = "ws://" + url;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set WebSocket-specific options
    long ws_options_mask = 0;
    if (use_raw_mode) {
        ws_options_mask |= CURLWS_RAW_MODE;
    }
    if (use_no_auto_pong) {
        ws_options_mask |= CURLWS_NOAUTOPONG;
    }
    curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, ws_options_mask);
    
    // Set CONNECT_ONLY to 2L for WebSocket style connections
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    // Set additional WebSocket protocol headers if provided
    if (!subprotocol.empty()) {
        struct curl_slist *headers = nullptr;
        std::string subprotocol_header = "Sec-WebSocket-Protocol: " + subprotocol;
        headers = curl_slist_append(headers, subprotocol_header.c_str());
        
        if (!origin.empty()) {
            std::string origin_header = "Origin: " + origin;
            headers = curl_slist_append(headers, origin_header.c_str());
        }
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // Note: In real use, we'd need to clean up headers, but for fuzzing we can leak
    }
    
    // Set basic options
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)connection_timeout);
    
    // Perform connection attempt (may fail with random URLs)
    res = curl_easy_perform(curl);
    
    // Test different WebSocket protocol scenarios based on test_scenario
    switch (test_scenario) {
        case WS_SCENARIO_BASIC_FRAMES: {
            // Test basic frame types with various combinations
            for (uint8_t i = 0; i < num_frames_to_test && i < 6; i++) {
                unsigned int frame_flags = 0;
                switch (i % 6) {
                    case 0: frame_flags = CURLWS_TEXT; break;
                    case 1: frame_flags = CURLWS_BINARY; break;
                    case 2: frame_flags = CURLWS_PING; break;
                    case 3: frame_flags = CURLWS_PONG; break;
                    case 4: frame_flags = CURLWS_CLOSE; break;
                    case 5: frame_flags = CURLWS_TEXT | CURLWS_CONT; break;
                }
                
                // Test sending frames
                if (!frame_data.empty() && frame_flags != CURLWS_CLOSE) {
                    size_t frame_size = std::min<size_t>(frame_data.size(), max_frame_size);
                    size_t data_offset = (i * frame_size) % frame_data.size();
                    size_t actual_size = std::min(frame_size, frame_data.size() - data_offset);
                    
                    if (actual_size > 0) {
                        size_t sent = 0;
                        curl_ws_send(curl, frame_data.data() + data_offset, actual_size,
                                    &sent, 0, frame_flags);
                    }
                }
                
                // Test receiving
                if (!remaining_data.empty()) {
                    size_t recv_size = std::min<size_t>(remaining_data.size(), 1024);
                    size_t received = 0;
                    const struct curl_ws_frame *meta = nullptr;
                    curl_ws_recv(curl, remaining_data.data(), recv_size, &received, &meta);
                }
            }
            break;
        }
        
        case WS_SCENARIO_FRAGMENTATION: {
            // Test frame fragmentation with continuation frames
            if (test_fragmentation && !frame_data.empty() && num_fragments > 1) {
                size_t fragment_size = frame_data.size() / num_fragments;
                if (fragment_size > 0) {
                    // Start with TEXT or BINARY frame
                    unsigned int initial_flags = fdp.ConsumeBool() ? CURLWS_TEXT : CURLWS_BINARY;
                    
                    // Start frame with total length
                    curl_ws_start_frame(curl, initial_flags, frame_data.size());
                    
                    // Send fragments
                    for (uint8_t frag = 0; frag < num_fragments; frag++) {
                        size_t offset = frag * fragment_size;
                        size_t this_frag_size = (frag == num_fragments - 1) ? 
                                               (frame_data.size() - offset) : fragment_size;
                        
                        unsigned int frag_flags = CURLWS_OFFSET;
                        if (frag > 0) {
                            frag_flags |= CURLWS_CONT;
                            if (frag == num_fragments - 1) {
                                // Last fragment doesn't have CONT flag
                                frag_flags &= ~CURLWS_CONT;
                            }
                        }
                        
                        size_t sent = 0;
                        curl_ws_send(curl, frame_data.data() + offset, this_frag_size,
                                    &sent, 0, frag_flags);
                    }
                }
            }
            break;
        }
        
        case WS_SCENARIO_CONTROL_SEQUENCE: {
            // Test control frame sequences (PING-PONG, CLOSE handshake)
            if (test_control_frames) {
                // Send PING
                if (!frame_data.empty()) {
                    size_t ping_size = std::min<size_t>(frame_data.size(), 125); // Control frame max
                    size_t sent = 0;
                    curl_ws_send(curl, frame_data.data(), ping_size, &sent, 0, CURLWS_PING);
                    
                    // Try to receive PONG (if auto-pong not disabled)
                    if (!use_no_auto_pong && !remaining_data.empty()) {
                        size_t received = 0;
                        const struct curl_ws_frame *meta = nullptr;
                        curl_ws_recv(curl, remaining_data.data(), 
                                    std::min<size_t>(remaining_data.size(), 1024),
                                    &received, &meta);
                    }
                }
                
                // Test CLOSE frame
                uint16_t close_code = fdp.ConsumeIntegral<uint16_t>();
                std::vector<uint8_t> close_data;
                close_data.push_back((close_code >> 8) & 0xFF);
                close_data.push_back(close_code & 0xFF);
                
                if (!frame_data.empty() && frame_data.size() >= 2) {
                    // Add optional close reason
                    size_t reason_len = std::min<size_t>(frame_data.size() - 2, 123);
                    for (size_t i = 0; i < reason_len; i++) {
                        close_data.push_back(frame_data[i + 2]);
                    }
                    
                    size_t sent = 0;
                    curl_ws_send(curl, close_data.data(), close_data.size(), 
                                &sent, 0, CURLWS_CLOSE);
                }
            }
            break;
        }
        
        case WS_SCENARIO_STATE_TRANSITIONS: {
            // Test protocol state transitions
            // Simulate connection lifecycle with different states
            
            // Test metadata retrieval at different points
            const struct curl_ws_frame *meta = curl_ws_meta(curl);
            if (meta) {
                // Access all metadata fields to ensure coverage
                (void)meta->age;
                (void)meta->flags;
                (void)meta->offset;
                (void)meta->bytesleft;
                (void)meta->len;
            }
            
            // Test sending with various fragment sizes (including 0)
            for (uint8_t i = 0; i < 3 && !frame_data.empty(); i++) {
                size_t test_sizes[] = {0, 1, frame_data.size()};
                size_t test_size = test_sizes[i % 3];
                test_size = std::min(test_size, frame_data.size());
                
                unsigned int test_flags = CURLWS_TEXT;
                if (i == 1) test_flags = CURLWS_BINARY;
                if (i == 2) test_flags = CURLWS_TEXT | CURLWS_CONT;
                
                size_t sent = 0;
                curl_ws_send(curl, frame_data.data(), test_size, &sent, 0, test_flags);
                
                // Test receiving after each send
                if (!remaining_data.empty()) {
                    size_t received = 0;
                    const struct curl_ws_frame *recv_meta = nullptr;
                    curl_ws_recv(curl, remaining_data.data(), 
                                std::min<size_t>(remaining_data.size(), 512),
                                &received, &recv_meta);
                }
            }
            break;
        }
        
        case WS_SCENARIO_ERROR_PATHS: {
            // Test error paths and edge cases
            if (test_error_recovery) {
                // Test invalid flag combinations
                unsigned int invalid_flags[] = {
                    CURLWS_CONT,  // CONT without message type
                    CURLWS_TEXT | CURLWS_BINARY,  // Both TEXT and BINARY
                    CURLWS_PING | CURLWS_PONG,    // Both PING and PONG
                    CURLWS_CLOSE | CURLWS_CONT,   // CLOSE with CONT (may be valid?)
                    0xFF  // All bits set
                };
                
                for (uint8_t i = 0; i < 5 && !frame_data.empty(); i++) {
                    size_t sent = 0;
                    size_t test_size = std::min<size_t>(frame_data.size(), 64);
                    curl_ws_send(curl, frame_data.data(), test_size, 
                                &sent, 0, invalid_flags[i]);
                }
                
                // Test with NULL pointers (should trigger error paths)
                if (!frame_data.empty()) {
                    size_t sent = 0;
                    // Pass NULL buffer with non-zero size
                    curl_ws_send(curl, nullptr, frame_data.size(), &sent, 0, CURLWS_TEXT);
                    
                    // Pass valid buffer with 0 size
                    curl_ws_send(curl, frame_data.data(), 0, &sent, 0, CURLWS_TEXT);
                }
            }
            break;
        }
        
        case WS_SCENARIO_PROTOCOL_EDGES: {
            // Test protocol edge cases
            // Large frames, boundary conditions, timing issues
            
            // Test with maximum control frame payload (125 bytes)
            if (!frame_data.empty()) {
                size_t max_control = std::min<size_t>(frame_data.size(), 125);
                size_t sent = 0;
                curl_ws_send(curl, frame_data.data(), max_control, 
                            &sent, 0, CURLWS_PING);
                
                // Test with various fragment sizes
                curl_off_t frag_sizes[] = {0, 1, 125, 126, 65535, 65536};
                for (uint8_t i = 0; i < 6 && i < frame_data.size(); i++) {
                    size_t test_size = std::min<size_t>(frame_data.size(), 
                                                       static_cast<size_t>(frag_sizes[i]));
                    size_t sent2 = 0;
                    curl_ws_send(curl, frame_data.data(), test_size, 
                                &sent2, frag_sizes[i], CURLWS_TEXT);
                }
            }
            
            // Test receiving with various buffer sizes
            if (!remaining_data.empty()) {
                size_t buffer_sizes[] = {0, 1, 128, 1024, 8192};
                for (uint8_t i = 0; i < 5; i++) {
                    size_t buf_size = std::min(buffer_sizes[i], remaining_data.size());
                    if (buf_size > 0) {
                        size_t received = 0;
                        const struct curl_ws_frame *meta = nullptr;
                        curl_ws_recv(curl, remaining_data.data(), buf_size, 
                                    &received, &meta);
                    }
                }
            }
            break;
        }
    }
    
    // Additional protocol testing regardless of scenario
    
    // Test curl_ws_meta function
    const struct curl_ws_frame *final_meta = curl_ws_meta(curl);
    if (final_meta) {
        // Exercise all metadata access paths
        (void)final_meta->age;
        (void)final_meta->flags;
        (void)final_meta->offset;
        (void)final_meta->bytesleft;
        (void)final_meta->len;
    }
    
    // Test receiving with remaining data
    if (!remaining_data.empty()) {
        for (uint8_t i = 0; i < 3; i++) {
            size_t recv_buf_size = std::min<size_t>(remaining_data.size(), 2048);
            size_t received = 0;
            const struct curl_ws_frame *recv_meta = nullptr;
            curl_ws_recv(curl, remaining_data.data(), recv_buf_size, 
                        &received, &recv_meta);
            
            if (recv_meta) {
                // Access metadata
                (void)recv_meta->age;
                (void)recv_meta->flags;
                (void)recv_meta->offset;
                (void)recv_meta->bytesleft;
                (void)recv_meta->len;
            }
        }
    }
    
    // Test curl_easy_getinfo with WebSocket-related info
    long response_code = 0;
    double total_time = 0.0;
    char *effective_url = nullptr;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    (void)response_code;
    (void)total_time;
    (void)effective_url;
    
    // Test additional connection info
    long http_connect = 0;
    long ssl_verifyresult = 0;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &http_connect);
    curl_easy_getinfo(curl, CURLINFO_SSL_VERIFYRESULT, &ssl_verifyresult);
    (void)http_connect;
    (void)ssl_verifyresult;
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_015
