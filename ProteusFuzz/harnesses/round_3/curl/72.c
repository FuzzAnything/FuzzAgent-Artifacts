// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_trace at easy.c:292:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_upkeep at easy.c:1320:10 in easy.h
// curl_easy_perform at easy.c:817:10 in easy.h
// Wcurl_easy_getinfo_err_curl_off_t at typecheck-gcc.h:389:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Fuzzing entry point */
int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    CURLcode res;
    CURL *curl = NULL;
    char *sanitized_config = NULL;
    char *sanitized_url = NULL;
    char *sanitized_useragent = NULL;
    char *error_buffer = NULL;

    /* 1. Prepare environment */
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl globally if needed, though curl_global_trace is our target */
    /* We attempt to call curl_global_trace with sanitized input */
    if (Size > 0) {
        sanitized_config = (char *)malloc(Size + 1);
        if (sanitized_config) {
            memcpy(sanitized_config, Data, Size);
            sanitized_config[Size] = '\0';
            /* Ensure no null byte in the middle breaks the string logic if we want full length,
             * but the requirement is just valid null-terminated. The memcpy above ensures it.
             */
            res = curl_global_trace(sanitized_config);
            /* Ignore return value for fuzzing stability, just ensure no crash */
            free(sanitized_config);
            sanitized_config = NULL;
        }
    }

    /* Initialize a CURL handle */
    curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Allocate error buffer dynamically as per instructions */
    error_buffer = (char *)malloc(CURL_ERROR_SIZE);
    if (!error_buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }

    /* Set error buffer */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error_buffer);

    /* 2. Diversify function calls */
    
    /* Try to set a URL from fuzz data (sanitized) */
    if (Size > 0) {
        sanitized_url = (char *)malloc(Size + 1);
        if (sanitized_url) {
            memcpy(sanitized_url, Data, Size);
            sanitized_url[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_URL, sanitized_url);
            free(sanitized_url);
            sanitized_url = NULL;
        }
    }

    /* Try to set User-Agent from fuzz data (sanitized) */
    if (Size > 0) {
        sanitized_useragent = (char *)malloc(Size + 1);
        if (sanitized_useragent) {
            memcpy(sanitized_useragent, Data, Size);
            sanitized_useragent[Size] = '\0';
            curl_easy_setopt(curl, CURLOPT_USERAGENT, sanitized_useragent);
            free(sanitized_useragent);
            sanitized_useragent = NULL;
        }
    }

    /* Set some other options to explore state */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

    /* Call curl_easy_upkeep */
    res = curl_easy_upkeep(curl);
    (void)res; /* Suppress unused warning */

    /* Call curl_easy_perform */
    res = curl_easy_perform(curl);
    (void)res; /* Suppress unused warning */

    /* Call curl_easy_getinfo after perform */
    long http_code = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    (void)res;

    double total_time = 0.0;
    res = curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);
    (void)res;

    /* 3. Cleanup */
    curl_easy_cleanup(curl);
    free(error_buffer);

    return 0;
}