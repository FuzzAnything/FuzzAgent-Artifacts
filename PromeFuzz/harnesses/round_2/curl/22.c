// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Create a dummy easy handle and add to multi
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    // Prepare file descriptors and structures
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = 0;
    
    // Call curl_multi_fdset
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

    // Call curl_multi_assign with various socket values
    if (Size > 0) {
        curl_socket_t sockfd = (curl_socket_t)(Data[0] % 256);
        void *sockp = (void*)(uintptr_t)Data[0];
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Prepare for curl_multi_wait and curl_multi_poll
    struct curl_waitfd extra_fds[3];
    unsigned int extra_nfds = 0;
    int timeout_ms = 0;
    int ret = 0;
    
    // Set up extra file descriptors based on input data
    if (Size >= 3) {
        extra_nfds = (Data[1] % 3) + 1;
        for (unsigned int i = 0; i < extra_nfds && i < 3; i++) {
            extra_fds[i].fd = (curl_socket_t)(Data[i] % 256);
            extra_fds[i].events = (short)(Data[i] % 4);
            extra_fds[i].revents = 0;
        }
        timeout_ms = (int)(Data[2] % 1000);
    }

    // Call curl_multi_wait
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    // Call curl_multi_poll
    curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    // Call curl_multi_timeout
    long milliseconds = 0;
    curl_multi_timeout(multi_handle, &milliseconds);

    // Call curl_multi_perform
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // Clean up
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    
    return 0;
}