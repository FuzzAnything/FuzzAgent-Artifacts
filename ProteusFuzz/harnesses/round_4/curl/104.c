// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void cleanup_multi_handle(CURLM *multi_handle) {
    if (multi_handle) {
        curl_multi_cleanup(multi_handle);
    }
}

static void cleanup_easy_handle(CURL *easy_handle) {
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        cleanup_multi_handle(multi_handle);
        return 0;
    }

    // Set a basic URL to avoid immediate failures, using dummy data if available
    const char *url = "http://example.com";
    if (Size > 20) {
        // Use part of the input as URL if it looks like a URL (simple heuristic)
        char url_buf[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(url_buf, Data, len);
        url_buf[len] = '\0';
        // Basic check to ensure it starts with http or https
        if (strncmp(url_buf, "http", 4) == 0) {
            url = url_buf;
        }
    }
    curl_easy_setopt(easy_handle, CURLOPT_URL, url);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);

    int running_handles = 0;
    CURLMcode res;

    // Add easy handle to multi handle
    res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        cleanup_easy_handle(easy_handle);
        cleanup_multi_handle(multi_handle);
        return 0;
    }

    // Fuzz curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res; // Ignore result for fuzzing stability

    // Fuzz curl_multi_socket_all (deprecated but valid for testing)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_wakeup
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    // Fuzz curl_multi_socket (deprecated)
    // We need a valid socket or -1. Since we don't have real sockets in fuzzer,
    // we pass -1 to trigger internal handling without crashing.
    curl_socket_t dummy_socket = -1;
    if (Size > 4) {
        dummy_socket = *(int*)(Data + 1);
    }
    res = curl_multi_socket(multi_handle, dummy_socket, &running_handles);
    (void)res;

    // Fuzz curl_multi_assign
    // Assign the dummy socket to a pointer derived from input
    void *sockp = NULL;
    if (Size > 8) {
        // Use a portion of input as a fake pointer (not dereferenced, just passed)
        sockp = (void*)(uintptr_t)(*(uint64_t*)(Data + 4));
    }
    res = curl_multi_assign(multi_handle, dummy_socket, sockp);
    (void)res;

    // Fuzz curl_multi_strerror with various error codes
    // Use input data to generate error codes
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLMcode code = (CURLMcode)(Data[i] + i);
        const char *str = curl_multi_strerror(code);
        (void)str; // Avoid unused warning
    }

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    cleanup_easy_handle(easy_handle);
    cleanup_multi_handle(multi_handle);

    return 0;
}