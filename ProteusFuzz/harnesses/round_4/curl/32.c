// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_notify_enable at multi.c:3982:11 in multi.h
// curl_multi_notify_disable at multi.c:3991:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_waitfds at multi.c:1267:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode res;
    const char *error_str;

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Initialize easy handle for removal test
    easy_handle = curl_easy_init();
    if (easy_handle) {
        // Add easy handle to multi to make removal meaningful
        res = curl_multi_add_handle(multi_handle, easy_handle);
        (void)res;
    }

    // Use input data to drive logic
    unsigned int notification_flag = Data[0];
    unsigned int size = (Size > 1) ? Data[1] : 0;
    unsigned int fd_count = 0;
    CURLMcode error_code = (CURLMcode)(Data[0] & 0xFF);

    // Test curl_multi_notify_enable
    res = curl_multi_notify_enable(multi_handle, notification_flag);
    (void)res;

    // Test curl_multi_notify_disable
    res = curl_multi_notify_disable(multi_handle, notification_flag);
    (void)res;

    // Test curl_multi_wakeup
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    // Test curl_multi_waitfds
    struct curl_waitfd *ufds = NULL;
    if (Size > 2) {
        size_t alloc_size = (size_t)Data[2] * sizeof(struct curl_waitfd);
        // Limit allocation to prevent OOM
        if (alloc_size > 0 && alloc_size <= 1024 * sizeof(struct curl_waitfd)) {
            ufds = malloc(alloc_size);
            if (ufds) {
                memset(ufds, 0, alloc_size);
                res = curl_multi_waitfds(multi_handle, ufds, (unsigned int)Data[2], &fd_count);
                (void)res;
                free(ufds);
            }
        }
    } else {
        // Query count only
        res = curl_multi_waitfds(multi_handle, NULL, 0, &fd_count);
        (void)res;
    }

    // Test curl_multi_remove_handle
    if (easy_handle) {
        res = curl_multi_remove_handle(multi_handle, easy_handle);
        (void)res;
    }

    // Test curl_multi_strerror with various codes
    error_str = curl_multi_strerror(error_code);
    if (error_str) {
        // Just ensure it doesn't crash
        (void)error_str;
    }

    // Cleanup
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}