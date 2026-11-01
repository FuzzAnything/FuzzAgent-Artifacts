// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
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

static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    int rc;
    char *sql = NULL;
    char *tail = NULL;
    char buffer[256] = {0};

    if (Size < 1) return 0;

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a table to work with for BLOB operations
    const char *create_sql = "CREATE TABLE test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Insert a row with some data to open a BLOB on
    const char *insert_sql = "INSERT INTO test_blob (data) VALUES (X'0000000000000000');";
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Open a BLOB handle for writing
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, SQLITE_OPEN_READWRITE, &blob);
    if (rc != SQLITE_OK || blob == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // 1. sqlite3_blob_write
    int write_offset = (Size > 1) ? (Data[0] % 10) : 0;
    int write_len = (Size > 2) ? (Data[1] % 10) : 0;
    // Ensure we don't write beyond bounds (BLOB is 8 bytes)
    if (write_offset + write_len > 8) write_len = 8 - write_offset;
    if (write_len > 0 && write_offset >= 0) {
        rc = sqlite3_blob_write(blob, &Data[2], write_len, write_offset);
    } else {
        rc = sqlite3_blob_write(blob, NULL, -1, -1);
    }

    // 2. sqlite3_errmsg
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg; // Suppress unused variable warning

    // 3. sqlite3_blob_read
    int read_offset = (Size > 3) ? (Data[2] % 10) : 0;
    int read_len = (Size > 4) ? (Data[3] % 10) : 0;
    if (read_offset + read_len > 8) read_len = 8 - read_offset;
    if (read_len > 0 && read_offset >= 0) {
        rc = sqlite3_blob_read(blob, buffer, read_len, read_offset);
    } else {
        rc = sqlite3_blob_read(blob, buffer, -1, -1);
    }

    // 4. sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    // Close the blob handle
    sqlite3_blob_close(blob);
    blob = NULL;

    // 5. sqlite3_create_function
    // Use data from input to vary function name slightly if possible, but keep it valid
    const char *func_name = "fuzz_func";
    int n_arg = (Size > 5) ? (Data[4] % 3) : 0; // 0, 1, or 2 args
    rc = sqlite3_create_function(db, func_name, n_arg, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);

    // 6. sqlite3_prepare_v2
    // Construct a simple SQL string using input data if possible, or a default
    if (Size > 10) {
        // Create a safe SQL string from input, ensuring it's null-terminated
        size_t sql_len = Size - 10;
        if (sql_len > 100) sql_len = 100;
        sql = malloc(sql_len + 1);
        if (sql) {
            memcpy(sql, &Data[10], sql_len);
            sql[sql_len] = '\0';
            // Ensure it's a valid SELECT or simple statement to avoid immediate parse errors if possible
            // But fuzzing benefits from raw data, so we pass it as is.
            rc = sqlite3_prepare_v2(db, sql, (int)sql_len, &stmt, &tail);
            free(sql);
        }
    } else {
        // Default fallback
        const char *default_sql = "SELECT 1;";
        rc = sqlite3_prepare_v2(db, default_sql, -1, &stmt, &tail);
    }

    // Cleanup
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}