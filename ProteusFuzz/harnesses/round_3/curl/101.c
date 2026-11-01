// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to make the handle valid */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);

    /* Add easy handle to multi handle */
    CURLMcode res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Prepare running_handles counter */
    int running_handles = 0;

    /* 
     * Fuzzing logic:
     * We interpret the input data to drive different code paths.
     */
    uint8_t action = Data[0] % 4;

    switch (action) {
        case 0:
            /* Call curl_multi_perform */
            res = curl_multi_perform(multi_handle, &running_handles);
            break;

        case 1:
            /* Call curl_multi_socket with a dummy socket */
            /* Using -1 (CURL_SOCKET_BAD) or small integers to test error handling */
            curl_socket_t sock = -1;
            if (Size > 1) {
                sock = (curl_socket_t)(Data[1] % 10);
                if (sock == 0) sock = -1; 
            }
            res = curl_multi_socket(multi_handle, sock, &running_handles);
            break;

        case 2:
            /* Call curl_multi_socket_all */
            res = curl_multi_socket_all(multi_handle, &running_handles);
            break;

        case 3:
            /* Call curl_multi_perform multiple times to explore state changes */
            for (int i = 0; i < 5 && Size > (size_t)(i + 1); i++) {
                res = curl_multi_perform(multi_handle, &running_handles);
                if (res != CURLM_OK) break;
            }
            break;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}