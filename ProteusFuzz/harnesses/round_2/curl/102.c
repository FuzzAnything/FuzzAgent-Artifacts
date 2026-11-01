// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t socket_fd = -1;
    int ev_bitmask = 0;

    // Extract values from input data if available
    if (Size > 0) {
        socket_fd = (curl_socket_t)Data[0];
        if (Size > 1) {
            ev_bitmask = (int)Data[1];
        }
    }

    // Test curl_multi_assign
    void *sockp = (void *)(intptr_t)(Size > 2 ? Data[2] : 0);
    curl_multi_assign(multi_handle, socket_fd, sockp);

    // Test curl_multi_socket_all (deprecated)
    curl_multi_socket_all(multi_handle, &running_handles);

    // Test curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Test curl_multi_socket (deprecated)
    curl_multi_socket(multi_handle, socket_fd, &running_handles);

    // Test curl_multi_socket_action
    curl_multi_socket_action(multi_handle, socket_fd, ev_bitmask, &running_handles);

    // Test curl_multi_strerror with various error codes
    CURLMcode error_codes[] = {
        CURLM_OK,
        CURLM_CALL_MULTI_PERFORM,
        CURLM_BAD_HANDLE,
        CURLM_BAD_EASY_HANDLE,
        CURLM_OUT_OF_MEMORY,
        CURLM_INTERNAL_ERROR,
        CURLM_BAD_SOCKET,
        CURLM_UNKNOWN_OPTION,
        CURLM_ADDED_ALREADY,
        CURLM_RECURSIVE_API_CALL,
        CURLM_LAST
    };

    for (size_t i = 0; i < sizeof(error_codes) / sizeof(error_codes[0]); i++) {
        const char *error_msg = curl_multi_strerror(error_codes[i]);
        if (error_msg) {
            // Just to ensure the function is called and result is used
            (void)strlen(error_msg);
        }
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}