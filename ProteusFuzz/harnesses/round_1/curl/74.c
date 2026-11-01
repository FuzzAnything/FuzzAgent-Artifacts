// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/multi.h>

static CURLM *create_multi_handle(void) {
    return curl_multi_init();
}

static void cleanup_multi_handle(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = create_multi_handle();
    if (!multi) {
        return 0;
    }

    curl_socket_t socket_fd = -1;
    int running_handles = 0;
    CURLMcode res;
    const char *error_str;

    // Extract values from input data if available
    if (Size >= 4) {
        socket_fd = *(int32_t *)(Data);
    }
    if (Size >= 8) {
        running_handles = *(int32_t *)(Data + 4);
    }

    // Test curl_multi_perform
    res = curl_multi_perform(multi, &running_handles);
    error_str = curl_multi_strerror(res);
    (void)error_str;

    // Test curl_multi_socket_action
    int ev_bitmask = (Size > 12) ? *(int32_t *)(Data + 8) : 0;
    res = curl_multi_socket_action(multi, socket_fd, ev_bitmask, &running_handles);
    error_str = curl_multi_strerror(res);
    (void)error_str;

    // Test curl_multi_socket (deprecated)
    res = curl_multi_socket(multi, socket_fd, &running_handles);
    error_str = curl_multi_strerror(res);
    (void)error_str;

    // Test curl_multi_socket_all (deprecated)
    res = curl_multi_socket_all(multi, &running_handles);
    error_str = curl_multi_strerror(res);
    (void)error_str;

    // Test curl_multi_assign
    void *sockp = (void *)(Data + 16);
    res = curl_multi_assign(multi, socket_fd, sockp);
    error_str = curl_multi_strerror(res);
    (void)error_str;

    // Cleanup
    cleanup_multi_handle(multi);

    return 0;
}