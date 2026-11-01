// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_getenv at getenv.c:26:7 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_bynum at http2.c:657:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_pushheader_byname at http2.c:673:7 in multi.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/multi.h>
#include <curl/mprintf.h>

/* 
 * Fuzz driver for curl library functions.
 * Target functions:
 * 1. curl_pushheader_bynum
 * 2. curl_version
 * 3. curl_pushheader_byname
 * 4. curl_maprintf
 * 5. curl_getenv
 * 6. curl_msnprintf
 */

static void safe_free(void *ptr) {
    if (ptr) {
        curl_free(ptr);
    }
}

static int is_safe_format_string(const char *str, size_t len) {
    if (!str || len == 0) return 0;
    
    /* Check for dangerous format specifiers that could cause issues */
    /* We avoid %p, %n, and large %s or %d specifiers that might cause issues */
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '%') {
            if (i + 1 < len) {
                char next = str[i + 1];
                /* Skip %n as it writes to memory */
                if (next == 'n') return 0;
                /* Skip %p as it might cause issues in some contexts */
                if (next == 'p') return 0;
                /* Skip repeated %s or %d which could cause issues */
                if (next == 's' || next == 'd' || next == 'i' || next == 'x' || next == 'X') {
                    /* Check if there's a width specifier that's too large */
                    size_t j = i + 1;
                    while (j < len && (str[j] >= '0' && str[j] <= '9')) {
                        j++;
                    }
                    if (j > i + 2) {
                        /* Has a width specifier, check if it's reasonable */
                        char width_str[20];
                        size_t width_len = j - (i + 1);
                        if (width_len >= sizeof(width_str)) return 0;
                        memcpy(width_str, str + i + 1, width_len);
                        width_str[width_len] = '\0';
                        long width = strtol(width_str, NULL, 10);
                        if (width > 1000) return 0;
                    }
                }
            }
        }
    }
    return 1;
}

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (!Data || Size == 0) {
        return 0;
    }

    /* Initialize libcurl */
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (res != CURLE_OK) {
        return 0;
    }

    /* 1. Test curl_version - always safe, no arguments needed */
    char *version = curl_version();
    if (version) {
        /* Just access the string to ensure it's valid */
        size_t ver_len = strlen(version);
        (void)ver_len; /* Use the value to avoid unused warning */
    }

    /* 2. Test curl_getenv with various inputs from fuzz data */
    char *env_var = NULL;
    if (Size > 0) {
        /* Create a null-terminated string from fuzz data using malloc */
        char *test_env = (char *)malloc(Size + 1);
        if (test_env) {
            memcpy(test_env, Data, Size);
            test_env[Size] = '\0';
            
            env_var = curl_getenv(test_env);
            safe_free(env_var);
            
            /* Test with empty string */
            test_env[0] = '\0';
            env_var = curl_getenv(test_env);
            safe_free(env_var);
            
            /* Test with special characters */
            if (Size > 2) {
                test_env[0] = ':';
                test_env[1] = '\0';
                env_var = curl_getenv(test_env);
                safe_free(env_var);
            }
            
            free(test_env);
        }
    }

    /* 3. Test curl_maprintf with safe format strings only */
    char *formatted = NULL;
    if (Size > 0) {
        /* Create a null-terminated format string using malloc */
        char *format = (char *)malloc(Size + 1);
        if (format) {
            memcpy(format, Data, Size);
            format[Size] = '\0';
            
            /* Only use the format string if it's safe */
            if (is_safe_format_string(format, Size)) {
                /* Test with simple format - use a safe string argument */
                formatted = curl_maprintf(format, "test");
                safe_free(formatted);
                
                /* Test with integer format */
                formatted = curl_maprintf(format, 12345);
                safe_free(formatted);
                
                /* Test with double format */
                formatted = curl_maprintf(format, 3.14159);
                safe_free(formatted);
            }
            
            /* Also test with a known safe format string */
            formatted = curl_maprintf("safe_format: %s %d %f", "test", 42, 3.14);
            safe_free(formatted);
            
            free(format);
        }
    }

    /* 4. Test curl_msnprintf with various inputs */
    char buffer[256];
    if (Size > 0) {
        char *format = (char *)malloc(Size + 1);
        if (format) {
            memcpy(format, Data, Size);
            format[Size] = '\0';
            
            /* Only use the format string if it's safe */
            if (is_safe_format_string(format, Size)) {
                /* Test with different buffer sizes */
                int ret = curl_msnprintf(buffer, sizeof(buffer), format, "test");
                (void)ret;
                
                ret = curl_msnprintf(buffer, sizeof(buffer), format, 12345);
                (void)ret;
                
                ret = curl_msnprintf(buffer, sizeof(buffer), format, 3.14159);
                (void)ret;
                
                /* Test with small buffer to trigger truncation */
                ret = curl_msnprintf(buffer, 1, format, "test");
                (void)ret;
                
                /* Test with zero buffer size */
                ret = curl_msnprintf(buffer, 0, format, "test");
                (void)ret;
            }
            
            /* Also test with a known safe format string */
            int ret = curl_msnprintf(buffer, sizeof(buffer), "safe: %s %d", "test", 42);
            (void)ret;
            
            free(format);
        }
    }

    /* 5. Test curl_pushheader_bynum and curl_pushheader_byname */
    /* These functions require a valid curl_pushheaders struct which is 
     * typically created by libcurl during server push callbacks.
     * Since the struct is opaque and incomplete in the public API, 
     * we can only test with NULL to check error handling.
     * We cannot construct a valid instance from user code. */
    
    char *header_value = NULL;
    
    /* Test with NULL struct - this is the only safe test we can do */
    header_value = curl_pushheader_bynum(NULL, 0);
    (void)header_value;
    
    header_value = curl_pushheader_bynum(NULL, 1);
    (void)header_value;
    
    header_value = curl_pushheader_bynum(NULL, 100);
    (void)header_value;
    
    header_value = curl_pushheader_byname(NULL, "Content-Type");
    (void)header_value;
    
    header_value = curl_pushheader_byname(NULL, "Authorization");
    (void)header_value;
    
    header_value = curl_pushheader_byname(NULL, "");
    (void)header_value;
    
    header_value = curl_pushheader_byname(NULL, ":");
    (void)header_value;
    
    header_value = curl_pushheader_byname(NULL, NULL);
    (void)header_value;
    
    /* Test with fuzz data as header name (with NULL struct) */
    if (Size > 0) {
        char *header_name = (char *)malloc(Size + 1);
        if (header_name) {
            memcpy(header_name, Data, Size);
            header_name[Size] = '\0';
            
            header_value = curl_pushheader_byname(NULL, header_name);
            (void)header_value;
            
            free(header_name);
        }
    }

    /* Cleanup */
    curl_global_cleanup();
    
    return 0;
}