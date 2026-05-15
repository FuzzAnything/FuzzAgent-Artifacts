// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static CURL *curl = NULL;
static int curl_initialized = 0;

static int init_curl(void) {
    if (!curl_initialized) {
        if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
            return 0;
        }
        curl_initialized = 1;
    }
    
    if (!curl) {
        curl = curl_easy_init();
        if (!curl) {
            return 0;
        }
        
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    if (!init_curl()) {
        return 0;
    }
    
    CURLcode res;
    size_t sent_or_recv = 0;
    uint8_t *buffer = NULL;
    size_t buffer_size = 0;
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    
    if (data_offset < Size) {
        buffer_size = Size - data_offset;
        buffer = (uint8_t *)malloc(buffer_size);
        if (buffer) {
            memcpy(buffer, Data + data_offset, buffer_size);
        }
    }
    
    switch (selector) {
        case 0: {
            res = curl_easy_send(curl, buffer, buffer_size, &sent_or_recv);
            break;
        }
        case 1: {
            unsigned int flags = 0;
            if (buffer_size > 0) {
                flags = buffer[0] % 16;
            }
            curl_off_t fragsize = 0;
            if (buffer_size > 1) {
                fragsize = buffer[1];
            }
            res = curl_ws_send(curl, buffer, buffer_size, &sent_or_recv, fragsize, flags);
            break;
        }
        case 2: {
            res = curl_easy_perform(curl);
            break;
        }
        case 3: {
            int bitmask = 0;
            if (buffer_size > 0) {
                bitmask = buffer[0];
            }
            res = curl_easy_pause(curl, bitmask);
            break;
        }
        case 4: {
            res = curl_easy_recv(curl, buffer, buffer_size, &sent_or_recv);
            break;
        }
        case 5: {
            const struct curl_ws_frame *meta = NULL;
            res = curl_ws_recv(curl, buffer, buffer_size, &sent_or_recv, &meta);
            break;
        }
        default:
            break;
    }
    
    if (buffer) {
        free(buffer);
    }
    
    if (selector == 1 || selector == 5) {
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
        res = curl_easy_perform(curl);
    }
    
    if (res == CURLE_OK) {
        uint8_t dummy_buffer[256];
        size_t dummy_sent = 0;
        curl_easy_send(curl, dummy_buffer, sizeof(dummy_buffer), &dummy_sent);
        curl_easy_recv(curl, dummy_buffer, sizeof(dummy_buffer), &dummy_sent);
    }
    
    return 0;
}