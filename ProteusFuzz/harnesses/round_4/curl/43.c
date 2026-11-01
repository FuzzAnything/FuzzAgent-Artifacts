// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_perform at multi.c:2829:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
/* Do not include typecheck-gcc.h directly as it causes macro conflicts in some environments.
   The type checking macros are typically included via curl/curl.h or curl/multi.h internally.
   If specific typecheck macros are needed, they are usually handled by the compiler's
   built-in checks or the library's own headers without explicit inclusion of the raw macro file. */

/* Dummy callback to satisfy type checking if needed */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

/* Helper to safely read integers from fuzz input */
static int read_int(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/* Helper to safely read socket_t from fuzz input */
static curl_socket_t read_socket(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(curl_socket_t) > size) {
        return CURL_SOCKET_BAD;
    }
    curl_socket_t val;
    memcpy(&val, data + *offset, sizeof(curl_socket_t));
    *offset += sizeof(curl_socket_t);
    return val;
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Set a dummy URL to avoid immediate errors, though network won't be used */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_NOPROGRESS, 1L);
        curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT, 0L);
        
        /* Add to multi handle */
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    int running_handles = 0;
    size_t offset = 0;

    /* Fuzz curl_multi_perform */
    if (offset < Size) {
        /* Perform once */
        curl_multi_perform(multi_handle, &running_handles);
    }

    /* Fuzz curl_multi_socket_all */
    if (offset < Size) {
        /* Perform once */
        curl_multi_socket_all(multi_handle, &running_handles);
    }

    /* Fuzz curl_multi_socket */
    if (offset < Size) {
        curl_socket_t s = read_socket(Data, Size, &offset);
        curl_multi_socket(multi_handle, s, &running_handles);
    }

    /* Fuzz curl_multi_socket_action with various event masks */
    if (offset < Size) {
        curl_socket_t s = read_socket(Data, Size, &offset);
        int ev_bitmask = read_int(Data, Size, &offset);
        curl_multi_socket_action(multi_handle, s, ev_bitmask, &running_handles);
    }

    /* Try to trigger more states by calling with invalid socket or mask */
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, 0, &running_handles);
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, CURL_CSELECT_IN, &running_handles);
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, CURL_CSELECT_OUT, &running_handles);
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, CURL_CSELECT_ERR, &running_handles);
    curl_multi_socket_action(multi_handle, CURL_SOCKET_BAD, CURL_CSELECT_IN | CURL_CSELECT_OUT, &running_handles);

    /* Cleanup easy handle if it was added */
    if (easy_handle) {
        curl_multi_remove_handle(multi_handle, easy_handle);
        curl_easy_cleanup(easy_handle);
    }

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global */
    curl_global_cleanup();

    return 0;
}