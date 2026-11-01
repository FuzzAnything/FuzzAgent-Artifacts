// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Use the input data to drive fuzzing logic
    uint8_t mode = Data[0] % 10;

    sqlite3_str *str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Extract format string and arguments from input if possible
    char format_buf[256];
    int fmt_len = (Size > 255) ? 255 : (int)Size;
    memcpy(format_buf, Data, fmt_len);
    format_buf[fmt_len] = '\0';

    if (mode == 0) {
        sqlite3_str_appendf(str, "%s", "test");
        if (Size > 1) {
            sqlite3_str_appendf(str, "%d", (int)(Data[1] & 0xFF));
        }
    } else if (mode == 1) {
        if (fmt_len > 0) {
            sqlite3_str_appendf(str, "%s", format_buf);
        }
    } else if (mode == 2) {
        if (Size > 2) {
            int len = (Data[1] % 100) + 1;
            const char *src = (const char *)(Data + 2);
            if (len > (int)(Size - 2)) len = (int)(Size - 2);
            if (len > 0) {
                sqlite3_str_append(str, src, len);
            }
        }
    } else if (mode == 3) {
        sqlite3_str_appendall(str, format_buf);
    } else if (mode == 4) {
        int count = 1;
        char ch = ' ';
        if (Size > 1) count = (Data[1] % 100) + 1;
        if (Size > 2) ch = (char)(Data[2] & 0xFF);
        sqlite3_str_appendchar(str, count, ch);
    } else if (mode == 5) {
        sqlite3_str_appendf(str, "before reset");
        sqlite3_str_reset(str);
        sqlite3_str_appendf(str, "after reset");
    } else if (mode == 6) {
        sqlite3_str_appendf(str, "0123456789");
        int truncate_len = 0;
        if (Size > 1) truncate_len = (Data[1] % 11);
        sqlite3_str_truncate(str, truncate_len);
    } else if (mode == 7) {
        sqlite3_str_appendf(str, "test value");
        char *val = sqlite3_str_value(str);
        (void)val;
    } else if (mode == 8) {
        sqlite3_str_appendf(str, "finish test");
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
        // str is now invalid
    } else {
        sqlite3_str_appendf(str, "mixed");
        int count = 5;
        char ch = ' ';
        if (Size > 1) count = (Data[1] % 100) + 1;
        if (Size > 2) ch = (char)(Data[2] & 0xFF);
        sqlite3_str_appendchar(str, count, ch);
        sqlite3_str_append(str, "data", 4);
        int truncate_len = 10;
        if (Size > 1) truncate_len = (Data[1] % 11);
        sqlite3_str_truncate(str, truncate_len);
        sqlite3_str_reset(str);
        sqlite3_str_appendall(str, "reset done");
        char *val = sqlite3_str_value(str);
        (void)val;
    }

    // Check for errors
    int err = sqlite3_str_errcode(str);
    (void)err;

    // Cleanup
    if (mode != 8) {
        char *result = sqlite3_str_finish(str);
        if (result) {
            sqlite3_free(result);
        }
    }

    sqlite3_close(db);
    return 0;
}