// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>
#include <unistd.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Target APIs:
 * 1. curl_multi_strerror
 * 2. curl_multi_wakeup
 * 3. curl_multi_setopt
 *
 * Note: Wcurl_* macros are compile-time checks and do not require runtime invocation.
 */

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char *content = "dummy content for fuzzing";
        fwrite(content, 1, strlen(content), f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
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

    /* Create an easy handle to add to the multi handle for more realistic state */
    CURL *easy_handle = curl_easy_init();
    if (easy_handle) {
        /* Set a simple URL to make the handle valid */
        curl_easy_setopt(easy_handle, CURLOPT_URL, "http://example.com");
        curl_easy_setopt(easy_handle, CURLOPT_NOBODY, 1L);
        curl_multi_add_handle(multi_handle, easy_handle);
    }

    /* Ensure dummy file exists for any potential file-based options */
    setup_dummy_file();

    /*
     * Fuzzing Strategy:
     * 1. Parse input data to generate error codes for curl_multi_strerror.
     * 2. Parse input data to generate options and values for curl_multi_setopt.
     * 3. Call curl_multi_wakeup to test wake-up logic.
     */

    /*
     * 1. Test curl_multi_strerror
     * We iterate through the input data, treating chunks as potential CURLMcode values.
     * We also test boundary conditions (0, -1, max int).
     */
    const char *error_msg;
    CURLMcode test_code;

    /* Test known good code */
    test_code = CURLM_OK;
    error_msg = curl_multi_strerror(test_code);
    (void)error_msg; /* Suppress unused variable warning */

    /* Test known bad code */
    test_code = CURLM_BAD_HANDLE;
    error_msg = curl_multi_strerror(test_code);
    (void)error_msg;

    /* Fuzz with input data */
    for (size_t i = 0; i < Size; i++) {
        /* Cast input byte to CURLMcode (usually int) */
        test_code = (CURLMcode)Data[i];
        error_msg = curl_multi_strerror(test_code);
        (void)error_msg;

        /* Also try larger values if Size permits */
        if (i + sizeof(CURLMcode) <= Size) {
            CURLMcode large_code;
            memcpy(&large_code, &Data[i], sizeof(CURLMcode));
            error_msg = curl_multi_strerror(large_code);
            (void)error_msg;
        }
    }

    /* Test with specific boundary values */
    test_code = 0;
    error_msg = curl_multi_strerror(test_code);
    (void)error_msg;

    test_code = -1;
    error_msg = curl_multi_strerror(test_code);
    (void)error_msg;

    test_code = (CURLMcode)0x7FFFFFFF;
    error_msg = curl_multi_strerror(test_code);
    (void)error_msg;

    test_code = (CURLMcode)0xFFFFFFFF;
    error_msg = curl_multi_strerror(test_code);
    (void)error_msg;

    /*
     * 2. Test curl_multi_setopt
     * We try to set various options using data from the input buffer.
     * We focus on options that accept pointers, longs, or callbacks.
     */
    CURLMcode res;

    /* Try to set CURLMOPT_MAXCONNECTS (expects long) */
    if (Size >= sizeof(long)) {
        long max_conn;
        memcpy(&max_conn, Data, sizeof(long));
        /* Clamp to reasonable range to avoid immediate crashes if libcurl validates strictly */
        if (max_conn < 0) max_conn = 0;
        if (max_conn > 10000) max_conn = 10000;
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conn);
        (void)res;
    }

    /* Try to set CURLMOPT_PIPELINING (expects long) */
    if (Size >= sizeof(long)) {
        long pipelining;
        memcpy(&pipelining, Data, sizeof(long));
        res = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
        (void)res;
    }

    /* Try to set CURLMOPT_PUSHFUNCTION (expects callback) */
    /* We cannot easily fuzz a function pointer directly from raw bytes without crashing,
       so we pass NULL or a dummy static function if we had one. Passing NULL is safe. */
    res = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    (void)res;

    /* Try to set CURLMOPT_MAX_HOST_CONNECTIONS (expects long) */
    if (Size >= sizeof(long)) {
        long max_host_conn;
        memcpy(&max_host_conn, Data, sizeof(long));
        if (max_host_conn < 0) max_host_conn = 0;
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conn);
        (void)res;
    }

    /* Try to set CURLMOPT_MAX_PIPELINE_LENGTH (expects long) */
    if (Size >= sizeof(long)) {
        long max_pipe_len;
        memcpy(&max_pipe_len, Data, sizeof(long));
        if (max_pipe_len < 0) max_pipe_len = 0;
        res = curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, max_pipe_len);
        (void)res;
    }

    /* Try to set CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE (expects curl_off_t) */
    if (Size >= sizeof(curl_off_t)) {
        curl_off_t penalty_size;
        memcpy(&penalty_size, Data, sizeof(curl_off_t));
        res = curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, penalty_size);
        (void)res;
    }

    /* Try to set CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE (expects curl_off_t) */
    if (Size >= sizeof(curl_off_t)) {
        curl_off_t chunk_penalty;
        memcpy(&chunk_penalty, Data, sizeof(curl_off_t));
        res = curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, chunk_penalty);
        (void)res;
    }

    /*
     * 3. Test curl_multi_wakeup
     * This function is designed to wake up a sleeping curl_multi_poll.
     * Calling it when not sleeping is safe and returns CURLM_OK or CURLM_WAKEUP_FAILURE.
     */
    res = curl_multi_wakeup(multi_handle);
    (void)res;

    /* Cleanup */
    if (easy_handle) {
        curl_easy_cleanup(easy_handle);
    }
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    /* Remove dummy file */
    unlink("./dummy_file");

    return 0;
}