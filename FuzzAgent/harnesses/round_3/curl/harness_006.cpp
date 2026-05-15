#include <fuzzer/FuzzedDataProvider.h>
#include <curl/curl.h>
#include <curl/websockets.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace harness_006 {
/*
 * Fuzzing harness for libcurl's WebSocket operations
 * Targets: curl_ws_recv (72 branches, 74 undiscovered), curl_ws_send, 
 *          curl_ws_meta, curl_ws_start_frame
 * Focus on WebSocket protocol manipulation with 0% coverage
 * Based on coverage guidance for WebSocket module
 */


int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    // Need minimum input for meaningful fuzzing
    // We need enough for: URL string, buffer data, flags, frame length, etc.
    if (size < 64) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize libcurl - required for all curl operations
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }
    
    // Create easy handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Consume WebSocket URL from input (ws:// or wss://)
    std::string url = fdp.ConsumeRandomLengthString(256);
    if (url.empty()) {
        // Provide a default URL if none consumed
        url = "ws://example.com/";
    }
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    
    // Set CONNECT_ONLY mode for WebSocket (value 2L for websocket style)
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    // Set WebSocket options based on fuzzed input
    long ws_options = 0;
    if (fdp.ConsumeBool()) {
        ws_options |= CURLWS_RAW_MODE;
    }
    if (fdp.ConsumeBool()) {
        ws_options |= CURLWS_NOAUTOPONG;
    }
    curl_easy_setopt(curl, CURLOPT_WS_OPTIONS, ws_options);
    
    // Try to perform the connection (will likely fail without network, but that's OK)
    // We still want to test the WebSocket API functions
    CURLcode perform_res = curl_easy_perform(curl);
    (void)perform_res; // Result may be error, but we continue testing APIs
    
    // Consume data for WebSocket operations
    // First, determine operation type from fuzzed input
    uint8_t operation_type = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Buffer for send/receive operations
    size_t buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 4096);
    std::vector<uint8_t> buffer;
    
    if (fdp.remaining_bytes() >= buffer_size) {
        buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    } else {
        // If not enough data, use what's left
        buffer = fdp.ConsumeRemainingBytes<uint8_t>();
        buffer_size = buffer.size();
    }
    
    // Flags for WebSocket operations
    unsigned int ws_flags = 0;
    uint8_t flag_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    switch (flag_choice) {
        case 0: ws_flags = CURLWS_TEXT; break;
        case 1: ws_flags = CURLWS_BINARY; break;
        case 2: ws_flags = CURLWS_CONT; break;
        case 3: ws_flags = CURLWS_CLOSE; break;
        case 4: ws_flags = CURLWS_PING; break;
        case 5: ws_flags = CURLWS_PING | CURLWS_OFFSET; break;
        case 6: ws_flags = CURLWS_PONG; break;
        case 7: ws_flags = CURLWS_TEXT | CURLWS_OFFSET; break;
    }
    
    // Test curl_ws_send() - send WebSocket message
    if (buffer_size > 0) {
        size_t sent = 0;
        curl_off_t fragsize = fdp.ConsumeIntegralInRange<curl_off_t>(0, 1024);
        
        // Note: curl_ws_send may fail if not connected, but we still call it
        // to exercise the code paths
        CURLcode send_res = curl_ws_send(curl, buffer.data(), buffer_size, 
                                         &sent, fragsize, ws_flags);
        (void)send_res; // Result may be error, that's OK for fuzzing
    }
    
    // Test curl_ws_recv() - receive WebSocket message
    if (fdp.remaining_bytes() > 0) {
        size_t recv_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);
        std::vector<uint8_t> recv_buffer(recv_size);
        size_t received = 0;
        const struct curl_ws_frame *meta = nullptr;
        
        CURLcode recv_res = curl_ws_recv(curl, recv_buffer.data(), recv_size,
                                         &received, &meta);
        (void)recv_res; // Result may be error, that's OK for fuzzing
        
        // If meta was returned, we can examine it (but no need to do anything)
        (void)meta;
    }
    
    // Test curl_ws_start_frame() - begin new WebSocket frame
    curl_off_t frame_len = fdp.ConsumeIntegralInRange<curl_off_t>(0, 65536);
    unsigned int frame_flags = 0;
    
    // Choose frame flags
    uint8_t frame_flag_choice = fdp.ConsumeIntegral<uint8_t>() % 4;
    switch (frame_flag_choice) {
        case 0: frame_flags = CURLWS_TEXT; break;
        case 1: frame_flags = CURLWS_BINARY; break;
        case 2: frame_flags = CURLWS_PING; break;
        case 3: frame_flags = CURLWS_CLOSE; break;
    }
    
    CURLcode frame_res = curl_ws_start_frame(curl, frame_flags, frame_len);
    (void)frame_res; // Result may be error, that's OK for fuzzing
    
    // Test curl_ws_meta() - retrieve WebSocket frame metadata
    const struct curl_ws_frame *frame_meta = curl_ws_meta(curl);
    (void)frame_meta; // May be nullptr, that's OK
    
    // Additional test: try to send after starting a frame
    if (fdp.remaining_bytes() > 10 && buffer_size > 0) {
        size_t more_sent = 0;
        std::vector<uint8_t> more_buffer = fdp.ConsumeBytes<uint8_t>(
            fdp.ConsumeIntegralInRange<size_t>(1, 100));
        
        if (!more_buffer.empty()) {
            CURLcode more_send_res = curl_ws_send(curl, more_buffer.data(), 
                                                 more_buffer.size(), &more_sent,
                                                 0, CURLWS_CONT);
            (void)more_send_res;
        }
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}

} // namespace harness_006
