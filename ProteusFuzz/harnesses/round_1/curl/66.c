// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_version at version.c:145:7 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_perform at easy.c:817:10 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl library functions.
 * Targets:
 * - curl_version
 * - curl_getdate
 * - curl_getenv
 * Note: Wcurl_* macros are compile-time type checks and cannot be invoked at runtime.
 */

int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare environment */
    CURL *curl = curl_easy_init();
    if (!curl) {
        return 0;
    }

    /* Ensure we have a null-terminated string for string-based functions */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 2. Invoke target functions */

    /* Test curl_version() */
    /* No arguments needed, returns static string */
    char *version = curl_version();
    if (version) {
        /* Just access to ensure no crash, ignore content */
        (void)version[0];
    }

    /* Test curl_getdate() */
    /* Parses RFC 2822 date strings */
    time_t unused_time = 0;
    time_t parsed_time = curl_getdate(input_str, &unused_time);
    (void)parsed_time;

    /* Test curl_getenv() */
    /* Retrieves environment variable. Note: input_str might not be a valid env var name,
     * but curl_getenv handles invalid names gracefully by returning NULL. */
    char *env_val = curl_getenv(input_str);
    if (env_val) {
        /* Must free to prevent leaks */
        curl_free(env_val);
    }

    /* Explore more states by setting some options on the CURL handle */
    /* Set URL to the input string (might be invalid URL, but curl handles it) */
    curl_easy_setopt(curl, CURLOPT_URL, input_str);

    /* Set a dummy long option */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    /* Set a dummy string option */
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "FuzzAgent/1.0");

    /* Try to perform a request (will likely fail due to invalid URL, but exercises code) */
    /* We don't care about the result, just want to exercise the path */
    CURLcode res = curl_easy_perform(curl);
    (void)res;

    /* Cleanup */
    curl_easy_cleanup(curl);
    free(input_str);

    return 0;
}