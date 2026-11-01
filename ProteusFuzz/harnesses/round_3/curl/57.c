// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
// curl_global_sslset at easy.c:312:12 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void safe_free(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

static void test_mvsprintf_variadic(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    va_list args_copy;
    va_copy(args_copy, args);
    int ret = curl_mvsprintf(buffer, format, args_copy);
    (void)ret;
    va_end(args_copy);
    va_end(args);
}

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure we have a null-terminated string for format strings */
    char *format_str = malloc(Size + 1);
    if (!format_str) {
        return 0;
    }
    memcpy(format_str, Data, Size);
    format_str[Size] = '\0';

    /* Ensure we have a null-terminated string for environment variable names */
    char *env_name = malloc(Size + 1);
    if (!env_name) {
        free(format_str);
        return 0;
    }
    memcpy(env_name, Data, Size);
    env_name[Size] = '\0';

    /* Test curl_version */
    char *version = curl_version();
    if (version) {
        /* Just access it to ensure it's valid */
        (void)version[0];
    }

    /* Test curl_getenv */
    char *env_val = curl_getenv(env_name);
    safe_free(env_val);

    /* Test curl_msnprintf */
    char buffer[256];
    int ret = curl_msnprintf(buffer, sizeof(buffer), format_str, "test", 123);
    (void)ret;

    /* Test curl_maprintf with a safe copy of format string */
    char *mapped = curl_maprintf("%s %s", format_str, "safe_string");
    safe_free(mapped);

    /* Test curl_mvsprintf via variadic wrapper */
    test_mvsprintf_variadic(format_str, "arg1", "arg2");

    /* Test curl_global_sslset */
    curl_ssl_backend **avail = NULL;
    curl_global_sslset(CURLSSLBACKEND_NONE, NULL, &avail);
    curl_global_sslset(CURLSSLBACKEND_OPENSSL, "openssl", &avail);
    curl_global_sslset(CURLSSLBACKEND_NONE, "invalid_name", &avail);

    free(format_str);
    free(env_name);

    return 0;
}