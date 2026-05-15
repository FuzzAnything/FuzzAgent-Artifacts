// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#define CURL_DISABLE_TYPECHECK
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/multi.h"
#include <sys/select.h>

static void dummy_callback(void) {}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Initialize multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Write data to dummy file if needed
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Use fuzzer data to influence parameters
    long timeout_val = 0;
    curl_socket_t sockfd = 0;
    void *sockp = NULL;
    int timeout_ms = 0;
    int running_handles = 0;
    int max_fd = 0;
    
    // Extract some values from fuzzer input
    if (Size >= sizeof(long)) {
        memcpy(&timeout_val, Data, sizeof(long));
    }
    if (Size >= sizeof(curl_socket_t) + sizeof(long)) {
        memcpy(&sockfd, Data + sizeof(long), sizeof(curl_socket_t));
    }
    if (Size >= sizeof(int) + sizeof(curl_socket_t) + sizeof(long)) {
        memcpy(&timeout_ms, Data + sizeof(long) + sizeof(curl_socket_t), sizeof(int));
    }

    // Create some dummy socket pointer from fuzzer data
    if (Size > 0) {
        sockp = (void *)(uintptr_t)Data[0];
    }

    // Test curl_multi_timeout
    long milliseconds = 0;
    curl_multi_timeout(multi_handle, &milliseconds);

    // Test curl_multi_assign
    curl_multi_assign(multi_handle, sockfd, sockp);

    // Test curl_multi_wait
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    
    if (Size >= sizeof(struct curl_waitfd) * 2) {
        // Initialize extra_fds with fuzzer data
        memcpy(extra_fds, Data, sizeof(struct curl_waitfd) * 2);
        extra_nfds = 2;
    }
    
    int ret = 0;
    curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    // Test curl_multi_poll
    curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret);

    // Test curl_multi_fdset
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);
    
    curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

    // Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Clean up
    curl_multi_cleanup(multi_handle);
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}