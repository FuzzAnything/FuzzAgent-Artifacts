// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:78471:25 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* We cannot directly create sqlite3_value objects since they're opaque.
   Instead, we need to create them through SQLite APIs. */

static void test_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* This function will be called by SQLite and will use the target APIs */
    
    if (argc > 0) {
        /* Test sqlite3_value_bytes */
        int bytes = sqlite3_value_bytes(argv[0]);
        (void)bytes;
        
        /* Test sqlite3_value_blob */
        const void *blob = sqlite3_value_blob(argv[0]);
        (void)blob;
        
        /* Test sqlite3_value_int */
        int int_val = sqlite3_value_int(argv[0]);
        (void)int_val;
        
        /* Test sqlite3_value_int64 */
        sqlite3_int64 int64_val = sqlite3_value_int64(argv[0]);
        (void)int64_val;
        
        /* Test sqlite3_result_int64 */
        sqlite3_result_int64(ctx, int64_val);
        
        if (argc > 1) {
            /* Test sqlite3_result_value */
            sqlite3_result_value(ctx, argv[1]);
        }
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    if (Size < 1) return 0;
    
    /* Initialize SQLite once */
    if (!initialized) {
        if (sqlite3_initialize() != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    /* Open an in-memory database */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    /* Create a test function that uses our target APIs */
    if (sqlite3_create_function(db, "test_func", -1, SQLITE_UTF8, NULL, 
                                test_function, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Prepare a statement that calls our function with fuzzer data */
    sqlite3_stmt *stmt = NULL;
    
    /* Use the fuzzer data to create different types of bindings */
    int use_blob = (Data[0] & 0x01);
    int use_text = (Data[0] & 0x02);
    int use_int = (Data[0] & 0x04);
    int use_int64 = (Data[0] & 0x08);
    
    const char *sql = "SELECT test_func(?, ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        /* Bind first parameter based on fuzzer input */
        if (use_blob && Size > 1) {
            sqlite3_bind_blob(stmt, 1, Data + 1, (int)(Size - 1), SQLITE_TRANSIENT);
        } else if (use_text && Size > 1) {
            /* Ensure null termination for text */
            char *text = malloc(Size);
            if (text) {
                memcpy(text, Data, Size);
                text[Size-1] = '\0';
                sqlite3_bind_text(stmt, 1, text, -1, SQLITE_TRANSIENT);
                free(text);
            } else {
                sqlite3_bind_text(stmt, 1, "", 0, SQLITE_STATIC);
            }
        } else if (use_int && Size >= sizeof(int)) {
            int val = *(int*)Data;
            sqlite3_bind_int(stmt, 1, val);
        } else if (use_int64 && Size >= sizeof(sqlite3_int64)) {
            sqlite3_int64 val = *(sqlite3_int64*)Data;
            sqlite3_bind_int64(stmt, 1, val);
        } else {
            /* Bind as integer using first byte */
            sqlite3_bind_int(stmt, 1, (int)Data[0]);
        }
        
        /* Bind second parameter with different type */
        if (Size > 1) {
            if (use_blob) {
                sqlite3_bind_text(stmt, 2, "test", 4, SQLITE_STATIC);
            } else {
                sqlite3_bind_blob(stmt, 2, Data, (int)(Size > 100 ? 100 : Size), SQLITE_TRANSIENT);
            }
        }
        
        /* Execute the statement - this will call our test_function */
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* We don't need to do anything with the result */
        }
        
        sqlite3_finalize(stmt);
    }
    
    /* Test with different SQL to exercise more code paths */
    const char *sql2 = "SELECT test_func(CAST(? AS BLOB), CAST(? AS INTEGER))";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL) == SQLITE_OK) {
        if (Size > 0) {
            sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 50 ? 50 : Size), SQLITE_TRANSIENT);
            sqlite3_bind_int64(stmt, 2, (sqlite3_int64)Size);
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Ignore results */
            }
        }
        sqlite3_finalize(stmt);
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}