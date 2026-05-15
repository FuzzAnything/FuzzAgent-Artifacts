// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void dummy_callback(void) {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    // Create a curl handle
    CURL *curl = curl_easy_init();
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set up a dummy URL for connection
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    
    // Set CONNECT_ONLY option for raw socket/WebSocket operations
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    // Disable verification for fuzzing
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
    
    // Set timeout to prevent hanging
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    
    // Perform the connection (may fail without server, but that's OK for fuzzing)
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Prepare buffers and variables for API calls
    size_t buffer_size = Size > 0 ? Size : 1;
    void *buffer = malloc(buffer_size);
    if (!buffer) {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
        return 0;
    }
    
    if (Size > 0) {
        memcpy(buffer, Data, Size > buffer_size ? buffer_size : Size);
    } else {
        // Ensure buffer has some data even if input is empty
        memset(buffer, 0, buffer_size);
    }
    
    size_t bytes_processed = 0;
    const struct curl_ws_frame *meta = NULL;
    curl_off_t fragsize = 0;
    unsigned int flags = 0;
    
    // Call the target API functions with fuzzer-provided data
    
    // 1. curl_ws_recv
    if (perform_res == CURLE_OK) {
        CURLcode recv_res = curl_ws_recv(curl, buffer, buffer_size, &bytes_processed, &meta);
        (void)recv_res; // Ignore result for fuzzing
    }
    
    // 2. curl_easy_recv
    if (perform_res == CURLE_OK) {
        CURLcode easy_recv_res = curl_easy_recv(curl, buffer, buffer_size, &bytes_processed);
        (void)easy_recv_res; // Ignore result for fuzzing
    }
    
    // 3. curl_easy_perform (already called above, but call again with different options)
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    CURLcode perform_res2 = curl_easy_perform(curl);
    (void)perform_res2; // Ignore result for fuzzing
    
    // 4. curl_ws_send
    if (perform_res == CURLE_OK) {
        // Use fuzzer data to determine flags and fragsize
        if (Size >= sizeof(unsigned int) + sizeof(curl_off_t)) {
            memcpy(&flags, Data, sizeof(unsigned int));
            memcpy(&fragsize, Data + sizeof(unsigned int), sizeof(curl_off_t));
        } else if (Size >= sizeof(unsigned int)) {
            memcpy(&flags, Data, sizeof(unsigned int));
        }
        
        CURLcode send_res = curl_ws_send(curl, buffer, buffer_size, &bytes_processed, fragsize, flags);
        (void)send_res; // Ignore result for fuzzing
    }
    
    // 5. curl_easy_pause
    int bitmask = 0;
    if (Size >= sizeof(int)) {
        memcpy(&bitmask, Data, sizeof(int));
    }
    CURLcode pause_res = curl_easy_pause(curl, bitmask);
    (void)pause_res; // Ignore result for fuzzing
    
    // 6. curl_easy_send
    if (perform_res == CURLE_OK) {
        CURLcode easy_send_res = curl_easy_send(curl, buffer, buffer_size, &bytes_processed);
        (void)easy_send_res; // Ignore result for fuzzing
    }
    
    // Cleanup
    free(buffer);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}