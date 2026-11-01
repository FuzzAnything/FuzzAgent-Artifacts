// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_poll at multi.c:1584:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(CURL *handle, int socket, int what, void *userp, void *socketp) {
    (void)handle;
    (void)socket;
    (void)what;
    (void)userp;
    (void)socketp;
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    int ret_val = 0;
    CURLMcode res;
    struct curl_waitfd extra_fds[10];
    unsigned int extra_nfds = 0;
    int timeout_ms = 0;

    // Initialize extra_fds with safe defaults
    memset(extra_fds, 0, sizeof(extra_fds));

    // Use fuzzing data to configure parameters safely
    if (Size >= 1) {
        extra_nfds = Data[0] % 10;
    }
    if (Size >= 2) {
        timeout_ms = (int)Data[1];
    }
    if (Size >= 3) {
        // Set a dummy socket for extra_fds if needed
        if (extra_nfds > 0) {
            extra_fds[0].fd = (curl_socket_t)(Data[2]);
            extra_fds[0].events = 1; // CURL_WAIT_POLLIN
        }
    }

    // Fuzz curl_multi_wait
    res = curl_multi_wait(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
    (void)res;

    // Fuzz curl_multi_poll
    res = curl_multi_poll(multi_handle, extra_fds, extra_nfds, timeout_ms, &ret_val);
    (void)res;

    // Fuzz curl_multi_strerror with various error codes derived from input
    if (Size >= 4) {
        CURLMcode err_code = (CURLMcode)Data[3];
        const char *str = curl_multi_strerror(err_code);
        (void)str;
    }

    // Test with a range of error codes to explore different paths in strerror
    for (int i = 0; i < 10 && Size > 4; i++) {
        CURLMcode test_err = (CURLMcode)(Data[4 + i] + i);
        const char *test_str = curl_multi_strerror(test_err);
        (void)test_str;
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}