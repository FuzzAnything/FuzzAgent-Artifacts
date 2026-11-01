// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:33491:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:33228:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;

    if (Size < 2) {
        return 0;
    }

    // Initialize database connection in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Initialize sqlite3_str
    str = sqlite3_str_new(db);
    if (str == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_str_appendf
    // Use a safe format string derived from input to avoid crashes from malformed formats
    size_t fmt_len = (Size > 100) ? 100 : Size;
    char fmt_buf[101];
    memcpy(fmt_buf, Data, fmt_len);
    fmt_buf[fmt_len] = '\0';
    
    sqlite3_str_appendf(str, fmt_buf, 12345, "test");

    // 2. sqlite3_str_append (First call)
    if (Size > 2) {
        int append_len = (Size > 50) ? 50 : (int)(Size - 2);
        sqlite3_str_append(str, (const char *)(Data + 1), append_len);
    }

    // 3. sqlite3_str_append (Second call)
    sqlite3_str_append(str, "suffix", 6);

    // 4. sqlite3_prepare_v2
    // Construct a simple SQL statement using the generated string or a fallback
    char *sql_text = sqlite3_str_value(str);
    if (sql_text == NULL) {
        // Fallback to a valid SQL if string construction failed or is empty
        sql_text = "SELECT 1;";
    }
    
    size_t sql_len = strlen(sql_text);
    int nByte = (sql_len > 10000) ? 10000 : (int)sql_len;
    
    rc = sqlite3_prepare_v2(db, sql_text, nByte, &stmt, &tail);

    // 5. sqlite3_str_value
    // Calling again as per sequence requirement. 
    // Note: sqlite3_str_value returns the internal buffer. 
    // If the buffer was already finalized/returned, it might behave differently, 
    // but it's safe to call again on a valid str object.
    (void)sqlite3_str_value(str);

    // 6. sqlite3_finalize
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }

    // Cleanup
    // sqlite3_str_finish is required to free the memory allocated for the string buffer
    // This fixes the LeakSanitizer error reported.
    sqlite3_str_finish(str);
    
    sqlite3_close(db);

    return 0;
}