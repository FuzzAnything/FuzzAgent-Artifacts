// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

/* Variadic wrapper for curl_mvprintf */
static void wrapper_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

/* Variadic wrapper for curl_mvsnprintf */
static int wrapper_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return res;
}

/* Variadic wrapper for curl_mvsprintf */
static int wrapper_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Prepare a safe buffer for fixed-size operations */
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    /* Create a null-terminated string from input for format specifiers */
    size_t safe_len = (Size < 1023) ? Size : 1023;
    char *input_str = malloc(safe_len + 1);
    if (!input_str) {
        curl_global_cleanup();
        return 0;
    }
    memcpy(input_str, Data, safe_len);
    input_str[safe_len] = '\0';

    /* 1. Test curl_msnprintf */
    /* Use safe format specifiers. Avoid %s with untrusted input unless controlled. */
    int ret = curl_msnprintf(buffer, sizeof(buffer), "Prefix: %.*s Suffix", (int)safe_len, input_str);
    (void)ret;

    /* Test curl_msnprintf with integer formatting */
    ret = curl_msnprintf(buffer, sizeof(buffer), "Value: %d", (int)Data[0]);
    (void)ret;

    /* 2. Test curl_maprintf */
    char *allocated_str = curl_maprintf("Allocated: %s", input_str);
    if (allocated_str) {
        curl_free(allocated_str);
    }

    /* Test curl_maprintf with numbers */
    allocated_str = curl_maprintf("Num: %d %x", (int)Data[0], (unsigned int)(Size > 1 ? Data[1] : 0));
    if (allocated_str) {
        curl_free(allocated_str);
    }

    /* 3. Test curl_mvprintf via wrapper */
    wrapper_mvprintf("Value: %d\n", (int)Data[0]);

    /* 4. Test curl_mvsnprintf via wrapper */
    ret = wrapper_mvsnprintf(buffer, sizeof(buffer), "Value: %d", (int)Data[0]);
    (void)ret;

    /* 5. Test curl_mvsprintf via wrapper */
    ret = wrapper_mvsprintf(buffer, "Value: %d", (int)Data[0]);
    (void)ret;

    /* 6. Test curl_unescape */
    char *unescaped = curl_unescape(input_str, -1);
    if (unescaped) {
        curl_free(unescaped);
    }

    /* Test curl_unescape with specific length */
    unescaped = curl_unescape(input_str, (int)safe_len);
    if (unescaped) {
        curl_free(unescaped);
    }

    /* Cleanup */
    free(input_str);
    curl_global_cleanup();

    return 0;
}