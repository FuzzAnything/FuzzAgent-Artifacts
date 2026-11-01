// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
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

/* Static helper to fuzz curl_mvfprintf with a variable argument list */
static void fuzz_mvfprintf_helper(const char *format, ...) {
    FILE *fd = fopen("./dummy_file", "w");
    if (fd) {
        va_list args;
        va_start(args, format);
        curl_mvfprintf(fd, format, args);
        va_end(args);
        fclose(fd);
    }
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Prepare a null-terminated string from the input data */
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    /* 1. Fuzz curl_escape */
    char *escaped = curl_escape(input_str, Size);
    if (escaped) {
        curl_free(escaped);
    }

    /* 2. Fuzz curl_easy_escape */
    char *escaped_easy = curl_easy_escape(handle, input_str, Size);
    if (escaped_easy) {
        curl_free(escaped_easy);
    }

    /* 3. Fuzz curl_easy_unescape */
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, input_str, Size, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }

    /* 4. Fuzz curl_mprintf */
    /* Ensure format string is safe; use %s with our null-terminated input */
    curl_mprintf("%s", input_str);

    /* 5. Fuzz curl_mfprintf */
    FILE *fd = fopen("./dummy_file", "w");
    if (fd) {
        curl_mfprintf(fd, "%s", input_str);
        fclose(fd);
    }

    /* 6. Fuzz curl_mvfprintf via helper function */
    fuzz_mvfprintf_helper("%s", input_str);

    /* Cleanup */
    curl_easy_cleanup(handle);
    free(input_str);

    return 0;
}