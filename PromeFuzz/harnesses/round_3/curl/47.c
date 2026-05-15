// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

// Include curl.h first as it provides base definitions
#include "curl/curl.h"

// Workaround for the GCC extension issue in multi.h
#ifndef __extension__
#define __extension__
#endif
#include "curl/multi.h"
#ifndef __extension__
#undef __extension__
#endif

#include "curl/mprintf.h"

static char *test_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from the fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_pushheader_byname with NULL pushheaders (outside callback context)
    char *push_result1 = curl_pushheader_byname(NULL, input_str);
    // Should return NULL when not in callback context

    // Test curl_pushheader_bynum with NULL pushheaders
    char *push_result2 = curl_pushheader_bynum(NULL, 0);
    // Should return NULL when not in callback context

    // Test curl_getenv with fuzzer input as environment variable name
    char *env_result = curl_getenv(input_str);
    if (env_result) {
        curl_free(env_result);
    }

    // Test curl_maprintf with simple format string
    char *maprintf_result = curl_maprintf("%s", input_str);
    if (maprintf_result) {
        curl_free(maprintf_result);
    }

    // Test curl_mvaprintf via helper function
    char *mvaprintf_result = test_mvaprintf("%s", input_str);
    if (mvaprintf_result) {
        curl_free(mvaprintf_result);
    }

    // Test curl_version - always safe to call
    char *version = curl_version();
    // No need to free, returns static string

    // Test with different format strings for maprintf
    if (Size > 1) {
        // Use first byte as integer for formatting
        int int_val = Data[0];
        char *maprintf_result2 = curl_maprintf("Number: %d, String: %s", int_val, input_str);
        if (maprintf_result2) {
            curl_free(maprintf_result2);
        }
    }

    // Test with empty string for getenv
    char *empty_env = curl_getenv("");
    if (empty_env) {
        curl_free(empty_env);
    }

    // Test curl_pushheader_bynum with different indices
    for (size_t i = 0; i < 5; i++) {
        char *push_result3 = curl_pushheader_bynum(NULL, i);
        // Should return NULL when not in callback context
    }

    free(input_str);
    return 0;
}