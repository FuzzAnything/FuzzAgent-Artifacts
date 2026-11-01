// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* 
 * Helper to create a null-terminated string from fuzz input.
 */
static char *make_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *s = malloc(1);
        if (s) s[0] = '\0';
        return s;
    }
    char *s = malloc(size + 1);
    if (!s) return NULL;
    memcpy(s, data, size);
    s[size] = '\0';
    return s;
}

/* 
 * Helper function to initialize va_list safely.
 * va_start requires the last named argument of a variadic function.
 * Since we are in a fixed-argument function (LLVMFuzzerTestOneInput), 
 * we cannot use va_start directly. Instead, we use __builtin_va_start 
 * with a dummy parameter or construct the list manually if the compiler allows,
 * but the standard compliant way in a non-variadic context for fuzzing 
 * is to create a wrapper variadic function or use specific compiler builtins.
 * However, curl_mvsnprintf etc expect a va_list.
 * A common pattern in fuzzers for this is to define a dummy variadic helper.
 */
static void dummy_va_helper(const char *fmt, ...) {
    (void)fmt;
    /* This function is never called, just to allow va_start usage if needed,
       but we will use a different approach below: 
       We will simply not use va_start in the main function. 
       Instead, we will rely on the fact that we can't easily construct 
       a valid va_list without a variadic caller. 
       
       Alternative: Use __builtin_va_start with a dummy last arg? 
       No, that's not standard.
       
       Correct approach for fuzzing va_list functions without a variadic caller:
       We cannot strictly initialize a va_list without a variadic function context 
       in standard C. However, many fuzzing harnesses use a trick:
       They define a local variadic function to capture the args, 
       or they pass a pre-constructed va_list if the compiler allows (undefined behavior but works on many).
       
       Better approach: Since we can't easily pass real args to a va_list 
       without a variadic function, we will skip the va_list specific tests 
       OR we define a small variadic helper function that calls the target.
    */
}

static void test_mvsnprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    curl_mvsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
}

static void test_mvsprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    curl_mvsprintf(buffer, fmt, args);
    va_end(args);
}

static void test_mvprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvprintf(fmt, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    if (Size < 2) {
        return 0;
    }

    size_t fmt_len = Size / 2;
    if (fmt_len == 0) fmt_len = 1;
    
    char *fmt_str = make_safe_string(Data, fmt_len);
    if (!fmt_str) return 0;

    /* Test curl_msnprintf (variadic, no va_list needed directly) */
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int res = curl_msnprintf(buffer, sizeof(buffer), fmt_str, 0, 0, 0);
        (void)res;
    }

    /* Test curl_mvsnprintf via helper */
    test_mvsnprintf(fmt_str, 123, 456, 789);

    /* Test curl_mvsprintf via helper */
    test_mvsprintf(fmt_str, 123, 456, 789);

    /* Test curl_mvprintf via helper */
    test_mvprintf(fmt_str, 123, 456, 789);

    /* Test curl_maprintf */
    {
        char *result = NULL;
        size_t data_start = fmt_len;
        size_t data_len = Size - data_start;
        char *safe_arg = make_safe_string(Data + data_start, data_len);
        
        if (safe_arg) {
            result = curl_maprintf(fmt_str, safe_arg);
            if (result) {
                curl_free(result);
                result = NULL;
            }
            free(safe_arg);
        }
        
        result = curl_maprintf(fmt_str, 12345, 67890);
        if (result) {
            curl_free(result);
        }
    }

    free(fmt_str);

    return 0;
}