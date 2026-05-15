// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
// curl_easy_duphandle at easy.c:952:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

static void dummy_callback(void *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    CURL *curl = NULL;
    CURL *dup_curl = NULL;
    CURLcode res;
    
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }
    
    if (Size > 0) {
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size, fp);
            fclose(fp);
        }
    }
    
    if (Size > 0) {
        uint8_t option_type = Data[0] % 8;
        
        switch (option_type) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
                break;
            case 3:
                if (Size > 1) {
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, (long)(Data[1] & 1));
                }
                break;
            case 4:
                curl_easy_setopt(curl, CURLOPT_URL, "./dummy_file");
                break;
            case 5:
                if (Size > 1) {
                    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)Data[1]);
                }
                break;
            case 6:
                curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
                break;
            case 7:
                if (Size > 1) {
                    size_t buf_size = Data[1] % 256;
                    if (buf_size > 0 && buf_size <= Size - 2) {
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Data + 2);
                        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)buf_size);
                    }
                }
                break;
        }
    }
    
    res = curl_easy_perform(curl);
    (void)res;
    
    if (Size > 0 && (Data[0] % 8) == 6) {
        size_t sent = 0;
        if (Size > 10) {
            curl_easy_send(curl, Data + 1, Size > 100 ? 100 : Size - 1, &sent);
        }
    }
    
    dup_curl = curl_easy_duphandle(curl);
    if (dup_curl) {
        curl_easy_perform(dup_curl);
        curl_easy_cleanup(dup_curl);
    }
    
    curl_easy_reset(curl);
    
    curl_easy_setopt(curl, CURLOPT_URL, "./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_callback);
    curl_easy_perform(curl);
    
    curl_easy_cleanup(curl);
    
    return 0;
}