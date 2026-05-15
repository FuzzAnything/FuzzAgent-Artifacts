// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/easy.h"

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

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    char url[256];
    snprintf(url, sizeof(url), "file://%s", "./dummy_file");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        size_t buffer_size = Size > 4096 ? 4096 : Size;
        void *buffer = malloc(buffer_size);
        if (buffer) {
            size_t n = 0;
            
            if (Size & 1) {
                curl_easy_send(curl, Data, buffer_size, &n);
            }
            
            if (Size & 2) {
                curl_easy_recv(curl, buffer, buffer_size, &n);
            }
            
            if (Size & 4) {
                const struct curl_ws_frame *meta = NULL;
                curl_ws_recv(curl, buffer, buffer_size, &n, &meta);
            }
            
            free(buffer);
        }
        
        if (Size & 8) {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            
            double total_time = 0;
            curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            
            const char *content_type = NULL;
            curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}