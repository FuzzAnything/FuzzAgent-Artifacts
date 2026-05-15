// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"
#include "easy.h"
#include "websockets.h"
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        size_t sent = 0;
        size_t received = 0;
        char buffer[4096];
        const struct curl_ws_frame *meta = NULL;
        
        if (Size > 0) {
            curl_easy_send(curl, Data, Size > 4096 ? 4096 : Size, &sent);
        }
        
        curl_easy_recv(curl, buffer, sizeof(buffer), &received);
        
        curl_ws_recv(curl, buffer, sizeof(buffer), &received, &meta);
        
        double total_time = 0;
        long response_code = 0;
        curl_off_t upload_size = 0;
        
        curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
        curl_easy_getinfo(curl, CURLINFO_SIZE_UPLOAD_T, &upload_size);
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, "ws://localhost:8080");
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
    
    res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        size_t received = 0;
        char buffer[4096];
        const struct curl_ws_frame *meta = NULL;
        
        curl_ws_recv(curl, buffer, sizeof(buffer), &received, &meta);
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}