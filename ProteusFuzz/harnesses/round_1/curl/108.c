// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* 
 * Fuzz driver for curl API functions:
 * - curl_escape
 * - curl_easy_unescape
 * - curl_mfprintf
 * - curl_mvfprintf
 * Note: Wcurl_* macros are compile-time warnings and cannot be fuzzed at runtime.
 */

static int test_mvfprintf(FILE *f, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = curl_mvfprintf(f, fmt, ap);
    va_end(ap);
    return res;
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file for mfprintf/mvfprintf testing */
    const char *dummy_file = "./dummy_file";
    FILE *fd = fopen(dummy_file, "w");
    if (!fd) {
        return 0;
    }

    /* Initialize curl handle for curl_easy_unescape */
    CURL *handle = curl_easy_init();
    if (!handle) {
        fclose(fd);
        return 0;
    }

    /* Ensure the input data is treated as a string (null-terminate if necessary) */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        fclose(fd);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Test curl_escape */
    char *escaped = curl_escape(input_str, (int)Size);
    if (escaped) {
        /* Optionally use the escaped string */
        (void)strlen(escaped);
        curl_free(escaped);
    }

    /* 2. Test curl_easy_unescape */
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, input_str, (int)Size, &outlength);
    if (unescaped) {
        (void)outlength;
        curl_free(unescaped);
    }

    /* 3. Test curl_mfprintf with various format strings from input */
    /* Use a safe subset of the input as format string to avoid crashes */
    size_t format_len = Size > 20 ? 20 : Size;
    char format_buf[21] = {0};
    if (format_len > 0) {
        memcpy(format_buf, input_str, format_len);
        /* Ensure format string is valid by limiting characters */
        for (size_t i = 0; i < format_len; i++) {
            if (format_buf[i] == '%') {
                /* Replace % with safe char to avoid format string vulnerabilities */
                format_buf[i] = 'x';
            }
        }
    } else {
        strcpy(format_buf, "test");
    }

    int mfprintf_result = curl_mfprintf(fd, format_buf, 123, 456.789, "hello");
    (void)mfprintf_result;

    /* 4. Test curl_mvfprintf with va_list using helper function */
    int mvfprintf_result = test_mvfprintf(fd, format_buf, 123, 456.789, "hello");
    (void)mvfprintf_result;

    /* Cleanup */
    curl_easy_cleanup(handle);
    fclose(fd);
    free(input_str);
    remove(dummy_file);

    return 0;
}