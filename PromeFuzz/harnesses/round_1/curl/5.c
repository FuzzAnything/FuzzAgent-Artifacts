// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_pushheader_bynum at http2.c:2997:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_pushheader_byname at http2.c:3004:7 in multi.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "curl/curl.h"
#include "curl/mprintf.h"
#include "curl/multi.h"

static int helper_mvsnprintf(const char *fmt, ...) {
    char buf[256];
    va_list args;
    va_start(args, fmt);
    int result = curl_mvsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return result;
}

static char *helper_mvaprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = curl_mvaprintf(fmt, args);
    va_end(args);
    return result;
}

static void test_curl_maprintf(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *format = malloc(size + 1);
    if (!format) return;
    
    memcpy(format, data, size);
    format[size] = '\0';
    
    char *result = curl_maprintf("%s", format);
    if (result) {
        curl_free(result);
    }
    
    int num = size;
    result = curl_maprintf("Number: %d", num);
    if (result) {
        curl_free(result);
    }
    
    free(format);
}

static void test_curl_getenv(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *var_name = malloc(size + 1);
    if (!var_name) return;
    
    memcpy(var_name, data, size);
    var_name[size] = '\0';
    
    char *result = curl_getenv(var_name);
    if (result) {
        curl_free(result);
    }
    
    free(var_name);
}

static void test_curl_mvsnprintf(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *format = malloc(size + 1);
    if (!format) return;
    
    memcpy(format, data, size);
    format[size] = '\0';
    
    int result = helper_mvsnprintf("%s", format);
    
    int num = 42;
    result = helper_mvsnprintf("%d", num);
    
    free(format);
}

static void test_curl_mvaprintf(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    char *format = malloc(size + 1);
    if (!format) return;
    
    memcpy(format, data, size);
    format[size] = '\0';
    
    char *result = helper_mvaprintf("%s", format);
    if (result) {
        curl_free(result);
    }
    
    int num = 42;
    result = helper_mvaprintf("%d", num);
    if (result) {
        curl_free(result);
    }
    
    free(format);
}

static void test_curl_pushheader_functions(const uint8_t *data, size_t size) {
    for (size_t i = 0; i < 10; i++) {
        char *result = curl_pushheader_bynum(NULL, i);
    }
    
    if (size > 0) {
        char *header_name = malloc(size + 1);
        if (!header_name) return;
        
        memcpy(header_name, data, size);
        header_name[size] = '\0';
        
        char *result = curl_pushheader_byname(NULL, header_name);
        
        result = curl_pushheader_byname(NULL, ":path");
        result = curl_pushheader_byname(NULL, ":method");
        result = curl_pushheader_byname(NULL, ":scheme");
        result = curl_pushheader_byname(NULL, ":authority");
        
        free(header_name);
    }
}

int LLVMFuzzerTestOneInput_5(const uint8_t *data, size_t size) {
    static int curl_initialized = 0;
    if (!curl_initialized) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_initialized = 1;
    }
    
    test_curl_maprintf(data, size);
    test_curl_getenv(data, size);
    test_curl_mvsnprintf(data, size);
    test_curl_mvaprintf(data, size);
    test_curl_pushheader_functions(data, size);
    
    return 0;
}