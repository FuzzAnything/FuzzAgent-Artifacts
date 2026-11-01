// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
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
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <poll.h>

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
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

    /* Set a dummy URL to ensure the handle is somewhat valid for multi operations */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost/dummy");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);

    CURLMcode res;
    res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    /* Prepare extra file descriptors for polling functions */
    int dummy_fd = open("./dummy_file", O_RDONLY);
    if (dummy_fd < 0) {
        dummy_fd = -1;
    }

    struct curl_waitfd extra_fds[2];
    memset(extra_fds, 0, sizeof(extra_fds));
    unsigned int extra_nfds = 0;
    int ret_count = 0;
    unsigned int fd_count = 0;
    struct curl_waitfd ufds[10];
    memset(ufds, 0, sizeof(ufds));

    /* Fuzzing Logic: Derive parameters from input data */
    unsigned int notification_flags = (Size > 0) ? Data[0] : 0;
    int timeout_ms = (Size > 1) ? (int)Data[1] : 0;
    if (timeout_ms < 0) timeout_ms = 0;
    if (timeout_ms > 5000) timeout_ms = 5000;

    /* 1. Test curl_multi_notify_enable */
    res = curl_multi_notify_enable(multi_handle, notification_flags);
    (void)res;

    /* 2. Test curl_multi_notify_disable */
    res = curl_multi_notify_disable(multi_handle, notification_flags);
    (void)res;

    /* 3. Test curl_multi_waitfds */
    res = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
    if (res == CURLM_OK && fd_count > 0) {
        if (fd_count < 10) {
            res = curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
        }
    }
    (void)res;

    /* Setup extra_fds if we have a valid dummy file descriptor */
    if (dummy_fd >= 0) {
        extra_fds[0].fd = dummy_fd;
        extra_fds[0].events = POLLIN;
        extra_nfds = 1;
    } else {
        extra_nfds = 0;
    }

    /* 4. Test curl_multi_wait */
    res = curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_count);
    (void)res;

    /* 5. Test curl_multi_poll */
    res = curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_count);
    (void)res;

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    if (dummy_fd >= 0) {
        close(dummy_fd);
    }

    return 0;
}