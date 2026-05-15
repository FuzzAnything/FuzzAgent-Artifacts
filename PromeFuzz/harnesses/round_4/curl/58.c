// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/websockets.h"
#include "curl/easy.h"

static void dummy_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res;
    
    if (Size < 1) {
        return 0;
    }
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    uint8_t op = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (op) {
        case 0: {
            char url[256];
            snprintf(url, sizeof(url), "http://example.com/%d", Data[0]);
            
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
            
            if (fuzz_size > 0) {
                long verbose = (fuzz_data[0] & 1) ? 1L : 0L;
                curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
            }
            
            res = curl_easy_perform(curl);
            (void)res;
            break;
        }
        
        case 1: {
            if (fuzz_size > 0) {
                char buffer[1024];
                size_t n = 0;
                size_t buflen = fuzz_size > sizeof(buffer) ? sizeof(buffer) : fuzz_size;
                
                memcpy(buffer, fuzz_data, buflen);
                res = curl_easy_send(curl, buffer, buflen, &n);
                (void)res;
            }
            break;
        }
        
        case 2: {
            char buffer[1024];
            size_t n = 0;
            size_t buflen = fuzz_size > sizeof(buffer) ? sizeof(buffer) : fuzz_size;
            
            res = curl_easy_recv(curl, buffer, buflen, &n);
            (void)res;
            break;
        }
        
        case 3: {
            char buffer[1024];
            size_t recv = 0;
            const struct curl_ws_frame *meta = NULL;
            size_t buflen = fuzz_size > sizeof(buffer) ? sizeof(buffer) : fuzz_size;
            
            res = curl_ws_recv(curl, buffer, buflen, &recv, &meta);
            (void)res;
            break;
        }
        
        case 4: {
            if (fuzz_size > 0) {
                CURLINFO info = CURLINFO_RESPONSE_CODE;
                long response_code = 0;
                
                switch (fuzz_data[0] % 4) {
                    case 0: info = CURLINFO_RESPONSE_CODE; break;
                    case 1: info = CURLINFO_CONTENT_TYPE; break;
                    case 2: info = CURLINFO_SIZE_DOWNLOAD; break;
                    case 3: info = CURLINFO_TOTAL_TIME; break;
                }
                
                res = curl_easy_getinfo(curl, info, &response_code);
                (void)res;
            }
            break;
        }
        
        case 5: {
            if (fuzz_size > 0) {
                const char *url = "http://example.com";
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
                
                for (size_t i = 0; i < fuzz_size && i < 10; i++) {
                    CURLoption opt;
                    switch (fuzz_data[i] % 5) {
                        case 0: opt = CURLOPT_VERBOSE; curl_easy_setopt(curl, opt, (fuzz_data[i] & 1) ? 1L : 0L); break;
                        case 1: opt = CURLOPT_FOLLOWLOCATION; curl_easy_setopt(curl, opt, (fuzz_data[i] & 1) ? 1L : 0L); break;
                        case 2: opt = CURLOPT_TIMEOUT; curl_easy_setopt(curl, opt, 5L); break;
                        case 3: opt = CURLOPT_CONNECT_ONLY; curl_easy_setopt(curl, opt, (fuzz_data[i] & 1) ? 1L : 0L); break;
                        case 4: opt = CURLOPT_SSL_VERIFYPEER; curl_easy_setopt(curl, opt, (fuzz_data[i] & 1) ? 1L : 0L); break;
                    }
                }
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    
    return 0;
}