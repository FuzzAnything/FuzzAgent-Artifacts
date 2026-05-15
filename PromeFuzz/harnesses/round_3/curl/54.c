// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    return size * nmemb;
}

static size_t read_callback(char *buffer, size_t size, size_t nitems, void *instream) {
    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
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
    
    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    
    CURLcode res = curl_easy_perform(curl);
    
    const char *error_str = curl_easy_strerror(res);
    
    if (res == CURLE_OK) {
        char send_buffer[1024];
        size_t sent = 0;
        size_t to_send = Size > sizeof(send_buffer) ? sizeof(send_buffer) : Size;
        
        if (to_send > 0) {
            memcpy(send_buffer, Data, to_send);
            curl_easy_send(curl, send_buffer, to_send, &sent);
        }
        
        char recv_buffer[1024];
        size_t received = 0;
        curl_easy_recv(curl, recv_buffer, sizeof(recv_buffer), &received);
    }
    
    long response_code;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    double total_time;
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    
    curl_easy_cleanup(curl);
    
    remove("./dummy_file");
    
    return 0;
}