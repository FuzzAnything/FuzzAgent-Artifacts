// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_ws_send at ws.c:1763:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURLcode res = CURLE_OK;
    
    if (Size < 1) {
        return 0;
    }
    
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    
    if (!curl) {
        curl_global_cleanup();
        return 0;
    }
    
    write_dummy_file(Data, Size);
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    size_t remaining = (Size > 1) ? Size - 1 : 0;
    
    switch (selector) {
        case 0: {
            if (remaining >= sizeof(CURLINFO)) {
                CURLINFO info;
                memcpy(&info, Data + data_offset, sizeof(CURLINFO));
                data_offset += sizeof(CURLINFO);
                remaining -= sizeof(CURLINFO);
                
                if (remaining >= sizeof(double)) {
                    double dval;
                    memcpy(&dval, Data + data_offset, sizeof(double));
                    curl_easy_getinfo(curl, info, &dval);
                } else if (remaining >= sizeof(long)) {
                    long lval;
                    memcpy(&lval, Data + data_offset, sizeof(long));
                    curl_easy_getinfo(curl, info, &lval);
                } else if (remaining >= sizeof(char*)) {
                    char *sval = (char*)(Data + data_offset);
                    curl_easy_getinfo(curl, info, &sval);
                }
            }
            break;
        }
        case 1: {
            if (remaining >= sizeof(CURLoption)) {
                CURLoption option;
                memcpy(&option, Data + data_offset, sizeof(CURLoption));
                data_offset += sizeof(CURLoption);
                remaining -= sizeof(CURLoption);
                
                if (remaining >= sizeof(long)) {
                    long lval;
                    memcpy(&lval, Data + data_offset, sizeof(long));
                    curl_easy_setopt(curl, option, lval);
                } else if (remaining >= sizeof(void*)) {
                    void *pval = (void*)(Data + data_offset);
                    curl_easy_setopt(curl, option, pval);
                } else if (remaining >= sizeof(char*)) {
                    char *sval = (char*)(Data + data_offset);
                    curl_easy_setopt(curl, option, sval);
                }
                
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 2L);
                
                res = curl_easy_perform(curl);
            }
            break;
        }
        case 2: {
            if (remaining > 0) {
                size_t buflen = remaining > 1024 ? 1024 : remaining;
                void *buffer = malloc(buflen);
                size_t n = 0;
                
                if (buffer) {
                    curl_easy_recv(curl, buffer, buflen, &n);
                    free(buffer);
                }
            }
            break;
        }
        case 3: {
            if (remaining > 0) {
                size_t buflen = remaining > 1024 ? 1024 : remaining;
                void *buffer = malloc(buflen);
                size_t sent = 0;
                curl_off_t fragsize = 0;
                unsigned int flags = 0;
                
                if (buffer && remaining >= sizeof(curl_off_t) + sizeof(unsigned int)) {
                    memcpy(&fragsize, Data + data_offset, sizeof(curl_off_t));
                    data_offset += sizeof(curl_off_t);
                    memcpy(&flags, Data + data_offset, sizeof(unsigned int));
                    
                    curl_ws_send(curl, buffer, buflen, &sent, fragsize, flags);
                }
                
                if (buffer) {
                    free(buffer);
                }
            }
            break;
        }
        case 4: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);
            
            res = curl_easy_perform(curl);
            break;
        }
        case 5: {
            if (remaining >= sizeof(CURLcode)) {
                CURLcode error;
                memcpy(&error, Data + data_offset, sizeof(CURLcode));
                const char *str = curl_easy_strerror(error);
                (void)str;
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    remove_dummy_file();
    
    return 0;
}