// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets:
 * 1. curl_multi_socket_action
 * 2. curl_multi_socket (deprecated)
 * 3. curl_multi_socket_all (deprecated)
 * 4. curl_multi_perform
 *
 * Note: Wcurl_* macros are compile-time checks and cannot be fuzzed directly.
 * We include curl/curl.h and curl/multi.h to get the necessary definitions.
 * We avoid including typecheck-gcc.h directly if it causes conflicts with the
 * standard headers which already include necessary macros or if the build
 * environment handles them differently. The error suggests a conflict with
 * the macro expansion of curl_multi_setopt in typecheck-gcc.h.
 * By including the standard headers, we ensure the correct macros are defined.
 */

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURLMcode res;
    int running_handles = 0;
    curl_socket_t socket_fd = CURL_SOCKET_BAD;
    int ev_bitmask = 0;

    /* Initialize the multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /*
     * Extract parameters from fuzz input to diversify states.
     * We use modulo and bounds checking to ensure valid ranges.
     */
    if (Size >= 1) {
        ev_bitmask = (int)Data[0];
    }
    if (Size >= 2) {
        /* Simulate a socket descriptor; avoid negative values unless CURL_SOCKET_BAD */
        socket_fd = (curl_socket_t)(Data[1]);
        if (socket_fd == 0) {
            socket_fd = CURL_SOCKET_BAD;
        }
    }

    /*
     * Add a dummy easy handle to the multi handle to ensure there is something to process.
     * This helps trigger more internal logic paths.
     */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com/fuzz");
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_MAXREDIRS, 5L);
        curl_easy_setopt(easy_handle, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(easy_handle, CURLOPT_ERRORBUFFER, NULL);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, NULL);
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, NULL);
        curl_easy_setopt(easy_handle, CURLOPT_READDATA, NULL);

        res = curl_multi_add_handle(multi_handle, easy_handle);
        if (res != CURLM_OK) {
            /* If adding fails, we still proceed to test the API with the empty handle */
        }
    }

    /*
     * 1. Test curl_multi_socket_action
     * We try different event bitmasks: CURL_CSELECT_IN, CURL_CSELECT_OUT, CURL_CSELECT_ERR, and combinations.
     */
    res = curl_multi_socket_action(multi_handle, socket_fd, ev_bitmask, &running_handles);
    (void)res; /* Suppress unused variable warning */

    /*
     * 2. Test curl_multi_socket (deprecated)
     * Try with the same socket and a different bitmask to explore state.
     */
    int ev_bitmask2 = (Size >= 3) ? (int)Data[2] : 0;
    res = curl_multi_socket(multi_handle, socket_fd, &running_handles);
    (void)res;

    /*
     * 3. Test curl_multi_socket_all (deprecated)
     */
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    /*
     * 4. Test curl_multi_perform
     * Call multiple times to ensure different internal states are exercised.
     */
    for (int i = 0; i < 5; i++) {
        res = curl_multi_perform(multi_handle, &running_handles);
        (void)res;
    }

    /*
     * Cleanup: Remove the easy handle if it was added, then cleanup the multi handle.
     */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    curl_multi_cleanup(multi_handle);

    return 0;
}