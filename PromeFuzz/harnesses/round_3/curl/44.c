// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl.h"
#include "multi.h"
#include "mprintf.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static char *test_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create null-terminated string from fuzzer input for string parameters
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_pushheader_byname with NULL pushheaders (outside callback context)
    char *push_result = curl_pushheader_byname(NULL, input_str);
    // No need to free - returns NULL or read-only string

    // Test curl_pushheader_bynum with NULL pushheaders
    push_result = curl_pushheader_bynum(NULL, 0);
    // No need to free - returns NULL or read-only string

    // Test curl_getenv
    char *env_result = curl_getenv(input_str);
    if (env_result) {
        curl_free(env_result);
    }

    // Test curl_maprintf with various format strings
    char *maprintf_result = curl_maprintf("%s", input_str);
    if (maprintf_result) {
        curl_free(maprintf_result);
    }

    maprintf_result = curl_maprintf("Test: %.*s", (int)(Size > 100 ? 100 : Size), input_str);
    if (maprintf_result) {
        curl_free(maprintf_result);
    }

    // Test curl_mvaprintf
    char *mvaprintf_result = test_mvaprintf("%s", input_str);
    if (mvaprintf_result) {
        curl_free(mvaprintf_result);
    }

    // Test curl_version (no arguments)
    char *version_str = curl_version();
    // No need to free - returns static string

    // Clean up
    free(input_str);

    return 0;
}