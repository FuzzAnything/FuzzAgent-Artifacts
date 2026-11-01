// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/typecheck-gcc.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static int test_mvsprintf_helper(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char buffer[4096];
    int result = curl_mvsprintf(buffer, fmt, ap);
    va_end(ap);
    return result;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl handle for functions that require it */
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Prepare buffers for testing */
    char buffer[4096];
    char *escaped_buf = NULL;
    char *mapped_buf = NULL;

    /* Use input data as string content carefully */
    /* Ensure null termination for string operations */
    char safe_input[Size + 1];
    memcpy(safe_input, Data, Size);
    safe_input[Size] = '\0';

    /* Test curl_msnprintf */
    /* Case 1: Simple format */
    int ret = curl_msnprintf(buffer, sizeof(buffer), "%s", safe_input);
    (void)ret;

    /* Case 2: With length specifier to handle binary data safely */
    ret = curl_msnprintf(buffer, sizeof(buffer), "%.*s", (int)Size, Data);
    (void)ret;

    /* Case 3: Mixed format */
    ret = curl_msnprintf(buffer, sizeof(buffer), "Data: %s, Len: %zu", safe_input, Size);
    (void)ret;

    /* Test curl_escape */
    escaped_buf = curl_escape(safe_input, (int)Size);
    if (escaped_buf) {
        curl_free(escaped_buf);
    }

    /* Test curl_maprintf */
    /* Case 1: Simple string */
    mapped_buf = curl_maprintf("%s", safe_input);
    if (mapped_buf) {
        curl_free(mapped_buf);
    }

    /* Case 2: With integer */
    mapped_buf = curl_maprintf("Size: %zu", Size);
    if (mapped_buf) {
        curl_free(mapped_buf);
    }

    /* Case 3: With hex */
    mapped_buf = curl_maprintf("Hex: %x", (unsigned int)Size);
    if (mapped_buf) {
        curl_free(mapped_buf);
    }

    /* Test curl_mvsprintf via helper */
    ret = test_mvsprintf_helper("%s %zu", safe_input, Size);
    (void)ret;

    /* Test curl_easy_escape */
    escaped_buf = curl_easy_escape(handle, safe_input, (int)Size);
    if (escaped_buf) {
        curl_free(escaped_buf);
    }

    /* Test curl_easy_escape with length 0 (null terminated) */
    escaped_buf = curl_easy_escape(handle, safe_input, 0);
    if (escaped_buf) {
        curl_free(escaped_buf);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);

    return 0;
}