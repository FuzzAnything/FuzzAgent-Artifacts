// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/*
 * Fuzz driver for curl API functions:
 * - curl_maprintf
 * - curl_share_strerror
 * - curl_version
 * - curl_getenv
 *
 * Note: Wcurl_easy_setopt_err_* are compile-time macros, not runtime functions.
 * They are exercised implicitly by the compiler when type-checking curl_easy_setopt calls.
 * We do not call them directly as they are not executable functions.
 */

int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * 1. Prepare environment
     * We will use the input data to construct various test cases.
     */

    /* Ensure we have at least one byte to avoid null pointer issues in string ops */
    if (Size < 1) {
        return 0;
    }

    /*
     * Test 1: curl_version
     * This function takes no arguments and returns a static string.
     * Safe to call multiple times.
     */
    const char *version = curl_version();
    if (version) {
        /* Just touch the string to ensure it's valid */
        (void)strlen(version);
    }

    /*
     * Test 2: curl_share_strerror
     * We iterate through possible CURLSHcode values.
     * Since we don't have the full enum definition in the prompt,
     * we try a range of integer values cast to CURLSHcode.
     * The input data can influence which codes we test.
     */
    for (size_t i = 0; i < Size && i < 100; i++) {
        CURLSHcode code = (CURLSHcode)Data[i];
        const char *str = curl_share_strerror(code);
        if (str) {
            (void)strlen(str);
        }
    }

    /*
     * Test 3: curl_maprintf
     * We need to construct a format string and arguments.
     * We will use the input data to create a safe format string.
     * To avoid format string vulnerabilities in the fuzzer itself,
     * we limit the format string to simple specifiers or use the data as arguments.
     *
     * Strategy:
     * - Use a fixed safe format string "%s %d %s"
     * - Use parts of the input data as the arguments.
     * - Ensure null termination for string arguments.
     */

    /* Create a buffer for string arguments from input data */
    char str_arg1[256] = {0};
    char str_arg2[256] = {0};
    int int_arg = 0;

    /* Copy data into string buffers, ensuring null termination */
    size_t copy_len1 = (Size > 255) ? 255 : Size;
    memcpy(str_arg1, Data, copy_len1);
    str_arg1[copy_len1] = '\0';

    if (Size > 1) {
        size_t copy_len2 = (Size - 1 > 255) ? 255 : Size - 1;
        memcpy(str_arg2, Data + 1, copy_len2);
        str_arg2[copy_len2] = '\0';
    }

    /* Extract an integer from the data if possible */
    if (Size >= 4) {
        int_arg = (int)Data[0] | ((int)Data[1] << 8) | ((int)Data[2] << 16) | ((int)Data[3] << 24);
    } else if (Size >= 1) {
        int_arg = (int)Data[0];
    }

    /* Call curl_maprintf with safe format and arguments */
    char *result = curl_maprintf("%s %d %s", str_arg1, int_arg, str_arg2);
    if (result) {
        /* Use the result to exercise memory handling */
        (void)strlen(result);
        curl_free(result);
    }

    /*
     * Test 4: curl_getenv
     * We use the input data to construct environment variable names.
     * We must ensure the name is a valid C string (null-terminated).
     * We will try to read the first N bytes as a variable name.
     */
    char env_var[256] = {0};
    size_t env_len = (Size > 255) ? 255 : Size;
    memcpy(env_var, Data, env_len);
    env_var[env_len] = '\0';

    /*
     * Filter out invalid characters for environment variable names if desired,
     * but curl_getenv should handle arbitrary strings gracefully (returning NULL).
     * We pass the raw string to test edge cases.
     */
    char *env_val = curl_getenv(env_var);
    if (env_val) {
        (void)strlen(env_val);
        curl_free(env_val);
    }

    /*
     * Additional exploration: Try different format strings for curl_maprintf
     * We will construct a format string from the input data, but sanitize it
     * to prevent the fuzzer from crashing due to invalid format specifiers.
     * We'll replace any '%' with '%%' or remove them to be safe,
     * OR we can just use a fixed format and vary the arguments.
     * The previous test varied arguments. Let's try varying the format slightly.
     */

    /* Create a safe format string from input, ensuring it doesn't crash */
    char safe_format[256] = {0};
    size_t fmt_len = 0;
    for (size_t i = 0; i < Size && fmt_len < 254; i++) {
        unsigned char c = Data[i];
        /* Skip '%' to avoid format string issues in the fuzzer logic itself */
        if (c == '%') {
            continue;
        }
        /* Only allow printable ASCII for safety in this fuzzer */
        if (c >= 32 && c < 127) {
            safe_format[fmt_len++] = c;
        }
    }
    safe_format[fmt_len] = '\0';

    /* If we have a safe format string, try to use it with a dummy argument */
    if (fmt_len > 0) {
        /* We need to match the format string. Since we stripped %, it's just a literal string.
         * curl_maprintf expects a format string. If there are no specifiers, it just prints the string.
         * But if we accidentally have a specifier, it might crash.
         * To be absolutely safe, let's just use a fixed format again but with different data.
         */
        char *result2 = curl_maprintf("Test: %s", safe_format);
        if (result2) {
            curl_free(result2);
        }
    }

    return 0;
}