// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_result_text16be at sqlite3.c:92880:17 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:93455:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
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

/*
 * Static helper to create a dummy database and prepare a statement
 * to trigger the user-defined function.
 */
static int create_test_db(sqlite3 **ppDb, const char *sql, sqlite3_stmt **ppStmt) {
    int rc;
    if (ppDb == NULL || ppStmt == NULL) return SQLITE_ERROR;

    rc = sqlite3_open(":memory:", ppDb);
    if (rc != SQLITE_OK) {
        *ppDb = NULL;
        *ppStmt = NULL;
        return rc;
    }

    rc = sqlite3_exec(*ppDb, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*ppDb);
        *ppDb = NULL;
        *ppStmt = NULL;
        return rc;
    }

    rc = sqlite3_prepare_v2(*ppDb, sql, -1, ppStmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*ppDb);
        *ppDb = NULL;
        *ppStmt = NULL;
        return rc;
    }

    return SQLITE_OK;
}

/*
 * Destructor for auxdata and result data.
 */
static void dummy_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * User-defined function implementation that exercises the target APIs.
 * It uses the fuzzing input to decide which API to call and what data to pass.
 */
static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (ctx == NULL) return;

    /* We need a way to get the fuzzing input inside the callback.
     * Since we can't pass global state easily in a thread-safe way without
     * complex setup, we will use the arguments passed to the function
     * to simulate the fuzzing input.
     * However, the standard approach for fuzzing custom functions is to
     * have the driver set up the context and call the function directly
     * or via a prepared statement with specific arguments.
     * Here, we will assume the first argument contains the "mode" byte
     * and the second argument (if present) contains the data buffer.
     */

    int mode = 0;
    const void *data = NULL;
    int len = 0;
    void (*destructor)(void*) = NULL;

    if (argc > 0) {
        mode = sqlite3_value_int(argv[0]);
    }

    if (argc > 1) {
        data = sqlite3_value_blob(argv[1]);
        len = sqlite3_value_bytes(argv[1]);
    }

    /* Allocate some dummy data for auxdata if needed */
    void *aux_data = NULL;
    if (mode & 0x10) {
        aux_data = malloc(len > 0 ? len : 1);
        if (aux_data && len > 0) {
            memcpy(aux_data, data, len);
        }
        destructor = dummy_destructor;
    }

    /*
     * Select the API to fuzz based on the mode byte.
     * We ensure we don't pass NULL context or invalid lengths where possible,
     * but we also try to trigger edge cases (e.g., odd lengths for 16-bit).
     */
    switch (mode & 0x0F) {
        case 0:
            /* sqlite3_result_text16 */
            /* Ensure even length for 16-bit */
            if (len % 2 != 0) len--;
            if (len < 0) len = 0;
            sqlite3_result_text16(ctx, data, len, destructor);
            break;

        case 1:
            /* sqlite3_result_text16be */
            if (len % 2 != 0) len--;
            if (len < 0) len = 0;
            sqlite3_result_text16be(ctx, data, len, destructor);
            break;

        case 2:
            /* sqlite3_set_auxdata */
            /* Store data for argument index 0 */
            sqlite3_set_auxdata(ctx, 0, aux_data, destructor);
            break;

        case 3:
            /* sqlite3_result_text16le */
            if (len % 2 != 0) len--;
            if (len < 0) len = 0;
            sqlite3_result_text16le(ctx, data, len, destructor);
            break;

        case 4:
            /* sqlite3_result_text */
            /* Treat data as UTF-8. If len is -1, it expects null-terminated.
             * We pass the calculated len. */
            sqlite3_result_text(ctx, (const char*)data, len, destructor);
            break;

        case 5:
            /* sqlite3_result_blob */
            sqlite3_result_blob(ctx, data, len, destructor);
            break;

        default:
            /* Fallback: result null */
            sqlite3_result_null(ctx);
            break;
    }

    /* Clean up aux_data if we allocated it and didn't pass it to set_auxdata
     * or if set_auxdata failed (it doesn't return error, but we might have
     * passed it to result functions instead).
     * Note: If we passed it to set_auxdata, SQLite will call the destructor.
     * If we passed it to result functions, SQLite will call the destructor.
     * We must not free it here if we passed it to SQLite.
     */
    /* In this logic, we pass 'destructor' to the functions.
     * If we allocated aux_data, we passed it to set_auxdata (case 2).
     * For other cases, we passed 'data' (from argv) to result functions.
     * We should not free 'aux_data' here if we passed it to SQLite.
     * However, for cases 0,1,3,4,5, we passed 'data' which is from argv (managed by SQLite).
     * We only allocated 'aux_data' for case 2.
     * So we don't free here.
     */
}

/*
 * LLVM Fuzzer Test Input
 */
int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /*
     * Create a database and register the fuzz function.
     * The function takes 2 arguments:
     * 1. An integer (mode)
     * 2. A blob (data)
     */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, val BLOB);";
    const char *insert_sql = "INSERT INTO test_table (val) VALUES (X'00');";
    const char *select_sql = "SELECT fuzz_func(?, ?) FROM test_table LIMIT 1;";

    if (Size < 2) {
        /* Minimum size to have a mode and some data */
        return 0;
    }

    /* Initialize database */
    rc = create_test_db(&db, create_sql, &stmt);
    if (rc != SQLITE_OK) {
        return 0;
    }
    sqlite3_finalize(stmt);

    /* Register the function */
    rc = sqlite3_create_function(db, "fuzz_func", 2, SQLITE_UTF8, NULL, fuzz_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Ensure table has data */
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare the select statement */
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Bind arguments from fuzzing input.
     * Byte 0: Mode (0-15)
     * Rest: Data blob
     */
    int mode = Data[0] & 0x0F;
    const uint8_t *blob_data = Data + 1;
    int blob_len = Size - 1;

    sqlite3_bind_int(stmt, 1, mode);
    sqlite3_bind_blob(stmt, 2, blob_data, blob_len, SQLITE_STATIC);

    /* Execute the statement to trigger the function */
    rc = sqlite3_step(stmt);
    /* We don't care about the result, just that it doesn't crash */
    (void)rc;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}