// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:95598:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:94103:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, val TEXT); "
                      "INSERT INTO test (val) VALUES (?); "
                      "SELECT val FROM test WHERE id = ?;";
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind a value from fuzz input if available */
    if (Size > 0) {
        /* Use first byte as a simple integer for binding */
        int bind_val = Data[0];
        rc = sqlite3_bind_int(stmt, 1, bind_val);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    /* Step to execute INSERT */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Reset statement for SELECT */
    sqlite3_reset(stmt);

    /* Bind second parameter for SELECT */
    if (Size > 1) {
        int bind_val2 = Data[1];
        rc = sqlite3_bind_int(stmt, 2, bind_val2);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    /* Step to execute SELECT */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Get value from column 0 */
    sqlite3_value *col_val = sqlite3_column_value(stmt, 0);
    if (col_val == NULL) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Test sqlite3_value_frombind */
    int from_bind = sqlite3_value_frombind(col_val);
    (void)from_bind;

    /* Test sqlite3_value_numeric_type */
    int num_type = sqlite3_value_numeric_type(col_val);
    (void)num_type;

    /* Test sqlite3_value_bytes16 */
    int bytes16 = sqlite3_value_bytes16(col_val);
    (void)bytes16;

    /* Test sqlite3_value_encoding */
    int encoding = sqlite3_value_encoding(col_val);
    (void)encoding;

    /* Test sqlite3_bind_value by binding the column value to a new statement */
    const char *sql2 = "INSERT INTO test (val) VALUES (?);";
    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_bind_value(stmt2, 1, col_val);
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt2);
        }
        sqlite3_finalize(stmt2);
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}