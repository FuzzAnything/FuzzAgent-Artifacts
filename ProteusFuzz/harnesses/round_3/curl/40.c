// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
// curl_mfprintf at mprintf.c:1204:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>
#include <limits.h>

static void cleanup_string(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

static void test_mvfprintf(FILE *fd, const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvfprintf(fd, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize CURL handle
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Ensure input is null-terminated for string functions
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    int length = (Size < INT_MAX) ? (int)Size : INT_MAX;
    int outlength = 0;
    char *result = NULL;

    // Fuzz curl_unescape
    result = curl_unescape(input_str, -1);
    cleanup_string(result);
    result = curl_unescape(input_str, length);
    cleanup_string(result);

    // Fuzz curl_easy_unescape
    result = curl_easy_unescape(handle, input_str, -1, NULL);
    cleanup_string(result);
    result = curl_easy_unescape(handle, input_str, length, &outlength);
    cleanup_string(result);
    result = curl_easy_unescape(handle, input_str, 0, &outlength);
    cleanup_string(result);

    // Fuzz curl_easy_escape
    result = curl_easy_escape(handle, input_str, -1);
    cleanup_string(result);
    result = curl_easy_escape(handle, input_str, length);
    cleanup_string(result);
    result = curl_easy_escape(handle, input_str, 0);
    cleanup_string(result);

    // Fuzz curl_escape
    result = curl_escape(input_str, -1);
    cleanup_string(result);
    result = curl_escape(input_str, length);
    cleanup_string(result);
    result = curl_escape(input_str, 0);
    cleanup_string(result);

    // Fuzz curl_mfprintf and curl_mvfprintf
    FILE *fd = fopen("./dummy_file", "w");
    if (fd) {
        // Test curl_mfprintf
        curl_mfprintf(fd, "%s", input_str);
        curl_mfprintf(fd, "%d", (int)Size);
        curl_mfprintf(fd, "%s %d", input_str, (int)Size);
        curl_mfprintf(fd, "");
        curl_mfprintf(fd, "%%");

        // Test curl_mvfprintf via helper function
        test_mvfprintf(fd, "%s", input_str);
        test_mvfprintf(fd, "%d", (int)Size);
        test_mvfprintf(fd, "%s %d", input_str, (int)Size);
        test_mvfprintf(fd, "");
        test_mvfprintf(fd, "%%");

        fclose(fd);
    }

    // Cleanup
    free(input_str);
    curl_easy_cleanup(handle);

    return 0;
}