// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Prepare a URL from the fuzz input, ensuring null termination
    char url[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url, Data, url_len);
    url[url_len] = '\0';
    
    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    curl_easy_setopt(easy_handle, CURLOPT_USERAGENT, "fuzz-test");

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    CURLMcode res;

    // 1. Fuzz curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    // 2. Fuzz curl_multi_socket_action
    // Derive socket and event bitmask from input data
    curl_socket_t sock = 0;
    int ev_bitmask = 0;
    
    if (Size >= 2) {
        sock = (curl_socket_t)(Data[0] | (Data[1] << 8));
    }
    if (Size >= 4) {
        ev_bitmask = (int)(Data[2] | (Data[3] << 8));
    }
    
    res = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    (void)res;

    // 3. Fuzz curl_multi_socket_all (deprecated but valid)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    // 4. Fuzz curl_multi_assign
    void *sockp = NULL;
    if (Size >= 8) {
        sockp = malloc(16);
        if (sockp) {
            memcpy(sockp, Data + 4, 16);
        }
    }
    
    res = curl_multi_assign(multi_handle, sock, sockp);
    (void)res;

    if (sockp) {
        free(sockp);
    }

    // 5. Fuzz curl_multi_strerror
    CURLMcode error_code = CURLM_OK;
    if (Size >= 2) {
        error_code = (CURLMcode)(Data[0] | (Data[1] << 8));
    }
    
    const char *str = curl_multi_strerror(error_code);
    (void)str;

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}