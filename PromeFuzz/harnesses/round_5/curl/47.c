// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_pause at easy.c:1136:10 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size > 1024 ? 1024 : size);
        } else {
            const char dummy[] = "dummy data";
            write(fd, dummy, sizeof(dummy) - 1);
        }
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_47(const uint8_t *data, size_t size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
    }
    
    create_dummy_file(data, size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        curl_easy_upkeep(curl);
        
        int bitmask = 0;
        if (size > 0) {
            bitmask = data[0] % 4;
            if (bitmask == 0) bitmask = CURLPAUSE_RECV;
            else if (bitmask == 1) bitmask = CURLPAUSE_SEND;
            else if (bitmask == 2) bitmask = CURLPAUSE_RECV | CURLPAUSE_SEND;
            else bitmask = 0;
        }
        curl_easy_pause(curl, bitmask);
        
        char buffer[1024];
        size_t sent = 0;
        size_t to_send = size > sizeof(buffer) ? sizeof(buffer) : size;
        if (to_send > 0) {
            memcpy(buffer, data, to_send);
            curl_easy_send(curl, buffer, to_send, &sent);
        }
        
        size_t received = 0;
        curl_easy_recv(curl, buffer, sizeof(buffer), &received);
        
        const struct curl_ws_frame *meta = NULL;
        curl_ws_recv(curl, buffer, sizeof(buffer), &received, &meta);
        
        curl_easy_perform(curl);
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}