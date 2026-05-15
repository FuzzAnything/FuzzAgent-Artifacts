// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_ws_recv at ws.c:1530:10 in websockets.h
// curl_easy_perform at easy.c:817:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "curl/curl.h"
#include "curl/easy.h"
#include "curl/websockets.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    CURL *curl = curl_easy_init();
    if (!curl) return 0;
    
    write_dummy_file(Data, Size);
    
    unsigned char selector = Data[0];
    size_t offset = 1;
    
    switch (selector % 6) {
        case 0: {
            if (Size - offset >= sizeof(long)) {
                long opt_value = *(long*)(Data + offset);
                curl_easy_setopt(curl, CURLOPT_VERBOSE, opt_value);
            }
            curl_easy_perform(curl);
            break;
        }
        
        case 1: {
            if (Size - offset >= sizeof(long)) {
                long opt_value = *(long*)(Data + offset);
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                curl_easy_perform(curl);
                
                char buffer[256];
                size_t recv_len = 0;
                curl_easy_recv(curl, buffer, sizeof(buffer), &recv_len);
            }
            break;
        }
        
        case 2: {
            if (Size - offset >= sizeof(long)) {
                long opt_value = *(long*)(Data + offset);
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                curl_easy_perform(curl);
                
                char buffer[256];
                size_t sent_len = 0;
                const char *send_data = "test";
                curl_easy_send(curl, send_data, strlen(send_data), &sent_len);
            }
            break;
        }
        
        case 3: {
            if (Size - offset >= sizeof(long)) {
                long opt_value = *(long*)(Data + offset);
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                curl_easy_perform(curl);
                
                char buffer[256];
                size_t recv_len = 0;
                const struct curl_ws_frame *meta = NULL;
                curl_ws_recv(curl, buffer, sizeof(buffer), &recv_len, &meta);
            }
            break;
        }
        
        case 4: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            CURLcode res = curl_easy_perform(curl);
            
            if (res == CURLE_OK) {
                long http_code = 0;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
                
                double total_time = 0;
                curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
            }
            break;
        }
        
        case 5: {
            if (Size - offset >= 2) {
                CURLoption opt = (CURLoption)(Data[offset] % 30000);
                offset++;
                
                if (opt == CURLOPT_WRITEDATA || opt == CURLOPT_READDATA) {
                    FILE *fp = fopen("./dummy_file", "rb");
                    if (fp) {
                        curl_easy_setopt(curl, opt, fp);
                        fclose(fp);
                    }
                } else if (opt == CURLOPT_URL) {
                    curl_easy_setopt(curl, opt, "file://./dummy_file");
                } else if (opt == CURLOPT_POSTFIELDS) {
                    size_t len = Size - offset;
                    if (len > 0) {
                        char *postdata = (char*)malloc(len + 1);
                        if (postdata) {
                            memcpy(postdata, Data + offset, len);
                            postdata[len] = 0;
                            curl_easy_setopt(curl, opt, postdata);
                            free(postdata);
                        }
                    }
                } else {
                    long val = (long)Data[offset];
                    curl_easy_setopt(curl, opt, val);
                }
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}