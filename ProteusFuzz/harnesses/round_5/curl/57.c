// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
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

    // Prepare a dummy URL from input data to ensure valid string
    char url[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url, Data, url_len);
    url[url_len] = '\0';

    // Set a basic URL to make the handle valid for multi operations
    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, NULL);

    CURLMcode rc = curl_multi_add_handle(multi_handle, easy_handle);
    if (rc != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Extract parameters from fuzz input
    int running_handles = 0;
    curl_socket_t s = (curl_socket_t)(Data[0] | (Data[1] << 8));
    int ev_bitmask = (int)(Data[2] | (Data[3] << 8));

    // Fuzz curl_multi_socket_action
    rc = curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
    (void)rc;

    // Fuzz curl_multi_socket (deprecated but valid for testing)
    rc = curl_multi_socket(multi_handle, s, &running_handles);
    (void)rc;

    // Fuzz curl_multi_socket_all (deprecated but valid for testing)
    rc = curl_multi_socket_all(multi_handle, &running_handles);
    (void)rc;

    // Fuzz curl_multi_assign with a dummy pointer
    void *dummy_ptr = (void *)(uintptr_t)(Data[4] | (Data[5] << 8));
    rc = curl_multi_assign(multi_handle, s, dummy_ptr);
    (void)rc;

    // Fuzz curl_multi_perform
    rc = curl_multi_perform(multi_handle, &running_handles);
    (void)rc;

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}