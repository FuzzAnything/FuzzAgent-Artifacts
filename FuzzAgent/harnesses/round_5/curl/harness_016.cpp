#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/websockets.h>

namespace harness_016 {
/*
 * Fuzzing harness for libcurl's WebSocket operations (recompiled)
 * Targets: curl_ws_start_frame (40 branches, 42 undiscovered), curl_ws_send,
 *          curl_ws_recv, curl_ws_meta (12 branches, 12 undiscovered),
 *          curl_easy_getinfo (4 branches, 18 undiscovered)
 * Helpers: curl_easy_init, curl_easy_setopt, curl_easy_perform, curl_easy_cleanup
 * Semantic differentiation from previous harnesses:
 *   - harness_000: Basic HTTP operations
 *   - harness_001: Header manipulation  
 *   - harness_002: MIME (multipart/form-data) operations
 *   - harness_003: Deprecated form data API
 *   - harness_016: WebSocket operations (recompiled) (completely untouched - 0% coverage)
 * Focus: Tests WebSocket lifecycle, frame operations, and metadata retrieval
 * Note: Modified to trigger recompilation for missing fuzzer binary
 */


// Simple write callback (not used for WebSockets but needed for curl_easy_perform)
static size_t discard_write_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;  // Always report success
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Minimum size needed for meaningful WebSocket fuzzing
    // Need enough for URL, flags, frame length, and some data
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume fixed-size configuration data first
    uint8_t ws_flags = fdp.ConsumeIntegral<uint8_t>();
    uint8_t ws_options = fdp.ConsumeIntegral<uint8_t>();
    uint8_t frame_type = fdp.ConsumeIntegral<uint8_t>() % 6; // 0-5 for different frame types
    uint16_t max_frame_len = fdp.ConsumeIntegralInRange<uint16_t>(1, 1024);
    uint8_t url_length = fdp.ConsumeIntegralInRange<uint8_t>(1, 128);
    bool use_start_frame = fdp.ConsumeBool();
    bool use_raw_mode = fdp.ConsumeBool();
    bool no_auto_pong = fdp.ConsumeBool();
    
    // Consume URL string (ws:// or wss://)
    std::string url = fdp.ConsumeBytesAsString(url_length);
    
    // Consume data for sending
    size_t send_data_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(max_frame_len, (uint16_t)4096));
    std::vector<uint8_t> send_data = fdp.ConsumeBytes<uint8_t>(send_data_len);
    
    // Consume remaining data for receive buffer
    std::vector<uint8_t> recv_buffer(fdp.ConsumeRemainingBytes<uint8_t>());
    if (recv_buffer.size() < 128) {
        // Ensure minimum buffer size for receiving
        recv_buffer.resize(128);
    }
    
    // Initialize curl global state
    curl_global_init(CURL_GLOBAL_ALL);
    
    // 1. Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    CURLcode res;
    
    // 2. Set URL - use WebSocket protocol (ws:// or wss://)
    // If URL doesn't start with ws:// or wss://, prepend ws://
    if (url.find("ws://") != 0 && url.find("wss://") != 0) {
        url = "ws://" + url;
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // 3. Set WebSocket options
    long ws_options_mask = 0;
    if (use_raw_mode) {
        ws_options_mask |= CURLWS_RAW_MODE;
    }
    if (no_auto_pong) {
        ws_options_mask |= CURLWS_NOAUTOPONG;
    }
    curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, ws_options_mask);
    
    // Set CONNECT_ONLY to 2L for WebSocket style connections
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    // Set write callback (though WebSockets use different APIs)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, discard_write_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullptr);
    
    // Set timeout to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    
    // 4. Perform the connection (this establishes WebSocket handshake)
    res = curl_easy_perform(curl);
    
    // Note: curl_easy_perform may fail since we're using random URLs,
    // but we can still test the WebSocket APIs in offline mode
    // The APIs should handle the connection state appropriately
    
    // Map frame_type to WebSocket flags
    unsigned int frame_flags = 0;
    switch (frame_type) {
        case 0:
            frame_flags = CURLWS_TEXT;
            break;
        case 1:
            frame_flags = CURLWS_BINARY;
            break;
        case 2:
            frame_flags = CURLWS_CONT;
            break;
        case 3:
            frame_flags = CURLWS_CLOSE;
            break;
        case 4:
            frame_flags = CURLWS_PING;
            break;
        case 5:
            frame_flags = CURLWS_PONG;
            break;
    }
    
    // 5. Start a WebSocket frame (if requested)
    if (use_start_frame && send_data.size() > 0) {
        curl_off_t frame_len = static_cast<curl_off_t>(send_data.size());
        res = curl_ws_start_frame(curl, frame_flags, frame_len);
        // Don't check result - we're fuzzing API usage
    }
    
    // 6. Send WebSocket data
    size_t sent = 0;
    if (send_data.size() > 0) {
        // Use CURLWS_OFFSET if we started a frame
        unsigned int send_flags = frame_flags;
        if (use_start_frame) {
            send_flags |= CURLWS_OFFSET;
        }
        
        res = curl_ws_send(curl, send_data.data(), send_data.size(), 
                          &sent, 0, send_flags);
        // Don't check result - we're fuzzing API usage
    }
    
    // 7. Receive WebSocket data
    size_t received = 0;
    const struct curl_ws_frame *meta = nullptr;
    
    // Try to receive data (non-blocking)
    res = curl_ws_recv(curl, recv_buffer.data(), recv_buffer.size(), 
                      &received, &meta);
    // Don't check result - we're fuzzing API usage
    
    // 8. Get WebSocket metadata
    if (meta != nullptr) {
        // Access metadata fields
        int age = meta->age;
        int flags = meta->flags;
        curl_off_t offset = meta->offset;
        curl_off_t bytesleft = meta->bytesleft;
        size_t len = meta->len;
        (void)age;
        (void)flags;
        (void)offset;
        (void)bytesleft;
        (void)len;
    }
    
    // Also test curl_ws_meta function
    const struct curl_ws_frame *meta2 = curl_ws_meta(curl);
    if (meta2 != nullptr) {
        // Access metadata fields
        int age2 = meta2->age;
        int flags2 = meta2->flags;
        curl_off_t offset2 = meta2->offset;
        curl_off_t bytesleft2 = meta2->bytesleft;
        size_t len2 = meta2->len;
        (void)age2;
        (void)flags2;
        (void)offset2;
        (void)bytesleft2;
        (void)len2;
    }
    
    // 9. Get connection info using curl_easy_getinfo
    long connect_code = 0;
    double total_time = 0.0;
    char *effective_url = nullptr;
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &connect_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &effective_url);
    
    (void)connect_code;
    (void)total_time;
    (void)effective_url;
    
    // Test additional getinfo options
    long http_connect = 0;
    long ssl_verifyresult = 0;
    curl_easy_getinfo(curl, CURLINFO_HTTP_CONNECTCODE, &http_connect);
    curl_easy_getinfo(curl, CURLINFO_SSL_VERIFYRESULT, &ssl_verifyresult);
    (void)http_connect;
    (void)ssl_verifyresult;
    
    // 10. Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_016
