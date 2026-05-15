// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include "curl/curl.h"
#include <stdarg.h>

static void test_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    if (result) {
        curl_free(result);
    }
}

static void test_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static void test_mvsprintf(char* buffer, const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create a null-terminated string from the fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_getenv
    char *env_result = curl_getenv(input_str);
    if (env_result) {
        curl_free(env_result);
    }

    // Test curl_maprintf with simple format string
    char *maprintf_result = curl_maprintf("%s", input_str);
    if (maprintf_result) {
        curl_free(maprintf_result);
    }

    // Test curl_mvaprintf via helper function
    test_mvaprintf("%s", input_str);

    // Test curl_mvprintf via helper function
    test_mvprintf("%s", input_str);

    // Test curl_mvsprintf
    size_t buffer_size = Size * 2 + 100; // Extra space for formatting
    char *buffer = (char *)malloc(buffer_size);
    if (buffer) {
        test_mvsprintf(buffer, "%s", input_str);
        free(buffer);
    }

    // Test curl_free with NULL (should be safe)
    curl_free(NULL);

    // Test curl_maprintf with different format strings
    char *maprintf_result2 = curl_maprintf("Test: %s", input_str);
    if (maprintf_result2) {
        curl_free(maprintf_result2);
    }

    // Test curl_maprintf with empty string
    char *empty_result = curl_maprintf("");
    if (empty_result) {
        curl_free(empty_result);
    }

    // Test curl_getenv with empty string
    char *empty_env = curl_getenv("");
    if (empty_env) {
        curl_free(empty_env);
    }

    // Create a dummy file for any potential file operations
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    free(input_str);
    return 0;
}