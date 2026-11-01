// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <sqlite3.h>

static void safe_free(void *ptr) {
    if (ptr) {
        sqlite3_free(ptr);
    }
}

// Helper function to allow va_start usage
static void test_vsnprintf_va(const char *fmt, char *buf, int size, ...) {
    va_list args;
    va_start(args, size);
    // Ensure format string is not NULL before passing to sqlite3_vsnprintf
    if (fmt) {
        char *res = sqlite3_vsnprintf(size, buf, fmt, args);
        if (res) {
            (void)strlen(res);
        }
    }
    va_end(args);
}

static void test_vmprintf_va(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    // Ensure format string is not NULL before passing to sqlite3_vmprintf
    if (fmt) {
        char *res = sqlite3_vmprintf(fmt, args);
        if (res) {
            (void)strlen(res);
            safe_free(res);
        }
    }
    va_end(args);
}

int LLVMFuzzerTestOneInput_155(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a null-terminated copy of the input to use as a format string
    char *format_buf = (char *)sqlite3_malloc64(Size + 1);
    if (!format_buf) return 0;
    memcpy(format_buf, Data, Size);
    format_buf[Size] = '\0';

    // 1. Test sqlite3_mprintf
    // Pass a non-NULL format string. The arguments are dummy values.
    // If the format string expects more arguments or invalid types, 
    // sqlite3_mprintf handles it gracefully or returns NULL, avoiding the crash.
    char *mprintf_result = sqlite3_mprintf(format_buf, 123, 45.6, "test");
    if (mprintf_result) {
        (void)strlen(mprintf_result);
        safe_free(mprintf_result);
    }

    // 2. Test sqlite3_snprintf
    char snbuf[256];
    char *snprintf_result = sqlite3_snprintf(sizeof(snbuf), snbuf, format_buf, 123, 45.6, "test");
    if (snprintf_result) {
        (void)strlen(snprintf_result);
    }

    // 3. Test sqlite3_str_appendf
    sqlite3_str *str_obj = sqlite3_str_new(NULL);
    if (str_obj) {
        sqlite3_str_appendf(str_obj, format_buf, 123, 45.6, "test");
        sqlite3_str_appendf(str_obj, " %s", "extra");
        sqlite3_str_appendf(str_obj, "%d", 999);
        
        // Check for errors
        int err = sqlite3_str_errcode(str_obj);
        (void)err;

        char *str_finish_result = sqlite3_str_finish(str_obj);
        if (str_finish_result) {
            (void)strlen(str_finish_result);
            safe_free(str_finish_result);
        }
    }

    // 4. Test sqlite3_vsnprintf using a helper function to satisfy va_start requirements
    char vsnbuf[256];
    test_vsnprintf_va(format_buf, vsnbuf, sizeof(vsnbuf), 123, 45.6, "test");

    // 5. Test sqlite3_vmprintf using a helper function
    test_vmprintf_va(format_buf, 123, 45.6, "test");

    // 6. Test sqlite3_str_finish with an empty state
    sqlite3_str *str_obj2 = sqlite3_str_new(NULL);
    if (str_obj2) {
        char *str_finish_empty = sqlite3_str_finish(str_obj2);
        if (str_finish_empty) {
            safe_free(str_finish_empty);
        }
    }

    // Cleanup
    safe_free(format_buf);

    return 0;
}