// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "INSERT INTO test_table (col1) VALUES (?);";
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to ensure the statement can execute */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (col1 TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare the statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Step 1: sqlite3_reset */
    sqlite3_reset(stmt);

    /* Step 2: sqlite3_bind_text */
    if (Size > 0) {
        int len = (Size > 10000) ? 10000 : (int)Size;
        sqlite3_bind_text(stmt, 1, (const char *)Data, len, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_text(stmt, 1, "", 0, SQLITE_TRANSIENT);
    }

    /* Step 3: sqlite3_step */
    sqlite3_step(stmt);

    /* Step 4: sqlite3_errmsg */
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    /* Step 5: sqlite3_reset */
    sqlite3_reset(stmt);

    /* Step 6: sqlite3_changes */
    int changes = sqlite3_changes(db);
    (void)changes;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}