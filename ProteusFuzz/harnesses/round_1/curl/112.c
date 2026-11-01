// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
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

static int dummy_push_callback(CURL *curl, void *clientp, void *pushp) {
    (void)curl;
    (void)clientp;
    (void)pushp;
    return 1;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Extract parameters from input data safely
    int sockfd = (int)(Data[0] << 8 | Data[1]);
    void *sockp = (void *)(intptr_t)(Data[0] << 8 | Data[1]);
    CURLMcode error_code = (CURLMcode)(Data[0]);
    CURLMoption option = (CURLMoption)(Data[1]);

    // Test curl_multi_setopt with various options
    if (Size > 2) {
        if (option == CURLMOPT_PUSHFUNCTION) {
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, dummy_push_callback);
            curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void *)(intptr_t)Data[2]);
        } else if (option == CURLMOPT_MAXCONNECTS && Size > 3) {
            long max_connects = (long)(Data[2] << 8 | Data[3]);
            curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
        }
    }

    // Test curl_multi_assign
    curl_multi_assign(multi_handle, sockfd, sockp);

    // Test curl_multi_strerror with various error codes
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        (void)strlen(error_str);
    }

    // Test curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Create a dummy easy handle to test removal
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        curl_multi_add_handle(multi_handle, easy_handle);
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    // Cleanup
    curl_multi_cleanup(multi_handle);

    return 0;
}