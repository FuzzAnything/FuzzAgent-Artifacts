// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void dummy_callback() {
    // Empty callback for fuzzing
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    // Use the fuzzer input to determine which function to test
    // We'll use the first byte as selector
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0] % 6;
    }
    
    // Use remaining data for function parameters
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size > 1 ? Size - 1 : 0;
    
    switch (selector) {
        case 0: {
            // Test curl_easy_pause
            int bitmask = 0;
            if (func_size >= 1) {
                bitmask = func_data[0];
            }
            curl_easy_pause(curl, bitmask);
            break;
        }
        
        case 1: {
            // Test curl_ws_recv
            char buffer[1024];
            size_t recv = 0;
            const struct curl_ws_frame *meta = NULL;
            size_t buflen = func_size < sizeof(buffer) ? func_size : sizeof(buffer);
            
            if (buflen > 0) {
                memcpy(buffer, func_data, buflen);
            }
            
            curl_ws_recv(curl, buffer, buflen, &recv, &meta);
            break;
        }
        
        case 2: {
            // Test curl_easy_recv
            char buffer[1024];
            size_t n = 0;
            size_t buflen = func_size < sizeof(buffer) ? func_size : sizeof(buffer);
            
            if (buflen > 0) {
                memcpy(buffer, func_data, buflen);
            }
            
            curl_easy_recv(curl, buffer, buflen, &n);
            break;
        }
        
        case 3: {
            // Test curl_ws_start_frame
            unsigned int flags = 0;
            curl_off_t frame_len = 0;
            
            if (func_size >= 1) {
                flags = func_data[0];
            }
            if (func_size >= 5) {
                memcpy(&frame_len, func_data + 1, sizeof(frame_len) < (func_size - 1) ? 
                       sizeof(frame_len) : (func_size - 1));
            }
            
            curl_ws_start_frame(curl, flags, frame_len);
            break;
        }
        
        case 4: {
            // Test curl_ws_send
            char buffer[1024];
            size_t sent = 0;
            curl_off_t fragsize = 0;
            unsigned int flags = 0;
            size_t buflen = func_size < sizeof(buffer) ? func_size : sizeof(buffer);
            
            if (buflen > 0) {
                memcpy(buffer, func_data, buflen);
            }
            
            if (func_size >= 1) {
                flags = func_data[0];
            }
            
            curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
            break;
        }
        
        case 5: {
            // Test curl_easy_send
            char buffer[1024];
            size_t n = 0;
            size_t buflen = func_size < sizeof(buffer) ? func_size : sizeof(buffer);
            
            if (buflen > 0) {
                memcpy(buffer, func_data, buflen);
            }
            
            curl_easy_send(curl, buffer, buflen, &n);
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    return 0;
}