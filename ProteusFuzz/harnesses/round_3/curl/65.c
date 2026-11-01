// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Helper function to allow va_list usage in a variadic context */
static void test_mvsnprintf_helper(const char *fmt, ...) {
    char buffer2[4096];
    memset(buffer2, 0, sizeof(buffer2));

    va_list args;
    va_start(args, fmt);
    
    int ret = curl_mvsnprintf(buffer2, sizeof(buffer2), fmt, args);
    (void)ret;
    
    va_end(args);
}

static void test_mvsnprintf_zero_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    int ret = curl_mvsnprintf(NULL, 0, fmt, args);
    (void)ret;
    
    va_end(args);
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Allocate buffers for testing */
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));

    /* Create safe string pointers from the input data */
    /* We must ensure we don't read past the buffer when passing to string functions */
    const char *s1 = (const char *)Data;
    const char *s2 = (const char *)(Data + (Size / 2));
    
    /* Ensure s2 is within bounds */
    if (Size / 2 >= Size) {
        s2 = (const char *)Data;
    }

    /* Determine a safe length n for comparison that doesn't exceed string lengths */
    /* Since input data might not be null-terminated, we limit n to the remaining size */
    size_t len1 = Size;
    size_t len2 = Size - (Size / 2);
    size_t n = (Size % 256);
    
    /* Cap n to the actual available data for s2 to prevent overflow in strequal */
    if (n > len2) {
        n = len2;
    }

    /* Test curl_strnequal */
    if (Size > 0) {
        int result1 = curl_strnequal(s1, s2, n);
        (void)result1;
    }

    /* Test curl_msnprintf with various formats derived from input */
    if (Size >= 2) {
        char fmt[64];
        size_t fmt_len = (Size % 10) + 1;
        if (fmt_len > 60) fmt_len = 60;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';

        int val1 = (int)(Data[0] & 0xFF);
        int val2 = (int)(Data[1] & 0xFF);
        long val3 = (long)Size;

        int ret1 = curl_msnprintf(buffer, sizeof(buffer), fmt, val1, val2, val3);
        (void)ret1;

        /* Test with empty format */
        if (Size > 10 && Data[10] == 0) {
            int ret2 = curl_msnprintf(buffer, sizeof(buffer), "", val1);
            (void)ret2;
        }
    }

    /* Test curl_mvsnprintf via helper function */
    if (Size >= 2) {
        char fmt[64];
        size_t fmt_len = (Size % 10) + 1;
        if (fmt_len > 60) fmt_len = 60;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';

        int val1 = (int)(Data[0] & 0xFF);
        int val2 = (int)(Data[1] & 0xFF);
        long val3 = (long)Size;

        test_mvsnprintf_helper(fmt, val1, val2, val3);
    }

    /* Test edge cases for curl_strnequal with NULLs */
    int result_null1 = curl_strnequal(NULL, NULL, 5);
    int result_null2 = curl_strnequal(NULL, s1, 5);
    int result_null3 = curl_strnequal(s1, NULL, 5);
    (void)result_null1;
    (void)result_null2;
    (void)result_null3;

    /* Test curl_msnprintf with 0 length buffer */
    int ret_zero = curl_msnprintf(NULL, 0, "test", 123);
    (void)ret_zero;

    /* Test curl_mvsnprintf with 0 length buffer via helper */
    test_mvsnprintf_zero_helper("test");

    return 0;
}