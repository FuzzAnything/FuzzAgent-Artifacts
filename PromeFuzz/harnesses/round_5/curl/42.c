// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
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

static void setup_curl_handle(CURL **curl) {
    *curl = curl_easy_init();
    if (*curl) {
        curl_easy_setopt(*curl, CURLOPT_URL, "file://./dummy_file");
        curl_easy_setopt(*curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(*curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(*curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(*curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(*curl, CURLOPT_TIMEOUT_MS, 100L);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = NULL;
    setup_curl_handle(&curl);
    
    if (!curl) return 0;
    
    uint8_t selector = Data[0] % 6;
    size_t usable_size = Size > 1 ? Size - 1 : 1;
    const uint8_t *usable_data = Size > 1 ? Data + 1 : Data;
    
    switch (selector) {
        case 0: {
            curl_easy_perform(curl);
            curl_easy_upkeep(curl);
            break;
        }
        case 1: {
            int bitmask = 0;
            if (usable_size > 0) bitmask = usable_data[0] & (CURLPAUSE_RECV | CURLPAUSE_SEND);
            curl_easy_pause(curl, bitmask);
            break;
        }
        case 2: {
            curl_easy_perform(curl);
            break;
        }
        case 3: {
            curl_easy_perform(curl);
            
            char buffer[1024];
            size_t recv_len = 0;
            const struct curl_ws_frame *meta = NULL;
            
            if (usable_size > 0) {
                size_t buflen = usable_size > sizeof(buffer) ? sizeof(buffer) : usable_size;
                curl_ws_recv(curl, buffer, buflen, &recv_len, &meta);
            }
            break;
        }
        case 4: {
            curl_easy_perform(curl);
            
            char buffer[1024];
            size_t n = 0;
            
            if (usable_size > 0) {
                size_t buflen = usable_size > sizeof(buffer) ? sizeof(buffer) : usable_size;
                curl_easy_recv(curl, buffer, buflen, &n);
            }
            break;
        }
        case 5: {
            curl_easy_perform(curl);
            
            size_t n = 0;
            if (usable_size > 0) {
                curl_easy_send(curl, usable_data, usable_size, &n);
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    return 0;
}