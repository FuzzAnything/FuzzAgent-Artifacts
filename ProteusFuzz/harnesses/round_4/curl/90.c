// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
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
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_strerror, curl_multi_wakeup, curl_multi_setopt
 */

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    CURLM *multi_handle = NULL;
    CURLMcode res;
    const char *str;

    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    multi_handle = curl_multi_init();
    if (multi_handle == NULL) {
        curl_global_cleanup();
        return 0;
    }

    /* 
     * 1. Fuzz curl_multi_strerror
     * We interpret the first byte of input as a CURLMcode enum value.
     * This covers known errors and potentially undefined behavior for unknown codes.
     */
    if (Size > 0) {
        CURLMcode error_code = (CURLMcode)Data[0];
        str = curl_multi_strerror(error_code);
        /* Just ensure it doesn't crash; str is usually a static string */
        if (str != NULL) {
            /* Consume the string to avoid unused variable warnings in some builds */
            volatile size_t len = strlen(str);
            (void)len;
        }
    }

    /* 
     * 2. Fuzz curl_multi_setopt
     * We try to set various options using data from the input buffer.
     * We use a subset of options that are safe to set on a multi handle without
     * additional setup (like callbacks or complex structures).
     */
    if (Size > 1) {
        /* Use the second byte to select an option type roughly */
        unsigned char option_selector = Data[1];

        /* CURLMOPT_MAXCONNECTS: expects long */
        if (option_selector % 3 == 0) {
            long max_connects = 0;
            if (Size > 5) {
                max_connects = (long)(Data[2] | (Data[3] << 8) | (Data[4] << 16) | (Data[5] << 24));
            }
            res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
            (void)res;
        }
        /* CURLMOPT_PIPELINING: expects long (enable/disable) */
        else if (option_selector % 3 == 1) {
            long pipelining = (Size > 2) ? (long)Data[2] : 0;
            res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
            (void)res;
        }
        /* CURLMOPT_MAX_HOST_CONNECTIONS: expects long */
        else {
            long max_host_conn = 0;
            if (Size > 5) {
                max_host_conn = (long)(Data[2] | (Data[3] << 8) | (Data[4] << 16) | (Data[5] << 24));
            }
            res = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conn);
            (void)res;
        }
    }

    /* 
     * 3. Fuzz curl_multi_wakeup
     * This function is designed to wake up a sleeping curl_multi_poll.
     * Calling it on a valid handle should return CURLM_OK or CURLM_WAKEUP_FAILURE.
     * We call it unconditionally on the valid handle we created.
     */
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}