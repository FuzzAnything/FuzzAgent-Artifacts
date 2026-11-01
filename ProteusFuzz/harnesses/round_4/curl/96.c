// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_socket_all at multi.c:3302:11 in multi.h
// curl_multi_socket_action at multi.c:3291:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl_multi_socket_all and curl_multi_socket.
 * Note: The Wcurl_* macros are compile-time type checks and cannot be
 * invoked at runtime. This driver focuses on the runtime APIs.
 */

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Set a dummy URL to ensure the handle is valid */
    curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
    curl_easy_setopt(easy_handle, CURLOPT_TIMEOUT_MS, 1L);
    curl_easy_setopt(easy_handle, CURLOPT_NOSIGNAL, 1L);

    /* Add easy handle to multi handle */
    curl_multi_add_handle(multi_handle, easy_handle);

    /* Prepare variables for running_handles */
    int running_handles = 1;

    /* Extract socket value from fuzz input if available */
    curl_socket_t socket_fd = CURL_SOCKET_BAD;
    if (Size >= sizeof(curl_socket_t)) {
        memcpy(&socket_fd, Data, sizeof(curl_socket_t));
        /* Ensure socket_fd is not CURL_SOCKET_BAD to test logic, 
           but keep it within reasonable bounds or use specific values */
        if (socket_fd == CURL_SOCKET_BAD) {
            socket_fd = 0; 
        }
    }

    /* Fuzz curl_multi_socket_all */
    /* This function processes all sockets in the multi-handle */
    curl_multi_socket_all(multi_handle, &running_handles);

    /* Fuzz curl_multi_socket with the extracted socket */
    /* This function processes I/O on a specific socket */
    curl_multi_socket(multi_handle, socket_fd, &running_handles);

    /* Additional state exploration: call with NULL running_handles pointer */
    /* Note: This might trigger internal assertions or errors, which is good for fuzzing */
    curl_multi_socket_all(multi_handle, NULL);
    curl_multi_socket(multi_handle, socket_fd, NULL);

    /* Cleanup */
    curl_multi_remove_handle(multi_handle, easy_handle);
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}