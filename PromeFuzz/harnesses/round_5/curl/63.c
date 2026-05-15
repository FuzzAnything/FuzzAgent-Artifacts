// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/websockets.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    unsigned int selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
            curl_easy_perform(curl);
            
            CURLINFO info_type = CURLINFO_RESPONSE_CODE;
            long response_code;
            curl_easy_getinfo(curl, info_type, &response_code);
            
            if (Size > offset) {
                info_type = (CURLINFO)(Data[offset] % 60);
                double dval;
                curl_easy_getinfo(curl, info_type, &dval);
            }
            break;
        }
        
        case 1: {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            curl_easy_setopt(curl, CURLOPT_URL, "ws://dummy");
            curl_easy_perform(curl);
            
            if (Size > offset + 8) {
                size_t buflen = Data[offset] % 256;
                void *buffer = malloc(buflen);
                size_t recv;
                const struct curl_ws_frame *meta;
                
                if (buffer) {
                    curl_ws_recv(curl, buffer, buflen, &recv, &meta);
                    free(buffer);
                }
            }
            break;
        }
        
        case 2: {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            curl_easy_setopt(curl, CURLOPT_URL, "http://dummy");
            curl_easy_perform(curl);
            
            if (Size > offset) {
                size_t buflen = Data[offset] % 256;
                void *buffer = malloc(buflen);
                size_t sent;
                
                if (buffer && Size > offset + buflen) {
                    memcpy(buffer, Data + offset + 1, buflen);
                    curl_easy_send(curl, buffer, buflen, &sent);
                }
                free(buffer);
            }
            break;
        }
        
        case 3: {
            if (Size > offset + 2) {
                CURLoption opt = (CURLoption)(Data[offset] % 30000);
                long param = (long)Data[offset + 1];
                curl_easy_setopt(curl, opt, param);
                
                if (Size > offset + 10) {
                    opt = (CURLoption)(Data[offset + 2] % 30000);
                    const char *str_param = (const char *)(Data + offset + 3);
                    curl_easy_setopt(curl, opt, str_param);
                }
            }
            break;
        }
        
        case 4: {
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
            curl_easy_setopt(curl, CURLOPT_URL, "ws://dummy");
            curl_easy_perform(curl);
            
            if (Size > offset + 12) {
                size_t buflen = Data[offset] % 256;
                void *buffer = malloc(buflen);
                size_t sent;
                curl_off_t fragsize = (curl_off_t)Data[offset + 1];
                unsigned int flags = Data[offset + 2] % 256;
                
                if (buffer && Size > offset + 3 + buflen) {
                    memcpy(buffer, Data + offset + 3, buflen);
                    curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
                }
                free(buffer);
            }
            break;
        }
        
        case 5: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
            curl_easy_perform(curl);
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    return 0;
}