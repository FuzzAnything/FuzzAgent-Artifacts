// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

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

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    size_t remaining = (Size > data_offset) ? Size - data_offset : 0;
    
    switch (selector) {
        case 0: { /* curl_easy_getinfo */
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_perform(curl);
            
            double dval;
            long lval;
            char *cval = NULL;
            curl_off_t oval;
            
            CURLINFO info_type = (CURLINFO)(Data[data_offset] % 60);
            switch (info_type) {
                case CURLINFO_RESPONSE_CODE:
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &lval);
                    break;
                case CURLINFO_TOTAL_TIME:
                    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &dval);
                    break;
                case CURLINFO_CONTENT_TYPE:
                    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &cval);
                    break;
                case CURLINFO_SIZE_DOWNLOAD_T:
                    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD_T, &oval);
                    break;
                default:
                    break;
            }
            break;
        }
        
        case 1: { /* curl_easy_send */
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            if (curl_easy_perform(curl) == CURLE_OK) {
                size_t sent = 0;
                char buffer[256];
                size_t buflen = (remaining > sizeof(buffer)) ? sizeof(buffer) : remaining;
                if (buflen > 0) {
                    memcpy(buffer, Data + data_offset, buflen);
                    curl_easy_send(curl, buffer, buflen, &sent);
                }
            }
            break;
        }
        
        case 2: { /* curl_ws_recv */
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            if (curl_easy_perform(curl) == CURLE_OK) {
                char buffer[256];
                size_t recv = 0;
                const struct curl_ws_frame *meta = NULL;
                curl_ws_recv(curl, buffer, sizeof(buffer), &recv, &meta);
            }
            break;
        }
        
        case 3: { /* curl_easy_setopt */
            CURLoption opt = (CURLoption)(Data[data_offset] % 30000);
            
            switch (opt) {
                case CURLOPT_URL:
                    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                    break;
                case CURLOPT_NOBODY:
                    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
                    break;
                case CURLOPT_VERBOSE:
                    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                    break;
                case CURLOPT_CONNECT_ONLY:
                    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
                    break;
                case CURLOPT_WRITEFUNCTION:
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
                    break;
                default:
                    if (remaining > 1) {
                        long lval = (long)Data[data_offset + 1];
                        curl_easy_setopt(curl, opt, lval);
                    }
                    break;
            }
            break;
        }
        
        case 4: { /* curl_easy_recv */
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            if (curl_easy_perform(curl) == CURLE_OK) {
                char buffer[256];
                size_t recv = 0;
                curl_easy_recv(curl, buffer, sizeof(buffer), &recv);
            }
            break;
        }
        
        case 5: { /* curl_easy_perform */
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, 100L);
            
            if (remaining > 0) {
                int flags = Data[data_offset] % 8;
                if (flags & 1) curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                if (flags & 2) curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
                if (flags & 4) curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            }
            
            curl_easy_perform(curl);
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}