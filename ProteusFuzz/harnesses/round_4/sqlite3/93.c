// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
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

/* Static helper to create a temporary database file if needed */
static int setup_temp_db(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return 0;
    /* Write some dummy data to ensure file exists */
    const char *dummy = "SQLite3 Fuzz Test Data";
    fwrite(dummy, 1, strlen(dummy), f);
    fclose(f);
    return 1;
}

/* Static callback for busy handler */
static int busy_handler_callback(void *arg, int count) {
    /* Fuzz the count to potentially trigger different logic paths, though logic is simple */
    (void)arg;
    (void)count;
    return 1; /* Retry */
}

/* Static callback for rollback hook */
static void rollback_callback(void *arg) {
    (void)arg;
    /* Do nothing */
}

/* Static callback for autovacuum pages */
static unsigned int autovac_callback(void *pClientData, const char *zSchema, 
                                     unsigned int nDbPage, unsigned int nFreePage, 
                                     unsigned int nBytePerPage) {
    (void)pClientData;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    /* Return a value based on free pages to test different branches */
    return nFreePage;
}

/* Destructor for autovacuum pages client data */
static void autovac_destructor(void *p) {
    (void)p;
    /* Cleanup if necessary */
}

int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    /* Ensure the file exists */
    if (!setup_temp_db(filename)) {
        return 0;
    }

    /* Open database in read-write mode */
    if (sqlite3_open(filename, &db) != SQLITE_OK) {
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    if (Size > 0) {
        /* Test sqlite3_busy_handler */
        /* Use first byte to decide if we set a handler or not */
        if (Data[0] % 2 == 0) {
            sqlite3_busy_handler(db, busy_handler_callback, (void *)(intptr_t)(Data[0]));
        } else {
            sqlite3_busy_handler(db, NULL, NULL);
        }

        /* Test sqlite3_wal_autocheckpoint */
        /* Use next byte for N value */
        int checkpoint_n = (Size > 1) ? (int)Data[1] : 0;
        sqlite3_wal_autocheckpoint(db, checkpoint_n);

        /* Test sqlite3_rollback_hook */
        /* Use next byte to decide callback or NULL */
        if (Size > 2) {
            if (Data[2] % 2 == 0) {
                sqlite3_rollback_hook(db, rollback_callback, (void *)(intptr_t)(Data[2]));
            } else {
                sqlite3_rollback_hook(db, NULL, NULL);
            }
        }

        /* Test sqlite3_autovacuum_pages */
        /* Use next byte to decide callback or NULL */
        if (Size > 3) {
            if (Data[3] % 2 == 0) {
                sqlite3_autovacuum_pages(db, autovac_callback, (void *)(intptr_t)(Data[3]), autovac_destructor);
            } else {
                sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
            }
        }
    }

    /* To test sqlite3_db_cacheflush, we need a write transaction */
    /* We execute a simple INSERT to start a transaction */
    const char *sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB); INSERT INTO fuzz_test (data) VALUES (X'00');";
    char *err_msg = NULL;
    if (sqlite3_exec(db, sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        /* If table creation fails, try to flush anyway to test error paths */
    }

    /* Start a transaction to allow cacheflush */
    if (sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL) == SQLITE_OK) {
        /* Perform some writes to create dirty pages */
        for (int i = 0; i < 10 && Size > (size_t)(i + 4); i++) {
            char insert_sql[256];
            int len = snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO fuzz_test (data) VALUES (X'%02x');", Data[i + 4]);
            if (len > 0 && len < (int)sizeof(insert_sql)) {
                sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
            }
        }
        
        /* Call sqlite3_db_cacheflush */
        sqlite3_db_cacheflush(db);
        
        /* Commit the transaction */
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }

    /* Cleanup */
    sqlite3_close_v2(db);
    
    /* Remove the dummy file */
    remove(filename);

    return 0;
}