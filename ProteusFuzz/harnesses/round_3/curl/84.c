// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURLMcode res;
    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;

    /* Initialize multi handle */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* 1. curl_multi_strerror: fuzz with various error codes */
    if (Size >= 1) {
        CURLMcode error_code = (CURLMcode)Data[0];
        const char *str = curl_multi_strerror(error_code);
        if (str) {
            (void)strlen(str);
        }
    }

    /* 2. curl_multi_assign: fuzz socket and pointer */
    if (Size >= 4) {
        sockfd = *(const int *)(Data);
        /* Use a dummy pointer based on input data */
        sockp = (void *)(uintptr_t)(Data[1] | (Data[2] << 8) | (Data[3] << 16));
        res = curl_multi_assign(multi_handle, sockfd, sockp);
        (void)res;
    }

    /* 3. curl_multi_perform */
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    /* 4. curl_multi_socket (deprecated) */
    if (Size >= 1) {
        sockfd = (curl_socket_t)Data[0];
        res = curl_multi_socket(multi_handle, sockfd, &running_handles);
        (void)res;
    }

    /* 5. curl_multi_wakeup */
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    /* 6. curl_multi_socket_all (deprecated) */
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    /* Cleanup */
    curl_multi_cleanup(multi_handle);

    return 0;
}