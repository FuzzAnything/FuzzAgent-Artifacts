// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:186775:16 in sqlite3.h
// sqlite3_changes64 at sqlite3.c:186766:26 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_total_changes64 at sqlite3.c:186782:26 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:24817:16 in sqlite3.h
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

static int create_in_memory_db(sqlite3 **ppDb) {
    return sqlite3_open(":memory:", ppDb);
}

static int cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
        return 0;
    }
    return 1;
}

static int execute_statements(sqlite3 *db, const uint8_t *Data, size_t Size) {
    if (!db || !Data || Size == 0) {
        return 0;
    }

    char *sql = (char *)malloc(Size + 1);
    if (!sql) return 0;

    memcpy(sql, Data, Size);
    sql[Size] = '\0';

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    free(sql);
    return rc;
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = create_in_memory_db(&db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // Execute some SQL statements to modify the database
    execute_statements(db, Data, Size);

    // Fuzz sqlite3_changes
    int changes = sqlite3_changes(db);
    (void)changes; // Use the return value to avoid warnings

    // Fuzz sqlite3_changes64
    sqlite3_int64 changes64 = sqlite3_changes64(db);
    (void)changes64;

    // Fuzz sqlite3_total_changes
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;

    // Fuzz sqlite3_total_changes64
    sqlite3_int64 total_changes64 = sqlite3_total_changes64(db);
    (void)total_changes64;

    // Fuzz sqlite3_errcode
    int errcode = sqlite3_errcode(db);
    (void)errcode;

    // Fuzz sqlite3_db_status64 with various status codes
    sqlite3_int64 cur, hiw;
    int status_rc = sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiw, 0);
    (void)status_rc;

    // Try resetting the high-water mark
    status_rc = sqlite3_db_status64(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &cur, &hiw, 1);
    (void)status_rc;

    // Try with an invalid status code
    status_rc = sqlite3_db_status64(db, -1, &cur, &hiw, 0);
    (void)status_rc;

    // Cleanup
    cleanup_db(db);

    return 0;
}