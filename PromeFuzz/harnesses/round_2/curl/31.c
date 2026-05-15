// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strequal at strequal.c:76:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include "/promefuzz/database/curl/latest/code/include/curl/mprintf.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static int test_mvsnprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    int ret = curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return ret;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    // Split input into two strings for comparison functions
    size_t s1_len = Size / 2;
    size_t s2_len = Size - s1_len;
    
    char *s1 = (char*)malloc(s1_len + 1);
    char *s2 = (char*)malloc(s2_len + 1);
    
    if (!s1 || !s2) {
        free(s1);
        free(s2);
        return 0;
    }
    
    memcpy(s1, Data, s1_len);
    memcpy(s2, Data + s1_len, s2_len);
    s1[s1_len] = '\0';
    s2[s2_len] = '\0';

    // Test curl_strnequal with various lengths
    for (size_t n = 0; n <= s1_len && n <= s2_len; n += (s1_len + s2_len) / 8 + 1) {
        curl_strnequal(s1, s2, n);
        curl_strnequal(NULL, NULL, n);
        curl_strnequal(s1, NULL, n);
        curl_strnequal(NULL, s2, n);
    }

    // Test curl_strequal
    curl_strequal(s1, s2);
    curl_strequal(NULL, NULL);
    curl_strequal(s1, NULL);
    curl_strequal(NULL, s2);

    // Test formatting functions with buffers
    char buffer[256];
    
    // Test curl_msnprintf
    curl_msnprintf(buffer, sizeof(buffer), "%s", s1);
    curl_msnprintf(buffer, sizeof(buffer), "%.*s", (int)(s2_len > 10 ? 10 : s2_len), s2);
    
    // Test curl_mvsnprintf via helper
    test_mvsnprintf("%s", s1);
    test_mvsnprintf("%.*s", (int)(s2_len > 5 ? 5 : s2_len), s2);

    // Test curl_mprintf (output to stdout, but fuzzing typically redirects)
    curl_mprintf("%s", s1);
    
    // Test curl_msprintf
    curl_msprintf(buffer, "%s", s1);
    curl_msprintf(buffer, "test: %.*s", (int)(s2_len > 20 ? 20 : s2_len), s2);

    // Test edge cases with empty strings
    curl_strnequal("", "", 1);
    curl_strequal("", "");
    
    // Test with maximum length
    curl_strnequal(s1, s2, s1_len > s2_len ? s1_len : s2_len);

    free(s1);
    free(s2);
    
    return 0;
}