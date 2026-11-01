// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int initialize_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return -1;
    }
    // Create a simple table to ensure we have data to select
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT);";
    rc = sqlite3_exec(*db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Initialize database
    if (initialize_db(&db) != 0) {
        return 0;
    }

    // Prepare a simple SELECT statement
    const char *sql = "SELECT id, name FROM test;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_realloc
    // We allocate a small buffer to resize. Note: sqlite3_realloc is for memory allocated by sqlite3_malloc.
    // For fuzzing, we can try to realloc a NULL pointer (which acts like malloc) or a small buffer.
    void *ptr = NULL;
    // Use fuzzing data to determine size, but clamp to reasonable limits to avoid OOM or crashes
    int new_size = (Size > 0) ? (Data[0] % 1024) + 1 : 1;
    if (new_size < 1) new_size = 1;
    
    ptr = sqlite3_realloc(ptr, new_size);
    if (ptr) {
        // Write some fuzzing data to the buffer to ensure it's used
        size_t write_len = (Size < (size_t)new_size) ? Size : (size_t)new_size;
        if (write_len > 0) {
            memcpy(ptr, Data, write_len);
        }
    }

    // 2. sqlite3_step
    // Execute the statement. It might return SQLITE_ROW, SQLITE_DONE, or error.
    rc = sqlite3_step(stmt);

    // 3. sqlite3_column_count
    // Get column count regardless of step result
    int col_count = sqlite3_column_count(stmt);

    // 4. sqlite3_mprintf
    // Format a string using fuzzing data if possible, or a fixed format
    char *formatted = NULL;
    if (Size > 0 && Data[0] != 0) {
        // Ensure we don't pass a pointer that looks like a format string if it's binary garbage
        // We'll use a safe format string and pass a substring of data if it's printable, or just a number.
        // To be safe and diverse, let's try to interpret a small part as an integer for %d
        int val = (Size > 1) ? (Data[1] % 100) : 0;
        formatted = sqlite3_mprintf("Value: %d, Count: %d", val, col_count);
    } else {
        formatted = sqlite3_mprintf("Default: %d", col_count);
    }

    // 5. sqlite3_column_text
    // Try to get text from the first column if we have rows and columns
    if (rc == SQLITE_ROW && col_count > 0) {
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        (void)text; // Use the pointer to avoid unused variable warning
        // We don't free 'text' as it is managed by SQLite
    }

    // 6. sqlite3_free
    // Free the memory allocated by sqlite3_mprintf
    if (formatted) {
        sqlite3_free(formatted);
    }

    // Also free the realloc'd buffer
    if (ptr) {
        sqlite3_free(ptr);
    }

    // 7. sqlite3_reset
    // Reset the statement to reuse it
    rc = sqlite3_reset(stmt);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}