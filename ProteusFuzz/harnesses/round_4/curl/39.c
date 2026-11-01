// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

/* Dummy callback to satisfy type checks if needed */
static size_t dummy_read_cb(char *ptr, size_t size, size_t nmemb, void *userp) {
    (void)ptr; (void)size; (void)nmemb; (void)userp;
    return 0;
}

/* Helper function to test curl_mvsnprintf with %s */
static void test_mvsnprintf_s(char *buffer, size_t buf_size, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvsnprintf(buffer, buf_size, fmt, args);
    va_end(args);
}

/* Helper function to test curl_mvsnprintf with %d */
static void test_mvsnprintf_d(char *buffer, size_t buf_size, int val, ...) {
    va_list args;
    va_start(args, val);
    curl_mvsnprintf(buffer, buf_size, "%d", args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Initialize curl */
    CURL *curl = curl_easy_init();
    if (!curl) return 0;

    /* Prepare buffers for fuzzing */
    size_t buf_size = (Size > 1024) ? 1024 : Size;
    char *buffer = malloc(buf_size + 1);
    if (!buffer) {
        curl_easy_cleanup(curl);
        return 0;
    }
    memset(buffer, 0, buf_size + 1);

    /* Copy input data to buffer, ensuring null termination */
    memcpy(buffer, Data, buf_size);
    buffer[buf_size] = '\0';

    /* Fuzz curl_mvsnprintf with %s */
    test_mvsnprintf_s(buffer, buf_size, "%s", buffer);

    /* Fuzz curl_mvsnprintf with %d */
    int dummy_int = (int)(Data[0] % 256);
    test_mvsnprintf_d(buffer, buf_size, dummy_int);

    /* Fuzz curl_maprintf */
    char *result_maprintf = NULL;
    /* Test with %s */
    result_maprintf = curl_maprintf("%s", buffer);
    if (result_maprintf) {
        (void)strlen(result_maprintf);
        curl_free(result_maprintf);
    }

    /* Test with %d */
    result_maprintf = curl_maprintf("%d", dummy_int);
    if (result_maprintf) {
        (void)strlen(result_maprintf);
        curl_free(result_maprintf);
    }

    /* Fuzz curl_msnprintf */
    int ret_msnprintf = curl_msnprintf(buffer, buf_size, "%s %d", buffer, dummy_int);
    (void)ret_msnprintf;

    /* Fuzz curl_strnequal */
    const char *s1 = buffer;
    const char *s2 = buffer;
    size_t n = (Size > 0) ? (Data[0] % Size) : 0;
    int ret_strnequal = curl_strnequal(s1, s2, n);
    (void)ret_strnequal;

    /* Test with NULL pointers if n > 0 */
    if (n > 0) {
        ret_strnequal = curl_strnequal(NULL, NULL, n);
        (void)ret_strnequal;
    }

    /* Trigger type-checking macros (compile-time only, but included for completeness) */
    /* Example: set a string option to trigger curlcheck_string_option */
    curl_easy_setopt(curl, CURLOPT_URL, buffer);

    /* Example: set a long option */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, (long)(Data[0] % 100));

    /* Example: set a callback option */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, dummy_read_cb);

    /* Cleanup */
    free(buffer);
    curl_easy_cleanup(curl);

    return 0;
}