// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:83994:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:84168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Helper function to create a valid database file for testing */
static int create_test_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    /* Insert some data to ensure pages exist */
    for (int i = 0; i < 10; i++) {
        rc = sqlite3_exec(db, "INSERT INTO test (data) VALUES (randomblob(100));", NULL, NULL, NULL);
        if (rc != SQLITE_OK) break;
    }

    sqlite3_close(db);
    return 1;
}

/* Helper to safely close database connections */
static void close_db(sqlite3 **db) {
    if (db && *db) {
        sqlite3_close(*db);
        *db = NULL;
    }
}

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Ensure we have enough data to parse basic parameters */
    if (Size < 4) return 0;

    /* Create dummy files for source and destination */
    const char *src_file = "./dummy_src.db";
    const char *dst_file = "./dummy_dst.db";

    /* Initialize source database with data */
    if (!create_test_db(src_file)) return 0;

    /* Initialize destination database (empty or with minimal schema) */
    if (!create_test_db(dst_file)) return 0;

    sqlite3 *db_src = NULL;
    sqlite3 *db_dst = NULL;
    sqlite3_backup *backup = NULL;
    int rc;

    /* Open source database */
    rc = sqlite3_open(src_file, &db_src);
    if (rc != SQLITE_OK) {
        close_db(&db_dst);
        return 0;
    }

    /* Open destination database */
    rc = sqlite3_open(dst_file, &db_dst);
    if (rc != SQLITE_OK) {
        close_db(&db_src);
        return 0;
    }

    /* Use input data to drive fuzzing parameters */
    int page_count = (Data[0] % 100) + 1; /* 1 to 100 pages */
    int flags = Data[1];
    int schema_idx = Data[2] % 3; /* 0: main, 1: temp (invalid for deserialize), 2: attached */
    
    /* Fuzz sqlite3_backup_init */
    /* Try different schema names based on input */
    const char *zDestName = (schema_idx == 0) ? "main" : (schema_idx == 1 ? "temp" : "main");
    const char *zSrcName = (schema_idx == 0) ? "main" : (schema_idx == 1 ? "temp" : "main");

    backup = sqlite3_backup_init(db_dst, zDestName, db_src, zSrcName);

    if (backup) {
        /* Fuzz sqlite3_backup_step */
        /* Perform multiple steps to explore state */
        int step_count = (Data[3] % 50) + 1;
        for (int i = 0; i < step_count; i++) {
            int nPage = (Data[(i % Size)] % 10) + 1;
            if (nPage == 0) nPage = -1; /* Copy all remaining */
            
            rc = sqlite3_backup_step(backup, nPage);
            
            /* Fuzz sqlite3_backup_remaining */
            int remaining = sqlite3_backup_remaining(backup);
            (void)remaining; /* Suppress unused warning */

            /* Fuzz sqlite3_backup_pagecount */
            int pagecount = sqlite3_backup_pagecount(backup);
            (void)pagecount;

            if (rc == SQLITE_DONE || rc == SQLITE_ERROR || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                break;
            }
        }
        
        /* Finalize backup */
        sqlite3_backup_finish(backup);
        backup = NULL;
    }

    /* Fuzz sqlite3_deserialize */
    /* We need a valid serialized database buffer. 
       Since we can't easily serialize the source DB in a simple fuzzer without complex logic,
       we will attempt to deserialize a crafted buffer or a portion of the input data 
       to test error handling and edge cases.
       Note: sqlite3_deserialize requires a valid DB header. We will try to pass 
       the input data directly to trigger parsing errors or valid deserialization if lucky.
    */
    
    /* Only attempt if we have enough data to simulate a header (at least 100 bytes) */
    if (Size >= 100) {
        /* Allocate a buffer for deserialization */
        unsigned char *buf = (unsigned char *)sqlite3_malloc(Size);
        if (buf) {
            memcpy(buf, Data, Size);
            
            /* Fuzz schema name */
            const char *zSchema = (flags & 1) ? "main" : (flags & 2 ? "temp" : NULL);
            
            /* Fuzz flags */
            unsigned mFlags = (flags & 0x0F); 
            
            /* Attempt deserialization */
            rc = sqlite3_deserialize(db_dst, zSchema, buf, Size, Size, mFlags);
            (void)rc;
            
            sqlite3_free(buf);
        }
    }

    /* Cleanup */
    close_db(&db_src);
    close_db(&db_dst);

    /* Remove dummy files */
    remove(src_file);
    remove(dst_file);

    return 0;
}