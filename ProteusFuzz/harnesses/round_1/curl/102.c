// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;

    // Extract values from input data safely
    int error_code = 0;
    int socket_fd = 0;
    int action = 0;
    int assign_flag = 0;

    if (Size >= 4) {
        memcpy(&error_code, Data, sizeof(int));
    }
    if (Size >= 8) {
        memcpy(&socket_fd, Data + 4, sizeof(int));
    }
    if (Size >= 12) {
        memcpy(&action, Data + 8, sizeof(int));
    }
    if (Size >= 16) {
        memcpy(&assign_flag, Data + 12, sizeof(int));
    }

    // Test curl_multi_strerror with various error codes
    const char *error_msg = curl_multi_strerror((CURLMcode)error_code);
    if (error_msg) {
        (void)strlen(error_msg);
    }

    // Test curl_multi_socket
    if (action & 1) {
        curl_multi_socket(multi_handle, (curl_socket_t)socket_fd, &running_handles);
    }

    // Test curl_multi_socket_all
    if (action & 2) {
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    // Test curl_multi_perform
    if (action & 4) {
        curl_multi_perform(multi_handle, &running_handles);
    }

    // Test curl_multi_wakeup
    if (action & 8) {
        curl_multi_wakeup(multi_handle);
    }

    // Test curl_multi_assign
    if (assign_flag) {
        sockfd = (curl_socket_t)socket_fd;
        sockp = (void *)(uintptr_t)socket_fd;
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}