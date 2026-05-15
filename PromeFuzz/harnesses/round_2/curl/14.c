// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static size_t dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    static const char dummy_data[] = "dummy";
    size_t to_copy = sizeof(dummy_data) - 1;
    if (to_copy > size * nitems) {
        to_copy = size * nitems;
    }
    memcpy(buffer, dummy_data, to_copy);
    return to_copy;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    unsigned int seed = Data[0];
    size_t action = seed % 6;
    
    switch (action) {
        case 0: {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(curl, CURLOPT_INFILESIZE, (long)Size);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                size_t sent = 0;
                curl_easy_send(curl, Data, Size, &sent);
            }
            break;
        }
        case 1: {
            curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                char buffer[1024];
                size_t recv = 0;
                const struct curl_ws_frame *meta = NULL;
                curl_ws_recv(curl, buffer, sizeof(buffer), &recv, &meta);
            }
            break;
        }
        case 2: {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_perform(curl);
            break;
        }
        case 3: {
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_VERBOSE, (seed & 1) ? 1L : 0L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (seed & 2) ? 1L : 0L);
            curl_easy_setopt(curl, CURLOPT_NOBODY, (seed & 4) ? 1L : 0L);
            break;
        }
        case 4: {
            curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                unsigned int flags = CURLWS_TEXT;
                if (seed & 1) flags |= CURLWS_BINARY;
                if (seed & 2) flags |= CURLWS_PING;
                if (seed & 4) flags |= CURLWS_PONG;
                
                curl_off_t frame_len = (curl_off_t)(Size % 65536);
                curl_ws_start_frame(curl, flags, frame_len);
            }
            break;
        }
        case 5: {
            curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost/dummy");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                size_t sent = 0;
                unsigned int flags = CURLWS_TEXT;
                if (seed & 1) flags |= CURLWS_BINARY;
                if (seed & 2) flags |= CURLWS_PING;
                if (seed & 4) flags |= CURLWS_PONG;
                
                curl_off_t fragsize = (seed & 8) ? (curl_off_t)(Size % 1024) : 0;
                curl_ws_send(curl, Data, Size, &sent, fragsize, flags);
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    return 0;
}