// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
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
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_wakeup, curl_multi_setopt, curl_multi_strerror.
 */

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    curl_global_init(CURL_GLOBAL_ALL);

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * 1. Fuzz curl_multi_setopt
     * We iterate through the input data to generate various option values.
     */
    if (Size > 0) {
        /* Try setting CURLMOPT_MAXCONNECTS (takes long) */
        long max_conns = (long)Data[0];
        (void)curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, max_conns);

        /* Try setting CURLMOPT_PIPELINING (takes long) */
        if (Size > 1) {
            long pipelining = (long)Data[1];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipelining);
        }

        /* Try setting CURLMOPT_MAX_HOST_CONNECTIONS (takes long) */
        if (Size > 2) {
            long max_host_conns = (long)Data[2];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, max_host_conns);
        }

        /* Try setting CURLMOPT_MAX_PIPELINE_LENGTH (takes long) */
        if (Size > 3) {
            long max_pipe_len = (long)Data[3];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, max_pipe_len);
        }

        /* Try setting CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE (takes long) */
        if (Size > 4) {
            long penalty_size = (long)Data[4];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, penalty_size);
        }

        /* Try setting CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE (takes long) */
        if (Size > 5) {
            long chunk_penalty = (long)Data[5];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, chunk_penalty);
        }

        /* Try setting CURLMOPT_TIMERFUNCTION (takes curl_multi_timer_callback) */
        if (Size > 6) {
            void *cb_data = (void *)(uintptr_t)Data[6];
            /* Passing a non-function pointer as a callback to test internal checks */
            (void)curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, (curl_multi_timer_callback)Data);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_TIMERDATA, cb_data);
        }

        /* Try setting CURLMOPT_SOCKETFUNCTION (takes curl_socket_callback) */
        if (Size > 7) {
            void *sock_data = (void *)(uintptr_t)Data[7];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, (curl_socket_callback)Data);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_SOCKETDATA, sock_data);
        }

        /* Try setting CURLMOPT_PUSHFUNCTION (takes curl_push_callback) */
        if (Size > 8) {
            void *push_data = (void *)(uintptr_t)Data[8];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, (curl_push_callback)Data);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, push_data);
        }
        
        /* Try setting CURLMOPT_NOTIFYFUNCTION (takes curl_notify_callback) */
        if (Size > 9) {
            void *notify_data = (void *)(uintptr_t)Data[9];
            (void)curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYFUNCTION, (curl_notify_callback)Data);
            (void)curl_multi_setopt(multi_handle, CURLMOPT_NOTIFYDATA, notify_data);
        }
    }

    /*
     * 2. Fuzz curl_multi_wakeup
     */
    (void)curl_multi_wakeup(multi_handle);

    /*
     * 3. Fuzz curl_multi_strerror
     */
    if (Size > 10) {
        CURLMcode error_code = (CURLMcode)Data[10];
        const char *error_str = curl_multi_strerror(error_code);
        if (error_str) {
            (void)strlen(error_str);
        }
    }

    /* Test known error codes */
    (void)curl_multi_strerror(CURLM_OK);
    (void)curl_multi_strerror(CURLM_BAD_HANDLE);
    (void)curl_multi_strerror(CURLM_OUT_OF_MEMORY);
    (void)curl_multi_strerror(CURLM_INTERNAL_ERROR);
    (void)curl_multi_strerror(CURLM_BAD_EASY_HANDLE);
    (void)curl_multi_strerror(CURLM_BAD_FUNCTION_ARGUMENT);
    (void)curl_multi_strerror(CURLM_WAKEUP_FAILURE);

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}