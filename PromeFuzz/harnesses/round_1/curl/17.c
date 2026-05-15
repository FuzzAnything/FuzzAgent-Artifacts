// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "mprintf.h"

static void helper_mvsprintf(char *buffer, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvsprintf(buffer, fmt, args);
    va_end(args);
}

static void helper_mvprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvprintf(fmt, args);
    va_end(args);
}

static void helper_mvfprintf(FILE *f, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvfprintf(f, fmt, args);
    va_end(args);
}

static void helper_mvaprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = curl_mvaprintf(fmt, args);
    if (result) {
        curl_free(result);
    }
    va_end(args);
}

static void test_curl_msprintf(const uint8_t *data, size_t size) {
    char buffer[1024];
    char format[101];
    
    size_t format_len = size > 100 ? 100 : size;
    if (format_len > 0) {
        memcpy(format, data, format_len);
        format[format_len] = '\0';
        
        curl_msprintf(buffer, "%s", format);
        
        if (size > format_len + 10) {
            char str_arg[11];
            size_t str_len = size - format_len > 10 ? 10 : size - format_len;
            memcpy(str_arg, data + format_len, str_len);
            str_arg[str_len] = '\0';
            curl_msprintf(buffer, "%s %s", format, str_arg);
        }
    }
}

static void test_curl_mvsprintf(const uint8_t *data, size_t size) {
    char buffer[1024];
    char format[101];
    
    size_t format_len = size > 100 ? 100 : size;
    if (format_len > 0) {
        memcpy(format, data, format_len);
        format[format_len] = '\0';
        
        helper_mvsprintf(buffer, "%s", format);
    }
}

static void test_curl_maprintf(const uint8_t *data, size_t size) {
    char format[101];
    
    size_t format_len = size > 100 ? 100 : size;
    if (format_len > 0) {
        memcpy(format, data, format_len);
        format[format_len] = '\0';
        
        char *result = curl_maprintf("%s", format);
        if (result) {
            curl_free(result);
        }
        
        if (size > format_len + 10) {
            char str_arg[11];
            size_t str_len = size - format_len > 10 ? 10 : size - format_len;
            memcpy(str_arg, data + format_len, str_len);
            str_arg[str_len] = '\0';
            
            result = curl_maprintf("%s %s", format, str_arg);
            if (result) {
                curl_free(result);
            }
        }
    }
}

static void test_curl_mvprintf(const uint8_t *data, size_t size) {
    char format[101];
    
    size_t format_len = size > 100 ? 100 : size;
    if (format_len > 0) {
        memcpy(format, data, format_len);
        format[format_len] = '\0';
        
        helper_mvprintf("%s", format);
    }
}

static void test_curl_mvfprintf(const uint8_t *data, size_t size) {
    char format[101];
    
    size_t format_len = size > 100 ? 100 : size;
    if (format_len > 0) {
        memcpy(format, data, format_len);
        format[format_len] = '\0';
        
        FILE *fp = fopen("./dummy_file", "w");
        if (fp) {
            helper_mvfprintf(fp, "%s", format);
            fclose(fp);
        }
    }
}

static void test_curl_mvaprintf(const uint8_t *data, size_t size) {
    char format[101];
    
    size_t format_len = size > 100 ? 100 : size;
    if (format_len > 0) {
        memcpy(format, data, format_len);
        format[format_len] = '\0';
        
        helper_mvaprintf("%s", format);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    FILE *fp = fopen("./dummy_file", "w");
    if (fp) {
        fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
        fclose(fp);
    }
    
    test_curl_msprintf(Data, Size);
    test_curl_mvsprintf(Data, Size);
    test_curl_maprintf(Data, Size);
    test_curl_mvprintf(Data, Size);
    test_curl_mvfprintf(Data, Size);
    test_curl_mvaprintf(Data, Size);
    
    return 0;
}