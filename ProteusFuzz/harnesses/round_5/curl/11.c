// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Helper to ensure input is null-terminated for string functions */
static char *make_null_terminated(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *buf = malloc(1);
        if (buf) buf[0] = '\0';
        return buf;
    }
    char *buf = malloc(size + 1);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    buf[size] = '\0';
    return buf;
}

/* Variadic wrapper to properly initialize va_list for curl_mvsprintf */
static void invoke_mvsprintf_variadic(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[256];
    int ret = curl_mvsprintf(buffer, fmt, args);
    va_end(args);
    (void)ret;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Prepare null-terminated strings for API calls */
    char *str1 = make_null_terminated(Data, Size);
    char *str2 = make_null_terminated(Data, Size);
    if (!str1 || !str2) {
        free(str1);
        free(str2);
        return 0;
    }

    /* 1. Fuzz curl_escape */
    char *escaped = curl_escape(str1, (int)Size);
    if (escaped) {
        curl_free(escaped);
    }

    /* 2. Fuzz curl_maprintf */
    /* Ensure format string is safe; use a fixed format with %s */
    char *mapped = curl_maprintf("%s", str1);
    if (mapped) {
        curl_free(mapped);
    }
    /* Try another format */
    mapped = curl_maprintf("prefix_%s_suffix", str2);
    if (mapped) {
        curl_free(mapped);
    }

    /* 3. Wcurl_easy_getinfo_err_curl_socket is a compile-time macro, not callable at runtime.
       No runtime code needed. */

    /* 4. Fuzz curl_msnprintf */
    char buffer[256];
    int ret = curl_msnprintf(buffer, sizeof(buffer), "%s", str1);
    (void)ret; /* Ignore return for fuzzing, just exercise logic */
    ret = curl_msnprintf(buffer, sizeof(buffer), "test: %s", str2);
    (void)ret;

    /* 5. Fuzz curl_strnequal */
    int eq = curl_strnequal(str1, str2, Size);
    (void)eq;
    /* Test with NULL handling */
    eq = curl_strnequal(NULL, NULL, 0);
    (void)eq;
    eq = curl_strnequal(str1, NULL, Size);
    (void)eq;

    /* 6. Fuzz curl_mvsprintf */
    /* Use the variadic wrapper to properly handle va_list */
    invoke_mvsprintf_variadic("%s", str1);
    invoke_mvsprintf_variadic("test: %s", str2);

    /* Cleanup */
    free(str1);
    free(str2);

    return 0;
}