// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_url_strerror at strerror.c:420:13 in urlapi.h
// curl_url at urlapi.c:1288:8 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_set at urlapi.c:1805:11 in urlapi.h
// curl_url_get at urlapi.c:1541:11 in urlapi.h
// curl_free at escape.c:189:6 in curl.h
// curl_url_cleanup at urlapi.c:1293:6 in urlapi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "curl/urlapi.h"
#include "curl/curl.h"

static void fuzz_share_strerror(const uint8_t *data, size_t size) {
    if (size == 0) return;
    CURLSHcode error = (CURLSHcode)(data[0] % 32);
    const char *str = curl_share_strerror(error);
    (void)str;
}

static void fuzz_easy_strerror(const uint8_t *data, size_t size) {
    if (size == 0) return;
    CURLcode error = (CURLcode)(data[0] % 100);
    const char *str = curl_easy_strerror(error);
    (void)str;
}

static void fuzz_url_strerror(const uint8_t *data, size_t size) {
    if (size == 0) return;
    CURLUcode error = (CURLUcode)(data[0] % 32);
    const char *str = curl_url_strerror(error);
    (void)str;
}

static void fuzz_url_operations(const uint8_t *data, size_t size) {
    CURLU *handle = curl_url();
    if (!handle) return;
    
    if (size > 0) {
        CURLUPart what = (CURLUPart)(data[0] % 10);
        unsigned int flags = 0;
        
        if (size > 1) {
            char *part = malloc(size);
            if (part) {
                memcpy(part, data, size);
                part[size-1] = '\0';
                
                CURLUcode set_res = curl_url_set(handle, what, part, flags);
                (void)set_res;
                
                free(part);
            }
        } else {
            CURLUcode set_res = curl_url_set(handle, what, NULL, flags);
            (void)set_res;
        }
        
        char *get_part = NULL;
        CURLUcode get_res = curl_url_get(handle, what, &get_part, flags);
        if (get_res == CURLUE_OK && get_part) {
            curl_free(get_part);
        }
    }
    
    curl_url_cleanup(handle);
}

int LLVMFuzzerTestOneInput_37(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    fuzz_share_strerror(data, size);
    fuzz_easy_strerror(data, size);
    fuzz_url_strerror(data, size);
    fuzz_url_operations(data, size);
    
    return 0;
}