// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
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

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi = curl_multi_init();
    if (!multi) {
        return 0;
    }

    CURL *easy = curl_easy_init();
    if (!easy) {
        curl_multi_cleanup(multi);
        return 0;
    }

    /* Initialize easy handle with a dummy URL to ensure valid state */
    curl_easy_setopt(easy, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy, CURLOPT_NOBODY, 1L);

    CURLMcode rc;
    int running_handles = 0;
    curl_socket_t sock = -1;
    void *sockp = NULL;

    /* Add easy handle to multi handle */
    rc = curl_multi_add_handle(multi, easy);
    if (rc != CURLM_OK) {
        curl_easy_cleanup(easy);
        curl_multi_cleanup(multi);
        return 0;
    }

    /* Fuzz curl_multi_strerror with various error codes derived from input */
    if (Size > 0) {
        CURLMcode err_code = (CURLMcode)Data[0];
        const char *str = curl_multi_strerror(err_code);
        (void)str; /* Use the result to avoid unused variable warnings */
    }

    /* Fuzz curl_multi_perform */
    rc = curl_multi_perform(multi, &running_handles);
    (void)rc;

    /* Fuzz curl_multi_socket_all (deprecated but valid for testing) */
    rc = curl_multi_socket_all(multi, &running_handles);
    (void)rc;

    /* Fuzz curl_multi_socket (deprecated but valid for testing) */
    /* We use -1 as an invalid socket to test error handling, or derive from input if size permits */
    curl_socket_t fuzz_sock = -1;
    if (Size > 1) {
        fuzz_sock = (curl_socket_t)Data[1];
    }
    rc = curl_multi_socket(multi, fuzz_sock, &running_handles);
    (void)rc;

    /* Fuzz curl_multi_assign */
    /* Create a dummy pointer from input if available */
    void *dummy_ptr = NULL;
    if (Size > 2) {
        /* Cast input to void* carefully, ensuring alignment is not an issue for the pointer itself */
        /* We just use the address of the input buffer as a dummy pointer */
        dummy_ptr = (void *)(uintptr_t)(Data[2]);
    }
    
    /* Try assigning with a valid socket (0) and the dummy pointer */
    rc = curl_multi_assign(multi, 0, dummy_ptr);
    (void)rc;

    /* Try assigning with fuzzed socket value */
    if (Size > 3) {
        curl_socket_t fuzz_sock_assign = (curl_socket_t)Data[3];
        rc = curl_multi_assign(multi, fuzz_sock_assign, dummy_ptr);
        (void)rc;
    }

    /* Cleanup */
    curl_multi_remove_handle(multi, easy);
    curl_easy_cleanup(easy);
    curl_multi_cleanup(multi);

    return 0;
}