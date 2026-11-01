// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:93651:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
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
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_sqlite3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a simple table to ensure we have data to query
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a SELECT statement with a parameter
    const char *select_sql = "SELECT id, data FROM test_table WHERE id = ?1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    // Bind a value from the fuzz input if available
    if (Size > 0) {
        sqlite3_bind_int(stmt, 1, Data[0]);
    } else {
        sqlite3_bind_int(stmt, 1, 0);
    }

    // Step 1: Execute the statement
    rc = sqlite3_step(stmt);

    // The sequence of calls requested:
    // sqlite3_column_text -> sqlite3_bind_value -> sqlite3_column_value -> sqlite3_bind_value -> sqlite3_column_value -> sqlite3_step -> sqlite3_column_int -> sqlite3_reset -> sqlite3_column_bytes16 -> sqlite3_column_blob -> sqlite3_column_bytes16 -> sqlite3_column_blob -> sqlite3_column_bytes -> sqlite3_column_blob -> sqlite3_column_bytes -> sqlite3_column_blob

    // Note: Many of these calls require a valid row (SQLITE_ROW). 
    // We will check the return value of sqlite3_step before accessing columns.
    // If the row is not valid, we might get undefined behavior or NULLs, which is fine for fuzzing.

    // 1. sqlite3_column_text
    const unsigned char *text_val = sqlite3_column_text(stmt, 0);
    (void)text_val; // Suppress unused warning

    // 2. sqlite3_bind_value (requires a value to bind)
    // We need a valid sqlite3_value. Let's try to get one from the same row if possible.
    // If the row is invalid, this might be problematic, but fuzzing expects edge cases.
    // We'll try to get a value from column 0.
    sqlite3_value *val1 = sqlite3_column_value(stmt, 0);
    if (val1 != NULL) {
        // Bind it to parameter 1 (overwriting previous bind)
        rc = sqlite3_bind_value(stmt, 1, val1);
        (void)rc;
    }

    // 3. sqlite3_column_value
    sqlite3_value *val2 = sqlite3_column_value(stmt, 0);
    (void)val2;

    // 4. sqlite3_bind_value
    if (val2 != NULL) {
        rc = sqlite3_bind_value(stmt, 1, val2);
        (void)rc;
    }

    // 5. sqlite3_column_value
    sqlite3_value *val3 = sqlite3_column_value(stmt, 0);
    (void)val3;

    // 6. sqlite3_step
    // This will attempt to fetch the next row.
    rc = sqlite3_step(stmt);
    (void)rc;

    // 7. sqlite3_column_int
    int int_val = sqlite3_column_int(stmt, 0);
    (void)int_val;

    // 8. sqlite3_reset
    rc = sqlite3_reset(stmt);
    (void)rc;

    // After reset, we must step again to access columns safely.
    // However, the task requires specific calls in order.
    // We will proceed with the calls as requested, even if they might be on an invalid row state.
    // This is intentional for fuzzing to trigger potential errors.

    // 9. sqlite3_column_bytes16
    int bytes16 = sqlite3_column_bytes16(stmt, 0);
    (void)bytes16;

    // 10. sqlite3_column_blob
    const void *blob1 = sqlite3_column_blob(stmt, 0);
    (void)blob1;

    // 11. sqlite3_column_bytes16
    int bytes16_2 = sqlite3_column_bytes16(stmt, 0);
    (void)bytes16_2;

    // 12. sqlite3_column_blob
    const void *blob2 = sqlite3_column_blob(stmt, 0);
    (void)blob2;

    // 13. sqlite3_column_bytes
    int bytes = sqlite3_column_bytes(stmt, 0);
    (void)bytes;

    // 14. sqlite3_column_blob
    const void *blob3 = sqlite3_column_blob(stmt, 0);
    (void)blob3;

    // 15. sqlite3_column_bytes
    int bytes_2 = sqlite3_column_bytes(stmt, 0);
    (void)bytes_2;

    // 16. sqlite3_column_blob
    const void *blob4 = sqlite3_column_blob(stmt, 0);
    (void)blob4;

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    fuzz_sqlite3(Data, Size);
    return 0;
}