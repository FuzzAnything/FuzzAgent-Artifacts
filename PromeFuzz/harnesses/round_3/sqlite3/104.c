// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:78958:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:78932:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:78751:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:78947:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Callback function that will be called by SQLite with a valid context */
static void test_callback(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* This function is called by SQLite and provides a valid context */
    /* We'll use the user_data to determine which API to test */
    int *selector = (int *)sqlite3_user_data(ctx);
    
    switch (*selector) {
        case 0:
            sqlite3_result_error_nomem(ctx);
            break;
        case 1:
            sqlite3_result_int(ctx, 42); /* Fixed value for simplicity */
            break;
        case 2:
            sqlite3_result_error_code(ctx, SQLITE_ERROR);
            break;
        case 3:
            {
                /* Use wchar_t for UTF-16 string on platforms where wchar_t is 16-bit */
                static const wchar_t error_msg[] = L"Test error";
                sqlite3_result_error16(ctx, error_msg, sizeof(error_msg) - sizeof(wchar_t));
            }
            break;
        case 4:
            {
                sqlite3 *db = sqlite3_context_db_handle(ctx);
                /* Just access it, no need to do anything with it */
                (void)db;
            }
            break;
        case 5:
            sqlite3_result_error_toobig(ctx);
            break;
        default:
            break;
    }
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    /* Use first byte to determine which function to test */
    int selector = Data[0] % 6;
    
    /* Create an in-memory database */
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    /* Register a custom SQL function that will call our test functions */
    if (sqlite3_create_function(db, "test_func", 0, SQLITE_UTF8, &selector, 
                                test_callback, NULL, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Execute the function to trigger the callback with a valid context */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT test_func()";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        /* Step once to execute the function */
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}