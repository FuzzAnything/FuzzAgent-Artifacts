// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void custom_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) return;

    // Fuzz sqlite3_value_encoding
    if (argc >= 1) {
        sqlite3_value_encoding(argv[0]);
    }

    // Fuzz sqlite3_value_bytes
    if (argc >= 1) {
        sqlite3_value_bytes(argv[0]);
    }

    // Fuzz sqlite3_value_blob
    if (argc >= 1) {
        const void *blob = sqlite3_value_blob(argv[0]);
        (void)blob; // Suppress unused variable warning
    }

    // Fuzz sqlite3_value_int64
    if (argc >= 1) {
        sqlite3_int64 val = sqlite3_value_int64(argv[0]);
        (void)val; // Suppress unused variable warning
    }

    // Fuzz sqlite3_value_text
    if (argc >= 1) {
        const unsigned char *text = sqlite3_value_text(argv[0]);
        (void)text; // Suppress unused variable warning
    }

    // Fuzz sqlite3_result_int64
    if (argc >= 2) {
        sqlite3_int64 res_val = sqlite3_value_int64(argv[1]);
        sqlite3_result_int64(ctx, res_val);
    } else {
        sqlite3_result_int64(ctx, 0);
    }
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a table to hold various data types
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB, text_col TEXT, int_col INTEGER);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Register a custom function to test the target APIs
    sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, NULL, custom_func, NULL, NULL);

    // Prepare an INSERT statement to fuzz data insertion
    const char *insert_sql = "INSERT INTO test_table (data, text_col, int_col) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Use input data to bind values
    size_t offset = 0;
    if (Size > 0) {
        // Bind BLOB data
        size_t blob_len = (Size > 1024) ? 1024 : Size;
        sqlite3_bind_blob(stmt, 1, Data, blob_len, SQLITE_TRANSIENT);

        // Bind TEXT data (ensure null termination)
        size_t text_len = (Size - blob_len > 1024) ? 1024 : (Size - blob_len);
        if (text_len > 0) {
            char *text_buf = (char *)malloc(text_len + 1);
            if (text_buf) {
                memcpy(text_buf, Data + blob_len, text_len);
                text_buf[text_len] = '\0';
                sqlite3_bind_text(stmt, 2, text_buf, -1, SQLITE_TRANSIENT);
                free(text_buf);
            }
        } else {
            sqlite3_bind_text(stmt, 2, "", 0, SQLITE_TRANSIENT);
        }

        // Bind INTEGER data
        sqlite3_int64 int_val = 0;
        if (Size > blob_len + text_len) {
            int_val = *(const int32_t *)(Data + blob_len + text_len);
        }
        sqlite3_bind_int64(stmt, 3, int_val);
    } else {
        sqlite3_bind_blob(stmt, 1, NULL, 0, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 2, "", 0, SQLITE_STATIC);
        sqlite3_bind_int64(stmt, 3, 0);
    }

    // Execute the INSERT
    rc = sqlite3_step(stmt);
    sqlite3_reset(stmt);

    // Prepare a SELECT statement to retrieve data and test APIs via custom function
    const char *select_sql = "SELECT fuzz_func(data, int_col) FROM test_table;";
    sqlite3_stmt *select_stmt = NULL;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL);
    if (rc == SQLITE_OK && select_stmt != NULL) {
        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // The custom function is called here, testing the target APIs
        }
        sqlite3_finalize(select_stmt);
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}