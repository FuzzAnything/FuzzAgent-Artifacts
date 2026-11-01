// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

/* 
 * Fuzz driver for curl_multi_wakeup, curl_multi_setopt, and curl_multi_strerror.
 * Note: The Wcurl_* macros are compile-time type checkers and do not generate
 * runtime code. They are included via curl/curl.h implicitly.
 */

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
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
     * Test curl_multi_setopt with various options derived from input data.
     * We use the input data to select options and values to explore different states.
     * Only valid CURLMOPT_* constants are used to avoid compilation errors.
     */
    if (Size > 0) {
        /* Fuzz CURLMOPT_PIPELINING (takes long) */
        long pipelining = (long)(Data[0] & 0xFF);
        (void)curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);

        /* Fuzz CURLMOPT_MAXCONNECTS (takes long) */
        if (Size > 1) {
            long max_connects = (long)(Data[1] & 0xFF);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_connects);
        }

        /* Fuzz CURLMOPT_MAX_HOST_CONNECTIONS (takes long) */
        if (Size > 2) {
            long max_host_conns = (long)(Data[2] & 0xFF);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conns);
        }

        /* Fuzz CURLMOPT_MAX_PIPELINE_LENGTH (takes long) */
        if (Size > 3) {
            long max_pipe_len = (long)(Data[3] & 0xFF);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, max_pipe_len);
        }

        /* Fuzz CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE (takes long) */
        if (Size > 4) {
            long penalty_size = (long)(Data[4] & 0xFF);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, penalty_size);
        }

        /* Fuzz CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE (takes long) */
        if (Size > 5) {
            long chunk_penalty = (long)(Data[5] & 0xFF);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, chunk_penalty);
        }

        /* Fuzz CURLMOPT_MAX_CONCURRENT_STREAMS (takes long) */
        if (Size > 6) {
            long max_streams = (long)(Data[6] & 0xFF);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_MAX_CONCURRENT_STREAMS, max_streams);
        }

        /* Fuzz CURLMOPT_PUSHFUNCTION (takes callback) - set to NULL to disable */
        (void)curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
        
        /* Fuzz CURLMOPT_PUSHDATA (takes void*) */
        (void)curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, (void*)Data);

        /* Fuzz CURLMOPT_SOCKETFUNCTION (takes callback) - set to NULL */
        (void)curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);

        /* Fuzz CURLMOPT_TIMERFUNCTION (takes callback) - set to NULL */
        (void)curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);

        /* Fuzz CURLMOPT_SOCKETDATA (takes void*) */
        (void)curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, (void*)Data);
    }

    /* 
     * Test curl_multi_wakeup.
     * This function is designed to wake up a sleeping curl_multi_poll call.
     * Calling it when not sleeping is safe but may return CURLM_WAKEUP_FAILURE or CURLM_OK.
     */
    CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
    (void)wakeup_result;

    /* 
     * Test curl_multi_strerror with various error codes.
     * We use the input data to generate a variety of error codes.
     */
    if (Size > 0) {
        CURLMcode error_code = (CURLMcode)(Data[0] & 0xFF);
        const char *str = curl_multi_strerror(error_code);
        (void)str; /* Use the string to avoid unused variable warning */
    }

    /* Test with a few specific known error codes */
    (void)curl_multi_strerror(CURLM_OK);
    (void)curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)curl_multi_strerror(CURLM_WAKEUP_FAILURE);

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}