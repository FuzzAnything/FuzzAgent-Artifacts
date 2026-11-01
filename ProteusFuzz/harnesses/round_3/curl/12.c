// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
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
#include <stdarg.h>

/* Fuzz driver for curl URL encoding/decoding and formatting functions */

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize libcurl global environment */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* Create a CURL handle for functions that require it */
    CURL *handle = curl_easy_init();
    if (!handle) {
        curl_global_cleanup();
        return 0;
    }

    /* Prepare input string from fuzz data, ensuring null termination */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        curl_global_cleanup();
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* Test curl_unescape (legacy wrapper) */
    {
        char *unescaped = curl_unescape(input_str, (int)Size);
        if (unescaped) {
            /* Use the result briefly to exercise logic */
            (void)strlen(unescaped);
            curl_free(unescaped);
        }

        /* Test with length -1 (null terminated) */
        unescaped = curl_unescape(input_str, -1);
        if (unescaped) {
            (void)strlen(unescaped);
            curl_free(unescaped);
        }
    }

    /* Test curl_easy_unescape */
    {
        int outlen = 0;
        char *unescaped = curl_easy_unescape(handle, input_str, (int)Size, &outlen);
        if (unescaped) {
            (void)strlen(unescaped);
            (void)outlen;
            curl_free(unescaped);
        }

        /* Test with length -1 */
        unescaped = curl_easy_unescape(handle, input_str, -1, &outlen);
        if (unescaped) {
            (void)strlen(unescaped);
            (void)outlen;
            curl_free(unescaped);
        }
    }

    /* Test curl_easy_escape */
    {
        char *escaped = curl_easy_escape(handle, input_str, (int)Size);
        if (escaped) {
            (void)strlen(escaped);
            curl_free(escaped);
        }

        /* Test with length 0 (null terminated) */
        escaped = curl_easy_escape(handle, input_str, 0);
        if (escaped) {
            (void)strlen(escaped);
            curl_free(escaped);
        }
    }

    /* Test curl_msprintf */
    {
        /* Allocate a buffer large enough for the input plus format overhead */
        size_t buf_size = Size + 64;
        char *buffer = (char *)malloc(buf_size);
        if (buffer) {
            /* Try a simple format string */
            int ret = curl_msprintf(buffer, "%s", input_str);
            (void)ret;

            /* Try a format with length specifier */
            ret = curl_msprintf(buffer, "%.*s", (int)Size, input_str);
            (void)ret;

            /* Try a format that might overflow if buffer is too small */
            ret = curl_msprintf(buffer, "prefix: %s suffix", input_str);
            (void)ret;

            free(buffer);
        }
    }

    /* Test curl_mprintf */
    {
        /* Suppress output by redirecting or just calling it */
        /* Note: curl_mprintf writes to stdout, which is acceptable in fuzzing */
        int ret = curl_mprintf("%s", input_str);
        (void)ret;

        ret = curl_mprintf("%.*s", (int)Size, input_str);
        (void)ret;
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    curl_global_cleanup();
    free(input_str);

    return 0;
}