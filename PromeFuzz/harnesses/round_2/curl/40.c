// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_version at version.c:145:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/mprintf.h"
#include "curl/multi.h"

static char *helper_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a null-terminated string from fuzzer input for string parameters
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // 1. Test curl_pushheader_byname - requires push callback context
    // Since we're not in a push callback, this should return NULL
    char *push_header = curl_pushheader_byname(NULL, input_str);
    (void)push_header; // Ignore result

    // 2. Test curl_version - always safe to call
    char *version = curl_version();
    (void)version; // Ignore result

    // 3. Test curl_maprintf with various format strings
    char *formatted = NULL;
    if (Size > 1) {
        // Use first byte as format specifier hint
        uint8_t format_hint = Data[0] % 4;
        switch (format_hint) {
            case 0:
                formatted = curl_maprintf("%s", input_str);
                break;
            case 1:
                formatted = curl_maprintf("Test: %s", input_str);
                break;
            case 2:
                formatted = curl_maprintf("%.*s", (int)(Size % 100), input_str);
                break;
            case 3:
                formatted = curl_maprintf("%s:%d", input_str, (int)Size);
                break;
        }
        if (formatted) {
            curl_free(formatted);
        }
    }

    // 4. Test curl_getenv - use fuzzer input as environment variable name
    char *env_value = curl_getenv(input_str);
    if (env_value) {
        curl_free(env_value);
    }

    // 5. Test curl_pushheader_bynum - requires push callback context
    // Since we're not in a push callback, this should return NULL
    char *push_header_num = curl_pushheader_bynum(NULL, Size % 10);
    (void)push_header_num; // Ignore result

    // 6. Test curl_mvaprintf with various format strings
    char *va_formatted = NULL;
    if (Size > 2) {
        // Use second byte as format specifier hint
        uint8_t va_format_hint = Data[1] % 4;
        switch (va_format_hint) {
            case 0:
                va_formatted = helper_mvaprintf("%s", input_str);
                break;
            case 1:
                va_formatted = helper_mvaprintf("VA: %s", input_str);
                break;
            case 2:
                va_formatted = helper_mvaprintf("%.*s", (int)(Size % 50), input_str);
                break;
            case 3:
                va_formatted = helper_mvaprintf("Size: %zu, Data: %s", Size, input_str);
                break;
        }
        if (va_formatted) {
            curl_free(va_formatted);
        }
    }

    free(input_str);
    return 0;
}