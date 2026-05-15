// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include "curl/mprintf.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static char* helper_create_string(const uint8_t* data, size_t size) {
    char* str = (char*)malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

// Helper function to test curl_mvsprintf
static void test_mvsprintf(const char* format, ...) {
    char buffer[4096];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

// Helper function to test curl_mvaprintf
static void test_mvaprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char* result = curl_mvaprintf(format, args);
    va_end(args);
    if (result) curl_free(result);
}

// Helper function to test curl_mvprintf
static void test_mvprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_3(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a null-terminated string from fuzzer input
    char* input_str = helper_create_string(Data, Size);
    if (!input_str) return 0;

    // Test curl_maprintf with various format strings
    char* result1 = curl_maprintf("%s", input_str);
    if (result1) curl_free(result1);

    char* result2 = curl_maprintf("Test: %s", input_str);
    if (result2) curl_free(result2);

    // Test curl_mvsprintf using helper function
    test_mvsprintf("%s", input_str);

    // Test curl_mvaprintf using helper function
    test_mvaprintf("%s", input_str);

    // Test curl_getenv - use a fixed variable name since we can't set env vars
    char* env_result = curl_getenv("PATH");
    if (env_result) curl_free(env_result);

    // Test curl_mvprintf using helper function
    test_mvprintf("%s", input_str);

    // Test with different format specifiers
    if (Size > 4) {
        int int_val = *(int*)Data;
        char* result4 = curl_maprintf("Number: %d, String: %s", int_val, input_str);
        if (result4) curl_free(result4);
    }

    // Test with empty format string
    char* result5 = curl_maprintf("");
    if (result5) curl_free(result5);

    // Test curl_free with NULL (should be safe)
    curl_free(NULL);

    free(input_str);
    return 0;
}