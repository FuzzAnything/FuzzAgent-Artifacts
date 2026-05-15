// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvfprintf at mprintf.c:1226:5 in mprintf.h
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "curl/mprintf.h"
#include <stdarg.h>
#include <unistd.h>

static void test_curl_mvaprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_mvaprintf(format, args);
    va_end(args);
    
    if (result) {
        curl_free(result);
    }
}

static void test_curl_mvsprintf(const char *format, ...) {
    char buffer[1024];
    
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_curl_maprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *result = curl_maprintf(format, args);
    va_end(args);
    
    if (result) {
        curl_free(result);
    }
}

static void test_curl_mvfprintf(const char *format, ...) {
    FILE *file = fopen("./dummy_file", "w");
    if (!file) return;
    
    va_list args;
    va_start(args, format);
    curl_mvfprintf(file, format, args);
    va_end(args);
    
    fclose(file);
}

static void test_curl_mvsnprintf(const char *format, ...) {
    char buffer[1024];
    
    va_list args;
    va_start(args, format);
    curl_mvsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
}

static void test_curl_mvprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    curl_mvprintf(format, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    size_t format_len = remaining_size / 2;
    size_t arg_len = remaining_size - format_len;
    
    if (format_len == 0 || arg_len == 0) return 0;
    
    char *format = (char*)malloc(format_len + 1);
    if (!format) return 0;
    
    memcpy(format, remaining_data, format_len);
    format[format_len] = '\0';
    
    char *arg = (char*)malloc(arg_len + 1);
    if (!arg) {
        free(format);
        return 0;
    }
    
    memcpy(arg, remaining_data + format_len, arg_len);
    arg[arg_len] = '\0';
    
    switch (selector % 6) {
        case 0:
            test_curl_mvaprintf(format, arg);
            break;
        case 1:
            test_curl_mvsprintf(format, arg);
            break;
        case 2:
            test_curl_maprintf(format, arg);
            break;
        case 3:
            test_curl_mvfprintf(format, arg);
            break;
        case 4:
            test_curl_mvsnprintf(format, arg);
            break;
        case 5:
            test_curl_mvprintf(format, arg);
            break;
    }
    
    free(format);
    free(arg);
    
    return 0;
}