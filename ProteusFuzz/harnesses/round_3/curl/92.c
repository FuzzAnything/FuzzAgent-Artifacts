// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Fuzz driver for curl_multi_perform, curl_multi_socket, and curl_multi_socket_all.
 * The Wcurl_* macros are compile-time checks and do not generate runtime code.
 */

static CURLM *multi_handle = NULL;
static CURL *easy_handle = NULL;
static int running_handles = 0;
static curl_socket_t socket_fd = CURL_SOCKET_BAD;

/* Simple read callback for easy handle setup */
static size_t dummy_read_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

/* Simple write callback for easy handle setup */
static size_t dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)userp;
    size_t n = size * nmemb;
    return n;
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    /* 1. Prepare the environment */
    if (Size < 1) {
        return 0;
    }

    /* Initialize curl global if needed (thread-safe check) */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle if not exists */
    if (multi_handle == NULL) {
        multi_handle = curl_multi_init();
        if (multi_handle == NULL) {
            curl_global_cleanup();
            return 0;
        }
    }

    /* Create easy handle if not exists */
    if (easy_handle == NULL) {
        easy_handle = curl_easy_init();
        if (easy_handle == NULL) {
            curl_multi_cleanup(multi_handle);
            multi_handle = NULL;
            curl_global_cleanup();
            return 0;
        }

        /* Configure easy handle to avoid immediate network errors or infinite loops */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://localhost:1");
        curl_easy_setopt(easy_handle, CURLOPT_READFUNCTION, dummy_read_callback);
        curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, dummy_write_callback);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 100L);
        curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);
        
        /* Add to multi handle */
        if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
            curl_easy_cleanup(easy_handle);
            easy_handle = NULL;
        }
    }

    /* 2. Conduct fuzzing logic */
    /* Use input data to drive decisions */
    uint8_t op = Data[0] % 5;

    switch (op) {
        case 0:
            /* Test curl_multi_perform */
            /* Reset running_handles to capture state */
            running_handles = 0;
            curl_multi_perform(multi_handle, &running_handles);
            break;

        case 1:
            /* Test curl_multi_socket_all (deprecated but valid for fuzzing) */
            running_handles = 0;
            curl_multi_socket_all(multi_handle, &running_handles);
            break;

        case 2:
            /* Test curl_multi_socket with a specific socket */
            /* Derive a socket from input or use CURL_SOCKET_BAD */
            if (Size > 1) {
                socket_fd = (curl_socket_t)(Data[1] % 1000);
            } else {
                socket_fd = CURL_SOCKET_BAD;
            }
            running_handles = 0;
            curl_multi_socket(multi_handle, socket_fd, &running_handles);
            break;

        case 3:
            /* Test with invalid handle to trigger error paths */
            {
                CURLM *bad_handle = NULL;
                running_handles = 0;
                curl_multi_perform(bad_handle, &running_handles);
            }
            break;

        case 4:
            /* Test with NULL running_handles pointer */
            curl_multi_perform(multi_handle, NULL);
            break;
    }

    /* 3. Cleanup and state exploration */
    /* 
     * We do not clean up handles immediately to allow state accumulation 
     * across inputs, which helps in reaching complex states. 
     */

    return 0;
}