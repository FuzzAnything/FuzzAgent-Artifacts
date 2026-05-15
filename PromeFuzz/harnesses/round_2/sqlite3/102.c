// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function16 at sqlite3.c:173042:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:174751:16 in sqlite3.h
// sqlite3_create_window_function at sqlite3.c:173025:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

/* Dummy callback functions for function creation */
static void dummy_xFunc(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_xStep(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_xFinal(sqlite3_context* ctx) {
    (void)ctx;
}

static void dummy_xValue(sqlite3_context* ctx) {
    (void)ctx;
}

static void dummy_xInverse(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
}

static void dummy_xDestroy(void* p) {
    (void)p;
}

static void dummy_clientdata_destructor(void* p) {
    (void)p;
}

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use the fuzzer data to generate function names and parameters */
    if (Size > 0) {
        /* Extract parameters from fuzzer input */
        int selector = Data[0] % 6; /* Which function to test */
        int nArg = (Size > 1) ? (Data[1] % 128) : 0; /* Limit to valid range */
        int eTextRep = (Size > 2) ? Data[2] : SQLITE_UTF8;
        
        /* Prepare function name - ensure null termination */
        size_t name_len = (Size > 3) ? (Size - 3) : 0;
        if (name_len > 255) name_len = 255; /* SQLite limit */
        
        char *zFunctionName = NULL;
        void *zFunctionName16 = NULL;
        
        if (name_len > 0) {
            zFunctionName = (char*)malloc(name_len + 1);
            if (zFunctionName) {
                memcpy(zFunctionName, Data + 3, name_len);
                zFunctionName[name_len] = '\0';
                
                /* Also prepare UTF-16 version */
                zFunctionName16 = malloc((name_len + 1) * 2);
                if (zFunctionName16) {
                    /* Simple ASCII to UTF-16 conversion */
                    uint16_t *dest = (uint16_t*)zFunctionName16;
                    for (size_t i = 0; i < name_len; i++) {
                        dest[i] = zFunctionName[i];
                    }
                    dest[name_len] = 0;
                }
            }
        } else {
            /* Default names if no data */
            zFunctionName = "fuzz_func";
        }
        
        /* Prepare application data */
        void *pApp = (void*)(uintptr_t)Data[0];
        
        /* Set flags based on fuzzer input */
        if (eTextRep & 0x80) {
            eTextRep = (eTextRep & 0x7F) | SQLITE_DETERMINISTIC;
        }
        if (eTextRep & 0x40) {
            eTextRep = (eTextRep & 0xBF) | SQLITE_DIRECTONLY;
        }
        
        /* Normalize encoding to valid values */
        switch (eTextRep & 0x0F) {
            case 0: eTextRep = (eTextRep & ~0x0F) | SQLITE_UTF8; break;
            case 1: eTextRep = (eTextRep & ~0x0F) | SQLITE_UTF16; break;
            case 2: eTextRep = (eTextRep & ~0x0F) | SQLITE_UTF16LE; break;
            case 3: eTextRep = (eTextRep & ~0x0F) | SQLITE_UTF16BE; break;
            default: eTextRep = (eTextRep & ~0x0F) | SQLITE_UTF8; break;
        }
        
        /* Call the selected function */
        switch (selector) {
            case 0:
                sqlite3_create_function(db, zFunctionName, nArg, eTextRep,
                                       pApp, dummy_xFunc, dummy_xStep, dummy_xFinal);
                break;
            case 1:
                if (zFunctionName16) {
                    sqlite3_create_function16(db, zFunctionName16, nArg, eTextRep,
                                            pApp, dummy_xFunc, dummy_xStep, dummy_xFinal);
                }
                break;
            case 2:
                if (zFunctionName) {
                    sqlite3_set_clientdata(db, zFunctionName, pApp, 
                                          dummy_clientdata_destructor);
                }
                break;
            case 3:
                sqlite3_create_window_function(db, zFunctionName, nArg, eTextRep,
                                              pApp, dummy_xStep, dummy_xFinal,
                                              dummy_xValue, dummy_xInverse,
                                              dummy_xDestroy);
                break;
            case 4:
                sqlite3_create_function_v2(db, zFunctionName, nArg, eTextRep,
                                          pApp, dummy_xFunc, dummy_xStep,
                                          dummy_xFinal, dummy_xDestroy);
                break;
            case 5:
                sqlite3_overload_function(db, zFunctionName, nArg);
                break;
        }
        
        /* Clean up */
        if (zFunctionName && name_len > 0) {
            free(zFunctionName);
        }
        if (zFunctionName16) {
            free(zFunctionName16);
        }
    }
    
    /* Try to exercise the functions by executing some SQL */
    if (db) {
        sqlite3_stmt *stmt = NULL;
        const char *sql = "SELECT 1";
        
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        /* Also try to delete a function */
        if (Size > 0) {
            sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8, 
                                   NULL, NULL, NULL, NULL);
        }
    }
    
    /* Close database */
    sqlite3_close(db);
    
    return 0;
}