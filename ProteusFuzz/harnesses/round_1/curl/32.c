// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <unistd.h>
#include <sys/select.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets:
 * 1. curl_multi_strerror
 * 2. curl_multi_fdset
 * 3. curl_multi_remove_handle
 * 4. curl_multi_wakeup
 * 5. curl_multi_assign
 *
 * Note: Wcurl_easy_setopt_err_curl_mimepost is a compile-time warning macro
 * and cannot be fuzzed at runtime.
 */

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Initialize an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to ensure the handle is valid for multi operations */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");

    /* Add easy handle to multi handle */
    if (curl_multi_add_handle(multi_handle, easy_handle) != CURLM_OK) {
        curl_easy_cleanup(easy_handle);
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /*
     * Fuzzing Logic
     */

    /* 1. Fuzz curl_multi_strerror with various error codes */
    if (Size >= sizeof(CURLMcode)) {
        CURLMcode error_code = *(const CURLMcode *)(Data);
        const char *str = curl_multi_strerror(error_code);
        if (str) {
            /* Consume the string to avoid unused warnings and ensure coverage */
            (void)str[0];
        }
    }

    /* 2. Fuzz curl_multi_fdset */
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = -1;
    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);

    CURLMcode fdset_result = curl_multi_fdset(
        multi_handle,
        &read_fd_set,
        &write_fd_set,
        &exc_fd_set,
        &max_fd
    );
    (void)fdset_result;

    /* 3. Fuzz curl_multi_remove_handle */
    /* Remove the handle we added */
    CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_result;

    /* Try to remove again (should return CURLM_OK or similar) */
    remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_result;

    /* Try to remove an invalid handle (NULL) - should return CURLM_BAD_EASY_HANDLE */
    remove_result = curl_multi_remove_handle(multi_handle, NULL);
    (void)remove_result;

    /* 4. Fuzz curl_multi_wakeup */
    /* Call wakeup on a valid handle */
    CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
    (void)wakeup_result;

    /* 5. Fuzz curl_multi_assign */
    /* Assign a dummy socket (0) with a dummy pointer */
    curl_socket_t dummy_sock = 0;
    void *dummy_ptr = (void *)Data;
    CURLMcode assign_result = curl_multi_assign(multi_handle, dummy_sock, dummy_ptr);
    (void)assign_result;

    /* Cleanup */
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}