// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_get_table at sqlite3.c:156571:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_free_table at sqlite3.c:156640:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int create_dummy_db(sqlite3 **out_db) {
    sqlite3 *db = NULL;
    int rc;
    const char *file = "./dummy_file";

    // Remove existing file to ensure a clean state
    unlink(file);

    rc = sqlite3_open(file, &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return -1;
    }

    // Create a simple table to ensure the DB is valid for queries
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, create_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return -1;
    }

    *out_db = db;
    return 0;
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char **azResult = NULL;
    int nRow = 0;
    int nColumn = 0;
    char *pzErrmsg = NULL;
    int rc;

    // 1. Prepare the environment
    if (create_dummy_db(&db) != 0) {
        return 0;
    }

    // Ensure Data is null-terminated for SQL usage if we treat it as a string
    // We create a buffer to hold the SQL string safely
    if (Size == 0) {
        cleanup_db(db);
        return 0;
    }

    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) {
        cleanup_db(db);
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    // 2. Invoke target functions diversely

    // Test sqlite3_extended_result_codes with both 0 and 1
    rc = sqlite3_extended_result_codes(db, 1);
    (void)rc; // Ignore return for fuzzing flow, but call it
    rc = sqlite3_extended_result_codes(db, 0);
    (void)rc;

    // Test sqlite3_set_errmsg with various codes
    rc = sqlite3_set_errmsg(db, SQLITE_OK, "Fuzz OK");
    (void)rc;
    rc = sqlite3_set_errmsg(db, SQLITE_ERROR, "Fuzz Error");
    (void)rc;
    rc = sqlite3_set_errmsg(db, SQLITE_MISUSE, NULL);
    (void)rc;

    // Test sqlite3_errcode and sqlite3_extended_errcode
    int err_code = sqlite3_errcode(db);
    int ext_err_code = sqlite3_extended_errcode(db);
    (void)err_code;
    (void)ext_err_code;

    // Test sqlite3_errstr with various codes
    const char *err_str = sqlite3_errstr(SQLITE_OK);
    const char *err_str_err = sqlite3_errstr(SQLITE_ERROR);
    const char *err_str_unknown = sqlite3_errstr(99999);
    (void)err_str;
    (void)err_str_err;
    (void)err_str_unknown;

    // Test sqlite3_get_table with the fuzzed SQL input
    // This is the primary target for exploration
    rc = sqlite3_get_table(
        db,
        sql_buf,
        &azResult,
        &nRow,
        &nColumn,
        &pzErrmsg
    );

    // Handle results of get_table
    if (rc != SQLITE_OK) {
        // Check error codes again after a failed operation
        (void)sqlite3_errcode(db);
        (void)sqlite3_extended_errcode(db);
        // Check error message
        const char *msg = sqlite3_errmsg(db);
        (void)msg;
    } else {
        // If successful, we can iterate or just verify the structure exists
        // We don't need to process the data deeply for fuzzing, just ensure no crash
        if (azResult) {
            // Optional: verify headers exist if nColumn > 0
            if (nColumn > 0 && nRow >= 0) {
                // Just accessing to ensure memory is valid
                (void)azResult[0];
            }
        }
    }

    // 3. Cleanup
    if (azResult) {
        sqlite3_free_table(azResult);
        azResult = NULL;
    }
    if (pzErrmsg) {
        sqlite3_free(pzErrmsg);
        pzErrmsg = NULL;
    }
    free(sql_buf);
    cleanup_db(db);

    return 0;
}