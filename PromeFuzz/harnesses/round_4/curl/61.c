// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/multi.h"
#include "curl/mprintf.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static char *my_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Function 1: curl_version
    const char *version = curl_version();
    if (version) {
        // Just to use the result, prevent optimization
        volatile char c = version[0];
        (void)c;
    }

    // Function 2: curl_maprintf
    char *formatted = curl_maprintf("Data: %.*s", (int)(Size > 100 ? 100 : Size), Data);
    if (formatted) {
        curl_free(formatted);
    }

    // Function 3: curl_mvaprintf via helper
    char *va_formatted = my_mvaprintf("Size: %zu, First: %c", Size, Size > 0 ? Data[0] : '0');
    if (va_formatted) {
        curl_free(va_formatted);
    }

    // Function 4: curl_getenv
    char env_name[256];
    size_t env_len = Size > 255 ? 255 : Size;
    memcpy(env_name, Data, env_len);
    env_name[env_len] = '\0';
    char *env_value = curl_getenv(env_name);
    if (env_value) {
        curl_free(env_value);
    }

    // Functions 5 & 6: curl_pushheader_bynum and curl_pushheader_byname
    // These require a valid curl_pushheaders pointer from within a push callback.
    // We'll call them with NULL to test error paths.
    char *push_by_num = curl_pushheader_bynum(NULL, 0);
    (void)push_by_num; // Should be NULL

    char header_name[256];
    size_t name_len = Size > 255 ? 255 : Size;
    memcpy(header_name, Data, name_len);
    header_name[name_len] = '\0';
    char *push_by_name = curl_pushheader_byname(NULL, header_name);
    (void)push_by_name; // Should be NULL

    return 0;
}