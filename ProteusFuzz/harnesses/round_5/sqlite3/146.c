// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_vsnprintf at sqlite3.c:33380:18 in sqlite3.h
// sqlite3_vmprintf at sqlite3.c:33331:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:33395:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdarg.h>

static void fuzz_vsnprintf_helper(const char *fmt, ...) {
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    int n = 1024;
    va_list args;
    va_start(args, fmt);
    char *res_vsnprintf = sqlite3_vsnprintf(n, buf, fmt, args);
    va_end(args);
    if (res_vsnprintf) {
        (void)res_vsnprintf[0];
    }
}

static void fuzz_vmprintf_helper(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char *res_vmprintf = sqlite3_vmprintf(fmt, args);
    va_end(args);
    if (res_vmprintf) {
        (void)res_vmprintf[0];
        sqlite3_free(res_vmprintf);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Interpret input data for fuzzing parameters
    int n = (Size > 0) ? (int)Data[0] : 100;
    if (n < 0) n = 100;
    if (n > 1024) n = 1024;

    char buf[1024];
    memset(buf, 0, sizeof(buf));

    // Prepare a safe format string
    const char *fmt_snprintf = "%s %d";
    char safe_fmt[256];
    memset(safe_fmt, 0, sizeof(safe_fmt));
    if (Size > 2) {
        size_t copy_len = (Size - 2 < 255) ? Size - 2 : 255;
        memcpy(safe_fmt, &Data[2], copy_len);
        fmt_snprintf = safe_fmt;
    }

    // 1. Test sqlite3_snprintf
    // Ensure we have enough data before accessing Data[1]
    int arg_val = 0;
    if (Size > 1) {
        arg_val = (int)Data[1];
    }
    char *res_snprintf = sqlite3_snprintf(n, buf, fmt_snprintf, arg_val, "test");
    if (res_snprintf) {
        (void)res_snprintf[0];
    }

    // 2. Test sqlite3_vsnprintf via helper
    fuzz_vsnprintf_helper(fmt_snprintf, arg_val, "test");

    // 3. Test sqlite3_mprintf
    char *res_mprintf = sqlite3_mprintf(fmt_snprintf, arg_val, "test");
    if (res_mprintf) {
        (void)res_mprintf[0];
        sqlite3_free(res_mprintf);
    }

    // 4. Test sqlite3_vmprintf via helper
    fuzz_vmprintf_helper(fmt_snprintf, arg_val, "test");

    // 5. Test sqlite3_str_* functions
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (str) {
        // Append formatted string
        sqlite3_str_appendf(str, fmt_snprintf, arg_val, "test");

        // Append raw bytes if available
        if (Size > 10) {
            int n_append = (int)Data[10];
            if (n_append < 0) n_append = 0;
            if (n_append > (int)(Size - 11)) n_append = (int)(Size - 11);
            if (n_append > 0) {
                sqlite3_str_append(str, (const char *)&Data[11], n_append);
            }
        }

        // Append all of a string (using a safe slice)
        if (Size > 20) {
            char temp_str[100];
            memset(temp_str, 0, sizeof(temp_str));
            size_t len = (Size - 20 < 99) ? Size - 20 : 99;
            memcpy(temp_str, &Data[20], len);
            sqlite3_str_appendall(str, temp_str);
        }

        // Append character
        if (Size > 31) {
            int count = (int)Data[30];
            char ch = (char)Data[31];
            sqlite3_str_appendchar(str, count, ch);
        }

        // Reset
        if (Size > 32 && Data[32]) {
            sqlite3_str_reset(str);
        }

        // Truncate
        if (Size > 33) {
            int trunc_len = (int)Data[33];
            sqlite3_str_truncate(str, trunc_len);
        }

        // Check error code
        int err = sqlite3_str_errcode(str);
        (void)err;

        // Finish and free
        char *finished_str = sqlite3_str_finish(str);
        if (finished_str) {
            (void)finished_str[0];
            sqlite3_free(finished_str);
        }
    }

    return 0;
}