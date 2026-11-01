// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_free at sqlite3.c:33251:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:33210:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_str_reset at sqlite3.c:33237:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    char *result = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Initialize a database connection (in-memory)
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a new sqlite3_str object
    str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Use the fuzzer input as a format string (carefully)
    // We limit the format string length to avoid excessive processing
    size_t format_len = Size;
    if (format_len > 1024) {
        format_len = 1024;
    }

    // Create a null-terminated buffer for the format string
    char *format_buf = (char *)malloc(format_len + 1);
    if (format_buf == NULL) {
        sqlite3_str_free(str);
        sqlite3_close(db);
        return 0;
    }

    memcpy(format_buf, Data, format_len);
    format_buf[format_len] = '\0';

    // Try to append formatted text
    sqlite3_str_appendf(str, "%s", format_buf);

    // Check for errors
    int errcode = sqlite3_str_errcode(str);
    (void)errcode; // Suppress unused variable warning

    // Try to get the value
    result = sqlite3_str_value(str);
    if (result != NULL) {
        // Do something with the result (e.g., check length)
        size_t len = strlen(result);
        (void)len; // Suppress unused variable warning
    }

    // Reset the string and try again
    sqlite3_str_reset(str);
    sqlite3_str_appendf(str, "Reset and appended: %s", format_buf);

    // Finish the string. 
    // NOTE: sqlite3_str_finish destroys the sqlite3_str object.
    // After this call, 'str' is invalid and must not be used.
    result = sqlite3_str_finish(str);
    if (result != NULL) {
        sqlite3_free(result);
    }
    
    // str is now invalid. Do NOT call sqlite3_str_free(str) here.
    // The object was already freed by sqlite3_str_finish.
    str = NULL;

    // Cleanup
    sqlite3_close(db);
    free(format_buf);

    return 0;
}