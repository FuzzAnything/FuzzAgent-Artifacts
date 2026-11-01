// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
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
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Suppress warnings for unused variables in fuzzing context */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-value"

/* Helper to safely extract integers from fuzz input */
static int32_t get_int32(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int32_t) > Size) {
        *offset = Size;
        return 0;
    }
    int32_t val;
    memcpy(&val, Data + *offset, sizeof(int32_t));
    *offset += sizeof(int32_t);
    return val;
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    size_t offset = 0;
    CURLM *multi_handle = NULL;
    CURLMcode ret;
    const char *str;

    /* Initialize libcurl if not already done */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle for testing */
    multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Test 1: curl_multi_strerror with various error codes */
    int32_t error_code = get_int32(Data, Size, &offset);
    str = curl_multi_strerror(error_code);
    (void)str; /* Use the return value to avoid unused warning */

    /* Test 2: curl_multi_wakeup with valid and NULL handles */
    /* First, try with the valid handle we created */
    ret = curl_multi_wakeup(multi_handle);
    (void)ret;

    /* Try with NULL handle */
    ret = curl_multi_wakeup(NULL);
    (void)ret;

    /* Note: We removed the garbage pointer test for curl_multi_wakeup
       because passing arbitrary pointers causes AddressSanitizer SEGV
       when curl tries to dereference them internally.
       Valid handles are only those returned by curl_multi_init().
    */

    /* Test 3: curl_multi_setopt with various options */
    /* Extract option and value from fuzz data */
    int32_t option = get_int32(Data, Size, &offset);
    
    /* Try setting different types of options based on option value */
    if (option == CURLMOPT_MAXCONNECTS) {
        int32_t max_connects = get_int32(Data, Size, &offset);
        ret = curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, (long)max_connects);
        (void)ret;
    } else if (option == CURLMOPT_PIPELINING) {
        int32_t pipelining = get_int32(Data, Size, &offset);
        ret = curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, (long)pipelining);
        (void)ret;
    } else if (option == CURLMOPT_PUSHFUNCTION) {
        /* Push function requires a callback, we'll pass NULL for fuzzing */
        ret = curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
        (void)ret;
    } else if (option == CURLMOPT_SOCKETFUNCTION) {
        /* Socket function requires a callback, we'll pass NULL for fuzzing */
        ret = curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
        (void)ret;
    } else if (option == CURLMOPT_TIMERFUNCTION) {
        /* Timer function requires a callback, we'll pass NULL for fuzzing */
        ret = curl_multi_setopt(multi_handle, CURLMOPT_TIMERFUNCTION, NULL);
        (void)ret;
    } else {
        /* Try setting with a long value for unknown options */
        int32_t long_val = get_int32(Data, Size, &offset);
        ret = curl_multi_setopt(multi_handle, (CURLMoption)option, (long)long_val);
        (void)ret;
    }

    /* Test with NULL handle for setopt */
    ret = curl_multi_setopt(NULL, (CURLMoption)option, 0);
    (void)ret;

    /* Cleanup */
    curl_multi_cleanup(multi_handle);
    curl_global_cleanup();

    return 0;
}

#pragma GCC diagnostic pop