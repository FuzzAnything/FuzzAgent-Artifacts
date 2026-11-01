// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:93805:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:93828:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *zTail = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Create an in-memory database to avoid file I/O issues
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Create a table with a declared type to test sqlite3_column_decltype
    const char *createTableSQL = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL, data BLOB);";
    rc = sqlite3_exec(db, createTableSQL, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Insert some test data to ensure rows exist for sqlite3_column_int
    const char *insertSQL = "INSERT INTO test_table (id, name, value, data) VALUES (1, 'test', 3.14, X'00FF');";
    rc = sqlite3_exec(db, insertSQL, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a SELECT statement using the fuzzed input as SQL
    // We limit the length to avoid potential issues with extremely long inputs
    int sqlLen = (int)Size;
    if (sqlLen > 10000) {
        sqlLen = 10000;
    }

    rc = sqlite3_prepare_v2(db, (const char *)Data, sqlLen, &stmt, &zTail);
    if (rc != SQLITE_OK) {
        // If preparation fails, we can still test sqlite3_sql on a valid statement
        // Let's prepare a known good statement to test the other APIs
        rc = sqlite3_prepare_v2(db, "SELECT id, name, value, data FROM test_table;", -1, &stmt, &zTail);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
    }

    if (stmt != NULL) {
        // Test sqlite3_sql
        const char *sql = sqlite3_sql(stmt);
        (void)sql; // Use the return value to avoid unused variable warning

        // Test sqlite3_column_count
        int colCount = sqlite3_column_count(stmt);
        if (colCount > 0) {
            // Step through the result set
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Test sqlite3_column_name for each column
                for (int i = 0; i < colCount; i++) {
                    const char *colName = sqlite3_column_name(stmt, i);
                    (void)colName;

                    // Test sqlite3_column_decltype for each column
                    const char *colDeclType = sqlite3_column_decltype(stmt, i);
                    (void)colDeclType;

                    // Test sqlite3_column_int for each column
                    int colInt = sqlite3_column_int(stmt, i);
                    (void)colInt;
                }
            }
        }

        // Reset the statement to allow re-execution if needed
        sqlite3_reset(stmt);
    }

    // Cleanup
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}