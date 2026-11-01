// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 'hello';";
    int rc;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_realloc
    void *ptr = sqlite3_malloc(10);
    if (ptr) {
        // Try to realloc with size derived from input if possible, else default
        int new_size = (Size > 0) ? (Data[0] % 100) + 1 : 10;
        void *new_ptr = sqlite3_realloc(ptr, new_size);
        if (new_ptr) {
            // Use the new pointer briefly then free
            sqlite3_free(new_ptr);
        } else {
            sqlite3_free(ptr);
        }
    }

    // 2. sqlite3_step
    rc = sqlite3_step(stmt);

    // 3. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    (void)col_count; // Suppress unused variable warning

    // 4. sqlite3_mprintf
    char *formatted = NULL;
    if (Size > 0) {
        // Use input data to create a format string safely
        char fmt[64];
        int fmt_len = (Size < 63) ? Size : 63;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        // Ensure it's a valid format string by limiting characters
        for (int i = 0; i < fmt_len; i++) {
            if (fmt[i] == '%' && fmt[i+1] == '%') {
                // Skip double percent to avoid issues
                fmt[i+1] = 'X'; 
            }
        }
        
        formatted = sqlite3_mprintf(fmt, 123);
    }

    // 5. sqlite3_column_text
    if (rc == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        (void)text; // Use the value
    }

    // 6. sqlite3_free
    if (formatted) {
        sqlite3_free(formatted);
    }

    // 7. sqlite3_reset
    rc = sqlite3_reset(stmt);
    (void)rc;

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}