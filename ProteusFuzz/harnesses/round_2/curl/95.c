// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_easy_cleanup(easy_handle);
        return 0;
    }

    // Fuzz curl_multi_strerror with various CURLMcode values
    // We use the input data to generate error codes
    CURLMcode error_code = (CURLMcode)(Data[0] & 0xFF);
    const char *error_str = curl_multi_strerror(error_code);
    (void)error_str; // Suppress unused variable warning

    // Fuzz curl_multi_setopt
    // We try to set various options using the input data
    // Ensure we have enough data before accessing Data[1] and Data[2]
    if (Size > 2) {
        CURLMoption option = (CURLMoption)(Data[1] & 0xFF);
        long long_value = (long)(Data[2] & 0xFF);
        curl_multi_setopt(multi_handle, option, long_value);
    }

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_assign
    // Ensure we have enough data before accessing Data[3] and Data[4]
    if (Size > 4) {
        curl_socket_t sockfd = (curl_socket_t)(Data[3] & 0xFF);
        void *sockp = (void *)(uintptr_t)(Data[4] & 0xFF);
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Add easy handle to multi handle
    curl_multi_add_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Cleanup
    curl_multi_cleanup(multi_handle);
    curl_easy_cleanup(easy_handle);

    return 0;
}