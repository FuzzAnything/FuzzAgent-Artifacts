// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_handles(CURLM *multi, CURL *easy) {
    if (multi) {
        if (easy) {
            curl_multi_remove_handle(multi, easy);
        }
        curl_multi_cleanup(multi);
    }
    if (easy) {
        curl_easy_cleanup(easy);
    }
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 7) {
        return 0;
    }

    CURLM *multi = NULL;
    CURL *easy = NULL;
    CURLMcode ret = CURLM_OK;

    // Initialize multi handle
    multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    // Initialize easy handle
    easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Fuzz curl_multi_strerror with various error codes
    // Use input data to generate error codes
    CURLMcode error_code = (CURLMcode)(Data[0] | (Data[1] << 8));
    const char *str = curl_multi_strerror(error_code);
    if (str) {
        (void)strlen(str);
    }

    // Fuzz curl_multi_assign
    // Use input data for socket fd and custom pointer
    curl_socket_t sockfd = (curl_socket_t)(Data[2] | (Data[3] << 8));
    void *sockp = (void *)(uintptr_t)(Data[4] | (Data[5] << 8));
    ret = curl_multi_assign(multi, sockfd, sockp);
    (void)ret;

    // Add easy handle to multi handle for other tests
    ret = curl_multi_add_handle(multi, easy);
    if (ret == CURLM_OK) {
        // Fuzz curl_multi_remove_handle
        ret = curl_multi_remove_handle(multi, easy);
        (void)ret;

        // Re-add for further tests
        ret = curl_multi_add_handle(multi, easy);
        if (ret == CURLM_OK) {
            // Fuzz curl_multi_timeout
            long milliseconds = 0;
            ret = curl_multi_timeout(multi, &milliseconds);
            (void)ret;

            // Fuzz curl_multi_get_offt
            curl_off_t value = 0;
            // Limit info_type to a safe range to avoid undefined behavior
            CURLMinfo_offt info_type = (CURLMinfo_offt)(Data[6] % 10);
            ret = curl_multi_get_offt(multi, info_type, &value);
            (void)ret;

            // Fuzz curl_multi_wakeup
            ret = curl_multi_wakeup(multi);
            (void)ret;
        }
    }

    // Cleanup
    cleanup_handles(multi, easy);

    return 0;
}