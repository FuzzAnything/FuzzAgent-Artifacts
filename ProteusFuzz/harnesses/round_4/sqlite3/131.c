// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_vappendf at sqlite3.c:32182:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

/* Helper to extract a C string from fuzz input safely */
static char *extract_string(const uint8_t *Data, size_t Size, size_t *outLen) {
    if (Size == 0) {
        *outLen = 0;
        return NULL;
    }
    size_t len = 0;
    while (len < Size && Data[len] != '\0') {
        len++;
    }
    *outLen = len;
    if (len == 0) return NULL;
    
    char *str = (char *)sqlite3_malloc64(len + 1);
    if (!str) return NULL;
    memcpy(str, Data, len);
    str[len] = '\0';
    return str;
}

/* Variadic helper to generate va_list for sqlite3_vmprintf */
static void dummy_variadic_for_vmprintf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char *vres = sqlite3_vmprintf(fmt, ap);
    va_end(ap);
    if (vres) {
        sqlite3_free(vres);
    }
}

/* Variadic helper to generate va_list for sqlite3_str_vappendf */
static void dummy_variadic_for_vappendf(sqlite3_str *s, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    sqlite3_str_vappendf(s, fmt, ap);
    va_end(ap);
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    /* 1. Prepare the environment */
    /* Initialize SQLite if necessary (some functions auto-init) */
    sqlite3_initialize();

    /* Extract potential format strings and data from input */
    size_t fmtLen = 0;
    char *fmt = extract_string(Data, Size, &fmtLen);
    
    /* If no null terminator found in the first part, try the whole buffer as a format string
     * but ensure it's null-terminated for safety.
     */
    if (!fmt && Size > 0) {
        /* Fallback: treat first chunk as format, ensure null termination */
        size_t maxFmt = Size < 256 ? Size : 256;
        fmt = (char *)sqlite3_malloc64(maxFmt + 1);
        if (fmt) {
            memcpy(fmt, Data, maxFmt);
            fmt[maxFmt] = '\0';
        }
    }

    /* 2. Test sqlite3_mprintf */
    if (fmt) {
        char *res = sqlite3_mprintf(fmt, 123, 456.789, "hello", 100);
        if (res) {
            sqlite3_free(res);
        }
    }

    /* 3. Test sqlite3_snprintf */
    char buf[1024];
    if (fmt) {
        sqlite3_snprintf(sizeof(buf), buf, fmt, 123, 456.789, "hello", 100);
    } else {
        /* Test with empty/null format handling */
        sqlite3_snprintf(sizeof(buf), buf, "%s", "safe");
    }

    /* 4. Test sqlite3_str_* functions */
    sqlite3_str *s = sqlite3_str_new(NULL);
    if (s) {
        if (fmt) {
            sqlite3_str_appendf(s, fmt, 123, 456.789, "hello", 100);
        }
        
        /* Test vappendf using the variadic helper function */
        const char *safe_fmt = "Value: %d, %f";
        dummy_variadic_for_vappendf(s, safe_fmt, 123, 456.789);
        
        /* Test append with raw data */
        if (Size > 0) {
            size_t appendLen = Size < 100 ? Size : 100;
            sqlite3_str_append(s, (const char *)Data, appendLen);
        }

        /* Test truncate */
        if (Size > 50) {
            int truncateTo = (int)(Data[50] & 0xFF);
            sqlite3_str_truncate(s, truncateTo);
        }

        /* Test reset */
        sqlite3_str_reset(s);

        /* Finalize and free */
        char *result = sqlite3_str_finish(s);
        if (result) {
            sqlite3_free(result);
        }
    }

    /* 5. Test sqlite3_vmprintf using the variadic helper */
    if (fmt) {
        dummy_variadic_for_vmprintf(fmt, 123, 456.789, "hello", 100);
    }

    /* 6. Cleanup */
    if (fmt) {
        sqlite3_free(fmt);
    }

    sqlite3_shutdown();
    return 0;
}