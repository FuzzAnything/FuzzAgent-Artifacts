// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
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

// Helper function to test curl_mvaprintf with a variadic argument list
// Must be variadic to allow va_start to work correctly
static char *test_mvaprintf_variadic(const char *format, ...) {
    va_list args;
    char *result = NULL;
    
    va_start(args, format);
    result = curl_mvaprintf(format, args);
    va_end(args);
    
    return result;
}

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    char *format_buf = NULL;
    char *env_var_buf = NULL;
    char *result = NULL;
    char *env_result = NULL;

    // Create a safe format string from input, ensuring it's null-terminated
    size_t format_len = (Size > 256) ? 256 : Size;
    format_buf = (char *)malloc(format_len + 1);
    if (!format_buf) return 0;
    memcpy(format_buf, Data, format_len);
    format_buf[format_len] = '\0';

    // Create a safe environment variable name from input
    size_t env_len = (Size > 64) ? 64 : Size;
    env_var_buf = (char *)malloc(env_len + 1);
    if (!env_var_buf) {
        free(format_buf);
        return 0;
    }
    memcpy(env_var_buf, Data, env_len);
    env_var_buf[env_len] = '\0';

    // 1. Test curl_version (No arguments, static string)
    const char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    // 2. Test curl_getenv
    env_result = curl_getenv(env_var_buf);
    if (env_result) {
        (void)strlen(env_result);
        curl_free(env_result);
    }

    // 3. Test curl_maprintf
    // Strategy A: Call with just format string
    result = curl_maprintf(format_buf);
    if (result) {
        curl_free(result);
        result = NULL;
    }

    // Strategy B: Call with format string + a dummy string argument
    result = curl_maprintf("%s", format_buf);
    if (result) {
        curl_free(result);
        result = NULL;
    }

    // Strategy C: Call with an integer argument
    long dummy_int = 0;
    if (Size > 0) {
        dummy_int = (long)Data[0];
    }
    result = curl_maprintf("%ld", dummy_int);
    if (result) {
        curl_free(result);
        result = NULL;
    }

    // 4. Test curl_mvaprintf via helper (which is variadic)
    char *temp_result = test_mvaprintf_variadic(format_buf, format_buf);
    if (temp_result) {
        curl_free(temp_result);
    }

    // Cleanup
    free(format_buf);
    free(env_var_buf);

    return 0;
}