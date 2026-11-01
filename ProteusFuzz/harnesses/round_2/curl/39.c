// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_unescape at escape.c:42:7 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <stdarg.h>

/* Helper variadic function to test curl_mvprintf */
static int local_mvprintf_wrapper(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = curl_mvprintf(format, args);
    va_end(args);
    return res;
}

/* Helper variadic function to test curl_mvsprintf */
static int local_mvsprintf_wrapper(char *buffer, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int res = curl_mvsprintf(buffer, format, args);
    va_end(args);
    return res;
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    /* Initialize curl global */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Prepare a buffer for fixed-size operations */
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));

    /* Create a CURL handle for curl_easy_escape */
    CURL *handle = curl_easy_init();

    /*
     * Test curl_msnprintf
     */
    if (Size >= 1) {
        char fmt[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        /* Use a simple format to avoid complex parsing issues if input is garbage */
        int ret = curl_msnprintf(buffer, sizeof(buffer), "%s", fmt);
        (void)ret;
    }

    /*
     * Test curl_mvprintf and curl_mvsprintf using wrappers
     */
    if (Size >= 1) {
        char fmt[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        /* Call via wrapper to handle va_list correctly */
        local_mvprintf_wrapper(fmt);
        local_mvsprintf_wrapper(buffer, fmt);
    }

    /*
     * Test curl_maprintf
     */
    if (Size >= 1) {
        char fmt[64];
        size_t fmt_len = (Size > 63) ? 63 : Size;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        char *result = curl_maprintf("%s", fmt);
        if (result) {
            curl_free(result);
        }
    }

    /*
     * Test curl_unescape
     */
    if (Size > 0) {
        /* Use input as the string to unescape */
        char *unescaped = curl_unescape((const char *)Data, -1);
        if (unescaped) {
            curl_free(unescaped);
        }
        
        /* Test with specific length */
        int len = (Size > 100) ? 100 : Size;
        unescaped = curl_unescape((const char *)Data, len);
        if (unescaped) {
            curl_free(unescaped);
        }
    }

    /*
     * Test curl_easy_escape
     */
    if (handle && Size > 0) {
        /* Use input as the string to escape */
        char *escaped = curl_easy_escape(handle, (const char *)Data, -1);
        if (escaped) {
            curl_free(escaped);
        }
        
        /* Test with specific length */
        int len = (Size > 100) ? 100 : Size;
        escaped = curl_easy_escape(handle, (const char *)Data, len);
        if (escaped) {
            curl_free(escaped);
        }
    }

    if (handle) {
        curl_easy_cleanup(handle);
    }

    curl_global_cleanup();

    return 0;
}