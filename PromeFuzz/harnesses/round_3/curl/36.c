// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
// curl_easy_header at headers.c:55:11 in header.h
// curl_easy_nextheader at headers.c:121:21 in header.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/header.h"
#include "curl/easy.h"

static int curl_initialized = 0;

static void init_curl(void) {
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl_initialized = 1;
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    init_curl();
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    // Use a dummy URL for testing
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    
    // Set various options using fuzzer data
    if (Size > 0) {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)(Data[0] & 1));
    }
    
    // Try to perform the transfer (will likely fail without real connection)
    CURLcode perform_res = curl_easy_perform(curl);
    
    // Test curl_ws_send if we have enough data
    if (Size > 0) {
        size_t sent = 0;
        curl_off_t fragsize = (Size > 4) ? (curl_off_t)(Data[0] | (Data[1] << 8)) : 0;
        unsigned int flags = (Size > 6) ? (unsigned int)(Data[2] | (Data[3] << 8)) : 0;
        
        // Use part of the input as buffer
        size_t buflen = (Size < 1024) ? Size : 1024;
        const void *buffer = Data;
        
        curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
    }
    
    // Test curl_ws_recv
    if (Size > 0) {
        char recv_buffer[1024];
        size_t recv_len = 0;
        const struct curl_ws_frame *meta = NULL;
        
        curl_ws_recv(curl, recv_buffer, sizeof(recv_buffer), &recv_len, &meta);
    }
    
    // Test curl_easy_header and curl_easy_nextheader
    struct curl_header *header = NULL;
    struct curl_header *prev = NULL;
    
    // Try to get various headers
    if (Size > 0) {
        // Create a dummy header name from input data
        char header_name[64];
        size_t name_len = (Size < 63) ? Size : 63;
        memcpy(header_name, Data, name_len);
        header_name[name_len] = '\0';
        
        CURLHcode hcode = curl_easy_header(curl, header_name, 0, 
                                          CURLH_HEADER, 0, &header);
        
        // Iterate through headers if we got one
        if (header) {
            while ((prev = curl_easy_nextheader(curl, CURLH_HEADER, 0, prev)) != NULL) {
                // Just iterate through headers
            }
        }
        
        // Try with different origins and requests
        for (unsigned int origin = 0; origin < 4; origin++) {
            for (int request = 0; request < 2; request++) {
                curl_easy_header(curl, "Content-Type", 0, origin, request, &header);
                prev = NULL;
                while ((prev = curl_easy_nextheader(curl, origin, request, prev)) != NULL) {
                    // Iterate
                }
            }
        }
    }
    
    // Test more curl_easy_setopt options with fuzzer data
    if (Size > 8) {
        long timeout = (long)(Data[4] | (Data[5] << 8));
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        
        long connect_timeout = (long)(Data[6] | (Data[7] << 8));
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connect_timeout);
    }
    
    // Try setting CONNECT_ONLY for WebSocket
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    // Try performing again with different settings
    if (Size > 0) {
        curl_easy_perform(curl);
    }
    
    curl_easy_cleanup(curl);
    return 0;
}