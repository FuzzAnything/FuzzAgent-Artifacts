// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_assign at multi.c:3653:11 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/* 
 * Define the callback type if not already defined by the headers.
 * curl_pushheaders_t is typically defined in curl/multi.h as struct curl_pushheaders.
 * If the header is missing or incomplete in the build environment, we define it here.
 */
#ifndef CURL_PUSHFUNCTION_DEFINED
typedef struct curl_pushheaders {
    struct curl_pushheaders *next;
    char *line;
} curl_pushheaders_t;
#endif

static int push_callback(CURL *easy, curl_pushheaders_t *headers, void *userp) {
    (void)easy;
    (void)headers;
    (void)userp;
    return CURL_PUSH_DENY;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
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

    // Extract values from input data
    int option_selector = Data[0] % 3;
    int socket_fd = (int)(Data[1] | (Data[2] << 8));
    int error_code = (int)(Data[3] | (Data[4] << 8) | (Data[5] << 16) | (Data[6] << 24));
    long long_value = (long)(Data[7] | (Data[8] << 8) | (Data[9] << 16) | (Data[10] << 24));

    // Test curl_multi_setopt with various options
    if (option_selector == 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, push_callback);
    } else if (option_selector == 1) {
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, long_value);
    } else {
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, 1);
    }

    // Test curl_multi_strerror
    const char *error_str = curl_multi_strerror(error_code);
    if (error_str) {
        (void)strlen(error_str); // Use the string to avoid unused warning
    }

    // Test curl_multi_assign
    curl_multi_assign(multi_handle, socket_fd, (void *)(intptr_t)socket_fd);

    // Add easy handle to multi handle for testing curl_multi_remove_handle
    curl_multi_add_handle(multi_handle, easy_handle);
    curl_multi_remove_handle(multi_handle, easy_handle);

    // Test curl_multi_wakeup
    curl_multi_wakeup(multi_handle);

    // Cleanup
    curl_easy_cleanup(easy_handle);
    curl_multi_cleanup(multi_handle);

    return 0;
}