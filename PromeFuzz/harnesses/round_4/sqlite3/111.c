// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:175014:16 in sqlite3.h
// sqlite3_trace_v2 at sqlite3.c:173158:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Trace callback for sqlite3_trace_v2 */
static int trace_callback(unsigned mask, void *context, void *p, void *x) {
    (void)mask;
    (void)context;
    (void)p;
    (void)x;
    return 0;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_111(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Enable extended result codes based on first byte */
    if (Size > 0) {
        sqlite3_extended_result_codes(db, Data[0] & 1);
    }
    
    /* Set up trace callback based on second byte */
    if (Size > 1) {
        unsigned trace_mask = 0;
        if (Data[1] & 0x01) trace_mask |= SQLITE_TRACE_STMT;
        if (Data[1] & 0x02) trace_mask |= SQLITE_TRACE_PROFILE;
        if (Data[1] & 0x04) trace_mask |= SQLITE_TRACE_ROW;
        if (Data[1] & 0x08) trace_mask |= SQLITE_TRACE_CLOSE;
        
        if (trace_mask != 0) {
            sqlite3_trace_v2(db, trace_mask, trace_callback, NULL);
        }
    }
    
    /* Prepare SQL statement using remaining data as SQL text */
    if (Size > 2) {
        /* Use data from byte 2 onward as SQL string */
        const char *sql = (const char *)(Data + 2);
        size_t sql_len = Size - 2;
        
        /* Ensure null termination if needed */
        char *sql_copy = NULL;
        if (sql_len > 0) {
            sql_copy = (char *)malloc(sql_len + 1);
            if (sql_copy) {
                memcpy(sql_copy, sql, sql_len);
                sql_copy[sql_len] = '\0';
                sql = sql_copy;
            }
        }
        
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
        
        if (sql_copy) {
            free(sql_copy);
        }
        
        /* If preparation succeeded, exercise the statement */
        if (rc == SQLITE_OK && stmt) {
            /* Step through the statement multiple times */
            for (int i = 0; i < 10; i++) {
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    /* Get column types for all columns */
                    int col_count = sqlite3_column_count(stmt);
                    for (int col = 0; col < col_count; col++) {
                        (void)sqlite3_column_type(stmt, col);
                    }
                } else if (rc == SQLITE_DONE) {
                    /* Reset and break */
                    sqlite3_reset(stmt);
                    break;
                } else {
                    /* Error - reset and break */
                    sqlite3_reset(stmt);
                    break;
                }
            }
            
            /* Final reset */
            sqlite3_reset(stmt);
            
            /* Clean up statement */
            sqlite3_finalize(stmt);
        }
    }
    
    /* Close database */
    sqlite3_close(db);
    
    return 0;
}