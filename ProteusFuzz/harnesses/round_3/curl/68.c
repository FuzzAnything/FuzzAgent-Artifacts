// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Helper function to test va_list functions. Must be variadic to use va_start. */
static void test_va_functions(const char *format_str, ...) {
    va_list args;
    va_start(args, format_str);

    /* Test curl_mvaprintf */
    char *result_mvaprintf = curl_mvaprintf(format_str, args);
    if (result_mvaprintf) {
        volatile size_t res_len2 = strlen(result_mvaprintf);
        (void)res_len2;
        curl_free(result_mvaprintf);
    }

    /* Reset args for next call */
    va_end(args);
    va_start(args, format_str);

    /* Test curl_mvsprintf */
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    int ret = curl_mvsprintf(buffer, format_str, args);
    if (ret >= 0) {
        volatile size_t buf_len = strlen(buffer);
        (void)buf_len;
    }
    va_end(args);
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test curl_version */
    const char *version = curl_version();
    if (version) {
        volatile size_t len = strlen(version);
        (void)len;
    }

    /* Test curl_getenv */
    /* Create a safe environment variable name from input */
    char env_name[256];
    size_t copy_len = Size < sizeof(env_name) - 1 ? Size : sizeof(env_name) - 1;
    memcpy(env_name, Data, copy_len);
    env_name[copy_len] = '\0';
    /* Ensure no null bytes in the middle for safety */
    for (size_t i = 0; i < copy_len; i++) {
        if (env_name[i] == '\0') {
            env_name[i] = 'X';
        }
    }
    char *env_val = curl_getenv(env_name);
    if (env_val) {
        volatile size_t env_len = strlen(env_val);
        (void)env_len;
        curl_free(env_val);
    }

    /* Test curl_maprintf */
    /* Ensure format string is null-terminated */
    char *format_str = malloc(Size + 1);
    if (!format_str) return 0;
    memcpy(format_str, Data, Size);
    format_str[Size] = '\0';

    /* Ensure any string arguments are null-terminated */
    char *arg_str = malloc(Size + 1);
    if (!arg_str) {
        free(format_str);
        return 0;
    }
    memcpy(arg_str, Data, Size);
    arg_str[Size] = '\0';

    char *result_maprintf = curl_maprintf(format_str, arg_str);
    if (result_maprintf) {
        volatile size_t res_len = strlen(result_maprintf);
        (void)res_len;
        curl_free(result_maprintf);
    }

    /* Test va_list functions via helper */
    test_va_functions(format_str, arg_str);

    /* Cleanup */
    free(format_str);
    free(arg_str);

    return 0;
}