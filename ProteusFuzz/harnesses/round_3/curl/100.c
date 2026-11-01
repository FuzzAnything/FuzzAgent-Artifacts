// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_multi_handle(CURLM *multi_handle) {
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
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

    // Extract socket file descriptor from input if possible
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&sockfd, Data, sizeof(curl_socket_t));
    }

    // Extract event bitmask from input if possible
    int ev_bitmask = 0;
    if (Size >= sizeof(curl_socket_t) + sizeof(int)) {
        memcpy(&ev_bitmask, Data + sizeof(curl_socket_t), sizeof(int));
    }

    // Test curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    // Test curl_multi_assign
    res = curl_multi_assign(multi_handle, sockfd, sockp);
    (void)res;

    // Test curl_multi_strerror with various error codes
    error_str = curl_multi_strerror(CURLM_OK);
    (void)error_str;
    error_str = curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)error_str;
    error_str = curl_multi_strerror(CURLM_RECURSIVE_API_CALL);
    (void)error_str;
    // Test with a potentially invalid error code
    error_str = curl_multi_strerror((CURLMcode)0xFFFFFFFF);
    (void)error_str;

    // Test curl_multi_socket (deprecated)
    res = curl_multi_socket(multi_handle, sockfd, &running_handles);
    (void)res;

    // Test curl_multi_socket_action
    res = curl_multi_socket_action(multi_handle, sockfd, ev_bitmask, &running_handles);
    (void)res;

    // Test curl_multi_socket_all (deprecated)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    cleanup_multi_handle(multi_handle);
    return 0;
}