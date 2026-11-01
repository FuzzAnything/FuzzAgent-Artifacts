// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Ensure the input is treated as a string by creating a safe copy with null terminator */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Test curl_escape */
    char *escaped = curl_escape(input_str, (int)Size);
    if (escaped) {
        curl_free(escaped);
    }

    /* 2. Test curl_easy_unescape */
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, input_str, (int)Size, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }

    /* 3. Test curl_strequal */
    /* Compare input with itself and with NULL */
    (void)curl_strequal(input_str, input_str);
    (void)curl_strequal(input_str, NULL);
    (void)curl_strequal(NULL, NULL);

    /* 4. Test curl_unescape */
    char *legacy_unescaped = curl_unescape(input_str, (int)Size);
    if (legacy_unescaped) {
        curl_free(legacy_unescaped);
    }
    /* Test with -1 length (null terminated) */
    char *legacy_unescaped_2 = curl_unescape(input_str, -1);
    if (legacy_unescaped_2) {
        curl_free(legacy_unescaped_2);
    }

    /* 5. Test curl_easy_setopt with various types to trigger type checking macros */
    /* We use dummy values to exercise the API paths without needing real resources */
    
    /* String option */
    curl_easy_setopt(handle, CURLOPT_URL, input_str);
    
    /* Long option */
    long dummy_long = (long)(Size % 1000);
    curl_easy_setopt(handle, CURLOPT_TIMEOUT, dummy_long);
    
    /* Off_t option */
    curl_off_t dummy_off = (curl_off_t)(Size);
    curl_easy_setopt(handle, CURLOPT_MAXFILESIZE, dummy_off);
    
    /* Callback option (using NULL to avoid complex setup, but tests the macro path) */
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, NULL);
    
    /* Error buffer */
    char error_buf[CURL_ERROR_SIZE];
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buf);

    /* 6. Test curl_msprintf */
    char buffer[256];
    /* Use a safe format string derived from input to avoid format string vulnerabilities */
    /* We limit the format string to simple characters to ensure safety */
    char safe_format[64];
    size_t format_len = (Size < 63) ? Size : 63;
    memcpy(safe_format, input_str, format_len);
    safe_format[format_len] = '\0';
    
    /* Remove potential format specifiers to prevent undefined behavior */
    for (size_t i = 0; i < format_len; i++) {
        if (safe_format[i] == '%') {
            safe_format[i] = 'X';
        }
    }
    
    int msprintf_result = curl_msprintf(buffer, safe_format, input_str);
    (void)msprintf_result; /* Suppress unused result warning */

    /* Cleanup */
    curl_easy_cleanup(handle);
    free(input_str);

    return 0;
}