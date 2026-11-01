// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* 
 * Fuzz driver for curl multi-handle functions.
 * Targets: curl_multi_strerror, curl_multi_setopt, curl_multi_wakeup.
 * Note: Wcurl_* macros are compile-time warnings and do not require runtime invocation.
 */

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        return 0;
    }

    /* 
     * 1. Fuzz curl_multi_strerror 
     * We interpret the input data as a stream of CURLMcode values to test error string generation.
     */
    for (size_t i = 0; i < Size; i++) {
        CURLMcode code = (CURLMcode)Data[i];
        const char *str = curl_multi_strerror(code);
        (void)str; /* Suppress unused variable warning */
    }

    /* 
     * 2. Fuzz curl_multi_setopt 
     * We attempt to set various options. We use the input data to derive option codes and values.
     * Note: Many options require specific types (callbacks, strings, etc.). 
     * We will try to pass raw data pointers where possible, or use NULL/0 for others.
     */
    for (size_t i = 0; i < Size; i++) {
        CURLMoption option = (CURLMoption)Data[i];
        
        /* 
         * We try to set the option with different types of values derived from the input.
         * Since we cannot easily construct valid callbacks or complex structs from raw bytes,
         * we pass NULL or 0 for most options, and raw pointers for those expecting pointers.
         * This may trigger internal error handling in curl_multi_setopt which is the goal.
         */
        
        /* Try setting as a long (0 or 1) */
        curl_multi_setopt(multi_handle, option, 0);
        curl_multi_setopt(multi_handle, option, 1);
        
        /* Try setting as a void pointer (NULL or raw data) */
        curl_multi_setopt(multi_handle, option, NULL);
        if (i + 1 < Size) {
            /* Pass a pointer to the next byte as a void* */
            curl_multi_setopt(multi_handle, option, (void *)&Data[i + 1]);
        }

        /* Try setting as a curl_off_t (0 or 1) */
        curl_multi_setopt(multi_handle, option, (curl_off_t)0);
        curl_multi_setopt(multi_handle, option, (curl_off_t)1);
    }

    /* 
     * 3. Fuzz curl_multi_wakeup 
     * Call wakeup on the handle. This is a simple function that doesn't take extra arguments.
     * We call it multiple times to explore state changes.
     */
    for (size_t i = 0; i < Size; i++) {
        curl_multi_wakeup(multi_handle);
    }

    /* Cleanup */
    curl_multi_cleanup(multi_handle);

    return 0;
}