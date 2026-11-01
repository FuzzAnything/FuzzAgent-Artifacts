// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Test curl_multi_strerror with various error codes derived from input */
    CURLMcode error_code = (CURLMcode)(Data[0] % 100);
    const char *error_str = curl_multi_strerror(error_code);
    (void)error_str; /* Suppress unused variable warning */

    /* Test curl_multi_assign */
    curl_socket_t sockfd = -1;
    if (Size > 1) {
        sockfd = (curl_socket_t)(Data[1] & 0x7F);
    }
    void *sockp = (void *)(uintptr_t)(Data[2] % 1000);
    if (Size > 2) {
        sockp = (void *)(uintptr_t)(Data[2]);
    }
    CURLMcode assign_res = curl_multi_assign(multi_handle, sockfd, sockp);
    (void)assign_res;

    /* Test curl_multi_wait */
    struct curl_waitfd extra_fds[2];
    memset(extra_fds, 0, sizeof(extra_fds));
    if (Size > 3) {
        extra_fds[0].fd = (curl_socket_t)(Data[3]);
        extra_fds[0].events = (short)(Data[4] % 10);
        extra_fds[0].revents = 0;
    }
    if (Size > 5) {
        extra_fds[1].fd = (curl_socket_t)(Data[5]);
        extra_fds[1].events = (short)(Data[6] % 10);
        extra_fds[1].revents = 0;
    }

    unsigned int extra_nfds = (Size > 3) ? 1 : 0;
    if (Size > 5) {
        extra_nfds = 2;
    }

    int timeout_ms = (int)(Data[7] % 1000);
    if (Size > 7) {
        timeout_ms = (int)(Data[7]);
    }

    int ret = 0;
    CURLMcode wait_res = curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);
    (void)wait_res;
    (void)ret;

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}