// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_strerror at strerror.c:34:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_init at curl_share.c:33:9 in curl.h
// curl_share_setopt at curl_share.c:58:12 in curl.h
// curl_share_cleanup at curl_share.c:221:12 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/easy.h>

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. curl_global_trace
    // Ensure null termination for the config string
    char config_str[256];
    size_t config_len = (Size < sizeof(config_str) - 1) ? Size : sizeof(config_str) - 1;
    memcpy(config_str, Data, config_len);
    config_str[config_len] = '\0';

    curl_global_trace(config_str);

    // 2. curl_easy_strerror
    // Iterate through a range of CURLcode values to exercise the function
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLcode code = (CURLcode)Data[i];
        const char *err_str = curl_easy_strerror(code);
        (void)err_str; // Use the result to avoid unused warnings
    }

    // 3. curl_share_strerror
    // Iterate through a range of CURLSHcode values
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLSHcode code = (CURLSHcode)Data[i];
        const char *share_err_str = curl_share_strerror(code);
        (void)share_err_str;
    }

    // 4, 5, 6: curl_share_setopt, curl_easy_setopt, curl_share_cleanup
    // These require valid handles. We initialize them and then fuzz options.
    
    CURLSH *share = curl_share_init();
    if (share) {
        // Try to set options on the share handle
        // We cast input bytes to CURLSHoption. 
        // We ensure we only access valid indices from Data.
        if (Size > 0) {
            CURLSHoption opt = (CURLSHoption)Data[0];
            curl_share_setopt(share, opt, NULL);
        }
        
        // Cleanup share
        curl_share_cleanup(share);
    }

    CURL *curl = curl_easy_init();
    if (curl) {
        // Prepare a null-terminated string for string-based options
        char url_buf[256];
        size_t url_len = (Size < sizeof(url_buf) - 1) ? Size : sizeof(url_buf) - 1;
        memcpy(url_buf, Data, url_len);
        url_buf[url_len] = '\0';

        // Fuzz CURLOPT_URL
        curl_easy_setopt(curl, CURLOPT_URL, url_buf);

        // Fuzz CURLOPT_COPYPOSTFIELDS
        char post_buf[256];
        size_t post_len = (Size < sizeof(post_buf) - 1) ? Size : sizeof(post_buf) - 1;
        memcpy(post_buf, Data, post_len);
        post_buf[post_len] = '\0';
        
        curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, post_buf);

        // Fuzz other options with integer values from input
        // Ensure we have enough data before accessing
        if (Size > 0) {
            long timeout = (long)Data[0] * 10;
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
        }

        if (Size > 1) {
            int verbose = (Data[1] > 127) ? 1 : 0;
            curl_easy_setopt(curl, CURLOPT_VERBOSE, verbose);
        }

        // Cleanup curl handle
        curl_easy_cleanup(curl);
    }

    return 0;
}