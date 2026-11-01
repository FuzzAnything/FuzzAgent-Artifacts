// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_wait at multi.c:1574:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Fuzz curl_multi_strerror with various error codes derived from input
    if (Size > 0) {
        CURLMcode code = (CURLMcode)(Data[0] % 100); 
        const char *err_str = curl_multi_strerror(code);
        (void)err_str; 
    }

    // Fuzz curl_multi_remove_handle
    // Attempt to remove the easy handle from the multi handle
    // Note: We didn't add it, so this might return an error, which is fine for fuzzing
    CURLMcode res = curl_multi_remove_handle(multi, easy);
    (void)res;

    // Fuzz curl_multi_waitfds
    // First, get the count
    unsigned int fd_count = 0;
    res = curl_multi_waitfds(multi, NULL, 0, &fd_count);
    (void)res;

    // Allocate buffer based on count or input size if count is 0
    unsigned int alloc_size = fd_count > 0 ? fd_count : (Size > 0 ? Size : 1);
    struct curl_waitfd *ufds = (struct curl_waitfd *)malloc(alloc_size * sizeof(struct curl_waitfd));
    if (ufds) {
        // Initialize with dummy data from input
        for (unsigned int i = 0; i < alloc_size && i < Size; i++) {
            ufds[i].fd = (curl_socket_t)(Data[i] & 0xFF);
            ufds[i].events = (short)((Data[i] >> 8) & 0xFF);
            ufds[i].revents = 0;
        }
        // Call with buffer
        res = curl_multi_waitfds(multi, ufds, alloc_size, &fd_count);
        (void)res;
        free(ufds);
    }

    // Fuzz curl_multi_wait
    // Prepare extra_fds if we have enough data
    struct curl_waitfd extra_fds[2];
    unsigned int extra_nfds = 0;
    int ret = 0;
    int timeout_ms = 0;

    if (Size >= 4) {
        extra_fds[0].fd = (curl_socket_t)(Data[1] & 0xFF);
        extra_fds[0].events = (short)((Data[2] >> 4) & 0xF);
        extra_fds[0].revents = 0;
        extra_fds[1].fd = (curl_socket_t)(Data[3] & 0xFF);
        extra_fds[1].events = (short)((Data[3] >> 4) & 0xF);
        extra_fds[1].revents = 0;
        extra_nfds = 2;
        timeout_ms = (int)(Data[0] % 1000);
    } else if (Size >= 1) {
        extra_fds[0].fd = (curl_socket_t)(Data[0] & 0xFF);
        extra_fds[0].events = 0;
        extra_fds[0].revents = 0;
        extra_nfds = 1;
        timeout_ms = 0;
    }

    res = curl_multi_wait(multi, extra_fds, extra_nfds, timeout_ms, &ret);
    (void)res;
    (void)ret;

    // Cleanup
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}