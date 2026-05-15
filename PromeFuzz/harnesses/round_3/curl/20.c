// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res = CURLE_OK;
    size_t bytes_transferred = 0;
    char buffer[4096];
    const struct curl_ws_frame *meta = NULL;
    
    if (Size == 0) {
        return 0;
    }
    
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
    
    res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_upkeep(curl);
        
        if (Size > 0) {
            size_t send_size = Size > sizeof(buffer) ? sizeof(buffer) : Size;
            memcpy(buffer, Data, send_size);
            
            curl_easy_send(curl, buffer, send_size, &bytes_transferred);
            
            curl_easy_recv(curl, buffer, sizeof(buffer), &bytes_transferred);
            
            curl_ws_recv(curl, buffer, sizeof(buffer), &bytes_transferred, &meta);
        }
        
        int bitmask = 0;
        if (Size > 0) {
            bitmask = Data[0] & 0x03;
            if (bitmask == 0) {
                bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND;
            }
        }
        curl_easy_pause(curl, bitmask);
    }
    
    curl_easy_cleanup(curl);
    
    return 0;
}