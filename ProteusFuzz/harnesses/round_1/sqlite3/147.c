// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_pointer at sqlite3.c:92485:18 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
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

static int init_db(sqlite3 **db) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

static int prepare_statement(sqlite3 *db, const char *sql, sqlite3_stmt **stmt) {
    if (sqlite3_prepare_v2(db, sql, -1, stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_statement(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = NULL;
    if (!init_db(&db)) return 0;

    // Create a simple table and insert data to have values to query
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT, blob_data BLOB);";
    const char *insert_sql = "INSERT INTO test (data, blob_data) VALUES ('hello', X'48454C4C4F');";
    
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);

    // Prepare a SELECT statement to retrieve values
    const char *select_sql = "SELECT id, data, blob_data FROM test WHERE id = ?;";
    sqlite3_stmt *stmt = NULL;
    if (!prepare_statement(db, select_sql, &stmt)) {
        cleanup_db(db);
        return 0;
    }

    // Bind an integer value from input data if available
    int bind_val = 0;
    if (Size > 0) {
        bind_val = Data[0];
    }
    sqlite3_bind_int(stmt, 1, bind_val);

    // Step through results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Test sqlite3_value_dup on different columns
        sqlite3_value *val_id = sqlite3_column_value(stmt, 0);
        sqlite3_value *val_data = sqlite3_column_value(stmt, 1);
        sqlite3_value *val_blob = sqlite3_column_value(stmt, 2);

        // Test sqlite3_value_dup
        sqlite3_value *dup_id = sqlite3_value_dup(val_id);
        sqlite3_value *dup_data = sqlite3_value_dup(val_data);
        sqlite3_value *dup_blob = sqlite3_value_dup(val_blob);

        // Test sqlite3_value_text16 on text column
        const void *text16_data = sqlite3_value_text16(val_data);
        (void)text16_data; // Suppress unused warning

        // Test sqlite3_value_text16le on text column
        const void *text16le_data = sqlite3_value_text16le(val_data);
        (void)text16le_data;

        // Test sqlite3_value_text16be on text column
        const void *text16be_data = sqlite3_value_text16be(val_data);
        (void)text16be_data;

        // Test sqlite3_value_pointer (will likely return NULL as we didn't bind a pointer)
        void *ptr_val = sqlite3_value_pointer(val_id, "test_type");
        (void)ptr_val;

        // Test sqlite3_value_free on duplicated values
        if (dup_id) sqlite3_value_free(dup_id);
        if (dup_data) sqlite3_value_free(dup_data);
        if (dup_blob) sqlite3_value_free(dup_blob);
    }

    cleanup_statement(stmt);
    cleanup_db(db);
    return 0;
}