// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_recv at easy.c:1224:10 in easy.h
// curl_easy_send at easy.c:1302:10 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/easy.h"

static size_t dummy_write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    return size * nmemb;
}

static size_t dummy_read_callback(void *ptr, size_t size, size_t nmemb, void *userp) {
    return 0;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    if (!initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        initialized = 1;
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

    curl_easy_setopt(curl, CURLOPT_URL, "file://./dummy_file");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dummy_write_callback);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_callback);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 0L);

    if (Size > 0) {
        int opt_selector = Data[0] % 10;
        switch (opt_selector) {
            case 0:
                curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
                break;
            case 1:
                curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                break;
            case 2:
                curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
                break;
            case 3:
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
                break;
            case 4:
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)Size);
                break;
            default:
                break;
        }
    }

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        const char *error_str = curl_easy_strerror(res);
        (void)error_str;
    }

    if (Size > 1) {
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        CURLcode connect_res = curl_easy_perform(curl);
        
        if (connect_res == CURLE_OK) {
            char buffer[1024];
            size_t n = 0;
            CURLcode recv_res = curl_easy_recv(curl, buffer, sizeof(buffer), &n);
            (void)recv_res;

            if (Size > 10) {
                size_t sent = 0;
                CURLcode send_res = curl_easy_send(curl, Data, Size > 1024 ? 1024 : Size, &sent);
                (void)send_res;
            }
        }
    }

    if (res == CURLE_OK || res == CURLE_UNSUPPORTED_PROTOCOL) {
        double val;
        CURLcode info_res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &val);
        (void)info_res;
        
        long http_code;
        info_res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        (void)info_res;
    }

    curl_easy_cleanup(curl);
    remove("./dummy_file");
    
    return 0;
}