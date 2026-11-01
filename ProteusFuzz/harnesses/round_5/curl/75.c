// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
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

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    curl_socket_t sock = -1;
    void *sockp = NULL;
    CURLMcode res;
    const char *err_str;

    if (Size < 2) {
        return 0;
    }

    /* Initialize libcurl if needed */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to ensure valid handle state */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);

    res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_multi_perform */
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    /* Fuzz curl_multi_socket_action with varied parameters derived from input */
    if (Size >= 4) {
        curl_socket_t s = (curl_socket_t)(Data[0] | (Data[1] << 8));
        int ev_bitmask = (int)(Data[2] | (Data[3] << 8));
        res = curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
        (void)res;
    }

    /* Fuzz curl_multi_socket (deprecated) */
    if (Size >= 2) {
        curl_socket_t s = (curl_socket_t)(Data[0] | (Data[1] << 8));
        res = curl_multi_socket(multi_handle, s, &running_handles);
        (void)res;
    }

    /* Fuzz curl_multi_socket_all (deprecated) */
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    /* Fuzz curl_multi_assign */
    if (Size >= 2) {
        curl_socket_t sockfd = (curl_socket_t)(Data[0] | (Data[1] << 8));
        void *sockp_local = malloc(1);
        if (sockp_local) {
            res = curl_multi_assign(multi_handle, sockfd, sockp_local);
            (void)res;
            free(sockp_local);
        }
    }

    /* Fuzz curl_multi_strerror with various error codes */
    if (Size >= 1) {
        CURLMcode error_code = (CURLMcode)(Data[0]);
        err_str = curl_multi_strerror(error_code);
        (void)err_str;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}