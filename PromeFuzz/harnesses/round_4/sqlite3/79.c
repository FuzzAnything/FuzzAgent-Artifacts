// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callback functions for testing */
static void dummy_step(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    /* Use aggregate context */
    void* agg_ctx = sqlite3_aggregate_context(ctx, 16);
    if (agg_ctx) {
        /* Write some data to the aggregate context */
        memset(agg_ctx, 0xAA, 16);
    }
}

static void dummy_final(sqlite3_context* ctx) {
    /* Get aggregate context and set result */
    void* agg_ctx = sqlite3_aggregate_context(ctx, 0);
    if (agg_ctx) {
        sqlite3_result_text(ctx, "dummy_result", 12, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_text(ctx, "no_context", 10, SQLITE_TRANSIENT);
    }
}

static void dummy_value(sqlite3_context* ctx) {
    sqlite3_result_text(ctx, "dummy_value", 11, SQLITE_TRANSIENT);
}

static void dummy_inverse(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    /* Do nothing */
}

static void dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_text(ctx, "dummy_scalar", 12, SQLITE_TRANSIENT);
}

static void dummy_destroy(void* p) {
    /* Do nothing */
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    sqlite3* db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use fuzzer data to influence parameters */
    int nArg = (Size > 0) ? (Data[0] % 8) : 0;  /* 0-7 arguments */
    int eTextRep = (Size > 1) ? (Data[1] & 0x0F) : SQLITE_UTF8;
    
    /* Add flags based on fuzzer input */
    if (Size > 2) {
        if (Data[2] & 0x01) eTextRep |= SQLITE_DETERMINISTIC;
        if (Data[2] & 0x02) eTextRep |= SQLITE_DIRECTONLY;
    }
    
    /* Create function names from fuzzer data */
    char func_name[256];
    size_t name_len = (Size > 100) ? 100 : Size;
    if (name_len > 0) {
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
    } else {
        strcpy(func_name, "fuzz_func");
    }
    
    /* Test sqlite3_create_function */
    rc = sqlite3_create_function(
        db,
        func_name,
        nArg,
        eTextRep,
        NULL,
        dummy_func,
        dummy_step,
        dummy_final
    );
    
    /* Test sqlite3_create_function_v2 */
    if (Size > 10) {
        char func_name2[256];
        size_t name2_len = (Size > 200) ? 100 : (Size > 100 ? Size - 100 : 0);
        if (name2_len > 0) {
            memcpy(func_name2, Data + 100, name2_len);
            func_name2[name2_len] = '\0';
        } else {
            strcpy(func_name2, "fuzz_func_v2");
        }
        
        rc = sqlite3_create_function_v2(
            db,
            func_name2,
            nArg,
            eTextRep,
            (void*)0x1234,
            dummy_func,
            dummy_step,
            dummy_final,
            dummy_destroy
        );
    }
    
    /* Test sqlite3_create_window_function */
    if (Size > 20) {
        char func_name3[256];
        size_t name3_len = (Size > 300) ? 100 : (Size > 200 ? Size - 200 : 0);
        if (name3_len > 0) {
            memcpy(func_name3, Data + 200, name3_len);
            func_name3[name3_len] = '\0';
        } else {
            strcpy(func_name3, "fuzz_window");
        }
        
        rc = sqlite3_create_window_function(
            db,
            func_name3,
            nArg,
            eTextRep,
            (void*)0x5678,
            dummy_step,
            dummy_final,
            dummy_value,
            dummy_inverse,
            dummy_destroy
        );
    }
    
    /* Test sqlite3_create_function16 with UTF-16 name */
    if (Size > 30) {
        /* Create a simple UTF-16 function name using uint16_t */
        static const uint16_t utf16_name[] = {'f','u','z','z','1','6',0};
        rc = sqlite3_create_function16(
            db,
            utf16_name,
            nArg,
            eTextRep,
            NULL,
            dummy_func,
            NULL,
            NULL
        );
    }
    
    /* Execute a simple query to trigger function calls */
    sqlite3_stmt* stmt;
    const char* sql = "SELECT fuzz_func(1,2,3)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}