// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static void setup_curl_handle(CURL **handle) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    *handle = curl_easy_init();
}

static void cleanup_curl_handle(CURL *handle) {
    if (handle) {
        curl_easy_cleanup(handle);
    }
    curl_global_cleanup();
}

static void free_result(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    CURL *handle = NULL;
    setup_curl_handle(&handle);

    // Ensure the input is treated as a valid C string by allocating a buffer with a null terminator
    char *input_buf = (char *)malloc(Size + 1);
    if (!input_buf) {
        return 0;
    }
    memcpy(input_buf, Data, Size);
    input_buf[Size] = '\0';

    const char *input_str = input_buf;
    int length = (int)Size;

    // Test curl_unescape
    char *unescaped_legacy = curl_unescape(input_str, length);
    free_result(unescaped_legacy);

    // Test curl_unescape with -1 length (null terminated)
    char *unescaped_legacy_null = curl_unescape(input_str, -1);
    free_result(unescaped_legacy_null);

    // Test curl_easy_unescape
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, input_str, length, &outlength);
    free_result(unescaped);

    // Test curl_easy_unescape with -1 length
    char *unescaped_null = curl_easy_unescape(handle, input_str, -1, &outlength);
    free_result(unescaped_null);

    // Test curl_easy_escape
    char *escaped = curl_easy_escape(handle, input_str, length);
    free_result(escaped);

    // Test curl_easy_escape with -1 length
    char *escaped_null = curl_easy_escape(handle, input_str, -1);
    free_result(escaped_null);

    // Test curl_strequal
    // Ensure both pointers point to valid null-terminated strings
    char *str1 = input_buf;
    char *str2 = input_buf;
    
    (void)curl_strequal(str1, str2);
    (void)curl_strequal(str1, NULL);
    (void)curl_strequal(NULL, str2);
    (void)curl_strequal(NULL, NULL);

    // Test curl_escape (legacy)
    char *escaped_legacy = curl_escape(input_str, length);
    free_result(escaped_legacy);

    // Test curl_escape with -1 length
    char *escaped_legacy_null = curl_escape(input_str, -1);
    free_result(escaped_legacy_null);

    // Test curl_mprintf with various formats
    // We use the input buffer as the format string, but ensure it's null-terminated
    // To prevent potential issues with format specifiers in fuzzed data, we limit usage
    // or use a safe format string with the data as an argument.
    // Here we try to use the data as a format string, but only if it's small and safe,
    // otherwise we print it safely.
    if (Size < 100) {
        // Safe to try as format string if small, but risky if it contains %
        // A safer approach for fuzzing is to use %s
        curl_mprintf("%s", input_str);
    } else {
        // For larger inputs, just print a prefix to avoid huge outputs
        curl_mprintf("Input size: %zu", Size);
    }

    free(input_buf);
    cleanup_curl_handle(handle);
    return 0;
}