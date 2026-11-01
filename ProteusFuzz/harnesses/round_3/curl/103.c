// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl multi-handle functions */

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Initialize curl global environment */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create a multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create an easy handle to add to the multi handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    uint8_t byte = Data[0];
    int option_index = byte % 10; /* Simulate option selection */

    /* Test curl_multi_add_handle */
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    (void)add_result; /* Suppress unused variable warning */

    /* Test curl_multi_setopt with various options */
    /* We try a few safe options that take long or pointer arguments */
    if (option_index < 3) {
        long val = (long)(Data[0] << 8 | Data[1]);
        /* CURLMOPT_MAXCONNECTS is a valid long option */
        curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, val);
    } else if (option_index < 6) {
        /* Set a callback option if available, using NULL for simplicity */
        curl_multi_setopt(multi_handle, CURLMOPT_SOCKETFUNCTION, NULL);
    } else {
        /* Try setting max pipelines or other valid long options */
        /* CURLMOPT_PIPELINING is a long option (0 or 1) */
        long pipeline = (long)Data[0] % 2;
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, pipeline);
    }

    /* Test curl_multi_wakeup */
    CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
    (void)wakeup_result;

    /* Test curl_multi_strerror with various error codes */
    /* We iterate through a range of error codes derived from input */
    for (size_t i = 0; i < Size && i < 20; i++) {
        CURLMcode error_code = (CURLMcode)(Data[i] % 100);
        const char *error_str = curl_multi_strerror(error_code);
        if (error_str) {
            /* Ensure the string is non-null and safe to read */
            (void)error_str[0];
        }
    }

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global curl environment */
    curl_global_cleanup();

    return 0;
}