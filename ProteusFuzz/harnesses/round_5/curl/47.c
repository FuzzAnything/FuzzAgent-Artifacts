// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

/* Helper to create a null-terminated string from fuzz input */
static char *create_null_terminated_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return NULL;
    }
    char *str = (char *)malloc(size + 1);
    if (!str) {
        return NULL;
    }
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

/* Helper function to test va_list based APIs with variadic arguments */
static void test_va_list_functions(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    /* Test curl_mvaprintf */
    char *mva_result = curl_mvaprintf(format, args);
    if (mva_result) {
        curl_free(mva_result);
    }

    /* Test curl_mvsprintf */
    char mvs_buffer[256];
    memset(mvs_buffer, 0, sizeof(mvs_buffer));
    int mvs_ret = curl_mvsprintf(mvs_buffer, format, args);
    (void)mvs_ret;

    va_end(args);
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test curl_version (no args, static return) */
    char *ver = curl_version();
    if (ver) {
        /* Just access to ensure validity, no free needed as it's static */
        (void)ver[0];
    }

    /* Test curl_getenv */
    /* Use a slice of input as the variable name, ensuring null termination */
    char *env_var = create_null_terminated_string(Data, Size);
    if (env_var) {
        char *env_val = curl_getenv(env_var);
        if (env_val) {
            curl_free(env_val);
        }
        free(env_var);
    }

    /* Test curl_maprintf */
    /* Create a safe string for %s specifier */
    char *safe_str = create_null_terminated_string(Data, Size);
    if (safe_str) {
        char *result = curl_maprintf("prefix: %s suffix", safe_str);
        if (result) {
            curl_free(result);
        }
        free(safe_str);
    }

    /* Test curl_msnprintf */
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    safe_str = create_null_terminated_string(Data, Size);
    if (safe_str) {
        int ret = curl_msnprintf(buffer, sizeof(buffer), "msg: %s", safe_str);
        (void)ret; /* Suppress unused warning */
        free(safe_str);
    }

    /* Test curl_mvaprintf and curl_mvsprintf via wrapper */
    safe_str = create_null_terminated_string(Data, Size);
    if (safe_str) {
        const char *format = "test: %s";
        test_va_list_functions(format, safe_str);
        free(safe_str);
    }

    return 0;
}