// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    const char *text = "fuzz_result";
    sqlite3_result_text(ctx, text, -1, SQLITE_TRANSIENT);
}

static void dummy_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int *p = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
    if (p) {
        *p = 42;
    }
}

static void dummy_aggregate_final(sqlite3_context *ctx) {
    int *p = (int*)sqlite3_aggregate_context(ctx, 0);
    if (p) {
        sqlite3_result_int(ctx, *p);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void dummy_window_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int *p = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
    if (p) {
        *p += 1;
    }
}

static void dummy_window_final(sqlite3_context *ctx) {
    int *p = (int*)sqlite3_aggregate_context(ctx, 0);
    if (p) {
        sqlite3_result_int(ctx, *p);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

static void dummy_window_value(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 100);
}

static void dummy_window_inverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int *p = (int*)sqlite3_aggregate_context(ctx, sizeof(int));
    if (p && *p > 0) {
        *p -= 1;
    }
}

static void dummy_destroy(void *p) {
    /* Nothing to do, just a placeholder */
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    if (!db) {
        return 0;
    }
    
    /* Use fuzz data to create function names */
    char func_name[256];
    int func_name_len = Size < 255 ? (int)Size : 255;
    if (func_name_len > 0) {
        memcpy(func_name, Data, func_name_len);
        func_name[func_name_len] = '\0';
    } else {
        strcpy(func_name, "fuzz_func");
    }
    
    /* Create UTF-16 function name using unsigned short */
    static const unsigned short utf16_name[] = { 'f', 'u', 'z', 'z', '_', 'u', 't', 'f', '1', '6', 0 };
    
    /* Try different API variations based on fuzz data */
    int variant = Size > 0 ? Data[0] % 6 : 0;
    int nArg = Size > 1 ? (Data[1] % 10) - 1 : -1; /* Range: -1 to 8 */
    int eTextRep = Size > 2 ? Data[2] % 8 : SQLITE_UTF8;
    
    /* Add deterministic flag sometimes */
    if (Size > 3 && (Data[3] & 0x80)) {
        eTextRep |= SQLITE_DETERMINISTIC;
    }
    
    /* Add directonly flag sometimes */
    if (Size > 4 && (Data[4] & 0x40)) {
        eTextRep |= SQLITE_DIRECTONLY;
    }
    
    int result;
    
    switch (variant) {
        case 0:
            /* sqlite3_create_function_v2 with destroy callback */
            result = sqlite3_create_function_v2(
                db,
                func_name,
                nArg,
                eTextRep,
                (void*)0x1234,
                dummy_scalar_func,
                NULL,
                NULL,
                dummy_destroy
            );
            break;
            
        case 1:
            /* sqlite3_create_function_v2 aggregate with destroy */
            result = sqlite3_create_function_v2(
                db,
                func_name,
                nArg,
                eTextRep,
                (void*)0x5678,
                NULL,
                dummy_aggregate_step,
                dummy_aggregate_final,
                dummy_destroy
            );
            break;
            
        case 2:
            /* sqlite3_create_function16 */
            result = sqlite3_create_function16(
                db,
                utf16_name,
                nArg,
                eTextRep,
                (void*)0x9ABC,
                dummy_scalar_func,
                NULL,
                NULL
            );
            break;
            
        case 3:
            /* sqlite3_create_function scalar */
            result = sqlite3_create_function(
                db,
                func_name,
                nArg,
                eTextRep,
                (void*)0xDEF0,
                dummy_scalar_func,
                NULL,
                NULL
            );
            break;
            
        case 4:
            /* sqlite3_create_function aggregate */
            result = sqlite3_create_function(
                db,
                func_name,
                nArg,
                eTextRep,
                (void*)0x1111,
                NULL,
                dummy_aggregate_step,
                dummy_aggregate_final
            );
            break;
            
        case 5:
            /* sqlite3_create_window_function */
            result = sqlite3_create_window_function(
                db,
                func_name,
                nArg,
                eTextRep,
                (void*)0x2222,
                dummy_window_step,
                dummy_window_final,
                dummy_window_value,
                dummy_window_inverse,
                dummy_destroy
            );
            break;
            
        default:
            result = SQLITE_ERROR;
            break;
    }
    
    /* Try to use the created function if successful */
    if (result == SQLITE_OK) {
        char sql[512];
        if (variant == 5) {
            /* Window function */
            snprintf(sql, sizeof(sql), 
                     "SELECT %s(1) OVER () FROM (SELECT 1)", func_name);
        } else if (variant == 1 || variant == 4) {
            /* Aggregate function */
            snprintf(sql, sizeof(sql), 
                     "SELECT %s(1) FROM (SELECT 1 UNION SELECT 2)", func_name);
        } else {
            /* Scalar function */
            snprintf(sql, sizeof(sql), 
                     "SELECT %s(1, 2, 3)", func_name);
        }
        
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Do nothing, just exercise the function */
            }
            sqlite3_finalize(stmt);
        }
        
        /* Delete the function */
        sqlite3_create_function(
            db,
            func_name,
            nArg,
            eTextRep,
            NULL,
            NULL,
            NULL,
            NULL
        );
    }
    
    sqlite3_close(db);
    
    return 0;
}