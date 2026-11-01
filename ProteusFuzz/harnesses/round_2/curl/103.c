// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions:
 * - curl_multi_socket_all
 * - curl_multi_perform
 * - curl_multi_socket
 *
 * Note: The Wcurl_* macros are compile-time type-checking helpers and do not
 * represent runtime functions to be fuzzed directly.
 */

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_TIMEOUT;
    CURLMcode res;

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle and add it to the multi handle */
    easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Set a dummy URL to ensure the handle is valid */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);

        /* Add to multi handle */
        if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
            curl_easy_cleanup(easy_handle);
            easy_handle = NULL;
        }
    }

    /*
     * Fuzzing logic: Use input data to drive different code paths.
     * We interpret parts of the input to decide which functions to call
     * and with what parameters (within safe bounds).
     */

    if (Size > 0) {
        /* Use first byte to decide which function to call */
        switch (Data[0] % 3) {
            case 0:
                /* Test curl_multi_socket_all */
                res = curl_multi_socket_all(multi_handle, &running_handles);
                (void)res; /* Suppress unused variable warning */
                break;

            case 1:
                /* Test curl_multi_perform */
                res = curl_multi_perform(multi_handle, &running_handles);
                (void)res;
                break;

            case 2:
                /* Test curl_multi_socket with a socket derived from input */
                if (Size > 1) {
                    s = (curl_socket_t)(Data[1] % 1000);
                    /* Also try CURL_SOCKET_TIMEOUT */
                    if (Size > 2 && Data[2] % 2 == 0) {
                        s = CURL_SOCKET_TIMEOUT;
                    }
                    res = curl_multi_socket(multi_handle, s, &running_handles);
                    (void)res;
                }
                break;
        }
    }

    /*
     * Additional exploration: Call functions multiple times with varying states
     * to trigger different internal states and potential edge cases.
     */
    for (int i = 0; i < 3; i++) {
        if (Size > (size_t)(i + 1)) {
            int choice = Data[i + 1] % 3;
            if (choice == 0) {
                curl_multi_socket_all(multi_handle, &running_handles);
            } else if (choice == 1) {
                curl_multi_perform(multi_handle, &running_handles);
            } else {
                curl_socket_t sock = (Size > (size_t)(i + 2)) ? (curl_socket_t)(Data[i + 2] % 500) : CURL_SOCKET_TIMEOUT;
                curl_multi_socket(multi_handle, sock, &running_handles);
            }
        }
    }

    /* Cleanup */
    if (easy_handle && multi_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}