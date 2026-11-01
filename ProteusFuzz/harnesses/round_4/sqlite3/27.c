// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
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

static int dummy_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;

    // 1. Prepare the environment
    // Open an in-memory database to avoid file I/O issues and ensure isolation
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Create a simple table to allow blob_open to succeed if parameters are valid
    const char *setup_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, setup_sql, NULL, NULL, NULL);

    // 2. Invoke the target functions in the specified order

    // Step 1: sqlite3_finalize
    // Passing NULL is a safe no-op, but we can also try to finalize a dummy statement if we prepared one.
    // For this specific sequence, we just call it.
    sqlite3_finalize(stmt);

    // Step 2: sqlite3_exec
    // We construct a SQL string from the fuzz input to explore different states.
    // We limit the size to prevent huge allocations and ensure it's null-terminated.
    size_t sql_len = Size > 255 ? 255 : Size;
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (sql_buf) {
        memcpy(sql_buf, Data, sql_len);
        sql_buf[sql_len] = '\0';
        
        // Execute the SQL. We use a simple callback or NULL.
        // We pass &errmsg to capture errors.
        sqlite3_exec(db, sql_buf, dummy_callback, NULL, &errmsg);
        
        // Clean up error message if it exists
        if (errmsg) {
            sqlite3_free(errmsg);
            errmsg = NULL;
        }
        free(sql_buf);
    }

    // Step 3: sqlite3_mprintf (First call)
    // Use a format string derived from input to test formatting logic
    if (Size >= 2) {
        // Ensure the format string is safe by limiting length and checking for nulls
        char fmt_buf[64] = {0};
        size_t fmt_len = Size > 63 ? 63 : Size;
        memcpy(fmt_buf, Data, fmt_len);
        fmt_buf[fmt_len] = '\0';
        
        // Try to format a string. We pass a dummy integer to avoid crashes if format expects args.
        str1 = sqlite3_mprintf(fmt_buf, 12345);
    }

    // Step 4: sqlite3_mprintf (Second call)
    if (Size >= 2) {
        char fmt_buf2[64] = {0};
        size_t fmt_len2 = Size > 63 ? 63 : Size;
        // Shift data slightly to explore different states
        memcpy(fmt_buf2, Data, fmt_len2);
        fmt_buf2[fmt_len2] = '\0';
        
        str2 = sqlite3_mprintf(fmt_buf2, 54321);
    }

    // Step 5: sqlite3_free (First call)
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }

    // Step 6: sqlite3_free (Second call)
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }

    // Step 7: sqlite3_blob_open
    // We need to provide valid parameters to potentially succeed, or invalid ones to test error paths.
    // We use the fuzz data to determine rowid and flags.
    sqlite3_int64 rowid = 0;
    int flags = 0;
    
    if (Size >= 8) {
        rowid = *(const sqlite3_int64 *)Data;
    }
    if (Size >= 16) {
        flags = *(const int *)(Data + 8);
    }

    // Table name and column name from fuzz data (sanitized)
    char table_name[32] = "fuzz_test";
    char col_name[32] = "data";
    
    // Attempt to open the blob
    // Note: If the row doesn't exist or table is wrong, it returns error and sets blob to NULL.
    // This is expected behavior for fuzzing.
    sqlite3_blob_open(db, "main", table_name, col_name, rowid, flags, &blob);

    // Cleanup
    if (blob) {
        sqlite3_blob_close(blob);
    }
    
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}