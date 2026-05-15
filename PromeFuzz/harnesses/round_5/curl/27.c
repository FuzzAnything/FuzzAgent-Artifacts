// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_strequal at strequal.c:76:5 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_mprintf at mprintf.c:1194:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_msprintf at mprintf.c:1175:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_strnequal at strequal.c:87:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "/promefuzz/database/curl/latest/code/include/curl/mprintf.h"
#include "/promefuzz/database/curl/latest/code/include/curl/curl.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static void call_curl_mvsprintf(const char *format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    curl_mvsprintf(buffer, format, args);
    va_end(args);
}

static void test_curl_mvsprintf(const uint8_t *data, size_t size) {
    if (size > 0) {
        int len = (int)(data[0] % (size + 1));
        if (len > 0 && size > 1) {
            size_t str_len = (size - 1) < (size_t)len ? (size - 1) : (size_t)len;
            char *str = (char*)malloc(str_len + 1);
            if (str) {
                memcpy(str, data + 1, str_len);
                str[str_len] = '\0';
                call_curl_mvsprintf("Test format: %.*s", len, str);
                free(str);
            }
        } else {
            call_curl_mvsprintf("Test format: %.*s", 0, "");
        }
    }
}

static void test_curl_strnequal(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    size_t s1_len = size > 0 ? data[0] % (size + 1) : 0;
    size_t s2_len = size > 1 ? data[1] % (size + 1) : 0;
    size_t n = size > 2 ? data[2] % (size + 1) : 0;
    
    char *s1 = NULL;
    char *s2 = NULL;
    
    if (s1_len > 0 && s1_len <= size) {
        s1 = (char*)malloc(s1_len + 1);
        if (s1) {
            size_t copy_len = s1_len < size ? s1_len : size;
            memcpy(s1, data, copy_len);
            s1[copy_len] = '\0';
        }
    }
    
    if (s2_len > 0 && s2_len <= size) {
        size_t offset = s1_len < size ? s1_len : 0;
        size_t remaining = size - offset;
        size_t copy_len = s2_len < remaining ? s2_len : remaining;
        s2 = (char*)malloc(s2_len + 1);
        if (s2) {
            if (copy_len > 0) {
                memcpy(s2, data + offset, copy_len);
            }
            s2[copy_len] = '\0';
        }
    }
    
    curl_strnequal(s1, s2, n);
    
    free(s1);
    free(s2);
}

static void test_curl_strequal(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    size_t s1_len = size > 0 ? data[0] % (size + 1) : 0;
    size_t s2_len = size > 1 ? data[1] % (size + 1) : 0;
    
    char *s1 = NULL;
    char *s2 = NULL;
    
    if (s1_len > 0 && s1_len <= size) {
        s1 = (char*)malloc(s1_len + 1);
        if (s1) {
            size_t copy_len = s1_len < size ? s1_len : size;
            memcpy(s1, data, copy_len);
            s1[copy_len] = '\0';
        }
    }
    
    if (s2_len > 0 && s2_len <= size) {
        size_t offset = s1_len < size ? s1_len : 0;
        size_t remaining = size - offset;
        size_t copy_len = s2_len < remaining ? s2_len : remaining;
        s2 = (char*)malloc(s2_len + 1);
        if (s2) {
            if (copy_len > 0) {
                memcpy(s2, data + offset, copy_len);
            }
            s2[copy_len] = '\0';
        }
    }
    
    curl_strequal(s1, s2);
    
    free(s1);
    free(s2);
}

static void test_curl_msnprintf(const uint8_t *data, size_t size) {
    char buffer[1024];
    const char *format = "Test: %.*s %d";
    
    int value = size > 0 ? (int)data[0] : 0;
    size_t str_len = size > 1 ? data[1] % (size + 1) : 0;
    
    if (str_len > 0 && size > 2) {
        size_t copy_len = (size - 2) < str_len ? (size - 2) : str_len;
        char *str = (char*)malloc(copy_len + 1);
        if (str) {
            memcpy(str, data + 2, copy_len);
            str[copy_len] = '\0';
            curl_msnprintf(buffer, sizeof(buffer), format, (int)str_len, str, value);
            free(str);
        } else {
            curl_msnprintf(buffer, sizeof(buffer), format, 0, "", value);
        }
    } else {
        curl_msnprintf(buffer, sizeof(buffer), format, 0, "", value);
    }
}

static void test_curl_mprintf(const uint8_t *data, size_t size) {
    const char *format = "Test: %.*s";
    size_t str_len = size > 0 ? data[0] % (size + 1) : 0;
    
    if (str_len > 0 && size > 1) {
        size_t copy_len = (size - 1) < str_len ? (size - 1) : str_len;
        char *str = (char*)malloc(copy_len + 1);
        if (str) {
            memcpy(str, data + 1, copy_len);
            str[copy_len] = '\0';
            curl_mprintf(format, (int)str_len, str);
            free(str);
        } else {
            curl_mprintf(format, 0, "");
        }
    } else {
        curl_mprintf(format, 0, "");
    }
}

static void test_curl_msprintf(const uint8_t *data, size_t size) {
    char buffer[1024];
    const char *format = "Test: %.*s";
    size_t str_len = size > 0 ? data[0] % (size + 1) : 0;
    
    if (str_len > 0 && size > 1) {
        size_t copy_len = (size - 1) < str_len ? (size - 1) : str_len;
        char *str = (char*)malloc(copy_len + 1);
        if (str) {
            memcpy(str, data + 1, copy_len);
            str[copy_len] = '\0';
            curl_msprintf(buffer, format, (int)str_len, str);
            free(str);
        } else {
            curl_msprintf(buffer, format, 0, "");
        }
    } else {
        curl_msprintf(buffer, format, 0, "");
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    test_curl_mvsprintf(Data, Size);
    test_curl_strnequal(Data, Size);
    test_curl_strequal(Data, Size);
    test_curl_msnprintf(Data, Size);
    test_curl_mprintf(Data, Size);
    test_curl_msprintf(Data, Size);
    
    return 0;
}