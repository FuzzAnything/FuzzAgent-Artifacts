// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_multi(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    // Extract data for parameters
    int timeout_ms = 0;
    unsigned int extra_nfds = 0;
    unsigned int notification = 0;
    unsigned int fd_count = 0;
    int ret = 0;

    if (Size >= sizeof(int)) {
        memcpy(&timeout_ms, Data, sizeof(int));
        // Clamp timeout to reasonable range to avoid extreme blocking
        if (timeout_ms < 0) timeout_ms = 0;
        if (timeout_ms > 10000) timeout_ms = 10000;
    }

    if (Size >= sizeof(unsigned int) + sizeof(int)) {
        memcpy(&extra_nfds, Data + sizeof(int), sizeof(unsigned int));
        if (extra_nfds > 100) extra_nfds = 100; // Limit extra fds
    }

    if (Size >= sizeof(unsigned int) * 2 + sizeof(int)) {
        memcpy(&notification, Data + sizeof(int) + sizeof(unsigned int), sizeof(unsigned int));
    }

    // Prepare extra_fds array
    struct curl_waitfd *extra_fds = NULL;
    if (extra_nfds > 0) {
        extra_fds = (struct curl_waitfd *)malloc(extra_nfds * sizeof(struct curl_waitfd));
        if (!extra_fds) {
            cleanup_multi(multi);
            return 0;
        }
        for (unsigned int i = 0; i < extra_nfds; i++) {
            // Use dummy file descriptor (e.g., /dev/null or stdin/stdout)
            // Here we use 0 (stdin) as a safe dummy fd
            extra_fds[i].fd = 0;
            extra_fds[i].events = (i % 4) + 1; // Vary events
            extra_fds[i].revents = 0;
        }
    }

    // Prepare buffer for waitfds
    struct curl_waitfd *ufds = NULL;
    unsigned int buf_size = 0;
    if (Size >= sizeof(unsigned int) * 3 + sizeof(int)) {
        memcpy(&buf_size, Data + sizeof(int) + sizeof(unsigned int) * 2, sizeof(unsigned int));
        if (buf_size > 100) buf_size = 100;
        if (buf_size > 0) {
            ufds = (struct curl_waitfd *)malloc(buf_size * sizeof(struct curl_waitfd));
            if (!ufds) {
                free(extra_fds);
                cleanup_multi(multi);
                return 0;
            }
        }
    }

    // Fuzz curl_multi_notify_enable
    curl_multi_notify_enable(multi, notification);

    // Fuzz curl_multi_notify_disable
    curl_multi_notify_disable(multi, notification);

    // Fuzz curl_multi_waitfds
    if (ufds) {
        curl_multi_waitfds(multi, ufds, buf_size, &fd_count);
    } else {
        // First call with size 0 to get count
        curl_multi_waitfds(multi, NULL, 0, &fd_count);
    }

    // Fuzz curl_multi_wait
    curl_multi_wait(multi, extra_fds, extra_nfds, timeout_ms, &ret);

    // Fuzz curl_multi_poll
    curl_multi_poll(multi, extra_fds, extra_nfds, timeout_ms, &ret);

    // Cleanup
    free(extra_fds);
    free(ufds);
    cleanup_multi(multi);

    return 0;
}