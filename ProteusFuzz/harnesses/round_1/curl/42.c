// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>

/* Helper to ensure null-termination for string operations */
static char *ensure_null_terminated(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    char *buf = malloc(size + 1);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    buf[size] = '\0';
    return buf;
}

/* Wrapper to allow va_start to work correctly in a variadic context */
static void test_mvsprintf_wrapper(const char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    (void)ret;
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl handle for functions requiring it */
    CURL *handle = curl_easy_init();
    if (!handle) return 0;

    /* Prepare buffers and strings from input */
    char *input_str = ensure_null_terminated(Data, Size);
    if (!input_str) {
        curl_easy_cleanup(handle);
        return 0;
    }

    /* Test 1: curl_msnprintf */
    char buffer[256];
    int ret1 = curl_msnprintf(buffer, sizeof(buffer), "Input: %s, Len: %zu", input_str, Size);
    (void)ret1;

    /* Test 2: curl_maprintf */
    char *result2 = curl_maprintf("Processed: %s", input_str);
    if (result2) {
        curl_free(result2);
    }

    /* Test 3: curl_mvsprintf via wrapper to satisfy variadic requirements */
    test_mvsprintf_wrapper("VA: %s", input_str);

    /* Test 4: curl_strnequal */
    int ret4 = curl_strnequal(input_str, input_str, Size > 10 ? 10 : Size);
    (void)ret4;

    /* Test 5: Wcurl_easy_setopt_err_curl_mimepost is a macro for compile-time checks.
       We skip direct invocation as it is not a runtime function. */

    /* Test 6: curl_easy_escape */
    char *escaped = curl_easy_escape(handle, input_str, (int)Size);
    if (escaped) {
        curl_free(escaped);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    free(input_str);

    return 0;
}