// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* 
 * Fuzz driver for libcurl multi-handle functions.
 * Targets: curl_multi_socket_action, curl_multi_socket, curl_multi_socket_all,
 *          curl_multi_perform, curl_multi_assign.
 */

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    curl_socket_t s = -1;
    int running_handles = 0;
    int ev_bitmask = 0;
    CURLMcode res;
    void *sockp = NULL;

    if (Size < 2) {
        return 0;
    }

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Initialize easy handle
    easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Set a dummy URL to allow the handle to be added
    if (curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com") != CURLE_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Derive parameters from fuzz input
    // Use first byte for ev_bitmask
    ev_bitmask = (int)Data[0];
    
    // Use second byte to decide socket (0 for -1, others for a small positive int)
    if (Data[1] == 0) {
        s = -1;
    } else {
        s = (curl_socket_t)(Data[1] % 100);
    }

    // Fuzz curl_multi_socket_action
    res = curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
    (void)res; // Suppress unused variable warning

    // Fuzz curl_multi_socket (deprecated)
    res = curl_multi_socket(multi_handle, s, &running_handles);
    (void)res;

    // Fuzz curl_multi_socket_all (deprecated)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_assign with a dummy pointer
    // Ensure we have enough data before accessing Data[2] to Data[5]
    if (Size >= 6) {
        size_t fake_ptr_val = ((size_t)Data[2] << 24) | ((size_t)Data[3] << 16) | 
                              ((size_t)Data[4] << 8) | (size_t)Data[5];
        sockp = (void *)(uintptr_t)fake_ptr_val;
    } else {
        // Fallback if size is small
        sockp = (void *)(uintptr_t)(Data[0] | (Data[1] << 8));
    }
    
    res = curl_multi_assign(multi_handle, s, sockp);
    (void)res;

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}