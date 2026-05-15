// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    size_t bytes_sent = 0;
    size_t bytes_recv = 0;
    const struct curl_ws_frame *meta = NULL;
    char buffer[4096];
    int bitmask = 0;
    
    // Initialize curl handle
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    // Set up a dummy URL for connection
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    
    // Set CONNECT_ONLY option to enable raw socket operations
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
    // Perform the connection (will fail without actual server, but that's OK for fuzzing)
    res = curl_easy_perform(curl);
    
    // Use fuzz data to determine which operations to perform
    if (Size > 0) {
        // Use first byte to determine bitmask for pause
        bitmask = Data[0];
        
        // Copy fuzz data to buffer (limited to buffer size)
        size_t copy_size = Size < sizeof(buffer) ? Size : sizeof(buffer);
        memcpy(buffer, Data, copy_size);
        
        // Try curl_easy_send with fuzz data
        curl_easy_send(curl, buffer, copy_size, &bytes_sent);
        
        // Try curl_easy_recv
        curl_easy_recv(curl, buffer, sizeof(buffer), &bytes_recv);
        
        // Try curl_ws_recv
        curl_ws_recv(curl, buffer, sizeof(buffer), &bytes_recv, &meta);
        
        // Try curl_easy_pause with fuzz-derived bitmask
        curl_easy_pause(curl, bitmask);
        
        // Try curl_ws_meta
        meta = curl_ws_meta(curl);
    }
    
    // Clean up
    curl_easy_cleanup(curl);
    
    return 0;
}