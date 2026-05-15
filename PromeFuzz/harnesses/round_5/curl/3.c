// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_meta at ws.c:1851:29 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0: {  // curl_easy_pause
            write_dummy_file(fuzz_data, fuzz_size);
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                int bitmask = 0;
                if (fuzz_size > 0) {
                    bitmask = fuzz_data[0] % 4;
                    if (bitmask == 1) bitmask = CURLPAUSE_RECV;
                    else if (bitmask == 2) bitmask = CURLPAUSE_SEND;
                    else if (bitmask == 3) bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND;
                }
                curl_easy_pause(curl, bitmask);
            }
            break;
        }
        
        case 1: {  // curl_ws_meta
            curl_easy_setopt(curl, CURLOPT_URL, "ws://dummy.example.com");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);  // CONNECT_ONLY for WS
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK) {
                const struct curl_ws_frame *meta = curl_ws_meta(curl);
                (void)meta;  // Use result to avoid unused variable warning
            }
            break;
        }
        
        case 2: {  // curl_ws_recv
            curl_easy_setopt(curl, CURLOPT_URL, "ws://dummy.example.com");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && fuzz_size > 0) {
                void *buffer = malloc(fuzz_size);
                if (buffer) {
                    size_t recv = 0;
                    const struct curl_ws_frame *meta = NULL;
                    memcpy(buffer, fuzz_data, fuzz_size);
                    curl_ws_recv(curl, buffer, fuzz_size, &recv, &meta);
                    free(buffer);
                }
            }
            break;
        }
        
        case 3: {  // curl_easy_send
            curl_easy_setopt(curl, CURLOPT_URL, "http://dummy.example.com");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && fuzz_size > 0) {
                void *buffer = malloc(fuzz_size);
                if (buffer) {
                    size_t sent = 0;
                    memcpy(buffer, fuzz_data, fuzz_size);
                    curl_easy_send(curl, buffer, fuzz_size, &sent);
                    free(buffer);
                }
            }
            break;
        }
        
        case 4: {  // curl_easy_recv
            curl_easy_setopt(curl, CURLOPT_URL, "http://dummy.example.com");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            
            CURLcode res = curl_easy_perform(curl);
            if (res == CURLE_OK && fuzz_size > 0) {
                void *buffer = malloc(fuzz_size);
                if (buffer) {
                    size_t recv = 0;
                    curl_easy_recv(curl, buffer, fuzz_size, &recv);
                    free(buffer);
                }
            }
            break;
        }
        
        case 5: {  // curl_easy_perform
            write_dummy_file(fuzz_data, fuzz_size);
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            
            curl_easy_perform(curl);
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    unlink("./dummy_file");
    return 0;
}