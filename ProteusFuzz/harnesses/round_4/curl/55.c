// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_mvaprintf at mprintf.c:1139:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

static int is_safe_format(const char *fmt, size_t len) {
    if (!fmt) return 0;
    
    // Basic sanity checks to avoid crashing on invalid format strings
    // We avoid %s, %p, %n, %S, %P, %N which can cause crashes with arbitrary data
    // or mismatched arguments.
    for (size_t i = 0; i < len; i++) {
        if (fmt[i] == '%') {
            if (i + 1 < len) {
                char next = fmt[i+1];
                // Skip width/precision modifiers
                while (i + 2 < len && (fmt[i+2] == '*' || (fmt[i+2] >= '0' && fmt[i+2] <= '9'))) {
                    i++;
                }
                // Skip flags
                while (i + 2 < len && (fmt[i+2] == '-' || fmt[i+2] == '+' || fmt[i+2] == ' ' || fmt[i+2] == '#' || fmt[i+2] == '0')) {
                    i++;
                }
                
                // Check the actual specifier (case-insensitive for string/pointer types)
                // Block s, p, n and their uppercase variants S, P, N
                if (next == 's' || next == 'S' || 
                    next == 'p' || next == 'P' || 
                    next == 'n' || next == 'N') {
                    return 0; // Unsafe specifier
                }
            }
        }
    }
    return 1;
}

static void test_curl_maprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *safe_format = (char *)malloc(size + 1);
    if (!safe_format) return;
    
    memcpy(safe_format, data, size);
    safe_format[size] = '\0';
    
    // Only test if format string is safe
    if (is_safe_format(safe_format, size)) {
        char *result = curl_maprintf(safe_format, 12345);
        if (result) {
            curl_free(result);
        }
    }
    
    free(safe_format);
}

static void test_curl_getenv(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *safe_var = (char *)malloc(size + 1);
    if (!safe_var) return;
    
    memcpy(safe_var, data, size);
    safe_var[size] = '\0';
    
    char *env_val = curl_getenv(safe_var);
    if (env_val) {
        curl_free(env_val);
    }
    
    free(safe_var);
}

static void test_curl_version(void) {
    char *ver = curl_version();
    if (ver) {
        (void)strlen(ver);
    }
}

static void test_curl_msnprintf(const uint8_t *data, size_t size) {
    if (size < 1) return;
    
    char *safe_format = (char *)malloc(size + 1);
    if (!safe_format) return;
    
    memcpy(safe_format, data, size);
    safe_format[size] = '\0';
    
    char buffer[256];
    // Only test if format string is safe
    if (is_safe_format(safe_format, size)) {
        int ret = curl_msnprintf(buffer, sizeof(buffer), safe_format, 12345);
        (void)ret;
    }
    
    free(safe_format);
}

static void dummy_va_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    if (fmt && is_safe_format(fmt, strlen(fmt))) {
        char *buffer = (char *)malloc(4096);
        if (buffer) {
            int ret = curl_mvsprintf(buffer, fmt, args);
            (void)ret;
            free(buffer);
        }
    }
    va_end(args);
}

static void dummy_mvaprintf_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    if (fmt && is_safe_format(fmt, strlen(fmt))) {
        char *result = curl_mvaprintf(fmt, args);
        if (result) {
            curl_free(result);
        }
    }
    va_end(args);
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    test_curl_version();
    test_curl_maprintf(Data, Size);
    test_curl_getenv(Data, Size);
    test_curl_msnprintf(Data, Size);
    
    if (Size > 0) {
        char *safe_format = (char *)malloc(Size + 1);
        if (safe_format) {
            memcpy(safe_format, Data, Size);
            safe_format[Size] = '\0';
            
            dummy_va_helper(safe_format, 12345);
            dummy_mvaprintf_helper(safe_format, 12345);
            
            free(safe_format);
        }
    }

    return 0;
}