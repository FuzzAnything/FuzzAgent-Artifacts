// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:92420:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:92616:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:92502:24 in sqlite3.h
// sqlite3_value_text16be at sqlite3.c:92505:24 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:92508:24 in sqlite3.h
// sqlite3_value_free at sqlite3.c:92642:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB); INSERT INTO test VALUES (1, ?); SELECT data FROM test WHERE id = 1;";
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Prepare the statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Bind the fuzzing data as a BLOB
    sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_STATIC);

    // Step to execute INSERT
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    // Prepare for SELECT to retrieve the value
    const char *select_sql = "SELECT data FROM test WHERE id = 1;";
    sqlite3_finalize(stmt);
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step to get the row
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        sqlite3_value *val = sqlite3_column_value(stmt, 0);
        
        if (val != NULL) {
            // Test sqlite3_value_dup
            sqlite3_value *dup_val = sqlite3_value_dup(val);
            
            if (dup_val != NULL) {
                // Test accessors on the duplicated value
                const void *blob_data = sqlite3_value_blob(dup_val);
                (void)blob_data; // Use to avoid unused warning

                const void *text16 = sqlite3_value_text16(dup_val);
                (void)text16;

                const void *text16be = sqlite3_value_text16be(dup_val);
                (void)text16be;

                const void *text16le = sqlite3_value_text16le(dup_val);
                (void)text16le;

                // Free the duplicated value
                sqlite3_value_free(dup_val);
            }
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}