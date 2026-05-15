// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#define CURL_DISABLE_TYPECHECK
#include "curl/multi.h"

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Test curl_multi_timeout
    long timeout_ms;
    curl_multi_timeout(multi_handle, &timeout_ms);

    // Test curl_multi_assign with fuzzer-derived socket descriptor
    if (Size >= sizeof(int)) {
        int sockfd;
        memcpy(&sockfd, Data, sizeof(int));
        void *sockp = (void *)(Data + sizeof(int));
        curl_multi_assign(multi_handle, (curl_socket_t)sockfd, sockp);
    }

    // Test curl_multi_wait with fuzzer-derived extra file descriptors
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    int wait_ret;
    
    if (Size >= sizeof(struct curl_waitfd)) {
        memcpy(&extra_fds[0], Data, sizeof(struct curl_waitfd));
        extra_nfds = 1;
    }
    
    int timeout_from_data = 1000; // Default timeout
    if (Size >= sizeof(int)) {
        memcpy(&timeout_from_data, Data, sizeof(int));
        if (timeout_from_data < 0) timeout_from_data = 0;
        if (timeout_from_data > 10000) timeout_from_data = 10000;
    }
    
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_from_data, &wait_ret);

    // Test curl_multi_poll (similar to wait but different implementation)
    int poll_ret;
    curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_from_data, &poll_ret);

    // Test curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    int max_fd = 0;
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

    // Test curl_multi_perform
    int running_handles = 0;
    curl_multi_perform(multi_handle, &running_handles);

    // Cleanup
    curl_multi_cleanup(multi_handle);
    
    return 0;
}