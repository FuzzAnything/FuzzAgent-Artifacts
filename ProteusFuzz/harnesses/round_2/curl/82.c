// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_multi_handle(CURLM *multi_handle) {
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Extract values from input data safely
    CURLMcode error_code = (CURLMcode)(Data[0]);
    curl_socket_t sockfd = 0;
    int running_handles = 0;
    int ev_bitmask = 0;
    void *sockp = NULL;

    // Safely extract sockfd if enough data is available
    if (Size > 2) {
        sockfd = (curl_socket_t)((Data[1] << 8) | Data[2]);
    }

    // Test curl_multi_strerror
    const char *str = curl_multi_strerror(error_code);
    (void)str; // Suppress unused variable warning

    // Test curl_multi_assign
    if (Size > 3) {
        sockp = (void *)(size_t)(Data[3]);
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Test curl_multi_socket (deprecated)
    curl_multi_socket(multi_handle, sockfd, &running_handles);

    // Test curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Test curl_multi_socket_action
    if (Size > 5) {
        ev_bitmask = (int)((Data[4] << 8) | Data[5]);
        curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
    }

    // Cleanup
    cleanup_multi_handle(multi_handle);

    return 0;
}