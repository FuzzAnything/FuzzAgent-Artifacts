// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int running_handles = 0;
    curl_socket_t s = -1;
    int ev_bitmask = 0;
    CURLMcode res;
    const char *err_str;

    // Parse input for diversification
    int op = Data[0] % 6;
    s = (curl_socket_t)(Data[1] * 256 + (Size > 2 ? Data[2] : 0));
    ev_bitmask = (Size > 3 ? Data[3] : 0) | (Size > 4 ? Data[4] << 8 : 0);
    int error_code = (Size > 5 ? Data[5] : 0) + (Size > 6 ? Data[6] << 8 : 0);

    // Ensure socket is non-negative for assign, but allow -1 for others to test error paths
    if (s < 0) s = -1;

    switch (op) {
        case 0:
            // curl_multi_socket_action
            res = curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
            err_str = curl_multi_strerror(res);
            (void)err_str;
            break;

        case 1:
            // curl_multi_socket (deprecated)
            res = curl_multi_socket(multi_handle, s, &running_handles);
            err_str = curl_multi_strerror(res);
            (void)err_str;
            break;

        case 2:
            // curl_multi_socket_all (deprecated)
            res = curl_multi_socket_all(multi_handle, &running_handles);
            err_str = curl_multi_strerror(res);
            (void)err_str;
            break;

        case 3:
            // curl_multi_assign
            if (s >= 0) {
                void *dummy_ptr = (void*)(size_t)(Size > 7 ? Data[7] : 0);
                res = curl_multi_assign(multi_handle, s, dummy_ptr);
                err_str = curl_multi_strerror(res);
                (void)err_str;
            }
            break;

        case 4:
            // curl_multi_strerror with fuzzed error code
            err_str = curl_multi_strerror((CURLMcode)error_code);
            (void)err_str;
            break;

        case 5:
            // curl_multi_perform
            res = curl_multi_perform(multi_handle, &running_handles);
            err_str = curl_multi_strerror(res);
            (void)err_str;
            break;
    }

    curl_multi_cleanup(multi_handle);
    return 0;
}