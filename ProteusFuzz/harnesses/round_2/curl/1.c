// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
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

/* Fuzz driver for curl multi-handle APIs */

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    /* Initialize the multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* Create a dummy easy handle to associate with the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Set a dummy URL to make the easy handle valid for multi operations */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/dummy");
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Extract data from input for fuzzing parameters safely */
    CURLMcode error_code = (CURLMcode)Data[0];
    curl_socket_t sockfd = (curl_socket_t)(Data[1] | (Data[2] << 8));
    int running_handles = 0;
    void *sockp = (void *)(intptr_t)(Data[3] | (Data[4] << 8));

    /* 1. Test curl_multi_strerror with various error codes */
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        /* Just use the string to avoid unused variable warning */
        (void)strlen(error_str);
    }

    /* 2. Test curl_multi_assign */
    curl_multi_assign(multi_handle, sockfd, sockp);

    /* 3. Test curl_multi_socket (deprecated but still valid for fuzzing) */
    curl_multi_socket(multi_handle, sockfd, &running_handles);

    /* 4. Test curl_multi_socket_all (deprecated but still valid for fuzzing) */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* 5. Test curl_multi_perform */
    curl_multi_perform(multi_handle, &running_handles);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}