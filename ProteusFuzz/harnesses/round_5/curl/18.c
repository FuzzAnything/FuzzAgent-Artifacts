// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    unsigned int fd_count = 0;
    struct curl_waitfd *ufds = NULL;
    int ret = 0;
    CURLMcode res;

    // Test curl_multi_waitfds to get count
    res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
    if (res == CURLM_OK && fd_count > 0) {
        ufds = (struct curl_waitfd *)malloc(fd_count * sizeof(struct curl_waitfd));
        if (ufds) {
            res = curl_multi_waitfds(multi, ufds, fd_count, &fd_count);
            free(ufds);
            ufds = NULL;
        }
    }

    // Test curl_multi_wait with extra_fds derived from input if possible
    struct curl_waitfd extra_fds[1];
    int extra_nfds = 0;
    if (Size >= 4) {
        int fd_val = *(const int *)(Data);
        extra_fds[0].fd = (curl_socket_t)fd_val;
        extra_fds[0].events = 0;
        extra_fds[0].revents = 0;
        extra_nfds = 1;
    }

    int timeout_ms = 0;
    if (Size >= 8) {
        timeout_ms = *(const int *)(Data + 4);
        if (timeout_ms < 0) timeout_ms = 0;
    }

    res = curl_multi_wait(multi, extra_nfds > 0 ? extra_fds : NULL, extra_nfds, timeout_ms, &ret);

    // Test curl_multi_notify_enable and disable with various flags
    unsigned int notification = 0;
    if (Size >= 12) {
        notification = *(const unsigned int *)(Data + 8);
    }

    res = curl_multi_notify_enable(multi, notification);
    res = curl_multi_notify_disable(multi, notification);

    // Test curl_multi_strerror with various error codes
    CURLMcode error_code = CURLM_OK;
    if (Size >= 16) {
        error_code = *(const CURLMcode *)(Data + 12);
    }
    const char *err_str = curl_multi_strerror(error_code);
    (void)err_str; // Suppress unused warning

    // Cleanup
    curl_multi_cleanup(multi);

    return 0;
}