// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Required headers for curl API and type checking */
#include "curl.h"
#include "mprintf.h"
#include "typecheck-gcc.h"

/* Helper to create a null-terminated string from fuzz input if needed */
static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        return strdup("");
    }
    /* Find first null byte to ensure string safety, or use full size if none */
    size_t len = size;
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '\0') {
            len = i;
            break;
        }
    }
    char *str = malloc(len + 1);
    if (!str) return NULL;
    memcpy(str, data, len);
    str[len] = '\0';
    return str;
}

/* Helper function to test curl_mvsprintf with a va_list */
static void test_mvsprintf(char *buffer, size_t buf_size, const char *format, const char *arg) {
    va_list args;
    /* va_start requires the last named argument before ... in a variadic function.
       Since this function is not variadic, we cannot use va_start directly.
       Instead, we manually construct the va_list using __builtin_va_start or similar,
       but for portability and correctness in this context, we will use curl_mvsnprintf
       directly with a constructed va_list via a variadic wrapper or avoid va_list here.
       However, curl_mvsprintf expects a va_list. To properly test it, we need a variadic helper.
    */
    (void)buffer;
    (void)buf_size;
    (void)format;
    (void)arg;
    /* We will skip direct mvsprintf testing here due to va_list limitations in non-variadic functions.
       Instead, we rely on curl_msnprintf and curl_maprintf which are easier to fuzz safely.
    */
}

/* Variadic wrapper to properly initialize va_list for curl_mvsprintf testing */
static void test_mvsprintf_wrapper(char *buffer, size_t buf_size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    (void)ret;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl handle for functions requiring it */
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Split input data for different test scenarios */
    size_t chunk1 = Size / 3;
    size_t chunk2 = Size / 3;
    size_t chunk3 = Size - chunk1 - chunk2;

    /* 1. Test curl_msnprintf */
    {
        char buffer[256];
        /* Use a fixed format to avoid undefined behavior with malicious format strings */
        int ret = curl_msnprintf(buffer, sizeof(buffer), "Input: %.*s", (int)chunk1, Data);
        (void)ret;
    }

    /* 2. Test curl_maprintf */
    {
        /* Use a safe format string to avoid format string vulnerabilities */
        char *result = curl_maprintf("Fuzzed: %.*s", (int)chunk2, Data + chunk1);
        if (result) {
            curl_free(result);
        }
    }

    /* 3. Test curl_mvsprintf via variadic wrapper */
    {
        char buffer[256];
        char *safe_arg = create_safe_string(Data + chunk1 + chunk2, chunk3);
        if (safe_arg) {
            test_mvsprintf_wrapper(buffer, sizeof(buffer), "Test: %s", safe_arg);
            free(safe_arg);
        }
    }

    /* 4. Test curl_strnequal */
    {
        /* Compare two parts of the input */
        char *s1 = create_safe_string(Data, chunk1);
        char *s2 = create_safe_string(Data + chunk1, chunk2);
        if (s1 && s2) {
            int ret = curl_strnequal(s1, s2, (size_t)chunk1);
            (void)ret;
            free(s1);
            free(s2);
        }
    }

    /* 5. Test curl_easy_escape */
    {
        char *escaped = curl_easy_escape(handle, (const char *)Data, (int)Size);
        if (escaped) {
            curl_free(escaped);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(handle);

    return 0;
}