// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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
#include <curl/typecheck-gcc.h>

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi_handle = NULL;
    CURL *easy_handle = NULL;
    CURLMcode rc;
    int running_handles = 0;
    curl_socket_t socket_fd = -1;
    void *sockp = NULL;
    const char *error_str = NULL;

    // Initialize multi handle
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    // Initialize easy handle and add to multi handle
    easy_handle = curl_easy_init();
    if (easy_handle) {
        // Set a dummy URL to ensure the handle is somewhat valid
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        
        rc = curl_multi_add_handle(multi_handle, easy_handle);
        if (rc != CURLM_OK) {
            curl_easy_cleanup(easy_handle);
            curl_multi_cleanup(multi_handle);
            return 0;
        }
    }

    // Fuzz curl_multi_strerror
    // Use data to generate various CURLMcode values
    for (size_t i = 0; i < Size; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        error_str = curl_multi_strerror(code);
        // Ensure no crash occurs; error_str should be non-NULL
        (void)error_str;
    }

    // Fuzz curl_multi_perform
    // Call multiple times to explore states
    rc = curl_multi_perform(multi_handle, &running_handles);
    (void)rc;

    // Fuzz curl_multi_socket
    // Use data to generate socket_fd values
    if (Size > 1) {
        socket_fd = (curl_socket_t)(Data[0] | (Data[1] << 8));
        rc = curl_multi_socket(multi_handle, socket_fd, &running_handles);
        (void)rc;
    }

    // Fuzz curl_multi_socket_all
    rc = curl_multi_socket_all(multi_handle, &running_handles);
    (void)rc;

    // Fuzz curl_multi_assign
    // Use data to generate sockp pointer (cast from data address)
    // Ensure we have enough data before accessing indices 2 and 3
    if (Size > 3) {
        sockp = (void *)(uintptr_t)(Data[2] | (Data[3] << 8));
        rc = curl_multi_assign(multi_handle, socket_fd, sockp);
        (void)rc;
    }

    // Cleanup
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);

    return 0;
}