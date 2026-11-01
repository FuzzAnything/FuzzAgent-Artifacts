// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <unistd.h>
#include <sys/socket.h>

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    CURLMcode res;
    const char *error_str;

    // Create a dummy socket for socket-related functions
    int dummy_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (dummy_sock >= 0) {
        sockfd = dummy_sock;
    }

    // Parse input to determine which functions to call and with what parameters
    unsigned char mode = Data[0] % 10;

    switch (mode) {
        case 0:
            // Test curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            error_str = curl_multi_strerror(res);
            (void)error_str; // Suppress unused warning
            break;

        case 1:
            // Test curl_multi_assign
            if (sockfd >= 0 && Size > 1) {
                sockp = (void *)(uintptr_t)(Data[1] % 1000);
                res = curl_multi_assign(multi_handle, sockfd, sockp);
                error_str = curl_multi_strerror(res);
                (void)error_str;
            }
            break;

        case 2:
            // Test curl_multi_strerror with various error codes
            if (Size > 1) {
                CURLMcode test_code = (CURLMcode)(Data[1] % 20);
                error_str = curl_multi_strerror(test_code);
                (void)error_str;
            }
            break;

        case 3:
            // Test deprecated curl_multi_socket
            if (sockfd >= 0) {
                res = curl_multi_socket(multi_handle, sockfd, &running_handles);
                error_str = curl_multi_strerror(res);
                (void)error_str;
            }
            break;

        case 4:
            // Test curl_multi_socket_action
            if (sockfd >= 0) {
                int ev_bitmask = (Size > 2) ? (Data[2] % 8) : 0;
                res = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
                error_str = curl_multi_strerror(res);
                (void)error_str;
            }
            break;

        case 5:
            // Test deprecated curl_multi_socket_all
            res = curl_multi_socket_all(multi_handle, &running_handles);
            error_str = curl_multi_strerror(res);
            (void)error_str;
            break;

        case 6:
            // Test multiple calls in sequence
            res = curl_multi_perform(multi_handle, &running_handles);
            if (sockfd >= 0) {
                res = curl_multi_socket_action(multi_handle, sockfd, 1, &running_handles);
            }
            error_str = curl_multi_strerror(res);
            (void)error_str;
            break;

        case 7:
            // Test with invalid socket
            res = curl_multi_socket_action(multi_handle, -1, 0, &running_handles);
            error_str = curl_multi_strerror(res);
            (void)error_str;
            break;

        case 8:
            // Test curl_multi_assign with NULL pointer
            if (sockfd >= 0) {
                res = curl_multi_assign(multi_handle, sockfd, NULL);
                error_str = curl_multi_strerror(res);
                (void)error_str;
            }
            break;

        case 9:
            // Test curl_multi_perform multiple times
            for (int i = 0; i < 5; i++) {
                res = curl_multi_perform(multi_handle, &running_handles);
                error_str = curl_multi_strerror(res);
                (void)error_str;
            }
            break;

        default:
            break;
    }

    // Cleanup
    if (sockfd >= 0) {
        close(sockfd);
    }

    curl_multi_cleanup(multi_handle);

    return 0;
}