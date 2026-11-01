// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_escape at escape.c:36:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

/* Helper to ensure input is null-terminated for string functions */
static char *make_null_terminated(const uint8_t *data, size_t size) {
    if (size == 0) {
        char *buf = (char *)malloc(1);
        if (!buf) return NULL;
        buf[0] = '\0';
        return buf;
    }
    char *buf = (char *)malloc(size + 1);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    buf[size] = '\0';
    return buf;
}

/* Helper to extract a format string safely from input */
static const char *extract_format_string(const uint8_t *data, size_t size, size_t *out_len) {
    if (size == 0) return NULL;
    
    /* Find first null terminator or use whole buffer if none */
    size_t len = 0;
    for (len = 0; len < size; len++) {
        if (data[len] == '\0') break;
    }
    
    /* Limit format string size to prevent huge allocations */
    if (len > 1024) len = 1024;
    
    if (out_len) *out_len = len;
    return (const char *)data;
}

/* Variadic wrapper to properly initialize va_list for testing */
static void test_va_functions_variadic(const char *fmt, ...) {
    if (!fmt) return;
    
    va_list args;
    va_start(args, fmt);
    
    /* Test curl_mvprintf */
    curl_mvprintf(fmt, args);
    va_end(args);
    
    /* Test curl_mvsprintf */
    char buffer[1024];
    va_start(args, fmt);
    int ret = curl_mvsprintf(buffer, fmt, args);
    (void)ret;
    va_end(args);
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure we have at least some data to work with */
    if (Size < 1) return 0;

    /* Prepare null-terminated string for functions expecting strings */
    char *input_str = make_null_terminated(Data, Size);
    if (!input_str) return 0;

    /* 1. Test curl_unescape */
    char *unescaped = curl_unescape(input_str, -1);
    if (unescaped) {
        /* Do something with unescaped to trigger more code paths */
        (void)strlen(unescaped);
        curl_free(unescaped);
    }
    
    /* Test curl_unescape with specific length */
    if (Size > 0) {
        unescaped = curl_unescape(input_str, (int)Size);
        if (unescaped) {
            (void)strlen(unescaped);
            curl_free(unescaped);
        }
    }

    /* 2, 3. Test curl_mvprintf and curl_mvsprintf via variadic helper */
    /* We need a null-terminated format string. Create a safe copy. */
    const char *fmt_ptr = extract_format_string(Data, Size, NULL);
    if (fmt_ptr) {
        /* Find the actual length of the format string to ensure safety */
        size_t fmt_len = 0;
        for (size_t i = 0; i < Size; i++) {
            if (Data[i] == '\0') {
                fmt_len = i;
                break;
            }
        }
        if (fmt_len == 0 && Size > 0) fmt_len = Size; /* Fallback if no null found, though extract_format_string handles logic */
        
        /* Limit format string size */
        if (fmt_len > 1024) fmt_len = 1024;
        
        if (fmt_len > 0) {
            char *safe_fmt = (char *)malloc(fmt_len + 1);
            if (safe_fmt) {
                memcpy(safe_fmt, fmt_ptr, fmt_len);
                safe_fmt[fmt_len] = '\0';
                
                test_va_functions_variadic(safe_fmt, input_str);
                
                free(safe_fmt);
            }
        }
    }

    /* 4. Test curl_escape */
    char *escaped = curl_escape(input_str, -1);
    if (escaped) {
        (void)strlen(escaped);
        curl_free(escaped);
    }
    
    if (Size > 0) {
        escaped = curl_escape(input_str, (int)Size);
        if (escaped) {
            (void)strlen(escaped);
            curl_free(escaped);
        }
    }

    /* 5. Test curl_msnprintf */
    char msnprintf_buf[1024];
    /* Re-use the safe format string logic or create a simple one */
    if (fmt_ptr) {
        size_t fmt_len = 0;
        for (size_t i = 0; i < Size; i++) {
            if (Data[i] == '\0') {
                fmt_len = i;
                break;
            }
        }
        if (fmt_len == 0 && Size > 0) fmt_len = Size;
        if (fmt_len > 1024) fmt_len = 1024;
        
        if (fmt_len > 0) {
            char *safe_fmt = (char *)malloc(fmt_len + 1);
            if (safe_fmt) {
                memcpy(safe_fmt, fmt_ptr, fmt_len);
                safe_fmt[fmt_len] = '\0';
                
                int ret = curl_msnprintf(msnprintf_buf, sizeof(msnprintf_buf), safe_fmt, input_str);
                (void)ret;
                
                free(safe_fmt);
            }
        }
    }
    
    /* Test with different format specifiers */
    int ret2 = curl_msnprintf(msnprintf_buf, sizeof(msnprintf_buf), "%s %d %x", input_str, 42, 0x1234);
    (void)ret2;

    /* 6. Test curl_maprintf */
    char *mapped = curl_maprintf("%s", input_str);
    if (mapped) {
        (void)strlen(mapped);
        curl_free(mapped);
    }
    
    /* Test with multiple arguments */
    mapped = curl_maprintf("%s-%d-%x", input_str, 100, 0xABCD);
    if (mapped) {
        (void)strlen(mapped);
        curl_free(mapped);
    }

    /* Cleanup */
    free(input_str);
    
    return 0;
}