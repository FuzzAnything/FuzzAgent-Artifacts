// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Fuzz driver for curl API functions:
 * - curl_getenv
 * - curl_version
 * - curl_pushheader_bynum
 * - curl_maprintf
 * - curl_pushheader_byname
 * - curl_msnprintf
 *
 * Note: curl_pushheader_bynum and curl_pushheader_byname require a valid
 * struct curl_pushheaders which is internal to libcurl and not constructible
 * by the user. We will pass NULL to test error handling.
 */

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null termination for string operations */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Test curl_version (no args, static string) */
    char *ver = curl_version();
    if (ver) {
        /* Just access it to ensure it's valid */
        (void)ver[0];
    }

    /* 2. Test curl_getenv with fuzzed input as variable name */
    char *env_val = curl_getenv(input_str);
    if (env_val) {
        /* Use it briefly */
        (void)strlen(env_val);
        curl_free(env_val);
    }

    /* 3. Test curl_maprintf with fuzzed format string */
    /* We limit the format string length to avoid excessive memory allocation */
    size_t fmt_len = (Size > 256) ? 256 : Size;
    char *fmt_str = (char *)malloc(fmt_len + 1);
    if (fmt_str) {
        memcpy(fmt_str, input_str, fmt_len);
        fmt_str[fmt_len] = '\0';

        /* Try with a simple argument to avoid complex format parsing issues */
        char *mapped = curl_maprintf(fmt_str, 12345);
        if (mapped) {
            (void)strlen(mapped);
            curl_free(mapped);
        }
    }
    if (fmt_str) free(fmt_str);

    /* 4. Test curl_msnprintf */
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    /* Use the same format string logic as above */
    size_t fmt_len2 = (Size > 256) ? 256 : Size;
    char *fmt_str2 = (char *)malloc(fmt_len2 + 1);
    if (fmt_str2) {
        memcpy(fmt_str2, input_str, fmt_len2);
        fmt_str2[fmt_len2] = '\0';
        int ret = curl_msnprintf(buffer, sizeof(buffer), fmt_str2, 12345);
        (void)ret; /* Ignore return value for fuzzing */
    }
    if (fmt_str2) free(fmt_str2);

    /* 5. Test curl_pushheader_bynum with NULL (invalid struct) */
    struct curl_pushheaders *invalid_h = NULL;
    char *header_by_num = curl_pushheader_bynum(invalid_h, 0);
    if (header_by_num) {
        /* Should not happen with NULL, but handle if it does */
        (void)strlen(header_by_num);
        /* Note: curl_pushheader returns internal pointers, do not free */
    }

    /* 6. Test curl_pushheader_byname with NULL (invalid struct) */
    char *header_by_name = curl_pushheader_byname(invalid_h, input_str);
    if (header_by_name) {
        (void)strlen(header_by_name);
    }

    /* Cleanup */
    free(input_str);

    return 0;
}