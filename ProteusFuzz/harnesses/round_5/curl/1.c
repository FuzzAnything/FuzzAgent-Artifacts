// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

static void cleanup_multi(CURLM *multi) {
    if (multi) {
        curl_multi_cleanup(multi);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        cleanup_multi(multi);
        return 0;
    }

    // Set a dummy URL to ensure the handle is valid for multi operations
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_PRIVATE, (void *)Data);
    curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);

    CURLMcode rc = curl_multi_add_handle(multi, easy);
    if (rc != CURLM_OK) {
        curl_easy_cleanup(easy);
        cleanup_multi(multi);
        return 0;
    }

    int running_handles = 0;
    curl_socket_t s = -1;
    int ev_bitmask = 0;

    // Fuzz curl_multi_socket_action
    if (Size > 0) {
        s = (curl_socket_t)Data[0];
    }
    if (Size > 1) {
        ev_bitmask = (int)Data[1];
    }
    rc = curl_multi_socket_action(multi, s, ev_bitmask, &running_handles);
    (void)curl_multi_strerror(rc);

    // Fuzz curl_multi_socket (deprecated)
    if (Size > 2) {
        s = (curl_socket_t)Data[2];
    }
    rc = curl_multi_socket(multi, s, &running_handles);
    (void)curl_multi_strerror(rc);

    // Fuzz curl_multi_socket_all (deprecated)
    rc = curl_multi_socket_all(multi, &running_handles);
    (void)curl_multi_strerror(rc);

    // Fuzz curl_multi_assign
    // Ensure we have enough data for sockfd (int) and sockp (pointer)
    // We use Data[3] for sockfd and construct a dummy pointer from Data[4] if available
    if (Size > 3) {
        s = (curl_socket_t)Data[3];
        void *sockp = NULL;
        if (Size > 4) {
            // Create a dummy pointer value from the byte, ensuring it's not NULL to test logic
            // Casting a single byte to a pointer is safe as a value, though not a valid address
            sockp = (void *)(uintptr_t)(Data[4] & 0xFF);
        }
        rc = curl_multi_assign(multi, s, sockp);
        (void)curl_multi_strerror(rc);
    }

    // Fuzz curl_multi_perform
    rc = curl_multi_perform(multi, &running_handles);
    (void)curl_multi_strerror(rc);

    // Test with invalid handle scenarios if possible (though multi is valid here)
    // Just ensure we call the functions with varied inputs from the buffer
    if (Size > 5) {
        int test_code = (int)Data[5];
        const char *err_str = curl_multi_strerror((CURLMcode)test_code);
        if (err_str) {
            (void)err_str; // Use the string to avoid unused warning
        }
    }

    // Cleanup
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    cleanup_multi(multi);

    return 0;
}