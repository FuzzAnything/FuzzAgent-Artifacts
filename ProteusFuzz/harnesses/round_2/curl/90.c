// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_strerror, curl_multi_assign, curl_multi_socket,
 *          curl_multi_socket_all, curl_multi_perform.
 */

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle for more realistic state */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* 1. Fuzz curl_multi_strerror */
    /* Try various error codes, including fuzzed values */
    CURLMcode error_code = (CURLMcode)(Data[0]);
    const char *str = curl_multi_strerror(error_code);
    (void)str; /* Use the result to avoid unused variable warnings */

    /* Also try a range of codes based on input */
    for (size_t i = 1; i < Size; i++) {
        error_code = (CURLMcode)(Data[i] | (i << 8));
        str = curl_multi_strerror(error_code);
        (void)str;
    }

    /* 2. Fuzz curl_multi_assign */
    /* We need a socket and a pointer. Use fuzzed data to derive these. */
    curl_socket_t sockfd = (curl_socket_t)(Size > 1 ? (Data[1] << 8) | Data[0] : 0);
    void *sockp = (void *)(size_t)(Size > 2 ? (Data[2] << 8) | Data[1] : 0);
    
    CURLMcode assign_result = curl_multi_assign(multi_handle, sockfd, sockp);
    (void)assign_result;

    /* Try assigning with invalid handle (NULL) to test error handling */
    curl_multi_assign(NULL, sockfd, sockp);

    /* 3. Fuzz curl_multi_socket (deprecated) */
    int running_handles = 0;
    curl_socket_t socket_fd = (curl_socket_t)(Size > 3 ? (Data[3] << 8) | Data[2] : 0);
    CURLMcode socket_result = curl_multi_socket(multi_handle, socket_fd, &running_handles);
    (void)socket_result;

    /* 4. Fuzz curl_multi_socket_all (deprecated) */
    CURLMcode socket_all_result = curl_multi_socket_all(multi_handle, &running_handles);
    (void)socket_all_result;

    /* 5. Fuzz curl_multi_perform */
    CURLMcode perform_result = curl_multi_perform(multi_handle, &running_handles);
    (void)perform_result;

    /* Cleanup */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}