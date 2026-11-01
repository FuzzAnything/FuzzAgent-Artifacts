// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    void *ptr = NULL;
    char *formatted = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to ensure we have something to query
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // Prepare a simple SELECT statement
    const char *sql = "SELECT data FROM test WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_realloc
    // Allocate some initial memory to reallocate
    ptr = sqlite3_malloc(10);
    if (ptr != NULL) {
        // Try to reallocate with size derived from input
        int new_size = (Size % 1000) + 1;
        ptr = sqlite3_realloc(ptr, new_size);
        // We don't free here immediately, we might use it later or let it be freed at the end
    }

    // Bind a value to the statement to make it executable
    sqlite3_bind_int(stmt, 1, (int)(Data[0] % 10));

    // 2. sqlite3_step
    // Execute the statement
    rc = sqlite3_step(stmt);

    // 3. sqlite3_column_count
    // Get column count
    int col_count = sqlite3_column_count(stmt);
    (void)col_count; // Use variable to avoid warning

    // 4. sqlite3_mprintf
    // Format a string using input data if possible
    if (Size > 0) {
        // Create a safe format string to avoid crashes
        char fmt[64];
        int fmt_len = (Size < 63) ? (int)Size : 63;
        memcpy(fmt, Data, fmt_len);
        fmt[fmt_len] = '\0';
        
        // Ensure it's a valid format string by replacing potential % with %%
        for (int i = 0; i < fmt_len; i++) {
            if (fmt[i] == '%') {
                fmt[i] = 'x'; // Replace % with a safe character
            }
        }
        
        formatted = sqlite3_mprintf("%s", fmt);
    }

    // 5. sqlite3_column_text
    // Try to get text from the first column if we have rows
    if (rc == SQLITE_ROW) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        (void)text; // Use variable to avoid warning
    }

    // 6. sqlite3_free
    // Free the memory allocated by sqlite3_mprintf
    if (formatted != NULL) {
        sqlite3_free(formatted);
    }
    // Free the memory allocated by sqlite3_realloc
    if (ptr != NULL) {
        sqlite3_free(ptr);
    }

    // 7. sqlite3_reset
    // Reset the statement
    rc = sqlite3_reset(stmt);
    (void)rc;

    // Cleanup
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}