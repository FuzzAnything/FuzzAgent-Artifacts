// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Fuzz driver for libcurl API functions:
 * - curl_easy_escape
 * - curl_easy_unescape
 * - curl_mprintf
 * - curl_escape
 * - curl_msprintf
 *
 * Note: Wcurl_multi_setopt_err_curl_off_t and related macros are compile-time
 * type-checking helpers and do not represent runtime functions to fuzz.
 */

static void setup_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Ensure null-termination for string operations if needed */
    char *input_str = malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* Initialize curl handle */
    CURL *handle = curl_easy_init();
    if (!handle) {
        free(input_str);
        return 0;
    }

    /* Test curl_easy_escape */
    char *escaped = curl_easy_escape(handle, input_str, (int)Size);
    if (escaped) {
        /* Use the escaped string briefly to exercise paths */
        if (escaped[0] == '%') {
            /* Just a no-op to ensure the pointer is used */
            (void)escaped[0];
        }
        curl_free(escaped);
    }

    /* Test curl_easy_unescape */
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, input_str, (int)Size, &outlength);
    if (unescaped) {
        /* Use the unescaped string briefly */
        if (outlength > 0 && unescaped[0] != '\0') {
            (void)unescaped[0];
        }
        curl_free(unescaped);
    }

    /* Test curl_escape (deprecated wrapper) */
    char *escaped_legacy = curl_escape(input_str, (int)Size);
    if (escaped_legacy) {
        if (escaped_legacy[0] == '%') {
            (void)escaped_legacy[0];
        }
        curl_free(escaped_legacy);
    }

    /* Test curl_mprintf with safe format strings derived from input */
    /* Limit format string length to avoid excessive output */
    size_t format_len = (Size < 100) ? Size : 100;
    char *format_str = malloc(format_len + 2);
    if (format_str) {
        memcpy(format_str, input_str, format_len);
        format_str[format_len] = '\0';
        /* Replace any % in format string to avoid format string vulnerabilities */
        for (size_t i = 0; i < format_len; i++) {
            if (format_str[i] == '%') {
                format_str[i] = 'X';
            }
        }
        format_str[format_len] = '\0';
        curl_mprintf("%s", format_str);
        free(format_str);
    }

    /* Test curl_msprintf */
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    /* Use a safe format string */
    if (Size > 0 && Size < 200) {
        char *safe_format = malloc(Size + 2);
        if (safe_format) {
            memcpy(safe_format, input_str, Size);
            safe_format[Size] = '\0';
            for (size_t i = 0; i < Size; i++) {
                if (safe_format[i] == '%') {
                    safe_format[i] = 'X';
                }
            }
            curl_msprintf(buffer, safe_format);
            free(safe_format);
        }
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    free(input_str);

    return 0;
}