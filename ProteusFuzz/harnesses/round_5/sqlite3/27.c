// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int callback(void *NotUsed, int argc, char **argv, char **colName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)colName;
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    sqlite3_blob *blob = NULL;

    // Initialize database in memory
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // 1. sqlite3_finalize
    // Invoking on NULL is safe and a no-op, but we can also try to finalize a dummy statement
    // to explore state if we had one. Here we just call it on NULL as per requirement order.
    sqlite3_finalize(stmt);

    // 2. sqlite3_exec
    // Prepare a safe SQL string from input to avoid immediate crashes, but allow fuzzing of content
    // We limit the length to avoid huge allocations and ensure null termination
    size_t sql_len = Size;
    if (sql_len > 4096) sql_len = 4096;
    
    char *sql = (char *)malloc(sql_len + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, sql_len);
    sql[sql_len] = '\0';

    // Execute the SQL. We pass NULL for callback to ignore results.
    sqlite3_exec(db, sql, callback, NULL, &errmsg);
    
    // Clean up error message if any
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = NULL;
    }

    free(sql);

    // 3. sqlite3_mprintf (first call)
    // Use a safe format string to avoid undefined behavior with fuzzed data as format string
    // We use a fixed format and fuzzed data as arguments if possible, or just fuzzed data as the string content
    // To be safe and explore states, we use a simple format.
    str1 = sqlite3_mprintf("Test: %s", (Size > 0 && Data[0] != 0) ? (char*)Data : "default");

    // 4. sqlite3_mprintf (second call)
    // Another call to explore memory allocation states
    if (Size > 1) {
        str2 = sqlite3_mprintf("Data: %d", (int)Data[1]);
    } else {
        str2 = sqlite3_mprintf("Data: 0");
    }

    // 5. sqlite3_free (first call)
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }

    // 6. sqlite3_free (second call)
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }

    // 7. sqlite3_blob_open
    // We need a table with a BLOB column to open a blob.
    // Create a table first if it doesn't exist.
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_blob (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // Insert a row to ensure we have something to open
    const char *insert_sql = "INSERT INTO fuzz_blob (data) VALUES (X'00000000');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);

    // Attempt to open the blob. 
    // zDb: "main", zTable: "fuzz_blob", zColumn: "data", iRow: 1, flags: 0 (readonly)
    sqlite3_blob_open(db, "main", "fuzz_blob", "data", 1, 0, &blob);

    // Cleanup blob handle if opened
    if (blob) {
        sqlite3_blob_close(blob);
    }

    // Final cleanup
    sqlite3_close(db);

    return 0;
}