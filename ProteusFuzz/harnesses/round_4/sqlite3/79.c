// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a valid sqlite3_context for testing.
 * Since sqlite3_context is opaque and internal, we cannot construct it manually
 * from scratch. Instead, we must invoke it via a real SQL function registration.
 * This approach ensures the context is valid and properly initialized by SQLite.
 */

/* xStep callback for aggregate function */
static void test_step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (ctx) {
        /* Fuzz sqlite3_aggregate_context with varying sizes */
        void *p = sqlite3_aggregate_context(ctx, 100);
        (void)p; /* Use the pointer to avoid unused warnings */
        
        /* Test error code setting */
        sqlite3_result_error_code(ctx, SQLITE_ERROR);
    }
}

/* xFinal callback for aggregate function */
static void test_final_func(sqlite3_context *ctx) {
    if (ctx) {
        /* Call with 0 to avoid reallocation as per docs */
        void *p = sqlite3_aggregate_context(ctx, 0);
        (void)p;
        
        /* Test result_int */
        sqlite3_result_int(ctx, 42);
        
        /* Test result_error16 with a properly sized UTF-16 string.
         * The previous error was due to passing a length (12) that did not match
         * the actual allocated size of the string literal or caused buffer issues.
         * We use a static array to ensure proper allocation and pass the correct byte count.
         * "Hello" in UTF-16 is 6 bytes (5 chars * 2) + 2 bytes for null terminator = 12 bytes.
         * However, to be safe and avoid any potential internal buffer assumptions,
         * we ensure the string is null-terminated and pass the correct length.
         */
        static const unsigned short utf16_msg[] = {'H', 'e', 'l', 'l', 'o', 0};
        sqlite3_result_error16(ctx, utf16_msg, sizeof(utf16_msg));
        
        /* Test result_zeroblob */
        sqlite3_result_zeroblob(ctx, 10);
        
        /* Test result_zeroblob64 */
        sqlite3_result_zeroblob64(ctx, 100);
    }
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;
    
    sqlite3 *db = NULL;
    int rc;
    
    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Register an aggregate function to trigger the callbacks */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_agg",
        0, /* nArg */
        SQLITE_UTF8,
        NULL, /* pApp */
        NULL, /* xFunc (scalar) */
        test_step_func, /* xStep (aggregate) */
        test_final_func, /* xFinal (aggregate) */
        NULL  /* xDestroy */
    );
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Execute a query that invokes the aggregate function */
    const char *sql = "SELECT fuzz_agg() FROM (SELECT 1 UNION ALL SELECT 2 UNION ALL SELECT 3);";
    
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Process row if needed */
        }
        sqlite3_finalize(stmt);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}