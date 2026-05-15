// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_ws_start_frame at ws.c:1866:22 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static int dummy_read_callback(char *buffer, size_t size, size_t nitems, void *userdata) {
    return 0;
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }

    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    CURLcode res = curl_easy_perform(curl);
    
    if (Size > 0) {
        size_t sent = 0;
        curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &sent);
        
        size_t ws_sent = 0;
        curl_ws_send(curl, Data, Size > 512 ? 512 : Size, &ws_sent, 0, 0);
        
        size_t received = 0;
        char recv_buffer[1024];
        curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &received);
        
        curl_easy_pause(curl, CURLPAUSE_RECV | CURLPAUSE_SEND);
        curl_easy_pause(curl, 0);
        
        curl_ws_start_frame(curl, 0, Size > 100 ? 100 : Size);
    }
    
    curl_easy_cleanup(curl);
    return 0;
}