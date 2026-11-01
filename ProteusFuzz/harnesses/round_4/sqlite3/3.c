// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *insert_stmt = NULL;
    int rc;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    const char *select_sql = "SELECT data FROM test WHERE id = ?;";

    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL);
    if (rc != SQLITE_OK || insert_stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_finalize(insert_stmt);
        sqlite3_close(db);
        return 0;
    }

    // Prepare some data to bind if available
    if (Size > 0) {
        // We cannot directly bind the raw fuzz input as a sqlite3_value pointer.
        // Instead, we bind it as a blob or text to create a valid row.
        rc = sqlite3_bind_blob(insert_stmt, 1, Data, Size, SQLITE_TRANSIENT);
        if (rc == SQLITE_OK) {
            sqlite3_step(insert_stmt);
            sqlite3_reset(insert_stmt);
        }
    }

    // Bind a value to the select statement to ensure we might get a row
    // We bind the first byte as an integer ID if available, otherwise 1
    int bind_id = 1;
    if (Size > 0) {
        bind_id = (int)Data[0];
    }
    sqlite3_bind_int(stmt, 1, bind_id);

    // Execute the select statement
    rc = sqlite3_step(stmt);

    // Invoke the target API functions in the specified order
    if (rc == SQLITE_ROW) {
        // 1. sqlite3_column_text
        const unsigned char *text = sqlite3_column_text(stmt, 0);
        (void)text;

        // 2. sqlite3_bind_value
        // We need a valid sqlite3_value to bind. We get one from the column.
        sqlite3_value *val = sqlite3_column_value(stmt, 0);
        if (val) {
            // Binding to the same parameter index (1) is valid for testing
            sqlite3_bind_value(stmt, 1, val);
        }

        // 3. sqlite3_column_value
        val = sqlite3_column_value(stmt, 0);
        (void)val;

        // 4. sqlite3_bind_value
        if (val) {
            sqlite3_bind_value(stmt, 1, val);
        }

        // 5. sqlite3_column_value
        val = sqlite3_column_value(stmt, 0);
        (void)val;

        // 6. sqlite3_step
        // Step again to see if state changes or returns DONE/ROW
        rc = sqlite3_step(stmt);

        // 7. sqlite3_column_int
        // Only safe if rc == SQLITE_ROW, but fuzzing explores undefined behavior too.
        // We guard it to avoid crashes on NULL pointers if possible, but the API handles NULL.
        int int_val = sqlite3_column_int(stmt, 0);
        (void)int_val;

        // 8. sqlite3_reset
        sqlite3_reset(stmt);

        // Re-step to get a row again for subsequent calls
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // 9. sqlite3_column_bytes16
            int bytes16 = sqlite3_column_bytes16(stmt, 0);
            (void)bytes16;

            // 10. sqlite3_column_blob
            const void *blob = sqlite3_column_blob(stmt, 0);
            (void)blob;

            // 11. sqlite3_column_bytes16
            bytes16 = sqlite3_column_bytes16(stmt, 0);
            (void)bytes16;

            // 12. sqlite3_column_blob
            blob = sqlite3_column_blob(stmt, 0);
            (void)blob;

            // 13. sqlite3_column_bytes
            int bytes = sqlite3_column_bytes(stmt, 0);
            (void)bytes;

            // 14. sqlite3_column_blob
            blob = sqlite3_column_blob(stmt, 0);
            (void)blob;

            // 15. sqlite3_column_bytes
            bytes = sqlite3_column_bytes(stmt, 0);
            (void)bytes;

            // 16. sqlite3_column_blob
            blob = sqlite3_column_blob(stmt, 0);
            (void)blob;
        }
    }

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_finalize(insert_stmt);
    sqlite3_close(db);

    return 0;
}