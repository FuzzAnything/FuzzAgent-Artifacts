// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
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

    // Set a dummy URL to make the handle valid for some operations
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");

    // Add the easy handle to the multi handle
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    // Fuzz curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Fuzz curl_multi_assign
    curl_socket_t dummy_socket = -1;
    void *dummy_ptr = (void *)(intptr_t)Data[0];
    curl_multi_assign(multi_handle, dummy_socket, dummy_ptr);

    // Fuzz curl_multi_remove_handle
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Fuzz curl_multi_setopt with valid options only to avoid typecheck macro issues
    // We use a known valid option: CURLMOPT_MAXCONNECTS
    long max_conns = (long)Data[0];
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conns);
    
    // Another valid option: CURLMOPT_PIPELINING
    if (Size > 1) {
        long pipelining = (long)Data[1];
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
    }

    // Fuzz curl_multi_strerror with various error codes
    if (Size > 2) {
        CURLMcode error_code = (CURLMcode)Data[2];
        const char *error_str = curl_multi_strerror(error_code);
        (void)error_str; // Use the result to avoid unused variable warning
    }

    // Cleanup
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }

    return 0;
}