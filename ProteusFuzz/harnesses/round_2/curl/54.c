// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>
#include <stdarg.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

// Helper function to test curl_mvsprintf with a variadic wrapper
static void test_mvsprintf(const char *format, ...) {
    char buffer_mvsprintf[256];
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer_mvsprintf, format, args);
    va_end(args);
    (void)ret;
}

// Helper function to test curl_mvaprintf with a variadic wrapper
static char* test_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for string operations
    char *input_str = NULL;
    if (Size > 0) {
        input_str = malloc(Size + 1);
        if (!input_str) return 0;
        memcpy(input_str, Data, Size);
        input_str[Size] = '\0';
    } else {
        input_str = strdup("");
        if (!input_str) return 0;
    }

    // Write dummy file for potential file-based operations
    write_dummy_file(Data, Size);

    // Test curl_msnprintf
    char buffer_msnprintf[256];
    if (input_str) {
        int ret1 = curl_msnprintf(buffer_msnprintf, sizeof(buffer_msnprintf), "%s", input_str);
        (void)ret1;
    }

    // Test curl_maprintf
    char *result_maprintf = NULL;
    if (input_str) {
        result_maprintf = curl_maprintf("%s", input_str);
        if (result_maprintf) {
            curl_free(result_maprintf);
        }
    }

    // Test curl_mvsprintf via helper
    if (input_str) {
        test_mvsprintf("%s", input_str);
    }

    // Test curl_mvaprintf via helper
    char *result_mvaprintf = NULL;
    if (input_str) {
        result_mvaprintf = test_mvaprintf("%s", input_str);
        if (result_mvaprintf) {
            curl_free(result_mvaprintf);
        }
    }

    // Test curl_escape
    char *result_escape = NULL;
    if (input_str) {
        result_escape = curl_escape(input_str, (int)strlen(input_str));
        if (result_escape) {
            curl_free(result_escape);
        }
    }

    // Cleanup
    if (input_str) {
        free(input_str);
    }

    return 0;
}