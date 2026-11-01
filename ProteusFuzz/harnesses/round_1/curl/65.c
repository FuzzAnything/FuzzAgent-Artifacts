// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
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

    // Set a simple URL to ensure the handle is valid for operations
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_CONNECTTIMEOUT, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);

    CURLMcode res = curl_multi_add_handle(multi_handle, easy_handle);
    if (res != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t s = CURL_SOCKET_BAD;
    int ev_bitmask = 0;

    // Parse input data to generate diverse states safely
    // Ensure we have enough data before reading
    if (Size >= 12) {
        uint32_t socket_val = *(const uint32_t *)(Data + 4);
        uint32_t event_val = *(const uint32_t *)(Data + 8);
        
        s = (curl_socket_t)(socket_val % 65535);
        ev_bitmask = (int)(event_val % 8);
    }

    // Fuzz curl_multi_socket_action
    res = curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
    (void)res;

    // Fuzz curl_multi_socket (deprecated)
    res = curl_multi_socket(multi_handle, s, &running_handles);
    (void)res;

    // Fuzz curl_multi_socket_all (deprecated)
    res = curl_multi_socket_all(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_perform
    res = curl_multi_perform(multi_handle, &running_handles);
    (void)res;

    // Fuzz curl_multi_assign
    // We assign the socket to a dummy pointer
    void *dummy_ptr = (void *)(uintptr_t)(s);
    res = curl_multi_assign(multi_handle, s, dummy_ptr);
    (void)res;

    // Cleanup
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}