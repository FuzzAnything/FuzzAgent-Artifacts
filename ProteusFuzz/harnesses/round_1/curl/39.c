// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static void safe_free(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

/* Helper function to test va_list APIs with variadic arguments */
static int test_mvsprintf_helper(char *buf, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int r = curl_mvsprintf(buf, fmt, args);
    va_end(args);
    return r;
}

static char *test_mvaprintf_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *r = curl_mvaprintf(fmt, args);
    va_end(args);
    return r;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure we have a null-terminated string for format specifiers if needed */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* Test curl_version */
    char *version = curl_version();
    if (version) {
        (void)strlen(version);
    }

    /* Test curl_getenv */
    char *env_val = curl_getenv("PATH");
    if (env_val) {
        (void)strlen(env_val);
        safe_free(env_val);
    }

    /* Test curl_msnprintf */
    char buffer[256];
    int ret;

    /* Safe format string test with %s using fixed string */
    ret = curl_msnprintf(buffer, sizeof(buffer), "Fixed: %s", "test");
    (void)ret;

    /* Test with %.*s to handle potentially non-null-terminated input safely */
    ret = curl_msnprintf(buffer, sizeof(buffer), "Input: %.*s", (int)Size, input_str);
    (void)ret;

    /* Test with numeric format */
    int val1 = (Size > 0) ? (int)Data[0] : 0;
    int val2 = (Size > 1) ? (int)Data[1] : 0;
    ret = curl_msnprintf(buffer, sizeof(buffer), "Num: %d %x", val1, (unsigned int)val2);
    (void)ret;

    /* Test curl_maprintf with safe format */
    char *alloc_str = curl_maprintf("Allocated: %s", "safe_string");
    if (alloc_str) {
        (void)strlen(alloc_str);
        safe_free(alloc_str);
    }

    /* Test curl_maprintf with numeric args */
    alloc_str = curl_maprintf("Nums: %d %d", val1, val2);
    if (alloc_str) {
        (void)strlen(alloc_str);
        safe_free(alloc_str);
    }

    /* Test curl_mvsprintf via helper */
    ret = test_mvsprintf_helper(buffer, "VA: %d %s", val1, "static");
    (void)ret;

    /* Test curl_mvaprintf via helper */
    alloc_str = test_mvaprintf_helper("VA Alloc: %d", val1);
    if (alloc_str) {
        (void)strlen(alloc_str);
        safe_free(alloc_str);
    }

    /* Test with different format specifiers to explore states */
    if (Size >= 2) {
        ret = curl_msnprintf(buffer, sizeof(buffer), "%c %c", Data[0], Data[1]);
        (void)ret;
    }

    free(input_str);
    return 0;
}