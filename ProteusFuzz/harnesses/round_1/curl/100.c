// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize curl global environment
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    // Create a multi handle
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    // Create an easy handle
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Add easy handle to multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    // Fuzz curl_multi_strerror with various CURLMcode values
    // Use input data to generate error codes
    for (size_t i = 0; i < Size; i++) {
        CURLMcode error_code = (CURLMcode)Data[i];
        const char *error_str = curl_multi_strerror(error_code);
        (void)error_str; // Use the result to avoid unused variable warning
    }

    // Fuzz curl_multi_setopt with various options
    // Use input data to select options, ensuring bounds check
    if (Size >= 2) {
        CURLMoption option = (CURLMoption)(Data[0] % 100); // Limit to a reasonable range
        long long_value = (long)Data[1];
        curl_multi_setopt(multi_handle, option, long_value);
    }

    // Fuzz curl_multi_remove_handle
    if (Size >= 3) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // Fuzz curl_multi_wakeup
    if (Size >= 4) {
        curl_multi_wakeup(multi_handle);
    }

    // Fuzz curl_multi_assign
    // Ensure we have enough data before accessing Data[4] and Data[5]
    if (Size >= 6) {
        curl_socket_t sockfd = (curl_socket_t)Data[4];
        void *sockp = (void *)(uintptr_t)Data[5];
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}