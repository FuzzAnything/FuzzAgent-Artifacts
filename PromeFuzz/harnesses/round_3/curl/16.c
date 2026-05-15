// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getdate at parsedate.c:561:8 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include "curl/curl.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

static void test_curl_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

static char *test_curl_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

static int test_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

static int test_curl_mvsprintf(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create null-terminated string from fuzzer input
    char *input_str = (char *)malloc(Size + 1);
    if (!input_str) return 0;
    memcpy(input_str, Data, Size);
    input_str[Size] = '\0';
    
    // Test curl_mvprintf
    test_curl_mvprintf("%s", input_str);
    
    // Test curl_mvaprintf
    char *result1 = test_curl_mvaprintf("%s", input_str);
    if (result1) curl_free(result1);
    
    // Test curl_mvsnprintf
    char buffer1[1024];
    test_curl_mvsnprintf(buffer1, sizeof(buffer1), "%s", input_str);
    
    // Test curl_maprintf
    char *result2 = curl_maprintf("%s", input_str);
    if (result2) curl_free(result2);
    
    // Test curl_getdate
    time_t date_result = curl_getdate(input_str, NULL);
    (void)date_result;
    
    // Test curl_mvsprintf
    char buffer2[1024];
    test_curl_mvsprintf(buffer2, "%s", input_str);
    
    free(input_str);
    return 0;
}

#ifdef __cplusplus
}
#endif