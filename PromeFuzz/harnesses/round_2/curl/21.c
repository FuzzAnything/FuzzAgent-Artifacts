// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    CURL *curl = curl_easy_init();
    if (!curl) {
        cleanup_dummy_file();
        return 0;
    }
    
    unsigned char mode = Data[0] % 4;
    size_t data_index = 1;
    
    switch (mode) {
        case 0: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_perform(curl);
            
            curl_easy_reset(curl);
            
            if (data_index < Size) {
                curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
                curl_easy_setopt(curl, CURLOPT_NOBODY, (Data[data_index] & 1) ? 1L : 0L);
                curl_easy_perform(curl);
            }
            break;
        }
        
        case 1: {
            CURL *dup = curl_easy_duphandle(curl);
            if (dup) {
                if (data_index < Size) {
                    curl_easy_setopt(dup, CURLOPT_URL, "file://./dummy_file");
                    curl_easy_setopt(dup, CURLOPT_NOBODY, (Data[data_index] & 1) ? 1L : 0L);
                    curl_easy_perform(dup);
                }
                curl_easy_cleanup(dup);
            }
            break;
        }
        
        case 2: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            
            if (data_index < Size) {
                curl_easy_setopt(curl, CURLOPT_VERBOSE, (Data[data_index] & 1) ? 1L : 0L);
                data_index++;
            }
            
            curl_easy_perform(curl);
            break;
        }
        
        case 3: {
            curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
            curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
            CURLcode res = curl_easy_perform(curl);
            
            if (res == CURLE_OK) {
                char buffer[256];
                size_t n = 0;
                if (data_index < Size) {
                    size_t buflen = (Data[data_index] % sizeof(buffer)) + 1;
                    data_index++;
                    curl_easy_recv(curl, buffer, buflen, &n);
                }
            }
            break;
        }
    }
    
    curl_easy_cleanup(curl);
    cleanup_dummy_file();
    
    return 0;
}