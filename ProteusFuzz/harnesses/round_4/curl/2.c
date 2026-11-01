// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Tests: curl_multi_init, curl_multi_add_handle, curl_multi_remove_handle,
 * curl_multi_poll, curl_multi_wait, curl_multi_strerror, curl_multi_cleanup.
 */

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
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

    /* Set minimal options to ensure the handle is valid */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);

    /* Add handle to multi */
    CURLMcode rc = curl_multi_add_handle(multi_handle, easy_handle);
    /* Ignore return code for fuzzing robustness, but proceed to remove */

    /* Prepare extra_fds for poll/wait functions based on input data */
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    int ret = 0;
    int timeout_ms = 0;

    if (Size >= 4) {
        timeout_ms = (int)(Data[0] | (Data[1] << 8));
        if (timeout_ms < 0) timeout_ms = 0;
        if (timeout_ms > 1000) timeout_ms = 1000;

        if (Size >= 8) {
            extra_nfds = (unsigned int)(Data[2] & 0x01);
            if (extra_nfds > 1) extra_nfds = 1;

            extra_fds[0].fd = (curl_socket_t)(Data[3] | (Data[4] << 8));
            extra_fds[0].events = (short)(Data[5] & 0x03);
            extra_fds[0].revents = 0;

            if (Size >= 12) {
                extra_nfds = (unsigned int)(Data[6] & 0x01);
                if (extra_nfds > 1) extra_nfds = 1;
                extra_fds[1].fd = (curl_socket_t)(Data[7] | (Data[8] << 8));
                extra_fds[1].events = (short)(Data[9] & 0x03);
                extra_fds[1].revents = 0;
            }
        }
    }

    /* Test curl_multi_poll */
    int ret_poll = 0;
    rc = curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_poll);
    (void)curl_multi_strerror(rc);

    /* Test curl_multi_wait */
    int ret_wait = 0;
    rc = curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_wait);
    (void)curl_multi_strerror(rc);

    /* Test curl_multi_remove_handle */
    rc = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)curl_multi_strerror(rc);

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}