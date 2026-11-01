// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <curl/multi.h>

static int create_dummy_socket(void) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return -1;
    }
    return sock;
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int ret_val = 0;
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    int timeout_ms = 0;
    unsigned int fd_count = 0;
    unsigned int notification_flags = 0;

    // Initialize extra_fds with dummy sockets if needed
    if (Size > 1 && (Data[1] % 2) == 1) {
        int sock1 = create_dummy_socket();
        int sock2 = create_dummy_socket();
        if (sock1 >= 0) {
            extra_fds[0].fd = sock1;
            extra_fds[0].events = 1;
            extra_fds[0].revents = 0;
            extra_nfds = 1;
        }
        if (sock2 >= 0) {
            extra_fds[1].fd = sock2;
            extra_fds[1].events = 1;
            extra_fds[1].revents = 0;
            extra_nfds = 2;
        }
    }

    // Derive timeout from input
    if (Size > 2) {
        timeout_ms = (int)(Data[2] % 1000);
    }

    // Derive notification flags
    if (Size > 3) {
        notification_flags = (unsigned int)(Data[3] % 256);
    }

    uint8_t op = Data[0] % 6;

    switch (op) {
        case 0:
            // curl_multi_poll
            curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
            break;
        case 1:
            // curl_multi_notify_disable
            curl_multi_notify_disable(multi_handle, notification_flags);
            break;
        case 2:
            // curl_multi_waitfds (get count first)
            curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
            // Then try to fill buffer if size allows
            if (Size > 4 && fd_count > 0) {
                struct curl_waitfd *ufds = malloc(fd_count * sizeof(struct curl_waitfd));
                if (ufds) {
                    curl_multi_waitfds(multi_handle, ufds, fd_count, &fd_count);
                    free(ufds);
                }
            }
            break;
        case 3:
            // curl_multi_notify_enable
            curl_multi_notify_enable(multi_handle, notification_flags);
            break;
        case 4:
            // curl_multi_wait
            curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
            break;
        case 5:
            // Combined operations
            curl_multi_notify_enable(multi_handle, notification_flags);
            curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
            curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
            curl_multi_notify_disable(multi_handle, notification_flags);
            break;
        default:
            break;
    }

    // Cleanup extra sockets
    for (unsigned int i = 0; i < extra_nfds; i++) {
        if (extra_fds[i].fd >= 0) {
            close(extra_fds[i].fd);
        }
    }

    curl_multi_cleanup(multi_handle);
    return 0;
}