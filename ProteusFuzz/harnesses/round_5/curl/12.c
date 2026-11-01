// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_code = (CURLMcode)Data[0];
    const char *str = curl_multi_strerror(error_code);
    (void)str;

    // Fuzz curl_multi_assign
    curl_socket_t sockfd = (curl_socket_t)(Data[1] << 8 | Data[2]);
    void *sockp = (void *)(uintptr_t)(Data[3] << 24 | Data[4] << 16 | Data[5] << 8 | Data[6]);
    (void)curl_multi_assign(multi_handle, sockfd, sockp);

    // Fuzz curl_multi_remove_handle
    (void)curl_multi_remove_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_timeout
    long milliseconds = 0;
    (void)curl_multi_timeout(multi_handle, &milliseconds);

    // Fuzz curl_multi_get_offt
    curl_off_t value = 0;
    CURLMinfo_offt info = (CURLMinfo_offt)Data[7];
    (void)curl_multi_get_offt(multi_handle, info, &value);

    // Fuzz curl_multi_wakeup
    (void)curl_multi_wakeup(multi_handle);

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}