// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_error at sqlite3.c:92749:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:92807:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
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
 * Fuzz driver for sqlite3 result setting functions.
 * This driver creates a database connection, registers custom functions,
 * and invokes them with fuzzed input to test:
 * - sqlite3_result_value
 * - sqlite3_user_data
 * - sqlite3_result_subtype
 * - sqlite3_result_error_nomem
 * - sqlite3_result_null
 * - sqlite3_result_error_toobig
 */

static void *fuzz_user_data_ptr;

/* Custom function that uses sqlite3_result_value */
static void fuzz_func_result_value(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

/* Custom function that uses sqlite3_user_data */
static void fuzz_func_user_data(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    void *data = sqlite3_user_data(ctx);
    if (data != fuzz_user_data_ptr) {
        sqlite3_result_error(ctx, "user data mismatch", -1);
        return;
    }
    sqlite3_result_int(ctx, 1);
}

/* Custom function that uses sqlite3_result_subtype */
static void fuzz_func_subtype(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        int subtype = sqlite3_value_int(argv[0]);
        sqlite3_result_subtype(ctx, (unsigned int)subtype);
    }
    sqlite3_result_int(ctx, 42);
}

/* Custom function that uses sqlite3_result_error_nomem */
static void fuzz_func_error_nomem(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0 && sqlite3_value_int(argv[0]) == 1) {
        sqlite3_result_error_nomem(ctx);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

/* Custom function that uses sqlite3_result_null */
static void fuzz_func_null(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0 && sqlite3_value_int(argv[0]) == 1) {
        sqlite3_result_null(ctx);
    } else {
        sqlite3_result_int(ctx, 1);
    }
}

/* Custom function that uses sqlite3_result_error_toobig */
static void fuzz_func_error_toobig(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0 && sqlite3_value_int(argv[0]) == 1) {
        sqlite3_result_error_toobig(ctx);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    if (Size < 1) {
        return 0;
    }

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Set user data pointer for testing */
    fuzz_user_data_ptr = (void *)(intptr_t)Data[0];

    /* Register custom functions */
    rc = sqlite3_create_function(db, "fuzz_result_value", -1, SQLITE_UTF8, NULL, fuzz_func_result_value, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_create_function(db, "fuzz_user_data", -1, SQLITE_UTF8, fuzz_user_data_ptr, fuzz_func_user_data, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_create_function(db, "fuzz_subtype", -1, SQLITE_UTF8 | SQLITE_RESULT_SUBTYPE, NULL, fuzz_func_subtype, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_create_function(db, "fuzz_error_nomem", -1, SQLITE_UTF8, NULL, fuzz_func_error_nomem, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_create_function(db, "fuzz_null", -1, SQLITE_UTF8, NULL, fuzz_func_null, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    rc = sqlite3_create_function(db, "fuzz_error_toobig", -1, SQLITE_UTF8, NULL, fuzz_func_error_toobig, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    /* Execute various SQL statements using the fuzzed data */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_result_value(?), fuzz_user_data(), fuzz_subtype(?), fuzz_error_nomem(?), fuzz_null(?), fuzz_error_toobig(?)";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) goto cleanup;

    /* Bind fuzzed data to parameters */
    for (int i = 1; i <= 6; i++) {
        int val = (Size >= i) ? Data[i-1] : 0;
        rc = sqlite3_bind_int(stmt, i, val);
        if (rc != SQLITE_OK) goto cleanup;
    }

    /* Step through the statement */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* Consume the result row */
    }

    /* Try executing with different parameter counts */
    const char *sql2 = "SELECT fuzz_result_value(), fuzz_subtype(0), fuzz_error_nomem(0), fuzz_null(0), fuzz_error_toobig(0)";
    sqlite3_finalize(stmt);
    rc = sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Consume */
        }
    }

    /* Test with NULL input */
    const char *sql3 = "SELECT fuzz_result_value(NULL), fuzz_subtype(NULL)";
    sqlite3_finalize(stmt);
    rc = sqlite3_prepare_v2(db, sql3, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Consume */
        }
    }

cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}