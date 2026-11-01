// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

static void safe_free(char *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

static int test_curl_maprintf(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    char format[256];
    size_t fmt_len = (size < 255) ? size : 255;
    memcpy(format, data, fmt_len);
    format[fmt_len] = '\0';
    
    char *result = curl_maprintf(format, 123, "test", 456.789);
    safe_free(result);
    
    result = curl_maprintf(format, 0);
    safe_free(result);
    
    result = curl_maprintf(format, "hello", "world");
    safe_free(result);
    
    return 0;
}

static int test_curl_getenv(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    char var_name[256];
    size_t var_len = (size < 255) ? size : 255;
    memcpy(var_name, data, var_len);
    var_name[var_len] = '\0';
    
    char *env_val = curl_getenv(var_name);
    safe_free(env_val);
    
    env_val = curl_getenv("");
    safe_free(env_val);
    
    return 0;
}

static int test_curl_share_strerror(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    CURLSHcode code = (CURLSHcode)(data[0] & 0xFF);
    const char *str = curl_share_strerror(code);
    (void)str;
    
    code = (CURLSHcode)(data[0] | 0x100);
    str = curl_share_strerror(code);
    (void)str;
    
    return 0;
}

static int test_curl_version(const uint8_t *data, size_t size) {
    (void)data;
    (void)size;
    
    char *ver = curl_version();
    (void)ver;
    
    return 0;
}

static int test_curl_msnprintf(const uint8_t *data, size_t size) {
    if (size < 2) return 0;
    
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    
    char format[256];
    size_t fmt_len = (size < 255) ? size : 255;
    memcpy(format, data, fmt_len);
    format[fmt_len] = '\0';
    
    int ret = curl_msnprintf(buffer, sizeof(buffer), format, 123, "test", 456.789);
    (void)ret;
    
    ret = curl_msnprintf(buffer, 10, format, 123);
    (void)ret;
    
    ret = curl_msnprintf(buffer, sizeof(buffer), format);
    (void)ret;
    
    return 0;
}

static void helper_mvsprintf(char *buffer, size_t buf_size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = curl_mvsprintf(buffer, format, args);
    va_end(args);
    (void)ret;
}

static int test_curl_mvsprintf(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    
    char *buffer = (char *)malloc(4096);
    if (!buffer) return 0;
    
    memset(buffer, 0, 4096);
    
    char format[256];
    size_t fmt_len = (size < 255) ? size : 255;
    memcpy(format, data, fmt_len);
    format[fmt_len] = '\0';
    
    // Test with no arguments
    helper_mvsprintf(buffer, 4096, format);
    
    // Test with one integer argument
    helper_mvsprintf(buffer, 4096, format, 123);
    
    // Test with one string argument
    helper_mvsprintf(buffer, 4096, format, "hello");
    
    // Test with multiple arguments
    helper_mvsprintf(buffer, 4096, format, 123, "test", 456.789);
    
    free(buffer);
    return 0;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_curl_maprintf(Data, Size);
    test_curl_getenv(Data, Size);
    test_curl_share_strerror(Data, Size);
    test_curl_version(Data, Size);
    test_curl_msnprintf(Data, Size);
    test_curl_mvsprintf(Data, Size);
    
    return 0;
}