// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>

/*
 * Fuzz driver for curl multi-handle functions.
 * Targets:
 * 1. curl_multi_strerror
 * 2. curl_multi_setopt
 * 3. curl_multi_wakeup
 *
 * Note: We include <curl/curl.h> first to ensure all types and macros are defined
 * before including <curl/multi.h> which relies on them for type checking macros.
 */

/* Dummy callback for CURLMOPT_PUSHFUNCTION */
static int dummy_push_callback(CURL *easy_handle,
                               const struct curl_pushheaders *headers,
                               void *userptr) {
    (void)easy_handle;
    (void)headers;
    (void)userptr;
    return CURL_PUSH_DENY;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /*
     * 1. Fuzz curl_multi_strerror
     * We extract a CURLMcode from the input data.
     */
    CURLMcode error_code = (CURLMcode)Data[0];
    const char *str = curl_multi_strerror(error_code);
    (void)str;

    /*
     * 2. Fuzz curl_multi_setopt
     * We try to set various options.
     */
    if (Size > 1) {
        CURLMoption option = (CURLMoption)Data[1];

        switch (option) {
            case CURLMOPT_MAXCONNECTS:
            case CURLMOPT_MAX_HOST_CONNECTIONS:
            case CURLMOPT_PIPELINING:
            case CURLMOPT_MAX_PIPELINE_LENGTH:
            case CURLMOPT_MAX_CONCURRENT_STREAMS:
                /* Pass a long value derived from input */
                if (Size > 2) {
                    long val = (long)Data[2];
                    curl_multi_setopt(multi_handle, option, val);
                } else {
                    curl_multi_setopt(multi_handle, option, 0L);
                }
                break;
            case CURLMOPT_PUSHFUNCTION:
                /* Pass a callback function */
                curl_multi_setopt(multi_handle, option, dummy_push_callback);
                break;
            case CURLMOPT_SOCKETFUNCTION:
            case CURLMOPT_TIMERFUNCTION:
                /* Pass NULL to test error handling for missing callbacks */
                curl_multi_setopt(multi_handle, option, NULL);
                break;
            default:
                /*
                 * For unknown or unsupported options, pass a generic value.
                 * This triggers internal error handling in libcurl.
                 */
                curl_multi_setopt(multi_handle, option, 0L);
                break;
        }
    }

    /*
     * 3. Fuzz curl_multi_wakeup
     */
    curl_multi_wakeup(multi_handle);

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}