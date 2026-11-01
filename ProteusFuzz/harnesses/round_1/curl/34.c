// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_easy_reset at easy.c:1083:6 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void test_mvfprintf(FILE *fp, const char *format_str, ...) {
    va_list args;
    va_start(args, format_str);
    curl_mvfprintf(fp, format_str, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Prepare environment */
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Ensure we have at least some data for format strings */
    const char *format_str = (const char *)Data;
    size_t max_len = Size > 1024 ? 1024 : Size;
    char safe_format[1025];
    memcpy(safe_format, Data, max_len);
    safe_format[max_len] = '\0';

    /* 1. Test curl_mfprintf */
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        curl_mfprintf(fp, safe_format, "test", 123, 45.6);
        fclose(fp);
    }

    /* 2. Test curl_mprintf */
    curl_mprintf(safe_format, "test", 123);

    /* 3. Test curl_easy_unescape */
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, (const char *)Data, (int)Size, &outlength);
    if (unescaped) {
        free(unescaped);
    }

    /* 4. Test curl_msprintf */
    char buffer[4096];
    curl_msprintf(buffer, safe_format, "test", 123);

    /* 5. Test curl_easy_reset */
    curl_easy_reset(handle);

    /* 6. Test curl_mvfprintf */
    fp = fopen("./dummy_file", "w");
    if (fp) {
        test_mvfprintf(fp, safe_format, 123, 45.6);
        fclose(fp);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);
    return 0;
}