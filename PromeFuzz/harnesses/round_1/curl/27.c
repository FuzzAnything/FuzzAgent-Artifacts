// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize CURL handle
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    // Create null-terminated string from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';

    // Test curl_easy_unescape
    int outlength;
    char *unescaped = curl_easy_unescape(handle, input_str, Size, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }

    // Test curl_strequal
    // Compare input string with itself and with NULL
    curl_strequal(input_str, input_str);
    curl_strequal(NULL, NULL);
    curl_strequal(input_str, NULL);
    curl_strequal(NULL, input_str);

    // Test curl_escape
    char *escaped = curl_escape(input_str, Size);
    if (escaped) {
        curl_free(escaped);
    }

    // Test curl_strnequal
    // Compare first n characters where n is derived from input size
    size_t n = Size > 0 ? (Data[0] % Size) + 1 : 0;
    curl_strnequal(input_str, input_str, n);
    curl_strnequal(NULL, NULL, n);
    curl_strnequal(input_str, NULL, n);
    curl_strnequal(NULL, input_str, n);

    // Test curl_easy_escape
    char *easy_escaped = curl_easy_escape(handle, input_str, Size);
    if (easy_escaped) {
        curl_free(easy_escaped);
    }

    // Test curl_unescape
    char *old_unescaped = curl_unescape(input_str, Size);
    if (old_unescaped) {
        curl_free(old_unescaped);
    }

    // Cleanup
    free(input_str);
    curl_easy_cleanup(handle);

    return 0;
}