// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_handles(CURLM *multi_handle, CURL **easy_handles, int count) {
    if (!multi_handle) return;
    for (int i = 0; i < count; i++) {
        if (easy_handles[i]) {
            curl_multi_remove_handle(multi_handle, easy_handles[i]);
            curl_easy_cleanup(easy_handles[i]);
            easy_handles[i] = NULL;
        }
    }
    curl_multi_cleanup(multi_handle);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) return 0;

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int result = 0;
    int ret;
    struct curl_waitfd extra_fds[1];
    curl_socket_t sockfd = -1;
    void *sockp = (void *)Data;

    // Initialize extra_fds
    extra_fds[0].fd = -1;
    extra_fds[0].events = 0;
    extra_fds[0].revents = 0;

    // Fuzz curl_multi_assign
    if (Size >= 4) {
        int fd_val = Data[0] | (Data[1] << 8);
        if (fd_val > 0 && fd_val < 1024) {
            sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd >= 0) {
                result = curl_multi_assign(multi_handle, sockfd, sockp);
                (void)result; // Use result to avoid warning
                close(sockfd);
            }
        }
    }

    // Fuzz curl_multi_remove_handle
    result = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)result;

    // Fuzz curl_multi_wakeup
    result = curl_multi_wakeup(multi_handle);
    (void)result;

    // Fuzz curl_multi_wait
    int timeout = (int)(Data[0] % 1000);
    extra_fds[0].fd = -1;
    extra_fds[0].events = (short)(Data[1] & 0xFF);
    extra_fds[0].revents = 0;
    result = curl_multi_wait(multi_handle, extra_fds, 1, timeout, &ret);
    (void)result;

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_codes[] = {
        CURLM_OK, CURLM_BAD_HANDLE, CURLM_BAD_EASY_HANDLE, 
        CURLM_OUT_OF_MEMORY, CURLM_INTERNAL_ERROR, CURLM_WAKEUP_FAILURE
    };
    for (int i = 0; i < 6; i++) {
        const char *str = curl_multi_strerror(error_codes[i]);
        if (str) {
            // Ensure we don't use the string to avoid potential issues
            (void)str;
        }
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}