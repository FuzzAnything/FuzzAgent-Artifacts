// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_collation_needed at sqlite3.c:189437:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:189458:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:189407:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:189369:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to create a dummy database file if needed, though in-memory is preferred for fuzzing */
static int setup_db(sqlite3 **db_out) {
    int rc;
    /* Use in-memory database for speed and isolation */
    rc = sqlite3_open(":memory:", db_out);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db_out);
        *db_out = NULL;
        return 0;
    }
    return 1;
}

/* Static helper to cleanup database */
static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/* Dummy collation compare function for testing */
static int dummy_collation_compare(void *pArg, int nLeft, const void *pLeft, int nRight, const void *pRight) {
    (void)pArg;
    /* Simple byte comparison */
    if (nLeft == 0 && nRight == 0) return 0;
    if (nLeft == 0) return -1;
    if (nRight == 0) return 1;
    return memcmp(pLeft, pRight, nLeft < nRight ? nLeft : nRight);
}

/* Dummy collation needed callback for UTF-8 */
static void dummy_collation_needed_cb(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
    /* In a real scenario, we would register the collation here. */
}

/* Dummy collation needed callback for UTF-16 */
static void dummy_collation_needed_cb16(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
}

/* Dummy destructor for collation context */
static void dummy_collation_destroy(void *pArg) {
    (void)pArg;
}

/* Dummy SQL function to test sqlite3_context_db_handle */
static void dummy_sql_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (ctx) {
        sqlite3 *db = sqlite3_context_db_handle(ctx);
        (void)db;
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *coll_name_utf8 = "test_collation";
    const void *coll_name_utf16 = L"test_collation";
    int text_rep = SQLITE_UTF8;
    void *pArg = (void *)0x12345678;

    /* 1. Prepare the environment */
    if (!setup_db(&db)) {
        return 0;
    }

    /* Ensure we have some data to work with */
    if (Size == 0) {
        cleanup_db(db);
        return 0;
    }

    /* Use input data to drive fuzzing parameters */
    int param1 = Size > 0 ? Data[0] : 0;
    int param2 = Size > 1 ? Data[1] : 0;
    int param3 = Size > 2 ? Data[2] : 0;

    /* Map param2 to valid text representations */
    int valid_reps[] = {SQLITE_UTF8, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16, SQLITE_UTF16_ALIGNED};
    int rep_count = 5;
    int selected_rep = valid_reps[param2 % rep_count];

    /* 2. Invoke target functions diversely */

    /* Test sqlite3_collation_needed (UTF-8) */
    rc = sqlite3_collation_needed(db, pArg, dummy_collation_needed_cb);
    (void)rc;

    /* Test sqlite3_collation_needed16 (UTF-16) */
    rc = sqlite3_collation_needed16(db, pArg, dummy_collation_needed_cb16);
    (void)rc;

    /* Test sqlite3_create_collation (UTF-8) */
    /* Use input data to potentially create a custom name if it's valid */
    const char *custom_name = coll_name_utf8;
    
    /* Only use input data if it's null-terminated and safe */
    if (Size > 10) {
        /* Check for a null terminator within the buffer to ensure safety */
        int null_pos = -1;
        for (int i = 0; i < Size; i++) {
            if (Data[i] == 0) {
                null_pos = i;
                break;
            }
        }
        
        if (null_pos > 0 && null_pos < 64) {
            /* Create a temporary buffer for a custom name from input */
            static char buf[64];
            int len = null_pos;
            if (len >= 64) len = 63;
            memcpy(buf, Data, len);
            buf[len] = '\0';
            custom_name = buf;
        }
    }

    rc = sqlite3_create_collation(db, custom_name, selected_rep, pArg, dummy_collation_compare);
    (void)rc;

    /* Test sqlite3_create_collation16 (UTF-16) */
    rc = sqlite3_create_collation16(db, coll_name_utf16, selected_rep, pArg, dummy_collation_compare);
    (void)rc;

    /* Test sqlite3_create_collation_v2 */
    rc = sqlite3_create_collation_v2(db, custom_name, selected_rep, pArg, dummy_collation_compare, dummy_collation_destroy);
    (void)rc;

    /* Test removing a collation by passing NULL compare function */
    rc = sqlite3_create_collation(db, custom_name, selected_rep, pArg, NULL);
    (void)rc;

    /* Test sqlite3_context_db_handle */
    rc = sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, pArg, dummy_sql_func, NULL, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_stmt *stmt = NULL;
        const char *sql = "SELECT dummy_func()";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Function is executed here */
            }
        }
        if (stmt) {
            sqlite3_finalize(stmt);
        }
        sqlite3_create_function(db, "dummy_func", 0, SQLITE_UTF8, pArg, NULL, NULL, NULL);
    }

    /* 3. Cleanup */
    cleanup_db(db);

    return 0;
}