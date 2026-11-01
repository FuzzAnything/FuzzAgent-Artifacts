// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int busy_callback(void *arg, int count) {
    (void)arg;
    (void)count;
    return 0;
}

static void setup_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int busy_handler_set = 0;

    if (Size < 1) {
        return 0;
    }

    setup_dummy_file();

    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Fuzz sqlite3_busy_handler
    if (Data[0] & 0x01) {
        rc = sqlite3_busy_handler(db, busy_callback, (void *)(uintptr_t)(Data[0] & 0xFF));
        if (rc == SQLITE_OK) {
            busy_handler_set = 1;
        }
    } else if (Data[0] & 0x02) {
        rc = sqlite3_busy_handler(db, NULL, NULL);
    }

    // Attempt to create a table and start a transaction to enable sqlite3_db_cacheflush
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        // Insert some data to make pages dirty
        char insert_sql[256];
        // Ensure we have enough data before accessing indices 1, 2, 3
        if (Size >= 4) {
            snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO test_table (data) VALUES (X'%02x%02x%02x');", 
                     Data[1] & 0xFF, Data[2] & 0xFF, Data[3] & 0xFF);
            sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
        } else {
            // Fallback for small inputs
            snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO test_table (data) VALUES (X'000000');");
            sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
        }
        
        // Fuzz sqlite3_db_cacheflush
        if (Data[0] & 0x04) {
            rc = sqlite3_db_cacheflush(db);
            // Check error codes after potential failure
            if (rc != SQLITE_OK) {
                (void)sqlite3_errcode(db);
                (void)sqlite3_extended_errcode(db);
                (void)sqlite3_system_errno(db);
            }
        }
        
        // Commit or rollback based on input
        if (Data[0] & 0x08) {
            sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        } else {
            sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        }
    }

    // Fuzz error retrieval functions directly
    if (Data[0] & 0x10) {
        (void)sqlite3_errcode(db);
        (void)sqlite3_extended_errcode(db);
        (void)sqlite3_system_errno(db);
    }

    // Close the database
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}