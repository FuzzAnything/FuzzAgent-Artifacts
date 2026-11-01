// This fuzz driver is generated for library curl, aiming to fuzz the following functions:
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_easy_escape at escape.c:50:7 in curl.h
// curl_free at escape.c:189:6 in curl.h
// curl_easy_cleanup at easy.c:837:6 in easy.h
// curl_mvsprintf at mprintf.c:1214:5 in mprintf.h
// curl_easy_init at easy.c:330:7 in easy.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_msnprintf at mprintf.c:1102:5 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_maprintf at mprintf.c:1157:7 in mprintf.h
// curl_free at escape.c:189:6 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
// curl_strnequal at strequal.c:87:5 in curl.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <curl/curl.h>
#include <curl/mprintf.h>
#include <curl/typecheck-gcc.h>

/* Helper to ensure string is null-terminated for safe %s usage */
static char *ensure_null_terminated(const uint8_t *data, size_t size, size_t *out_size) {
    if (size == 0) {
        *out_size = 0;
        return NULL;
    }
    char *buf = malloc(size + 1);
    if (!buf) return NULL;
    memcpy(buf, data, size);
    buf[size] = '\0';
    *out_size = size;
    return buf;
}

/* Helper function to test curl_mvsprintf with variadic arguments */
static void test_mvsprintf(char *buffer, size_t buflen, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    curl_mvsprintf(buffer, fmt, args);
    va_end(args);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize curl handle for curl_easy_escape */
    CURL *handle = curl_easy_init();
    if (!handle) {
        return 0;
    }

    /* Split input for diverse testing */
    size_t chunk_size = Size / 4;
    const uint8_t *p1 = Data;
    const uint8_t *p2 = Data + chunk_size;
    const uint8_t *p3 = Data + 2 * chunk_size;
    const uint8_t *p4 = Data + 3 * chunk_size;
    size_t s1 = chunk_size;
    size_t s2 = chunk_size;
    size_t s3 = chunk_size;
    size_t s4 = Size - 3 * chunk_size;

    /* Test 1: curl_msnprintf with safe format specifiers */
    char buffer1[256];
    memset(buffer1, 0, sizeof(buffer1));
    
    /* Use %.*s to safely handle raw binary data without assuming null-termination */
    if (s1 > 0) {
        int ret1 = curl_msnprintf(buffer1, sizeof(buffer1), "Data: %.*s", (int)s1, p1);
        (void)ret1;
    } else {
        curl_msnprintf(buffer1, sizeof(buffer1), "Empty");
    }

    /* Test 2: curl_maprintf with safe format specifiers */
    char *allocated_str = NULL;
    if (s2 > 0) {
        allocated_str = curl_maprintf("Prefix: %.*s", (int)s2, p2);
    } else {
        allocated_str = curl_maprintf("Empty");
    }
    if (allocated_str) {
        curl_free(allocated_str);
    }

    /* Test 3: curl_mvsprintf with va_list via helper function */
    char buffer3[256];
    memset(buffer3, 0, sizeof(buffer3));
    
    if (s3 > 0) {
        char *safe_str = ensure_null_terminated(p3, s3, &s3);
        if (safe_str) {
            test_mvsprintf(buffer3, sizeof(buffer3), "VA: %s", safe_str);
            free(safe_str);
        }
    } else {
        test_mvsprintf(buffer3, sizeof(buffer3), "VA Empty");
    }

    /* Test 4: curl_strnequal */
    if (s1 > 0 && s2 > 0) {
        size_t compare_len = (s1 < s2) ? s1 : s2;
        int equal = curl_strnequal((const char *)p1, (const char *)p2, compare_len);
        (void)equal;
    }
    /* Test with NULL pointers */
    int null_equal = curl_strnequal(NULL, NULL, 5);
    (void)null_equal;

    /* Test 5: curl_easy_escape */
    char *escaped = NULL;
    if (s4 > 0) {
        escaped = curl_easy_escape(handle, (const char *)p4, (int)s4);
    } else {
        escaped = curl_easy_escape(handle, "", 0);
    }
    if (escaped) {
        curl_free(escaped);
    }

    /* Cleanup */
    curl_easy_cleanup(handle);

    return 0;
}