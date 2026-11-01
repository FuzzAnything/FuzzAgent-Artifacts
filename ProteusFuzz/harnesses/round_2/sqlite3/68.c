// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_backup_remaining at sqlite3.c:84479:16 in sqlite3.h
// sqlite3_backup_pagecount at sqlite3.c:84493:16 in sqlite3.h
// sqlite3_backup_finish at sqlite3.c:84425:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_backup_init at sqlite3.c:83994:28 in sqlite3.h
// sqlite3_backup_step at sqlite3.c:84168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 backup and deserialize APIs.
 * Target functions:
 * - sqlite3_backup_init
 * - sqlite3_backup_step
 * - sqlite3_backup_finish
 * - sqlite3_backup_remaining
 * - sqlite3_backup_pagecount
 * - sqlite3_deserialize
 */

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    sqlite3 *dbSource = NULL;
    sqlite3 *dbDest = NULL;
    sqlite3_backup *backup = NULL;
    int rc;
    int i;
    int nPage;
    int remaining;
    int pagecount;
    unsigned mFlags = 0;

    /*
     * Use a deterministic subset of the input to drive logic.
     * We need at least some bytes to create valid database files.
     */
    if (Size < 2) {
        return 0;
    }

    /*
     * Create source and destination databases in memory.
     * Using ":memory:" ensures no file I/O issues for the fuzzer.
     */
    rc = sqlite3_open(":memory:", &dbSource);
    if (rc != SQLITE_OK) {
        return 0;
    }

    rc = sqlite3_open(":memory:", &dbDest);
    if (rc != SQLITE_OK) {
        sqlite3_close(dbSource);
        return 0;
    }

    /*
     * Prepare a simple table and some data in the source database.
     * This ensures the backup has something to copy.
     */
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    char *err = NULL;
    rc = sqlite3_exec(dbSource, sql, NULL, NULL, &err);
    if (rc != SQLITE_OK) {
        sqlite3_free(err);
        sqlite3_close(dbSource);
        sqlite3_close(dbDest);
        return 0;
    }

    /*
     * Insert some data based on fuzz input to vary state.
     */
    for (i = 0; i < 10 && i < Size; i++) {
        char stmt[256];
        int val = Data[i];
        snprintf(stmt, sizeof(stmt), "INSERT INTO test (data) VALUES (X'%02X');", val);
        rc = sqlite3_exec(dbSource, stmt, NULL, NULL, NULL);
        (void)rc; /* Ignore errors for fuzzing diversity */
    }

    /*
     * Fuzz backup operations.
     * We attempt to initialize a backup from source to destination.
     */
    backup = sqlite3_backup_init(dbDest, "main", dbSource, "main");
    
    if (backup != NULL) {
        /*
         * Perform backup steps.
         * Use input data to determine number of pages to copy.
         * If Data[0] is 0, copy all remaining pages (-1).
         */
        nPage = (Size > 0) ? (int)Data[0] : 1;
        if (nPage == 0) nPage = -1; /* Copy all */
        
        /*
         * Loop to step through the backup.
         * Limit iterations to prevent infinite loops in case of weird states.
         */
        for (int step = 0; step < 100; step++) {
            rc = sqlite3_backup_step(backup, nPage);
            
            /*
             * Call remaining and pagecount to exercise those APIs.
             */
            remaining = sqlite3_backup_remaining(backup);
            pagecount = sqlite3_backup_pagecount(backup);
            
            if (rc == SQLITE_DONE || rc == SQLITE_BUSY || rc == SQLITE_LOCKED) {
                /*
                 * If DONE, we can stop.
                 * If BUSY/LOCKED, we might retry, but for fuzzing we stop after a few tries.
                 */
                if (rc == SQLITE_DONE) break;
                if (step > 5) break; 
            } else if (rc != SQLITE_OK) {
                /*
                 * On error, stop stepping.
                 */
                break;
            }
        }
        
        /*
         * Finish the backup.
         */
        rc = sqlite3_backup_finish(backup);
        backup = NULL;
    }

    /*
     * Fuzz sqlite3_deserialize.
     * We need a valid serialized database buffer.
     * We can create one by dumping the source database to a file, reading it,
     * or simply using the input data if it looks like a valid header.
     * However, to ensure we test the API, we will try to serialize the source DB.
     * Note: sqlite3_serialize is not in the target list, but we can use the input
     * as a potential serialized blob if it's large enough, or create a dummy one.
     * 
     * Strategy: Use the input data as the serialized buffer if Size is sufficient.
     * SQLite expects a valid database header (100 bytes).
     */
    
    /*
     * Attempt to deserialize into a new connection or reuse dbDest.
     * Let's create a new connection for this test to avoid conflicts with backup state.
     */
    sqlite3 *dbDeserialize = NULL;
    rc = sqlite3_open(":memory:", &dbDeserialize);
    if (rc == SQLITE_OK) {
        /*
         * We need a valid database image.
         * If the input data is too small to be a valid DB, we skip this part
         * or create a minimal valid DB in memory and serialize it.
         * Since we don't have sqlite3_serialize in the target list, we will
         * attempt to use the input data directly as the buffer, but only if
         * it's large enough to potentially contain a header.
         * 
         * To make this robust, we will create a small valid database in memory,
         * then use that as the source for deserialization if the input is invalid.
         * But wait, we can't easily serialize without the API.
         * 
         * Alternative: Use the input data as the buffer. If it fails, that's fine.
         * We just need to pass valid pointers.
         * 
         * Let's try to use the input data as the serialized content.
         * We need to ensure the buffer is large enough.
         */
        if (Size >= 100) {
            /*
             * Try to deserialize using the input data.
             * Flags: SQLITE_DESERIALIZE_FREEONCLOSE is risky if we don't own the memory.
             * We will not use FREEONCLOSE here since Data is owned by the fuzzer.
             * We might use RESIZEABLE if Size is large.
             */
            mFlags = 0;
            if (Size > 1000) {
                mFlags |= SQLITE_DESERIALIZE_RESIZEABLE;
            }
            
            /*
             * Cast Data to unsigned char*.
             */
            rc = sqlite3_deserialize(dbDeserialize, "main", (unsigned char*)Data, Size, Size, mFlags);
            (void)rc; /* Ignore return for fuzzing, just exercise the path */
        } else {
            /*
             * If input is too small, we can't form a valid DB.
             * We can try to create a minimal valid DB header manually?
             * No, that's complex. Just skip or pass NULL/invalid to test error handling.
             * Let's pass a small buffer to test error paths.
             */
            unsigned char dummy[100];
            memset(dummy, 0, sizeof(dummy));
            /* Write minimal header magic "SQLite format 3\000" */
            memcpy(dummy, "SQLite format 3\000", 16);
            rc = sqlite3_deserialize(dbDeserialize, "main", dummy, 100, 100, 0);
            (void)rc;
        }
        
        sqlite3_close(dbDeserialize);
    }

    /*
     * Cleanup
     */
    sqlite3_close(dbSource);
    sqlite3_close(dbDest);

    return 0;
}