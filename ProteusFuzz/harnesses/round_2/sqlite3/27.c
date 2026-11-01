// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:33355:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int dummy_callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *err_msg = NULL;
    char *str1 = NULL;
    char *str2 = NULL;
    sqlite3_blob *blob = NULL;
    
    // 1. Prepare environment: Open database in memory
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // Ensure we have a table to work with for blob_open later
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, init_sql, NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }

    // Prepare a dummy statement to finalize later
    const char *prep_sql = "SELECT 1;";
    sqlite3_prepare_v2(db, prep_sql, -1, &stmt, NULL);

    // 2. Invoke Target Functions in Order

    // 1. sqlite3_finalize
    sqlite3_finalize(stmt);
    stmt = NULL;

    // 2. sqlite3_exec
    size_t sql_len = (Size < 4096) ? Size : 4096;
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (sql_buf) {
        memcpy(sql_buf, Data, sql_len);
        sql_buf[sql_len] = '\0';
        
        sqlite3_exec(db, sql_buf, dummy_callback, NULL, &err_msg);
        
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
        free(sql_buf);
    }

    // 3. sqlite3_mprintf (First call)
    int val1 = 0;
    if (Size > 0) val1 = Data[0];
    if (Size > 1) val1 = (val1 << 8) | Data[1];
    
    // Fixed: Check Size > 0 before accessing Data[0]
    char c1 = (Size > 0) ? Data[0] : 'x';
    str1 = sqlite3_mprintf("Value: %d, InputStart: %c", val1, c1);

    // 4. sqlite3_mprintf (Second call)
    int val2 = 0;
    if (Size > 2) val2 = Data[2];
    if (Size > 3) val2 = (val2 << 8) | Data[3];
    
    // Fixed: Check Size > 1 before accessing Data[1]
    char c2 = (Size > 1) ? Data[1] : 'x';
    str2 = sqlite3_mprintf("Value: %d, InputStart: %c", val2, c2);

    // 5. sqlite3_free (First call)
    if (str1) {
        sqlite3_free(str1);
        str1 = NULL;
    }

    // 6. sqlite3_free (Second call)
    if (str2) {
        sqlite3_free(str2);
        str2 = NULL;
    }

    // 7. sqlite3_blob_open
    const char *insert_sql = "INSERT OR IGNORE INTO test_table (id, data) VALUES (1, X'00');";
    sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = NULL;
    }

    sqlite3_int64 rowid = 1;
    if (Size > 4) {
        rowid = (sqlite3_int64)Data[4];
        if (Size > 5) rowid = (rowid << 8) | Data[5];
    }
    
    sqlite3_blob_open(db, "main", "test_table", "data", rowid, 0, &blob);

    // Cleanup
    if (blob) {
        sqlite3_blob_close(blob);
    }
    
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}