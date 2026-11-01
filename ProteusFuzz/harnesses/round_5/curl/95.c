// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sock = -1;
    void *sockp = NULL;
    CURLMcode res;

    // Use first byte to determine which function to fuzz
    uint8_t op = Data[0] % 6;

    switch (op) {
        case 0:
            // Fuzz curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            (void)res;
            break;

        case 1:
            // Fuzz curl_multi_socket (deprecated)
            if (Size > 1) {
                sock = (curl_socket_t)Data[1];
            }
            res = curl_multi_socket(multi_handle, sock, &running_handles);
            (void)res;
            break;

        case 2:
            // Fuzz curl_multi_strerror
            if (Size > 1) {
                CURLMcode err = (CURLMcode)Data[1];
                const char *str = curl_multi_strerror(err);
                (void)str;
            }
            break;

        case 3:
            // Fuzz curl_multi_socket_all (deprecated)
            res = curl_multi_socket_all(multi_handle, &running_handles);
            (void)res;
            break;

        case 4:
            // Fuzz curl_multi_assign
            if (Size > 2) {
                sock = (curl_socket_t)Data[1];
                // Create a dummy pointer based on input data
                sockp = (void *)(intptr_t)(Data[2]);
            }
            res = curl_multi_assign(multi_handle, sock, sockp);
            (void)res;
            break;

        case 5:
            // Fuzz curl_multi_wakeup
            res = curl_multi_wakeup(multi_handle);
            (void)res;
            break;
    }

    curl_multi_cleanup(multi_handle);
    return 0;
}