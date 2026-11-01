// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
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

    // Add easy handle to multi handle to enable removal tests
    if (curl_multi_add_handle(multi_handle, easy_handle) == CURLM_OK) {
        // Test curl_multi_remove_handle
        curl_multi_remove_handle(multi_handle, easy_handle);
    }

    // Fuzz curl_multi_setopt with various options based on input
    if (Size > 0) {
        // Use CURLMOPT_MAXCONNECTS as a safe integer option
        long max_connects = (long)(Data[0] % 1000);
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
        
        // Try another safe option if available
        if (Size > 1) {
            long timeout = (long)(Data[1] % 10000);
            // Set timer function to NULL to clear previous settings
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
            // Set timer data
            curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, (void*)(uintptr_t)timeout);
        }
    }

    // Fuzz curl_multi_assign with socket and pointer
    if (Size > 2) {
        curl_socket_t sockfd = (curl_socket_t)(Data[2] % 65535);
        void *sockp = (void *)(uintptr_t)(Data[2] * 1000);
        curl_multi_assign(multi_handle, sockfd, sockp);
    }

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_strerror with various error codes
    if (Size > 3) {
        CURLMcode error_code = (CURLMcode)(Data[3] % 256);
        const char *error_msg = curl_multi_strerror(error_code);
        if (error_msg) {
            // Use the string to prevent compiler warnings about unused variable
            volatile const char *unused = error_msg;
            (void)unused;
        }
    }

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}