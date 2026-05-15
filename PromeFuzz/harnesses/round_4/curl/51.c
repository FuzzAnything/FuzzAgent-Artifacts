// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    // Do nothing
    (void)ptr;
    (void)userdata;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    // Initialize curl
    curl_global_init(CURL_GLOBAL_ALL);
    CURL *curl = curl_easy_init();
    
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    // Set up a dummy URL and options to simulate a connection
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L); // For WebSocket
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    
    // Try to perform the connection (will likely fail, but sets up internal state)
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Use the fuzzer input for various function calls
    if (Size > 0) {
        // Prepare buffers and variables
        void *buffer = malloc(Size);
        if (!buffer) {
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return 0;
        }
        memcpy(buffer, Data, Size);
        size_t n = 0;
        size_t sent = 0;
        size_t recv = 0;
        const struct curl_ws_frame *meta = NULL;
        curl_off_t fragsize = 0;
        unsigned int flags = 0;
        
        // Extract parameters from fuzzer input
        if (Size >= sizeof(curl_off_t) + sizeof(unsigned int)) {
            memcpy(&fragsize, Data, sizeof(curl_off_t));
            memcpy(&flags, Data + sizeof(curl_off_t), sizeof(unsigned int));
        }
        
        // Call curl_ws_recv
        curl_ws_recv(curl, buffer, Size, &recv, &meta);
        
        // Call curl_easy_recv
        curl_easy_recv(curl, buffer, Size, &n);
        
        // Call curl_easy_pause - use first byte as bitmask if available
        int bitmask = Size > 0 ? Data[0] : 0;
        curl_easy_pause(curl, bitmask);
        
        // Call curl_ws_send
        curl_ws_send(curl, buffer, Size, &sent, fragsize, flags);
        
        // Call curl_ws_meta
        meta = curl_ws_meta(curl);
        
        // Call curl_easy_send
        curl_easy_send(curl, buffer, Size, &n);
        
        free(buffer);
    }
    
    // Cleanup
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}