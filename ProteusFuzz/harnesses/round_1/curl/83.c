// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
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

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets:
 * - curl_multi_strerror
 * - curl_multi_setopt
 * - curl_multi_wakeup
 */

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * 1. Fuzz curl_multi_strerror
     * We interpret the input data as a stream of CURLMcode values.
     * Since CURLMcode is an enum, we cast the input bytes to it.
     * We also test known values and random values.
     */
    const char *str;
    
    /* Test with data from input */
    if (Size >= sizeof(CURLMcode)) {
        CURLMcode code = *(const CURLMcode *)Data;
        str = curl_multi_strerror(code);
        (void)str; /* Use the result to avoid unused variable warning */
    }

    /* Test with known valid codes */
    str = curl_multi_strerror(CURLM_OK);
    (void)str;
    str = curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)str;
    str = curl_multi_strerror(CURLM_OUT_OF_MEMORY);
    (void)str;
    str = curl_multi_strerror(CURLM_INTERNAL_ERROR);
    (void)str;
    str = curl_multi_strerror(CURLM_BAD_EASY_HANDLE);
    (void)str;
    str = curl_multi_strerror(CURLM_BAD_SOCKET);
    (void)str;
    str = curl_multi_strerror(CURLM_UNKNOWN_OPTION);
    (void)str;
    str = curl_multi_strerror(CURLM_ADDED_ALREADY);
    (void)str;
    /* CURLM_RECURSIVE is not a standard CURLMcode in all versions, skipping to avoid compile error */
    str = curl_multi_strerror(CURLM_WAKEUP_FAILURE);
    (void)str;

    /* Test with arbitrary large values to check for undefined behavior handling */
    CURLMcode large_code = 999999;
    str = curl_multi_strerror(large_code);
    (void)str;

    /*
     * 2. Fuzz curl_multi_setopt
     * We try to set various options using data from the input.
     * We focus on options that take different types of arguments.
     */
    CURLMcode ret;

    /* Option: CURLMOPT_MAXCONNECTS (long) */
    if (Size >= sizeof(long)) {
        long max_conns = *(const long *)Data;
        ret = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conns);
        (void)ret;
    }

    /* Option: CURLMOPT_MAX_HOST_CONNECTIONS (long) */
    if (Size >= sizeof(long)) {
        long max_host_conns = *(const long *)Data;
        ret = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conns);
        (void)ret;
    }

    /* Option: CURLMOPT_PIPELINING (long) */
    if (Size >= sizeof(long)) {
        long pipelining = *(const long *)Data;
        ret = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
        (void)ret;
    }

    /* Option: CURLMOPT_PUSHFUNCTION (callback) */
    /* We pass NULL to test the default behavior (deny all pushes) */
    ret = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    (void)ret;

    /* Option: CURLMOPT_SOCKETFUNCTION (callback) */
    ret = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
    (void)ret;

    /* Option: CURLMOPT_TIMERFUNCTION (callback) */
    ret = curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
    (void)ret;

    /* Option: CURLMOPT_SOCKETDATA (void*) */
    if (Size > 0) {
        void *socket_data = (void *)Data;
        ret = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, socket_data);
        (void)ret;
    }

    /* Option: CURLMOPT_TIMERDATA (void*) */
    if (Size > 0) {
        void *timer_data = (void *)Data;
        ret = curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, timer_data);
        (void)ret;
    }

    /* Option: CURLMOPT_PUSHDATA (void*) */
    if (Size > 0) {
        void *push_data = (void *)Data;
        ret = curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, push_data);
        (void)ret;
    }

    /* Option: CURLMOPT_MAXCONNECTS with invalid value (negative) */
    long neg_val = -1;
    ret = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, neg_val);
    (void)ret;

    /*
     * 3. Fuzz curl_multi_wakeup
     * This function is designed to wake up a sleeping curl_multi_poll.
     * We call it to ensure it handles the multi handle correctly.
     */
    ret = curl_multi_wakeup(multi_handle);
    (void)ret;

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}