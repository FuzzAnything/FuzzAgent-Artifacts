// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>
#include <time.h>

static int test_mvsprintf(const char *format, ...) {
    char buffer_mvs[256];
    va_list args;
    va_start(args, format);
    int ret_mvs = curl_mvsprintf(buffer_mvs, format, args);
    va_end(args);
    return ret_mvs;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a mutable copy of input to ensure null termination */
    char *input_copy = (char *)malloc(Size + 1);
    if (!input_copy) {
        return 0;
    }
    memcpy(input_copy, Data, Size);
    input_copy[Size] = '\0';

    /* Test curl_version */
    char *version_str = curl_version();
    if (version_str) {
        (void)strlen(version_str); /* Use the string to avoid unused warnings */
    }

    /* Test curl_getenv */
    char *env_val = curl_getenv(input_copy);
    if (env_val) {
        (void)strlen(env_val);
        curl_free(env_val);
    }

    /* Test curl_getdate */
    time_t timestamp = curl_getdate(input_copy, NULL);
    (void)timestamp;

    /* Test curl_msnprintf */
    char buffer_msn[256];
    int ret_msn = curl_msnprintf(buffer_msn, sizeof(buffer_msn), "%s", input_copy);
    (void)ret_msn;

    /* Test curl_maprintf */
    char *mapped_str = curl_maprintf("%s", input_copy);
    if (mapped_str) {
        (void)strlen(mapped_str);
        curl_free(mapped_str);
    }

    /* Test curl_mvsprintf with va_list */
    int ret_mvs = test_mvsprintf("%s", input_copy);
    (void)ret_mvs;

    /* Cleanup */
    free(input_copy);

    return 0;
}