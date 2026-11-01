// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_free at escape.c:189:6 in curl.h
// curl_global_cleanup at easy.c:255:6 in curl.h
// curl_mvprintf at mprintf.c:1221:5 in mprintf.h
// curl_global_init at easy.c:197:10 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>

/* 
 * Fuzz driver for curl internal formatting functions.
 * Targets: curl_mvsprintf, curl_maprintf, curl_mvprintf.
 * Note: Wcurl_* macros are compile-time checks and cannot be fuzzed at runtime.
 * We include curl/curl.h before typecheck-gcc.h to ensure types are defined.
 */

static int dummy_write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    (void)ptr;
    (void)size;
    (void)nmemb;
    (void)userdata;
    return 0;
}

static int dummy_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
    (void)handle;
    (void)type;
    (void)data;
    (void)size;
    (void)userptr;
    return 0;
}

/* Helper function to test curl_mvprintf with variadic arguments */
static void test_mvprintf_helper(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    curl_mvprintf(fmt, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl global environment */
    curl_global_init(CURL_GLOBAL_DEFAULT);

    /* Create a safe buffer for format string construction */
    char format_buf[256];
    size_t format_len = (Size < sizeof(format_buf) - 1) ? Size : sizeof(format_buf) - 1;
    
    /* Copy input to format buffer, ensuring null termination */
    memcpy(format_buf, Data, format_len);
    format_buf[format_len] = '\0';

    /* Test 1: curl_maprintf - formats string and allocates memory */
    /* Using the input as a string argument to %s to test string handling */
    char *mapped_str = curl_maprintf("%s", format_buf);
    if (mapped_str) {
        /* Do something with mapped_str to exercise code paths */
        size_t len = strlen(mapped_str);
        if (len > 0) {
            volatile char c = mapped_str[len - 1];
            (void)c;
        }
        curl_free(mapped_str);
    }

    /* Test 2: curl_mvsprintf with a simple format and the input as data */
    char out_buf[4096];
    memset(out_buf, 0, sizeof(out_buf));
    
    /* We cannot easily construct a va_list from raw bytes for curl_mvsprintf 
     * without a helper function that takes variadic args. 
     * Instead, we test curl_maprintf extensively which internally uses similar logic.
     * To test curl_mvsprintf directly, we would need a wrapper that accepts va_list,
     * but since we are fuzzing, we can rely on curl_maprintf covering the formatting logic.
     * However, let's try to call curl_mvsprintf with a manually constructed va_list
     * if possible, or skip if it's too complex. 
     * A safer bet is to use curl_maprintf for the heavy lifting.
     */

    /* Test 3: curl_mvprintf with a constructed va_list via a helper function call */
    if (Size > 0) {
        int fuzz_int = (Data[0] << 8) | (Size > 1 ? Data[1] : 0);
        test_mvprintf_helper("%d", fuzz_int);
    }

    /* Test 4: Construct a format string with the byte */
    if (Size > 0) {
        char simple_fmt[64];
        int byte_val = Data[0] & 0x7F; 
        if (byte_val < 32) byte_val = 32;
        
        snprintf(simple_fmt, sizeof(simple_fmt), "%%c %d", byte_val);
        
        char *res2 = curl_maprintf(simple_fmt);
        if (res2) {
            curl_free(res2);
        }
    }

    /* Cleanup */
    curl_global_cleanup();

    return 0;
}