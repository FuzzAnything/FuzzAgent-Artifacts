// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_init, curl_multi_strerror, curl_multi_poll, curl_multi_wait
 * Note: Wcurl_easy_getinfo_err_* are compile-time macros and do not require runtime fuzzing.
 */

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize the multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        /* If init fails, we can still test strerror with various codes */
        CURLMcode error_codes[] = {
            CURLM_OK, CURLM_CALL_MULTI_PERFORM, CURLM_OUT_OF_MEMORY,
            CURLM_INTERNAL_ERROR, CURLM_BAD_HANDLE, CURLM_BAD_EASY_HANDLE,
            CURLM_BAD_SOCKET, CURLM_UNKNOWN_OPTION, CURLM_ADDED_ALREADY
        };
        size_t num_errors = sizeof(error_codes) / sizeof(error_codes[0]);
        for (size_t i = 0; i < num_errors; i++) {
            const char *msg = curl_multi_strerror(error_codes[i]);
            if (msg) {
                /* Consume the string to avoid unused warnings */
                (void)strlen(msg);
            }
        }
        /* Also fuzz with random codes from input */
        for (size_t i = 0; i < Size; i++) {
            CURLMcode fuzzed_code = (CURLMcode)(Data[i]);
            const char *msg = curl_multi_strerror(fuzzed_code);
            if (msg) {
                (void)strlen(msg);
            }
        }
        return 0;
    }

    /* Prepare extra file descriptors for poll/wait */
    struct curl_waitfd extra_fds[1];
    extra_fds[0].fd = -1; /* Invalid fd to test error handling */
    extra_fds[0].events = 0;
    extra_fds[0].revents = 0;

    int ret = 0;
    int timeout_ms = 0;

    /* Derive parameters from input data */
    if (Size >= 2) {
        timeout_ms = (int)(Data[0] * 256 + Data[1]);
        /* Ensure timeout is not too large to avoid long waits */
        if (timeout_ms > 1000) timeout_ms = 1000;
        if (timeout_ms < 0) timeout_ms = 0;
    }

    /* Test curl_multi_poll */
    CURLMcode poll_res = curl_multi_poll(multi_handle, extra_fds, 1, timeout_ms, &ret);
    /* Test curl_multi_wait */
    CURLMcode wait_res = curl_multi_wait(multi_handle, extra_fds, 1, timeout_ms, &ret);

    /* Test curl_multi_strerror with various codes */
    const char *poll_msg = curl_multi_strerror(poll_res);
    const char *wait_msg = curl_multi_strerror(wait_res);
    if (poll_msg) (void)strlen(poll_msg);
    if (wait_msg) (void)strlen(wait_msg);

    /* Fuzz strerror with random codes from input */
    for (size_t i = 0; i < Size; i++) {
        CURLMcode fuzzed_code = (CURLMcode)(Data[i]);
        const char *msg = curl_multi_strerror(fuzzed_code);
        if (msg) {
            (void)strlen(msg);
        }
    }

    /* Cleanup */
    curl_multi_cleanup(multi_handle);

    return 0;
}