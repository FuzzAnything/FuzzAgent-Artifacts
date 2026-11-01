// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

/*
 * Fuzz driver for curl multi-handle functions.
 * Target APIs:
 * 1. curl_multi_strerror
 * 2. curl_multi_wakeup
 * 3. curl_multi_setopt
 *
 * Note: Wcurl_* macros are compile-time type checkers and not runtime functions.
 * They are included via headers to ensure type safety during compilation if
 * curl_easy_setopt or curl_multi_setopt macros are used directly, but here
 * we call the underlying functions directly to fuzz them.
 */

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
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

    /*
     * 1. Fuzz curl_multi_strerror
     * We iterate through the input data to generate various CURLMcode values.
     * Since CURLMcode is an enum, we cast integers from the input to it.
     */
    for (size_t i = 0; i < Size; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *str = curl_multi_strerror(code);
        (void)str; /* Suppress unused variable warning, just exercise the function */

        /* Also try with larger integer values derived from input if Size allows */
        if (i + 4 < Size) {
            uint32_t large_code = 0;
            memcpy(&large_code, &Data[i], 4);
            str = curl_multi_strerror((CURLMcode)large_code);
            (void)str;
        }
    }

    /*
     * 2. Fuzz curl_multi_wakeup
     * This function expects a valid CURLM handle. We call it with our handle.
     * We also try to call it with a NULL handle to test error handling.
     */
    curl_multi_wakeup(multi_handle);
    curl_multi_wakeup(NULL);

    /*
     * 3. Fuzz curl_multi_setopt
     * We try to set various options using values derived from the input.
     * We focus on options that take simple types (long, void*, char*) to avoid
     * complex struct initialization that might crash the fuzzer.
     */

    /* Option: CURLMOPT_MAXCONNECTS (takes long) */
    if (Size >= 4) {
        long max_connects = 0;
        memcpy(&max_connects, Data, 4);
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
    }

    /* Option: CURLMOPT_MAX_HOST_CONNECTIONS (takes long) */
    if (Size >= 8) {
        long max_host_conn = 0;
        memcpy(&max_host_conn, &Data[4], 4);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conn);
    }

    /* Option: CURLMOPT_PIPELINING (takes long) */
    if (Size >= 12) {
        long pipelining = 0;
        memcpy(&pipelining, &Data[8], 4);
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
    }

    /* Option: CURLMOPT_PUSHFUNCTION (takes curl_push_callback) - passing NULL or invalid ptr */
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    if (Size > 0) {
        /* Pass a pointer to the fuzz data as a callback (invalid, but tests robustness) */
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, (curl_push_callback)Data);
    }

    /* Option: CURLMOPT_PUSHDATA (takes void *) */
    if (Size > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void *)Data);
    }

    /* Option: CURLMOPT_SOCKETFUNCTION (takes curl_socket_callback) */
    curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
    if (Size > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, (curl_socket_callback)Data);
    }

    /* Option: CURLMOPT_SOCKETDATA (takes void *) */
    if (Size > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, (void *)Data);
    }

    /* Option: CURLMOPT_TIMERFUNCTION (takes curl_multi_timer_callback) */
    curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
    if (Size > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, (curl_multi_timer_callback)Data);
    }

    /* Option: CURLMOPT_TIMERDATA (takes void *) */
    if (Size > 0) {
        curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, (void *)Data);
    }

    /* Option: CURLMOPT_MAX_TOTAL_CONNECTIONS (takes long) */
    if (Size >= 16) {
        long max_total_conn = 0;
        memcpy(&max_total_conn, &Data[12], 4);
        curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_total_conn);
    }

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}