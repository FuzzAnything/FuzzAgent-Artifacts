// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_share_strerror at strerror.c:385:13 in curl.h
// curl_version at version.c:145:7 in curl.h
// curl_getenv at getenv.c:26:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// Wcurl_easy_setopt_err_CURLSH at typecheck-gcc.h:371:1 in typecheck-gcc.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Fuzz driver for curl API functions */

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare environment */
    CURL *curl_handle = curl_easy_init();
    if (!curl_handle) {
        return 0;
    }

    /* 2. Invoke target functions with diverse inputs */

    /* Test curl_share_strerror with various CURLSHcode values */
    /* We use the input data to generate a range of error codes */
    CURLSHcode error_code = (CURLSHcode)(Data[0] & 0xFF);
    const char *error_str = curl_share_strerror(error_code);
    (void)error_str; /* Use the result to avoid unused variable warnings */

    /* Test curl_version (no arguments needed) */
    const char *version_str = curl_version();
    (void)version_str;

    /* Test curl_getenv with a variable name derived from input */
    /* Ensure the variable name is null-terminated */
    size_t env_name_len = (Size > 10) ? 10 : Size;
    char env_name[11];
    memset(env_name, 0, sizeof(env_name));
    memcpy(env_name, Data, env_name_len);
    char *env_val = curl_getenv(env_name);
    if (env_val) {
        curl_free(env_val);
    }

    /* Test curl_maprintf with format strings and arguments */
    /* Ensure format string is null-terminated and safe */
    size_t format_len = (Size > 20) ? 20 : Size;
    char format[21];
    memset(format, 0, sizeof(format));
    memcpy(format, Data, format_len);

    /* Sanitize format string to avoid arbitrary format specifiers */
    /* Only allow %s, %d, %x for safety */
    for (size_t i = 0; i < format_len; i++) {
        if (format[i] == '%') {
            if (i + 1 < format_len) {
                char next = format[i + 1];
                if (next != 's' && next != 'd' && next != 'x') {
                    format[i] = ' '; /* Replace unsafe format specifier */
                }
            } else {
                format[i] = ' ';
            }
        }
    }

    /* Create a null-terminated string from input for %s specifier */
    char safe_string[Size + 1];
    memset(safe_string, 0, sizeof(safe_string));
    memcpy(safe_string, Data, Size);

    /* Check size before accessing Data[1] */
    if (Size >= 2) {
        /* Use explicit format specifiers to match arguments exactly */
        char *formatted_str = curl_maprintf("%s %d %x", safe_string, (int)Data[0], (unsigned int)Data[1]);
        if (formatted_str) {
            curl_free(formatted_str);
        }
    } else if (Size == 1) {
        char *formatted_str = curl_maprintf("%s %d", safe_string, (int)Data[0]);
        if (formatted_str) {
            curl_free(formatted_str);
        }
    } else {
        /* Fallback if Size is 0, though handled at start */
        char *formatted_str = curl_maprintf("%s", safe_string);
        if (formatted_str) {
            curl_free(formatted_str);
        }
    }

    /* Test curl_easy_setopt with various options */
    /* Use input data to set different options */
    long long_val = (long)Data[0];
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, long_val);

    /* Set a string option with a safe, null-terminated string */
    /* Allocate buffer dynamically to avoid stack overflow with large inputs */
    size_t url_buffer_size = Size + 8; /* +7 for "http://" + 1 for null */
    char *url_string = (char *)malloc(url_buffer_size);
    if (url_string) {
        memset(url_string, 0, url_buffer_size);
        
        /* Ensure the URL is valid by prefixing with http:// if needed */
        if (Size > 0 && Data[0] != 'h') {
            memcpy(url_string, "http://", 7);
            memcpy(url_string + 7, Data, Size);
        } else {
            memcpy(url_string, Data, Size);
        }
        
        curl_easy_setopt(curl_handle, CURLOPT_URL, url_string);
        free(url_string);
    }

    /* Test with callback data option */
    void *cb_data = (void *)Data;
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, cb_data);

    /* 3. Cleanup */
    curl_easy_cleanup(curl_handle);

    return 0;
}