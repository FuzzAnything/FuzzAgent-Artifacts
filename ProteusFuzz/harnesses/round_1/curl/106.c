// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
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

    // Add easy handle to multi handle to ensure valid state
    (void)curl_multi_add_handle(multi_handle, easy_handle);

    // Use input data to drive fuzzing logic
    unsigned int seed = Data[0];
    int socket_fd = (int)(Data[1] % 1000);
    void *sockp = (void *)(uintptr_t)(Data[2] % 10000);

    // Fuzz curl_multi_assign
    if (seed % 3 == 0) {
        curl_multi_assign(multi_handle, socket_fd, sockp);
    }

    // Fuzz curl_multi_strerror with various error codes
    CURLMcode error_code = (CURLMcode)(Data[3] % 20);
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        (void)strlen(error_str);
    }

    // Fuzz curl_multi_setopt with specific valid options to avoid typecheck macro issues
    // We only test options that are safe to call with variadic arguments in a fuzzing context
    // without triggering complex macro expansion errors related to type checking.
    if (Data[4] % 2 == 0) {
        long max_connects = (long)(Data[5] % 100);
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
    } else {
        long pipelining = (long)(Data[6] % 2);
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
    }

    // Fuzz curl_multi_wakeup
    if (seed % 5 == 0) {
        curl_multi_wakeup(multi_handle);
    }

    // Fuzz curl_multi_remove_handle
    if (seed % 4 == 0) {
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}