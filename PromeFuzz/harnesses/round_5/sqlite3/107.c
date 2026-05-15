// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Global variables to pass fuzzer data to callbacks */
const uint8_t *g_fuzz_data = NULL;
size_t g_fuzz_size = 0;
size_t g_fuzz_offset = 0;

/* Dummy callbacks for function creation */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    sqlite3_result_text(ctx, "dummy", 5, SQLITE_TRANSIENT);
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    void *agg = sqlite3_aggregate_context(ctx, 128);
    if (agg) {
        memset(agg, 0xAA, 128);
    }
}

static void dummy_xFinal(sqlite3_context *ctx) {
    void *agg = sqlite3_aggregate_context(ctx, 0);
    if (agg) {
        sqlite3_result_text(ctx, "final", 5, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_text(ctx, "noagg", 5, SQLITE_TRANSIENT);
    }
}

static void dummy_xValue(sqlite3_context *ctx) {
    sqlite3_result_text(ctx, "value", 5, SQLITE_TRANSIENT);
}

static void dummy_xInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    void *agg = sqlite3_aggregate_context(ctx, 64);
    if (agg) {
        memset(agg, 0xBB, 64);
    }
}

static void dummy_xDestroy(void *p) {
    /* Nothing to do */
}

static void test_result_text(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    const char *text = "fuzz";
    int len = 4;
    
    if (g_fuzz_size - g_fuzz_offset > 60) {
        size_t text_len = g_fuzz_data[g_fuzz_offset + 4] % 256;
        if (text_len > 0 && g_fuzz_offset + 5 + text_len <= g_fuzz_size) {
            text = (const char*)&g_fuzz_data[g_fuzz_offset + 5];
            len = (int)text_len;
        }
    }
    
    if (g_fuzz_size - g_fuzz_offset > 5) {
        uint8_t destructor_type = g_fuzz_data[g_fuzz_offset + 5] % 4;
        switch (destructor_type) {
            case 0:
                sqlite3_result_text(ctx, text, len, SQLITE_TRANSIENT);
                break;
            case 1:
                sqlite3_result_text(ctx, text, len, SQLITE_STATIC);
                break;
            case 2:
                sqlite3_result_text(ctx, text, -1, SQLITE_TRANSIENT);
                break;
            case 3:
                sqlite3_result_text(ctx, "", 0, SQLITE_STATIC);
                break;
        }
    } else {
        sqlite3_result_text(ctx, text, len, SQLITE_TRANSIENT);
    }
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *errmsg = NULL;
    
    /* Set global fuzzer data for callbacks */
    g_fuzz_data = Data;
    g_fuzz_size = Size;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use first byte to determine test variations */
    uint8_t variant = Size > 0 ? Data[0] : 0;
    g_fuzz_offset = Size > 0 ? 1 : 0;
    
    /* Test sqlite3_create_function */
    if (Size - g_fuzz_offset > 10) {
        const char *func_name = "test_func";
        rc = sqlite3_create_function(
            db,
            func_name,
            (int)(Data[g_fuzz_offset] % 10) - 1,  /* nArg: -1 to 8 */
            SQLITE_UTF8 | (variant & 1 ? SQLITE_DETERMINISTIC : 0),
            (void*)0x1234,
            dummy_xFunc,
            NULL,
            NULL
        );
        
        /* Try to use the function */
        if (rc == SQLITE_OK) {
            sqlite3_exec(db, "SELECT test_func(1)", NULL, NULL, &errmsg);
            if (errmsg) sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    /* Test sqlite3_create_function_v2 */
    if (Size - g_fuzz_offset > 20) {
        const char *func_name2 = "test_func_v2";
        rc = sqlite3_create_function_v2(
            db,
            func_name2,
            (int)(Data[g_fuzz_offset + 1] % 5),  /* nArg: 0 to 4 */
            SQLITE_UTF16LE,
            (void*)0x5678,
            NULL,
            dummy_xStep,
            dummy_xFinal,
            dummy_xDestroy
        );
        
        if (rc == SQLITE_OK) {
            sqlite3_exec(db, "SELECT test_func_v2(1)", NULL, NULL, &errmsg);
            if (errmsg) sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    /* Test sqlite3_create_function16 (UTF-16 function name) */
    if (Size - g_fuzz_offset > 30) {
        static const unsigned short func_name16[] = {'t','e','s','t','1','6',0};
        rc = sqlite3_create_function16(
            db,
            func_name16,
            (int)(Data[g_fuzz_offset + 2] % 3),  /* nArg: 0 to 2 */
            SQLITE_UTF8,
            NULL,
            dummy_xFunc,
            NULL,
            NULL
        );
        
        if (rc == SQLITE_OK) {
            sqlite3_exec(db, "SELECT test16(1,2)", NULL, NULL, &errmsg);
            if (errmsg) sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    /* Test sqlite3_create_window_function */
    if (Size - g_fuzz_offset > 40) {
        const char *win_func = "test_win";
        rc = sqlite3_create_window_function(
            db,
            win_func,
            (int)(Data[g_fuzz_offset + 3] % 4),  /* nArg: 0 to 3 */
            SQLITE_UTF8,
            (void*)0x9ABC,
            dummy_xStep,
            dummy_xFinal,
            dummy_xValue,
            dummy_xInverse,
            dummy_xDestroy
        );
        
        if (rc == SQLITE_OK) {
            sqlite3_exec(db, "CREATE TABLE t1(x); INSERT INTO t1 VALUES(1),(2),(3);", 
                        NULL, NULL, &errmsg);
            if (!errmsg) {
                sqlite3_exec(db, "SELECT test_win(x) OVER () FROM t1", 
                            NULL, NULL, &errmsg);
            }
            if (errmsg) sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    /* Test sqlite3_aggregate_context directly via a custom aggregate */
    {
        static int agg_context_test_done = 0;
        if (!agg_context_test_done && Size - g_fuzz_offset > 50) {
            /* Create a custom aggregate that uses sqlite3_aggregate_context */
            rc = sqlite3_create_function(
                db,
                "test_agg_ctx",
                1,
                SQLITE_UTF8,
                NULL,
                NULL,
                dummy_xStep,
                dummy_xFinal
            );
            
            if (rc == SQLITE_OK) {
                sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t2(y); "
                                "INSERT INTO t2 VALUES(10),(20);", 
                            NULL, NULL, &errmsg);
                if (!errmsg) {
                    sqlite3_exec(db, "SELECT test_agg_ctx(y) FROM t2", 
                                NULL, NULL, &errmsg);
                }
                if (errmsg) sqlite3_free(errmsg);
                errmsg = NULL;
            }
            agg_context_test_done = 1;
        }
    }
    
    /* Test sqlite3_result_text with various inputs */
    {
        rc = sqlite3_create_function(
            db,
            "test_result",
            0,
            SQLITE_UTF8,
            NULL,
            test_result_text,
            NULL,
            NULL
        );
        
        if (rc == SQLITE_OK) {
            sqlite3_exec(db, "SELECT test_result()", NULL, NULL, &errmsg);
            if (errmsg) sqlite3_free(errmsg);
            errmsg = NULL;
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    /* Reset global variables */
    g_fuzz_data = NULL;
    g_fuzz_size = 0;
    g_fuzz_offset = 0;
    
    return 0;
}