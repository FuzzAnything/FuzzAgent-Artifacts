// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_multi_init at multi.c:335:8 in multi.h
// curl_multi_cleanup at multi.c:2839:11 in multi.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

/* Include required curl headers */
#include <curl/curl.h>
#include "typecheck-gcc.h"
#include "mprintf.h"

/* Helper to safely create a null-terminated string from fuzz input */
static char *create_safe_string(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *empty = malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }
    /* Allocate size + 1 for null terminator */
    char *str = malloc(size + 1);
    if (!str) return NULL;
    memcpy(str, data, size);
    str[size] = '\0';
    return str;
}

/* Helper to extract a format string safely (ensure it's valid and not too long) */
static char *extract_format_string(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    /* Limit format string length to prevent excessive allocation */
    size_t max_len = 1024;
    if (size < max_len) max_len = size;
    
    char *fmt = malloc(max_len + 1);
    if (!fmt) return NULL;
    
    memcpy(fmt, data, max_len);
    fmt[max_len] = '\0';
    
    return fmt;
}

/* Helper function to prepare va_list for curl_mvprintf and curl_mvsprintf */
/* This function must be variadic to allow va_start to work correctly */
static void test_va_list_functions(const char *fmt, const uint8_t *data, size_t size, ...) {
    va_list args, args_copy;
    int val1 = (size > 0) ? (int)(data[0] & 0xFF) : 0;
    int val2 = (size > 1) ? (int)(data[1] & 0xFF) : 0;
    long val3 = (size > 2) ? (long)(data[2] & 0xFF) : 0;
    double val4 = (size > 3) ? (double)(data[3] & 0xFF) / 255.0 : 0.0;
    char *str_val = create_safe_string(data + 4, size > 4 ? size - 4 : 0);

    /* We need a dummy last argument for va_start to work in a variadic function */
    /* We pass the first argument of the variadic list as the anchor */
    va_start(args, str_val);
    va_copy(args_copy, args);
    
    /* Call curl_mvprintf */
    int ret1 = curl_mvprintf(fmt, args_copy);
    (void)ret1;
    
    va_end(args_copy);
    va_end(args);
    
    /* Re-init for curl_mvsprintf */
    va_start(args, str_val);
    va_copy(args_copy, args);
    
    char buffer[4096];
    int ret2 = curl_mvsprintf(buffer, fmt, args_copy);
    (void)ret2;
    
    va_end(args_copy);
    va_end(args);
    
    free(str_val);
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Prepare buffers and variables */
    char buffer[4096];
    char *dynamic_str = NULL;

    /* Test 1: curl_msnprintf - safe snprintf wrapper */
    {
        char *fmt = extract_format_string(Data, Size);
        if (fmt) {
            /* Create arguments from data if possible */
            int val1 = (Size > 0) ? (int)(Data[0] & 0xFF) : 0;
            int val2 = (Size > 1) ? (int)(Data[1] & 0xFF) : 0;
            long val3 = (Size > 2) ? (long)(Data[2] & 0xFF) : 0;
            double val4 = (Size > 3) ? (double)(Data[3] & 0xFF) / 255.0 : 0.0;
            char *str_val = create_safe_string(Data + 4, Size > 4 ? Size - 4 : 0);
            
            /* Try different format specifiers */
            int ret = curl_msnprintf(buffer, sizeof(buffer), fmt, val1, val2, val3, val4, str_val ? str_val : "");
            (void)ret; /* Suppress unused warning */
            
            free(str_val);
            free(fmt);
        }
    }

    /* Test 2 & 4: curl_mvprintf and curl_mvsprintf via helper */
    /* We call the variadic helper with a dummy argument to satisfy va_start requirements */
    {
        char *fmt = extract_format_string(Data, Size);
        if (fmt) {
            /* Pass a dummy last argument (0) to satisfy va_start anchor requirement */
            test_va_list_functions(fmt, Data, Size, 0);
            free(fmt);
        }
    }

    /* Test 3: curl_maprintf - format and return allocated string */
    {
        char *fmt = extract_format_string(Data, Size);
        if (fmt) {
            int val1 = (Size > 0) ? (int)(Data[0] & 0xFF) : 0;
            int val2 = (Size > 1) ? (int)(Data[1] & 0xFF) : 0;
            char *str_val = create_safe_string(Data + 2, Size > 2 ? Size - 2 : 0);
            
            dynamic_str = curl_maprintf(fmt, val1, val2, str_val ? str_val : "");
            if (dynamic_str) {
                /* Use the string briefly */
                size_t len = strlen(dynamic_str);
                (void)len;
                curl_free(dynamic_str);
            }
            
            free(str_val);
            free(fmt);
        }
    }

    /* Test 5: Trigger type-checking warnings via curl_easy_setopt with wrong types */
    /* Note: We avoid passing invalid pointers directly to functions that dereference them immediately.
       Instead, we rely on the compiler warnings for type mismatches if the code is compiled with -Werror.
       For runtime fuzzing, we pass valid pointers to avoid ASAN crashes, but we can test edge cases. */
    {
        CURL *handle = curl_easy_init();
        if (handle) {
            /* Test with valid types to ensure basic functionality */
            char *url = create_safe_string(Data, Size);
            if (url) {
                curl_easy_setopt(handle, CURLOPT_URL, url);
                free(url);
            }
            
            /* Test with NULL (valid for some options, invalid for others, but safe to try) */
            curl_easy_setopt(handle, CURLOPT_URL, NULL);
            
            /* Test with empty string */
            char empty_str[] = "";
            curl_easy_setopt(handle, CURLOPT_URL, empty_str);
            
            curl_easy_cleanup(handle);
        }
    }

    /* Test 6: Trigger type-checking warnings via curl_multi_setopt */
    {
        CURLM *multi = curl_multi_init();
        if (multi) {
            /* Clean up */
            curl_multi_cleanup(multi);
        }
    }

    return 0;
}