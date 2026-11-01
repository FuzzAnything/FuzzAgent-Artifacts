// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:187881:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Dummy callback function for sqlite3_trace.
 * It receives a void pointer and a const char pointer.
 */
static void dummy_trace_callback(void *pArg, const char *zSql) {
    (void)pArg;
    (void)zSql;
}

/*
 * Dummy scalar function implementation for sqlite3_create_function.
 */
static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
    sqlite3_result_null(context);
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /*
     * Step 1: Prepare the environment.
     * We use a dummy file for the database.
     */
    const char *filename = "./dummy_file";

    /*
     * Step 2: Invoke sqlite3_open_v2.
     * We use SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE to allow creation.
     */
    rc = sqlite3_open_v2(filename, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);

    if (rc != SQLITE_OK || db == NULL) {
        if (db != NULL) {
            sqlite3_close(db);
        }
        return 0;
    }

    /*
     * Step 3: Invoke sqlite3_db_config.
     * We use SQLITE_DBCONFIG_LOOKASIDE as the operation.
     * 
     * CRITICAL FIX: The original code passed integers where a buffer pointer (void*)
     * was expected as the 3rd argument. This caused a NULL/invalid pointer dereference.
     * We must allocate a valid buffer to pass to the API.
     */
    
    int lookaside_size = (Size > 0) ? (int)Data[0] : 128;
    int lookaside_count = (Size > 1) ? (int)Data[1] : 10;
    int lookaside_highwater = (Size > 2) ? (int)Data[2] : 0;

    /* Sanitize lookaside_size: must be > 0 and multiple of sizeof(void*). */
    if (lookaside_size <= 0 || (lookaside_size % sizeof(void*)) != 0) {
        lookaside_size = 128;
    }
    
    /* Allocate a buffer to pass as the 'pBuf' argument */
    void *lookaside_buf = malloc(lookaside_size * lookaside_count);
    
    if (lookaside_buf) {
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, lookaside_buf, lookaside_size, lookaside_count);
        (void)rc;
        /* Note: SQLite takes ownership of this buffer if successful. 
           We do not free it here to avoid double-free if SQLite manages it. 
           If the call fails, we should free it, but for fuzzing simplicity 
           and since we close the DB immediately, we let the OS reclaim it 
           or rely on SQLite's internal cleanup if it didn't take ownership.
           However, strictly speaking, if rc != SQLITE_OK, we should free. 
           Given the fuzzing context, leaking a small buffer on failure is acceptable 
           compared to crashing. */
        if (rc != SQLITE_OK) {
            free(lookaside_buf);
        }
    } else {
        /* If malloc fails, skip the config to avoid crash */
        (void)rc;
    }

    /*
     * Step 4: Invoke sqlite3_file_control.
     * We use "main" as the database name.
     * We use SQLITE_FCNTL_PERSIST_WAL as the opcode.
     */
    int file_control_arg = (Size > 3) ? (int)Data[3] : 0;
    rc = sqlite3_file_control(db, "main", SQLITE_FCNTL_PERSIST_WAL, &file_control_arg);
    (void)rc;

    /*
     * Step 5: Invoke sqlite3_db_config again.
     * We use SQLITE_DBCONFIG_ENABLE_FKEY.
     */
    int enable_fkey = (Size > 4) ? (int)Data[4] : 0;
    rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, enable_fkey, NULL);
    (void)rc;

    /*
     * Step 6: Invoke sqlite3_create_function.
     */
    int nArg = (Size > 5) ? (int)Data[5] : 0;
    rc = sqlite3_create_function(db, "fuzz_func", nArg, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
    (void)rc;

    /*
     * Step 7: Invoke sqlite3_trace.
     */
    void *prev_trace_arg = sqlite3_trace(db, dummy_trace_callback, NULL);
    (void)prev_trace_arg;

    /*
     * Step 8: Cleanup.
     */
    sqlite3_close(db);

    return 0;
}