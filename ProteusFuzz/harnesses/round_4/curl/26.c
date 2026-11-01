// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
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

    // Use input data to set a URL if possible, ensuring null termination
    char url_buf[256];
    size_t url_len = (Size < 255) ? Size : 255;
    memcpy(url_buf, Data, url_len);
    url_buf[url_len] = '\0';

    // Set a basic URL to make the handle valid for multi operations
    curl_easy_setopt(easy_handle, CURLOPT_URL, url_buf);
    curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L); // Make it a HEAD request to avoid large data
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 1L);

    CURLMcode mcode = curl_multi_add_handle(multi_handle, easy_handle);
    if (mcode != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t sock = CURL_SOCKET_TIMEOUT;
    int ev_bitmask = 0;

    // Extract parameters from fuzz input
    uint8_t b0 = Data[0];
    uint8_t b1 = Data[1];

    // Fuzz curl_multi_socket_all
    // Note: Deprecated, but still callable
    mcode = curl_multi_socket_all(multi_handle, &running_handles);
    (void)mcode;

    // Fuzz curl_multi_socket
    // Use a socket derived from input or CURL_SOCKET_TIMEOUT
    if (b0 & 0x01) {
        sock = CURL_SOCKET_TIMEOUT;
    } else {
        // Try to use a potentially invalid socket to test error handling
        sock = (curl_socket_t)(b1);
    }
    mcode = curl_multi_socket(multi_handle, sock, &running_handles);
    (void)mcode;

    // Fuzz curl_multi_socket_action
    // Vary the event bitmask
    ev_bitmask = b1;
    if (b0 & 0x02) {
        sock = CURL_SOCKET_TIMEOUT;
    } else {
        sock = (curl_socket_t)(b0);
    }
    mcode = curl_multi_socket_action(multi_handle, sock, ev_bitmask, &running_handles);
    (void)mcode;

    // Fuzz curl_multi_perform
    mcode = curl_multi_perform(multi_handle, &running_handles);
    (void)mcode;

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}