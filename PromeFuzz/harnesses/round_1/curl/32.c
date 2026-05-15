// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_unescape at escape.c:163:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    CURL *handle = curl_easy_init();
    if (!handle) return 0;
    
    // Prepare strings for testing
    char *test_str = (char *)malloc(Size + 1);
    if (!test_str) {
        curl_easy_cleanup(handle);
        return 0;
    }
    memcpy(test_str, Data, Size);
    test_str[Size] = '\0';
    
    // Prepare another string for comparison functions
    char *test_str2 = (char *)malloc(Size + 1);
    if (test_str2) {
        memcpy(test_str2, Data, Size);
        test_str2[Size] = '\0';
    }
    
    // Test curl_easy_unescape
    int outlength = 0;
    char *unescaped = curl_easy_unescape(handle, test_str, (int)Size, &outlength);
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // Test with null-terminated string
    unescaped = curl_easy_unescape(handle, test_str, 0, NULL);
    if (unescaped) {
        curl_free(unescaped);
    }
    
    // Test curl_strequal
    if (test_str2) {
        curl_strequal(test_str, test_str2);
        curl_strequal(NULL, NULL);
        curl_strequal(test_str, NULL);
    }
    
    // Test curl_escape
    char *escaped = curl_escape(test_str, (int)Size);
    if (escaped) {
        curl_free(escaped);
    }
    
    // Test with auto-detected length
    escaped = curl_escape(test_str, 0);
    if (escaped) {
        curl_free(escaped);
    }
    
    // Test curl_strnequal
    if (test_str2) {
        size_t n = Size > 0 ? Size % 256 : 0;
        curl_strnequal(test_str, test_str2, n);
        curl_strnequal(NULL, NULL, n);
        curl_strnequal(test_str, NULL, n);
    }
    
    // Test curl_easy_escape
    escaped = curl_easy_escape(handle, test_str, (int)Size);
    if (escaped) {
        curl_free(escaped);
    }
    
    // Test with auto-detected length
    escaped = curl_easy_escape(handle, test_str, 0);
    if (escaped) {
        curl_free(escaped);
    }
    
    // Test curl_unescape
    char *old_unescaped = curl_unescape(test_str, (int)Size);
    if (old_unescaped) {
        curl_free(old_unescaped);
    }
    
    // Test with auto-detected length
    old_unescaped = curl_unescape(test_str, 0);
    if (old_unescaped) {
        curl_free(old_unescaped);
    }
    
    // Test with file operations
    write_dummy_file(Data, Size);
    
    // Cleanup
    free(test_str);
    if (test_str2) free(test_str2);
    curl_easy_cleanup(handle);
    
    return 0;
}