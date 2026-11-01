// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        const char *dummy_data = "Fuzzing data for curl multi handle testing";
        fwrite(dummy_data, 1, strlen(dummy_data), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    setup_dummy_file();

    int running_handles = 0;
    curl_socket_t socket_fd = -1;
    void *sockp = (void *)Data;

    // Fuzz curl_multi_assign
    if (Size > 0) {
        socket_fd = (curl_socket_t)(Data[0] & 0xFF);
        curl_multi_assign(multi_handle, socket_fd, sockp);
    }

    // Fuzz curl_multi_perform
    curl_multi_perform(multi_handle, &running_handles);

    // Fuzz curl_multi_socket
    if (Size > 1) {
        curl_socket_t s = (curl_socket_t)(Data[1] & 0xFF);
        curl_multi_socket(multi_handle, s, &running_handles);
    }

    // Fuzz curl_multi_socket_all
    curl_multi_socket_all(multi_handle, &running_handles);

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_codes[] = {
        CURLM_OK, 
        CURLM_BAD_HANDLE, 
        CURLM_BAD_EASY_HANDLE,
        CURLM_OUT_OF_MEMORY, 
        CURLM_INTERNAL_ERROR, 
        CURLM_BAD_SOCKET,
        CURLM_UNKNOWN_OPTION, 
        CURLM_ADDED_ALREADY, 
        CURLM_RECURSIVE_API_CALL,
        CURLM_WAKEUP_FAILURE
    };
    size_t num_errors = sizeof(error_codes) / sizeof(error_codes[0]);
    for (size_t i = 0; i < num_errors; i++) {
        const char *msg = curl_multi_strerror(error_codes[i]);
        (void)msg; // Suppress unused variable warning
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}