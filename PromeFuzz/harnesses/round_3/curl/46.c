// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static int curl_initialized = 0;

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl_initialized = 1;
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
    
    char url[256];
    snprintf(url, sizeof(url), "file://%s", "./dummy_file");
    
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    
    CURLcode res = curl_easy_perform(curl);
    
    if (res == CURLE_OK) {
        char buffer[1024];
        size_t n = 0;
        
        curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &n);
        
        n = 0;
        curl_easy_recv(curl, buffer, sizeof(buffer), &n);
    }
    
    const char *error_str = curl_easy_strerror(res);
    (void)error_str;
    
    double time_val;
    if (curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &time_val) == CURLE_OK) {
        (void)time_val;
    }
    
    long response_code;
    if (curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code) == CURLE_OK) {
        (void)response_code;
    }
    
    curl_easy_cleanup(curl);
    
    remove("./dummy_file");
    
    return 0;
}