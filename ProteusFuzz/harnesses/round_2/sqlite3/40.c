// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_appendall at sqlite3.c:33137:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:33092:17 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:33220:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    char *result = NULL;
    int rc;

    // Initialize database connection (optional, can be NULL for sqlite3_str_new)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a new dynamic string object
    str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Use input data to drive fuzzing
    const char *input_str = (const char *)Data;
    size_t input_len = Size;

    // Fuzz sqlite3_str_append
    if (input_len > 0) {
        int append_len = (input_len % 100) + 1;
        if (append_len > (int)input_len) append_len = (int)input_len;
        sqlite3_str_append(str, input_str, append_len);
    }

    // Fuzz sqlite3_str_appendall with a null-terminated substring
    char *temp_buf = (char *)malloc(input_len + 1);
    if (temp_buf != NULL) {
        memcpy(temp_buf, input_str, input_len);
        temp_buf[input_len] = '\0';
        sqlite3_str_appendall(str, temp_buf);
        free(temp_buf);
    }

    // Fuzz sqlite3_str_appendf with format strings
    // We construct safe format strings from input to avoid crashes
    char fmt[64];
    int fmt_len = (input_len > 63) ? 63 : (int)input_len;
    memcpy(fmt, input_str, fmt_len);
    fmt[fmt_len] = '\0';

    // Sanitize format string to prevent undefined behavior
    char safe_fmt[64] = "%s";
    if (fmt_len > 0 && fmt[0] == '%') {
        // Only allow simple format specifiers
        if (fmt_len == 1 || (fmt_len == 2 && (fmt[1] == 's' || fmt[1] == 'd' || fmt[1] == 'x'))) {
            memcpy(safe_fmt, fmt, fmt_len + 1);
        }
    }

    sqlite3_str_appendf(str, safe_fmt, "fuzzed_data");

    // Fuzz sqlite3_str_appendchar
    if (input_len > 0) {
        int count = (input_len % 10) + 1;
        char ch = input_str[0];
        sqlite3_str_appendchar(str, count, ch);
    }

    // Fuzz sqlite3_str_reset
    sqlite3_str_reset(str);

    // Fuzz sqlite3_str_truncate
    if (input_len > 0) {
        int truncate_len = (input_len % 50);
        sqlite3_str_truncate(str, truncate_len);
    }

    // Fuzz sqlite3_str_value
    char *value = sqlite3_str_value(str);
    if (value != NULL) {
        // Just read it, no action needed
        (void)value;
    }

    // Finalize the string
    result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }

    // Cleanup
    sqlite3_close(db);

    return 0;
}