// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_fdset at multi.c:1213:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/select.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* Dummy callback to satisfy type checking if needed */
static int dummy_socket_callback(CURLM *multi, curl_socket_t s, int what, void *userp, void *socketp) {
    (void)multi; (void)s; (void)what; (void)userp; (void)socketp;
    return 0;
}

static int dummy_timer_callback(CURLM *multi, long timeout_ms, void *userp) {
    (void)multi; (void)timeout_ms; (void)userp;
    return 0;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        return 0;
    }

    /* 1. Prepare environment for curl_multi_strerror */
    /* We can derive error codes from the input data to explore various states */
    CURLMcode error_code = (CURLMcode)(Data[0] & 0xFF);
    
    /* Fuzz curl_multi_strerror with various error codes */
    const char *str = curl_multi_strerror(error_code);
    (void)str; /* Suppress unused variable warning, ensure it's called */

    /* 2. Prepare environment for curl_multi functions */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_multi_wakeup with valid handle */
    CURLMcode wakeup_res = curl_multi_wakeup(multi_handle);
    (void)wakeup_res;

    /* Fuzz curl_multi_wakeup with invalid handle (NULL) to test error handling */
    curl_multi_wakeup(NULL);

    /* 3. Prepare environment for curl_multi_fdset */
    fd_set read_fd_set, write_fd_set, exc_fd_set;
    int max_fd = 0;

    FD_ZERO(&read_fd_set);
    FD_ZERO(&write_fd_set);
    FD_ZERO(&exc_fd_set);

    /* Fuzz curl_multi_fdset with valid handle */
    CURLMcode fdset_res = curl_multi_fdset(multi_handle, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);
    (void)fdset_res;

    /* Fuzz curl_multi_fdset with NULL handle */
    curl_multi_fdset(NULL, &read_fd_set, &write_fd_set, &exc_fd_set, &max_fd);

    /* Fuzz curl_multi_fdset with NULL fd_sets (avoid passing NULL max_fd as it causes dereference) */
    curl_multi_fdset(multi_handle, NULL, NULL, NULL, &max_fd);

    /* 4. Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}