// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_init at easy.c:197:10 in curl.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_multi_strerror at strerror.c:326:13 in multi.h
// curl_multi_add_handle at multi.c:422:11 in multi.h
// curl_multi_remove_handle at multi.c:749:11 in multi.h
// curl_multi_wakeup at multi.c:1593:11 in multi.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
// Wcurl_multi_setopt_err_timercb at typecheck-gcc.h:291:1 in typecheck-gcc.h
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

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Initialize libcurl */
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        return 0;
    }

    /* Create multi handle */
    CURLM *multi_handle = curl_multi_init();
    if (!multi_handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Create easy handle */
    CURL *easy_handle = curl_easy_init();
    if (!easy_handle) {
        curl_multi_cleanup(multi_handle);
        curl_global_cleanup();
        return 0;
    }

    /* Fuzz curl_multi_strerror with various error codes */
    for (size_t i = 0; i < Size; i++) {
        CURLMcode error_code = (CURLMcode)(Data[i] + (i % 100));
        const char *msg = curl_multi_strerror(error_code);
        (void)msg; /* Suppress unused variable warning */
    }

    /* Fuzz curl_multi_add_handle */
    CURLMcode add_result = curl_multi_add_handle(multi_handle, easy_handle);
    (void)add_result;

    /* Fuzz curl_multi_remove_handle */
    CURLMcode remove_result = curl_multi_remove_handle(multi_handle, easy_handle);
    (void)remove_result;

    /* Fuzz curl_multi_wakeup */
    CURLMcode wakeup_result = curl_multi_wakeup(multi_handle);
    (void)wakeup_result;

    /* Fuzz curl_multi_setopt with various options */
    /* Use a subset of options to avoid complexity */
    long long_option = (long)Data[0];
    curl_off_t off_t_option = (curl_off_t)Data[1];
    void *cb_data = (void *)Data;
    /* Avoid passing raw fuzz data as a string pointer to prevent out-of-bounds reads if not null-terminated */
    /* Instead, we use a safe static string or NULL for string options if we need to test them */
    char safe_string[] = "test";
    const char *string_option = safe_string;

    /* Set some options */
    curl_multi_setopt(multi_handle, CURLMOPT_MAXCONNECTS, long_option);
    curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, long_option);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, long_option);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_PIPELINE_LENGTH, long_option);
    curl_multi_setopt(multi_handle, CURLMOPT_CONTENT_LENGTH_PENALTY_SIZE, off_t_option);
    curl_multi_setopt(multi_handle, CURLMOPT_CHUNK_LENGTH_PENALTY_SIZE, off_t_option);
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHFUNCTION, NULL);
    curl_multi_setopt(multi_handle, CURLMOPT_PUSHDATA, cb_data);

    /* Cleanup easy handle */
    curl_easy_cleanup(easy_handle);

    /* Cleanup multi handle */
    curl_multi_cleanup(multi_handle);

    /* Cleanup global */
    curl_global_cleanup();

    return 0;
}