// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    unsigned char selector = Data[0];
    size_t offset = 1;
    
    switch (selector % 6) {
        case 0: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            CURLcode res = curl_easy_perform(curl);
            
            if (res == CURLE_OK) {
                long http_code = 0;
                double total_time = 0.0;
                curl_off_t download_size = 0;
                
                if (Size > offset) {
                    CURLINFO info_type;
                    switch (Data[offset] % 3) {
                        case 0: info_type = CURLINFO_RESPONSE_CODE; break;
                        case 1: info_type = CURLINFO_TOTAL_TIME; break;
                        case 2: info_type = CURLINFO_SIZE_DOWNLOAD_T; break;
                        default: info_type = CURLINFO_RESPONSE_CODE;
                    }
                    
                    if (info_type == CURLINFO_RESPONSE_CODE) {
                        curl_easy_getinfo(curl, info_type, &http_code);
                    } else if (info_type == CURLINFO_TOTAL_TIME) {
                        curl_easy_getinfo(curl, info_type, &total_time);
                    } else {
                        curl_easy_getinfo(curl, info_type, &download_size);
                    }
                }
            }
            break;
        }
        
        case 1: {
            if (Size > offset + 1) {
                CURLoption opt;
                switch (Data[offset] % 4) {
                    case 0: opt = CURLOPT_URL; break;
                    case 1: opt = CURLOPT_NOBODY; break;
                    case 2: opt = CURLOPT_VERBOSE; break;
                    case 3: opt = CURLOPT_FOLLOWLOCATION; break;
                    default: opt = CURLOPT_VERBOSE;
                }
                
                if (opt == CURLOPT_URL) {
                    curl_easy_setopt(curl, opt, "file://./dummy_file");
                } else {
                    long val = Data[offset + 1] % 2;
                    curl_easy_setopt(curl, opt, val);
                }
            }
            break;
        }
        
        case 2: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            CURLcode res = curl_easy_perform(curl);
            
            if (res == CURLE_OK) {
                char buffer[256];
                size_t n = 0;
                if (Size > offset) {
                    size_t buflen = (Data[offset] % 255) + 1;
                    if (buflen > sizeof(buffer)) buflen = sizeof(buffer);
                    curl_easy_recv(curl, buffer, buflen, &n);
                }
            }
            break;
        }
        
        case 3: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            CURLcode res = curl_easy_perform(curl);
            
            if (res == CURLE_OK) {
                char buffer[256];
                size_t sent = 0;
                if (Size > offset + 1) {
                    size_t buflen = (Data[offset] % 255) + 1;
                    if (buflen > sizeof(buffer)) buflen = sizeof(buffer);
                    
                    if (buflen > 0 && Size - offset - 1 >= buflen) {
                        memcpy(buffer, Data + offset + 1, buflen);
                    }
                    
                    curl_off_t fragsize = 0;
                    unsigned int flags = CURLWS_TEXT;
                    
                    if (Size > offset + buflen + 1) {
                        fragsize = Data[offset + buflen + 1] % 128;
                        flags = Data[offset + buflen + 2] % 4;
                    }
                    
                    curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
                }
            }
            break;
        }
        
        case 4: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            
            if (Size > offset) {
                if (Data[offset] % 2) {
                    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                }
            }
            
            curl_easy_perform(curl);
            break;
        }
        
        case 5: {
            if (Size > offset) {
                CURLcode err = (CURLcode)(Data[offset] % 95);
                const char *str = curl_easy_strerror(err);
                (void)str;
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}