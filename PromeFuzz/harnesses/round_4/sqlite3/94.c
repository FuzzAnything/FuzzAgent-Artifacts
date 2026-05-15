// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Dummy function for sqlite3_create_function_v2 */
static void dummy_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

/* Dummy step function for aggregate */
static void dummy_step(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

/* Dummy final function for aggregate */
static void dummy_final(sqlite3_context* ctx) {
    (void)ctx;
}

/* Dummy destroy function */
static void dummy_destroy(void* p) {
    (void)p;
}

/* Static variable to store context */
static sqlite3_context* captured_ctx = NULL;

/* Function that captures the context */
static void capture_context_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)argc;
    (void)argv;
    captured_ctx = ctx;
    
    /* Test sqlite3_result_null */
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    /* Reset captured context */
    captured_ctx = NULL;
    
    /* Open an in-memory database to get a valid sqlite3 context */
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    /* Create a user-defined function to get a valid sqlite3_context */
    int rc = sqlite3_create_function_v2(
        db,
        "test_func",
        0,
        SQLITE_UTF8,
        NULL,
        dummy_func,
        NULL,
        NULL,
        NULL
    );
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Prepare a statement that calls our function */
    sqlite3_stmt* stmt = NULL;
    const char* sql = "SELECT test_func()";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Step once to create the context */
    rc = sqlite3_step(stmt);
    
    /* Register the capture function */
    rc = sqlite3_create_function_v2(
        db,
        "capture_func",
        0,
        SQLITE_UTF8,
        NULL,
        capture_context_func,
        NULL,
        NULL,
        NULL
    );
    
    if (rc == SQLITE_OK) {
        /* Prepare and execute to capture context */
        sqlite3_stmt* stmt2 = NULL;
        const char* sql2 = "SELECT capture_func()";
        rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL);
        
        if (rc == SQLITE_OK) {
            sqlite3_step(stmt2);
            sqlite3_finalize(stmt2);
            
            /* Now we have a valid context in captured_ctx */
            if (captured_ctx != NULL) {
                /* Test sqlite3_result_null */
                sqlite3_result_null(captured_ctx);
                
                /* Test sqlite3_result_text64 */
                if (Size > 0) {
                    const char* text = (const char*)Data;
                    sqlite3_uint64 len = (Size > 100) ? 100 : Size;
                    unsigned char encoding = SQLITE_UTF8;
                    
                    /* Use different encodings based on input */
                    if (Size > 1) {
                        unsigned char enc_selector = Data[0] % 3;
                        if (enc_selector == 0) encoding = SQLITE_UTF8;
                        else if (enc_selector == 1) encoding = SQLITE_UTF16LE;
                        else encoding = SQLITE_UTF16BE;
                    }
                    
                    sqlite3_result_text64(captured_ctx, text, len, NULL, encoding);
                }
                
                /* Test sqlite3_result_zeroblob */
                int n = 0;
                if (Size > 0) {
                    n = Data[0] % 256;
                }
                sqlite3_result_zeroblob(captured_ctx, n);
                
                /* Test sqlite3_aggregate_count */
                /* Note: This should be called from within an aggregate finalize function */
                /* We'll still call it but it may not return meaningful results */
                int count = sqlite3_aggregate_count(captured_ctx);
                (void)count;
                
                /* Test sqlite3_result_int */
                int int_val = 0;
                if (Size > 0) {
                    int_val = (int)Data[0];
                    if (Size > 1) {
                        int_val = (int_val << 8) | Data[1];
                    }
                }
                sqlite3_result_int(captured_ctx, int_val);
            }
        }
    }
    
    /* Test sqlite3_create_function_v2 with various parameters */
    const char* zFunctionName = "fuzz_func";
    int nArg = (Size > 0) ? (Data[0] % 10) : 1;
    int eTextRep = SQLITE_UTF8;
    
    if (Size > 1) {
        unsigned char textrep_selector = Data[1] % 4;
        if (textrep_selector == 0) eTextRep = SQLITE_UTF8;
        else if (textrep_selector == 1) eTextRep = SQLITE_UTF16;
        else if (textrep_selector == 2) eTextRep = SQLITE_UTF16LE;
        else eTextRep = SQLITE_UTF16BE;
    }
    
    void* pApp = NULL;
    
    rc = sqlite3_create_function_v2(
        db,
        zFunctionName,
        nArg,
        eTextRep,
        pApp,
        dummy_func,
        dummy_step,
        dummy_final,
        dummy_destroy
    );
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}