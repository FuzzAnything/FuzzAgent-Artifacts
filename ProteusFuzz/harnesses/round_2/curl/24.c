// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
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

static void cleanup_handles(CURLM **handles, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (handles[i] != NULL) {
            curl_multi_cleanup(handles[i]);
        }
    }
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURLM *handles[10];
    size_t num_handles = 0;
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;

    // Initialize up to 10 multi handles based on input
    for (size_t i = 0; i < 10 && i < Size; i++) {
        handles[i] = curl_multi_init();
        if (handles[i] != NULL) {
            num_handles++;
        }
    }

    if (num_handles == 0) {
        return 0;
    }

    multi_handle = handles[0];

    // Fuzz curl_multi_strerror with various error codes
    for (size_t i = 0; i < Size; i++) {
        CURLMcode error_code = (CURLMcode)(Data[i] + i);
        const char *msg = curl_multi_strerror(error_code);
        (void)msg; // Suppress unused variable warning
    }

    // Fuzz curl_multi_wakeup
    for (size_t i = 0; i < num_handles; i++) {
        curl_multi_wakeup(handles[i]);
    }

    // Fuzz curl_multi_assign
    sockfd = (curl_socket_t)(Data[Size - 1]);
    sockp = (void *)Data;
    for (size_t i = 0; i < num_handles; i++) {
        curl_multi_assign(handles[i], sockfd, sockp);
    }

    // Fuzz deprecated curl_multi_socket
    for (size_t i = 0; i < num_handles; i++) {
        curl_multi_socket(handles[i], sockfd, &running_handles);
    }

    // Fuzz deprecated curl_multi_socket_all
    for (size_t i = 0; i < num_handles; i++) {
        curl_multi_socket_all(handles[i], &running_handles);
    }

    // Fuzz curl_multi_perform
    for (size_t i = 0; i < num_handles; i++) {
        curl_multi_perform(handles[i], &running_handles);
    }

    // Cleanup
    cleanup_handles(handles, num_handles);

    return 0;
}