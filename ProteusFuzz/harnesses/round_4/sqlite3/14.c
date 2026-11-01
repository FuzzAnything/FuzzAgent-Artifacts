// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_blob_read at sqlite3.c:104957:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_blob_write at sqlite3.c:104964:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_scalar_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
    sqlite3_result_text(context, "dummy", -1, SQLITE_STATIC);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *err_msg = NULL;
    int rc;
    int i;

    if (Size < 1) {
        return 0;
    }

    // Initialize database in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a table to work with for BLOB operations
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Insert a row with some data to have a valid BLOB to read/write
    // We use a simple fixed string to ensure the row exists
    rc = sqlite3_exec(db, "INSERT INTO test_blob (data) VALUES (X'00000000000000000000000000000000');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Open a BLOB handle for writing
    // We use the first row (id=1) and ensure we have write access (flags=1)
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, 1, &blob);
    
    if (blob != NULL) {
        // 1. sqlite3_blob_write
        // We try to write fuzzed data into the BLOB.
        // We limit the write size to avoid out of bounds if the BLOB is small.
        int blob_size = sqlite3_blob_bytes(blob);
        int write_len = 0;
        int offset = 0;

        if (Size > 0) {
            write_len = (Size < 100) ? (int)Size : 100;
            if (blob_size > 0) {
                write_len = (write_len > blob_size) ? blob_size : write_len;
                offset = (Data[0] % blob_size);
            } else {
                // If blob is empty, we can't write, but we call the function to test error handling
                write_len = 0; 
            }
        }

        if (write_len > 0) {
            rc = sqlite3_blob_write(blob, (const void*)(Data), write_len, offset);
        } else {
            // Attempt a write with 0 length or invalid offset to trigger error paths
            rc = sqlite3_blob_write(blob, (const void*)"dummy", 10, -1);
        }

        // 2. sqlite3_errmsg
        err_msg = sqlite3_errmsg(db);
        (void)err_msg; // Consume the result

        // 3. sqlite3_blob_read
        char read_buf[256];
        int read_len = 0;
        int read_offset = 0;

        if (blob_size > 0) {
            read_len = (Size < 256) ? (int)Size : 256;
            if (read_len > blob_size) read_len = blob_size;
            // Fix: Ensure Size >= 2 before accessing Data[1]
            if (Size >= 2) {
                read_offset = (Data[1] % blob_size);
            } else {
                read_offset = 0;
            }
        } else {
            read_len = 0;
        }

        if (read_len > 0) {
            rc = sqlite3_blob_read(blob, read_buf, read_len, read_offset);
        } else {
            // Attempt read with invalid parameters
            rc = sqlite3_blob_read(blob, read_buf, 10, -1);
        }

        // 4. sqlite3_errmsg
        err_msg = sqlite3_errmsg(db);
        (void)err_msg;

        // Close the BLOB handle
        sqlite3_blob_close(blob);
        blob = NULL;
    }

    // 5. sqlite3_create_function
    // Register a custom function using fuzzed data for name (sanitized)
    char func_name[32];
    if (Size > 0) {
        int name_len = (Data[0] % 31) + 1;
        if (name_len > Size) name_len = (int)Size;
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
    } else {
        strcpy(func_name, "fuzz_func");
    }

    // Ensure name is valid (no nulls in middle, though memcpy handles it)
    // Register the function
    rc = sqlite3_create_function(db, func_name, 0, SQLITE_UTF8, NULL, dummy_scalar_func, NULL, NULL);
    (void)rc;

    // 6. sqlite3_prepare_v2
    // Prepare a statement using fuzzed data as SQL
    // We ensure the SQL string is null-terminated for safety
    char sql_buf[1024];
    int sql_len = (Size < 1023) ? (int)Size : 1023;
    memcpy(sql_buf, Data, sql_len);
    sql_buf[sql_len] = '\0';

    // Try to prepare the statement
    rc = sqlite3_prepare_v2(db, sql_buf, sql_len, &stmt, NULL);

    // Cleanup
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }

    // Drop the table to clean up
    sqlite3_exec(db, "DROP TABLE IF EXISTS test_blob;", NULL, NULL, NULL);

    sqlite3_close(db);

    return 0;
}