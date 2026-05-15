// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include "curl/curl.h"
#include "curl/multi.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static char *call_curl_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    return result;
}

static int call_curl_mvsnprintf(char *buffer, size_t maxlength, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int result = curl_mvsnprintf(buffer, maxlength, format, args);
    va_end(args);
    return result;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create null-terminated strings from fuzzer input
    char *str = (char *)malloc(Size + 1);
    if (!str) return 0;
    memcpy(str, Data, Size);
    str[Size] = '\0';
    
    // Test curl_maprintf
    char *result1 = curl_maprintf("%s", str);
    if (result1) curl_free(result1);
    
    // Test curl_mvaprintf
    char *result2 = call_curl_mvaprintf("%s", str);
    if (result2) curl_free(result2);
    
    // Test curl_mvsnprintf
    char buffer[1024];
    call_curl_mvsnprintf(buffer, sizeof(buffer), "%s", str);
    
    // Test curl_getenv
    // First set an environment variable with the fuzzer data
    FILE *env_file = fopen("./dummy_file", "w");
    if (env_file) {
        fwrite(Data, 1, Size, env_file);
        fclose(env_file);
    }
    
    // Use the fuzzer input as environment variable name
    char *env_result = curl_getenv(str);
    if (env_result) curl_free(env_result);
    
    // Test curl_pushheader_bynum and curl_pushheader_byname
    // These require a valid curl_pushheaders structure which is opaque
    // We can only pass NULL to test error handling
    char *push_result1 = curl_pushheader_bynum(NULL, 0);
    char *push_result2 = curl_pushheader_byname(NULL, str);
    
    free(str);
    return 0;
}