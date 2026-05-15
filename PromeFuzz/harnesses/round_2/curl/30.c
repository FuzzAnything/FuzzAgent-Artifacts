// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsnprintf at mprintf.c:1077:5 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "curl/curl.h"
#include "curl/mprintf.h"

static char dummy_buffer[1024];

static void helper_mvsnprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvsnprintf(dummy_buffer, sizeof(dummy_buffer), fmt, args);
    va_end(args);
}

static void helper_mvsprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvsprintf(dummy_buffer, fmt, args);
    va_end(args);
}

static char* helper_mvaprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *result = curl_mvaprintf(fmt, args);
    va_end(args);
    return result;
}

static void test_curl_maprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char format[32];
    switch (data[0] % 4) {
        case 0:
            snprintf(format, sizeof(format), "%%s");
            break;
        case 1:
            snprintf(format, sizeof(format), "%%d");
            break;
        case 2:
            snprintf(format, sizeof(format), "%%s %%d");
            break;
        default:
            snprintf(format, sizeof(format), "%%d %%s %%d");
            break;
    }
    
    char *result = NULL;
    if (data[0] % 4 == 0) {
        char str_arg[256];
        size_t str_len = size > 256 ? 256 : size;
        memcpy(str_arg, data, str_len);
        str_arg[str_len - 1] = '\0';
        result = curl_maprintf(format, str_arg);
    } else if (data[0] % 4 == 1) {
        int int_arg = (int)(size > 4 ? *(int*)data : size);
        result = curl_maprintf(format, int_arg);
    } else if (data[0] % 4 == 2) {
        char str_arg[256];
        size_t str_len = size > 256 ? 256 : size;
        memcpy(str_arg, data, str_len);
        str_arg[str_len - 1] = '\0';
        int int_arg = (int)(size > 4 ? *(int*)data : size);
        result = curl_maprintf(format, str_arg, int_arg);
    } else {
        char str_arg[256];
        size_t str_len = size > 256 ? 256 : size;
        memcpy(str_arg, data, str_len);
        str_arg[str_len - 1] = '\0';
        int int_arg1 = (int)(size > 4 ? *(int*)data : size);
        int int_arg2 = (int)(size > 8 ? *((int*)data + 1) : size * 2);
        result = curl_maprintf(format, int_arg1, str_arg, int_arg2);
    }
    
    if (result) {
        curl_free(result);
    }
}

static void test_curl_getenv(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char var_name[256];
    size_t name_len = size > 256 ? 256 : size;
    memcpy(var_name, data, name_len);
    var_name[name_len - 1] = '\0';
    
    char *result = curl_getenv(var_name);
    if (result) {
        curl_free(result);
    }
}

static void test_curl_mvsnprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char str_arg[256];
    size_t str_len = size > 256 ? 256 : size;
    memcpy(str_arg, data, str_len);
    str_arg[str_len - 1] = '\0';
    int int_arg = (int)(size > 4 ? *(int*)data : size);
    
    switch (data[0] % 3) {
        case 0:
            helper_mvsnprintf("%s", str_arg);
            break;
        case 1:
            helper_mvsnprintf("%d", int_arg);
            break;
        case 2:
            helper_mvsnprintf("%s %d", str_arg, int_arg);
            break;
    }
}

static void test_curl_mvsprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char str_arg[256];
    size_t str_len = size > 256 ? 256 : size;
    memcpy(str_arg, data, str_len);
    str_arg[str_len - 1] = '\0';
    int int_arg = (int)(size > 4 ? *(int*)data : size);
    
    switch (data[0] % 3) {
        case 0:
            helper_mvsprintf("%s", str_arg);
            break;
        case 1:
            helper_mvsprintf("%d", int_arg);
            break;
        case 2:
            helper_mvsprintf("%s %d", str_arg, int_arg);
            break;
    }
}

static void test_curl_mvaprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char str_arg[256];
    size_t str_len = size > 256 ? 256 : size;
    memcpy(str_arg, data, str_len);
    str_arg[str_len - 1] = '\0';
    int int_arg = (int)(size > 4 ? *(int*)data : size);
    
    char *result = NULL;
    
    switch (data[0] % 3) {
        case 0:
            result = helper_mvaprintf("%s", str_arg);
            break;
        case 1:
            result = helper_mvaprintf("%d", int_arg);
            break;
        case 2:
            result = helper_mvaprintf("%s %d", str_arg, int_arg);
            break;
    }
    
    if (result) {
        curl_free(result);
    }
}

static void test_curl_free(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *test_str = curl_maprintf("test %d", (int)size);
    if (test_str) {
        curl_free(test_str);
    }
    
    curl_free(NULL);
    
    char *test_str2 = curl_maprintf("another %s", "test");
    if (test_str2) {
        curl_free(test_str2);
    }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    uint8_t selector = Data[0];
    
    if (selector % 7 == 0) {
        test_curl_maprintf(Data, Size);
    }
    if (selector % 5 == 0) {
        test_curl_getenv(Data, Size);
    }
    if (selector % 3 == 0) {
        test_curl_mvsnprintf(Data, Size);
    }
    if (selector % 4 == 0) {
        test_curl_mvsprintf(Data, Size);
    }
    if (selector % 6 == 0) {
        test_curl_mvaprintf(Data, Size);
    }
    if (selector % 2 == 0) {
        test_curl_free(Data, Size);
    }
    
    return 0;
}