// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 
 * Static helper to create a dummy easy handle with minimal setup.
 * We use a dummy file or URL to ensure the handle is valid for multi operations.
 */
static CURL *create_dummy_easy_handle(const uint8_t *data, size_t size) {
    CURL *handle = curl_easy_init();
    if (!handle) {
        return NULL;
    }

    /* Set a dummy URL to avoid immediate failures in some contexts, 
       though for multi_socket_all we might not strictly need a valid URL 
       if we are just testing handle management. */
    if (size > 0) {
        /* Use the fuzz data as a potential URL string, ensuring null termination */
        char *url_buf = malloc(size + 1);
        if (url_buf) {
            memcpy(url_buf, data, size);
            url_buf[size] = '\0';
            /* Try to set URL, ignore errors as data might be invalid */
            curl_easy_setopt(handle, CURLOPT_URL, url_buf);
            free(url_buf);
        }
    } else {
        /* Fallback to a safe dummy URL if no data provided */
        curl_easy_setopt(handle, CURLOPT_URL, "http://example.com/dummy");
    }

    /* Disable actual network transfer to speed up fuzzing and avoid hangs */
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT_MS, 100L);
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 0L);
    
    return handle;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    /* Initialize global curl environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Initialize multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create one or more easy handles based on input size to explore states */
    CURL *easy_handle = NULL;
    int num_handles = (Size > 0) ? (Size % 5) + 1 : 1;
    CURL *handles[5];
    int handle_count = 0;

    for (int i = 0; i < num_handles; i++) {
        easy_handle = create_dummy_easy_handle(Data, Size);
        if (easy_handle) {
            handles[handle_count++] = easy_handle;
            CURLMcode res = curl_multi_add_handle(multi_handle, easy_handle);
            (void)res; /* We expect potential errors if URL is invalid, which is fine for fuzzing */
        }
    }

    int running_handles = 0;
    curl_socket_t sockfd = -1;
    void *sockp = (void *)(intptr_t)(Size % 1000); /* Arbitrary pointer based on size */

    /* 1. Test curl_multi_perform */
    /* This processes pending I/O */
    curl_multi_perform(multi_handle, &running_handles);

    /* 2. Test curl_multi_socket_all (Deprecated but target API) */
    /* Processes all sockets. Note: might return CURLM_RECURSIVE_API_CALL if called inside callbacks,
       but here we call it directly. */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* 3. Test curl_multi_assign */
    /* Link a socket to an app pointer. We use -1 as a dummy socket or a valid one if available.
       Since we don't have real sockets, we pass -1 or a small integer derived from data. */
    if (Size > 0) {
        sockfd = (curl_socket_t)(Data[0] & 0x7F);
    }
    curl_multi_assign(multi_handle, sockfd, sockp);

    /* 4. Test curl_multi_remove_handle */
    /* Remove handles one by one to test cleanup logic */
    for (int i = 0; i < handle_count; i++) {
        if (handles[i]) {
            curl_multi_remove_handle(multi_handle, handles[i]);
            curl_easy_cleanup(handles[i]);
            handles[i] = NULL;
        }
    }

    /* 5. Test curl_multi_strerror */
    /* Test with various error codes derived from input data */
    if (Size > 0) {
        CURLMcode error_code = (CURLMcode)(Data[0] % 20); /* Limit to reasonable range */
        const char *str = curl_multi_strerror(error_code);
        (void)str; /* Use the string to avoid unused variable warning */
    }
    
    /* Test with a specific known error code */
    curl_multi_strerror(CURLM_OK);
    curl_multi_strerror(CURLM_BAD_HANDLE);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global curl environment */
    curl_global_cleanup();

    return 0;
}