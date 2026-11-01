// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:104989:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to create a valid database and BLOB handle for fuzzing.
 * Returns 0 on success, non-zero on failure.
 * Sets *pDb and *pBlob to valid pointers on success.
 */
static int setup_blob_context(sqlite3 **pDb, sqlite3_blob **pBlob, const uint8_t *data, size_t size) {
    int rc;
    sqlite3_stmt *stmt = NULL;
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    const char *zData = NULL;
    int nData = 0;

    if (pDb == NULL || pBlob == NULL) return 1;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    /* Create a table with a BLOB column */
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    /* Insert a row with data to ensure the BLOB handle can be opened */
    const char *insert_sql = "INSERT INTO test_table (id, data) VALUES (1, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }

    /* 
     * FIX: Ensure the data pointer passed to sqlite3_bind_blob is valid and has a known lifetime.
     * Previously, if Size was 0 or Data was NULL, we used a local stack variable 'dummy'.
     * If the binding was deferred or the statement was re-executed, this could lead to 
     * stack-use-after-scope if the memory was accessed later.
     * We now allocate a heap buffer for the data to ensure it remains valid for the lifetime 
     * of the statement execution.
     */
    if (size > 0 && data != NULL) {
        zData = (const char *)data;
        nData = (int)size;
        sqlite3_bind_blob(stmt, 1, zData, nData, SQLITE_STATIC);
    } else {
        /* Allocate a small buffer on the heap to avoid stack issues */
        char *heap_dummy = (char *)sqlite3_malloc(1);
        if (heap_dummy) {
            heap_dummy[0] = 0;
            sqlite3_bind_blob(stmt, 1, heap_dummy, 1, SQLITE_TRANSIENT);
            sqlite3_free(heap_dummy);
        } else {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 1;
        }
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_finalize(stmt);

    /* Open the BLOB handle */
    rc = sqlite3_blob_open(db, "main", "test_table", "data", 1, 0, &blob);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    *pDb = db;
    *pBlob = blob;
    return 0;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    int rc;
    const char *err_msg;
    int blob_size;
    void *ptr;
    uint8_t random_buf[256];
    sqlite3_int64 new_rowid;

    /* 1. Prepare the environment */
    /* Setup database and blob handle */
    if (setup_blob_context(&db, &blob, Data, Size) != 0) {
        /* If setup fails, try to exercise the error path of sqlite3_errmsg with a valid db */
        sqlite3_open(":memory:", &db);
        if (db) {
            sqlite3_errmsg(db);
            sqlite3_close(db);
        }
        return 0;
    }

    /* 2. Invoke the target functions in the specified order */

    /* Step 1: sqlite3_errmsg */
    /* Retrieve error message (likely NULL if no error yet) */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Use the result to avoid unused variable warnings */

    /* Step 2: sqlite3_blob_reopen */
    /* Try to reopen to a new rowid derived from input data */
    /* If Size is large enough, use input bytes to form a rowid, else use a fixed value */
    if (Size >= 8) {
        memcpy(&new_rowid, Data, 8);
    } else {
        new_rowid = 1; /* Default to existing row to avoid SQLITE_ABORT initially */
    }

    /* Attempt to reopen. This might fail if the row doesn't exist or handle is invalid.
       The spec says if it fails, the handle becomes aborted. */
    rc = sqlite3_blob_reopen(blob, new_rowid);
    (void)rc;

    /* Step 3: sqlite3_blob_bytes */
    /* Get the size. If handle was aborted in previous step, this returns 0. */
    blob_size = sqlite3_blob_bytes(blob);
    (void)blob_size;

    /* Step 4: sqlite3_realloc64 */
    /* Fuzz memory allocation.
       If Size > 0, use Size as the new size.
       If Size == 0, allocate a small buffer.
       We pass NULL to simulate allocation, or a pointer to simulate reallocation.
       To explore more states, we try both. */
    if (Size > 0) {
        /* Try reallocating a small existing buffer */
        void *old_ptr = sqlite3_malloc(10);
        if (old_ptr) {
            ptr = sqlite3_realloc64(old_ptr, (sqlite3_uint64)Size);
            if (ptr) {
                sqlite3_free(ptr);
            }
        }
    } else {
        /* Try allocating a new buffer with a fixed size */
        ptr = sqlite3_realloc64(NULL, 1024);
        if (ptr) {
            sqlite3_free(ptr);
        }
    }

    /* Step 5: sqlite3_randomness */
    /* Fill a buffer with randomness. Use Size to determine how many bytes to request. */
    int n_random = (Size > 256) ? 256 : (int)Size;
    if (n_random <= 0) n_random = 16;
    
    sqlite3_randomness(n_random, random_buf);

    /* 3. Cleanup */
    /* Close the BLOB handle */
    if (blob) {
        sqlite3_blob_close(blob);
    }

    /* Close the database connection */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}