// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)userdata;
    return size * nmemb;
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    (void)buffer;
    (void)userdata;
    return 0;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    uint8_t function_selector = 0;
    if (Size > 0) {
        function_selector = Data[0] % 6;
        Data++;
        Size--;
    }

    curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    if (function_selector >= 2 && function_selector <= 5) {
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    }

    CURLcode perform_res = curl_easy_perform(curl);
    (void)perform_res;

    char *buffer = NULL;
    size_t buffer_size = 0;
    if (Size > 0) {
        buffer_size = Size > 1024 ? 1024 : Size;
        buffer = (char *)malloc(buffer_size);
        if (buffer) {
            memcpy(buffer, Data, buffer_size);
        }
    }

    size_t sent_or_received = 0;
    CURLcode res = CURLE_OK;

    switch (function_selector) {
        case 0:
            if (Size > 0) {
                int bitmask = (int)Data[0];
                res = curl_easy_pause(curl, bitmask);
            }
            break;
            
        case 1:
            res = curl_easy_perform(curl);
            break;
            
        case 2:
            if (buffer && buffer_size > 0) {
                unsigned int flags = 0;
                curl_off_t fragsize = 0;
                
                if (buffer_size >= sizeof(unsigned int)) {
                    memcpy(&flags, buffer, sizeof(unsigned int));
                }
                if (buffer_size >= sizeof(unsigned int) + sizeof(curl_off_t)) {
                    memcpy(&fragsize, buffer + sizeof(unsigned int), sizeof(curl_off_t));
                }
                
                res = curl_ws_send(curl, buffer, buffer_size, &sent_or_received, 
                                  fragsize, flags);
            }
            break;
            
        case 3:
            if (buffer && buffer_size > 0) {
                res = curl_easy_recv(curl, buffer, buffer_size, &sent_or_received);
            }
            break;
            
        case 4:
            if (Size >= sizeof(unsigned int) + sizeof(curl_off_t)) {
                unsigned int flags = 0;
                curl_off_t frame_len = 0;
                
                memcpy(&flags, Data, sizeof(unsigned int));
                memcpy(&frame_len, Data + sizeof(unsigned int), sizeof(curl_off_t));
                
                res = curl_ws_start_frame(curl, flags, frame_len);
            }
            break;
            
        case 5:
            if (buffer && buffer_size > 0) {
                res = curl_easy_send(curl, buffer, buffer_size, &sent_or_received);
            }
            break;
    }

    if (buffer) {
        free(buffer);
    }
    curl_easy_cleanup(curl);
    
    return 0;
}