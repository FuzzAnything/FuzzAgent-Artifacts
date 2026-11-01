// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

static void cleanup_multi_handle(CURLM *multi_handle) {
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURLMcode res;
    int running_handles = 0;
    curl_socket_t sockfd;
    void *sockp;
    CURLMcode error_code;
    int ev_bitmask;

    // Parse input data
    sockfd = (curl_socket_t)(Data[0]);
    error_code = (CURLMcode)(Data[1]);
    if (Size > 2) {
        ev_bitmask = (int)(Data[2]);
    } else {
        ev_bitmask = 0;
    }

    // Allocate a dummy pointer for sockp
    sockp = malloc(16);
    if (!sockp) {
        return 0;
    }

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        free(sockp);
        return 0;
    }

    // Test curl_multi_strerror
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        (void)strlen(error_str); // Use the result to avoid unused variable warning
    }

    // Test curl_multi_assign
    res = curl_multi_assign(multi_handle, sockfd, sockp);
    (void)res; // Use the result to avoid unused variable warning

    // Test curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    // Test curl_multi_socket (deprecated)
    res = curl_multi_socket(multi_handle, sockfd, &running_handles);
    (void)res;

    // Test curl_multi_socket_action
    res = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
    (void)res;

    // Test curl_multi_socket_all (deprecated)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    // Cleanup
    free(sockp);
    cleanup_multi_handle(multi_handle);

    return 0;
}