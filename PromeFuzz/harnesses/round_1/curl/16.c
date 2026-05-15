// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
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

static void dummy_write(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void dummy_cleanup(void) {
    unlink("./dummy_file");
}

static CURL* init_curl_handle(const uint8_t *data, size_t size) {
    CURL *curl = curl_easy_init();
    if (!curl) return NULL;
    
    dummy_write(data, size);
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    return curl;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    if (Size < 1) return 0;
    
    CURL *curl = init_curl_handle(Data, Size);
    if (!curl) return 0;
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = (Size > 1) ? 1 : 0;
    size_t data_size = (Size > data_offset) ? Size - data_offset : 0;
    
    switch (selector) {
        case 0: {
            size_t sent = 0;
            void *buffer = (data_size > 0) ? malloc(data_size) : NULL;
            if (buffer && data_size > 0) {
                memcpy(buffer, Data + data_offset, data_size);
                curl_easy_perform(curl);
                curl_easy_send(curl, buffer, data_size, &sent);
                free(buffer);
            }
            break;
        }
        case 1: {
            size_t recv = 0;
            const struct curl_ws_frame *meta = NULL;
            void *buffer = (data_size > 0) ? malloc(data_size) : NULL;
            if (buffer && data_size > 0) {
                memcpy(buffer, Data + data_offset, data_size);
                curl_easy_perform(curl);
                curl_ws_recv(curl, buffer, data_size, &recv, &meta);
                free(buffer);
            }
            break;
        }
        case 2: {
            int bitmask = (data_size > 0) ? Data[data_offset] : 0;
            bitmask = (bitmask & 1) ? CURLPAUSE_RECV : CURLPAUSE_SEND;
            curl_easy_pause(curl, bitmask);
            break;
        }
        case 3: {
            curl_easy_perform(curl);
            break;
        }
        case 4: {
            size_t recv = 0;
            void *buffer = (data_size > 0) ? malloc(data_size) : NULL;
            if (buffer && data_size > 0) {
                memcpy(buffer, Data + data_offset, data_size);
                curl_easy_perform(curl);
                curl_easy_recv(curl, buffer, data_size, &recv);
                free(buffer);
            }
            break;
        }
        case 5: {
            curl_easy_perform(curl);
            curl_easy_upkeep(curl);
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    dummy_cleanup();
    
    return 0;
}