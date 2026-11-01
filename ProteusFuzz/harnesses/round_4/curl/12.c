// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_get_offt at multi.c:3747:11 in multi.h
// curl_multi_timeout at multi.c:3394:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    CURLM *multi = NULL;
    CURL *easy = NULL;
    curl_socket_t sockfd = -1;
    void *sockp = NULL;
    curl_off_t value;
    long timeout_ms;
    CURLMcode res;
    const char *err_str;

    if (Size < 2) {
        return 0;
    }

    multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    // Prepare a safe, null-terminated string for URL
    // We copy the fuzz data into a buffer and ensure it is null-terminated
    // to prevent heap-buffer-overflow when curl_easy_setopt reads the string.
    char *safe_url = NULL;
    if (Size > 0) {
        safe_url = (char *)malloc(Size + 1);
        if (safe_url) {
            memcpy(safe_url, Data, Size);
            safe_url[Size] = '\0';
            
            // Limit URL length to avoid excessive memory usage or internal limits
            // though the main fix is the null termination.
            curl_easy_setopt(easy, CURLOPT_URL, safe_url);
            curl_easy_setopt(easy, CURLOPT_TIMEOUT, Data[0]);
            curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT, Data[1] & 0xFF);
        }
    }

    // Add easy handle to multi
    curl_multi_add_handle(multi, easy);

    // Fuzz curl_multi_assign
    // We use a dummy socket descriptor. In a real fuzzer, this might be a real socket.
    // Here we just pass a value derived from input to test internal logic.
    sockfd = (Size > 2) ? (curl_socket_t)(Data[2] & 0x7F) : 0;
    sockp = (Size > 3) ? (void *)(uintptr_t)(Data[3]) : NULL;
    res = curl_multi_assign(multi, sockfd, sockp);
    (void)res; // Consume result

    // Fuzz curl_multi_wakeup
    // This function is meant to be called from another thread, but we call it here to test error paths.
    res = curl_multi_wakeup(multi);
    (void)res;

    // Fuzz curl_multi_get_offt
    // We try different info types if we have enough data, otherwise default to 0
    CURLMinfo_offt info_type = (Size > 4) ? (CURLMinfo_offt)(Data[4] & 0x07) : 0;
    res = curl_multi_get_offt(multi, info_type, &value);
    (void)res;

    // Fuzz curl_multi_timeout
    res = curl_multi_timeout(multi, &timeout_ms);
    (void)res;

    // Fuzz curl_multi_remove_handle
    res = curl_multi_remove_handle(multi, easy);
    (void)res;

    // Fuzz curl_multi_strerror with various error codes
    // Generate error codes from input data
    for (size_t i = 0; i < Size && i < 10; i++) {
        CURLMcode test_code = (CURLMcode)(Data[i]);
        err_str = curl_multi_strerror(test_code);
        (void)err_str; // Consume result
    }

    // Cleanup allocated memory
    if (safe_url) {
        free(safe_url);
    }

    // Cleanup curl handles
    cleanup_handles(multi, easy);

    return 0;
}