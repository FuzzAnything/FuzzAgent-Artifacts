// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_trace at sqlite3.c:187881:18 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
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

/* Callback for sqlite3_trace */
static void trace_callback(void *pArg, const char *zSql) {
    (void)pArg;
    (void)zSql;
    /* Do nothing, just prevent crash */
}

/* Callback for sqlite3_create_function (scalar) */
static void dummy_func(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
    sqlite3_result_null(context);
}

/* Callback for sqlite3_create_function (aggregate step) */
static void dummy_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)context;
    (void)argc;
    (void)argv;
}

/* Callback for sqlite3_create_function (aggregate final) */
static void dummy_final(sqlite3_context *context) {
    (void)context;
    sqlite3_result_null(context);
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int op;
    void *pArg = NULL;

    if (Size < 10) {
        return 0;
    }

    /* 1. Prepare environment: sqlite3_open_v2 */
    /* Use a dummy file as requested */
    const char *filename = "./dummy_file";
    
    /* Extract flags from input if possible, otherwise default */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 1) {
        flags = (int)Data[0];
        if (flags == 0) flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    }

    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        /* If open fails, we can't proceed with the sequence */
        return 0;
    }

    /* 2. Invoke sqlite3_db_config (First call) */
    /* Use input data to determine operation code */
    op = (int)Data[1];
    /* Ensure we don't use invalid opcodes that might crash immediately without setup */
    /* Common opcodes: SQLITE_DBCONFIG_LOOKASIDE, SQLITE_DBCONFIG_ENABLE_FKEY, etc. */
    /* We will pass raw input to explore states, but handle potential errors */
    rc = sqlite3_db_config(db, op, 0, 0, 0);
    (void)rc; /* Ignore return for fuzzing state exploration */

    /* 3. Invoke sqlite3_file_control */
    /* zDbName: NULL or "main" */
    const char *zDbName = (Size > 2 && Data[2] % 2 == 0) ? "main" : NULL;
    int fileControlOp = (int)Data[3];
    /* Argument for file control */
    void *fcArg = (void *)&Data[4]; 
    
    rc = sqlite3_file_control(db, zDbName, fileControlOp, fcArg);
    (void)rc;

    /* 4. Invoke sqlite3_db_config (Second call) */
    op = (int)Data[4];
    rc = sqlite3_db_config(db, op, 0, 0, 0);
    (void)rc;

    /* 5. Invoke sqlite3_create_function */
    /* Function name: use a safe prefix or part of input */
    const char *funcName = "fuzz_func";
    int nArg = (int)Data[5];
    /* Clamp nArg to valid range [-1, 127] to avoid undefined behavior */
    if (nArg < -1) nArg = -1;
    if (nArg > 127) nArg = 127;

    int eTextRep = SQLITE_UTF8;
    if (Size > 6) {
        eTextRep = (int)Data[6];
        if (eTextRep != SQLITE_UTF8 && eTextRep != SQLITE_UTF16LE && eTextRep != SQLITE_UTF16BE) {
            eTextRep = SQLITE_UTF8;
        }
    }

    /* pApp: user data */
    void *pApp = (void *)&Data[7];

    /* Callbacks: scalar function */
    rc = sqlite3_create_function(db, funcName, nArg, eTextRep, pApp, dummy_func, NULL, NULL);
    (void)rc;

    /* Try creating an aggregate function as well to explore more states */
    rc = sqlite3_create_function(db, "fuzz_agg", nArg, eTextRep, pApp, NULL, dummy_step, dummy_final);
    (void)rc;

    /* 6. Invoke sqlite3_trace */
    /* Register the trace callback */
    void *prevArg = sqlite3_trace(db, trace_callback, pArg);
    (void)prevArg;

    /* Cleanup */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}