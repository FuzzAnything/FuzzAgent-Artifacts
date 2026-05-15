// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
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

static void fuzz_prepare_and_test(sqlite3 *db, const uint8_t *sql_data, size_t sql_size, 
                                  unsigned int prep_flags, int use_utf16) {
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const void *tail16 = NULL;
    int rc;
    
    if (use_utf16) {
        /* For UTF-16, we need to ensure even number of bytes */
        size_t utf16_size = sql_size;
        if (utf16_size % 2 != 0 && utf16_size > 0) {
            utf16_size--;
        }
        
        if (utf16_size > 0) {
            rc = sqlite3_prepare16_v2(db, (const void*)sql_data, (int)utf16_size, 
                                     &stmt, &tail16);
            
            if (rc == SQLITE_OK && stmt != NULL) {
                /* Test the prepared statement */
                const char *norm_sql = sqlite3_normalized_sql(stmt);
                (void)norm_sql; /* Use result to avoid compiler warning */
                
                /* Test various status operations */
                int status_ops[] = {
                    SQLITE_STMTSTATUS_FULLSCAN_STEP,
                    SQLITE_STMTSTATUS_SORT,
                    SQLITE_STMTSTATUS_AUTOINDEX,
                    SQLITE_STMTSTATUS_VM_STEP,
                    SQLITE_STMTSTATUS_REPREPARE,
                    SQLITE_STMTSTATUS_RUN,
                    SQLITE_STMTSTATUS_FILTER_MISS,
                    SQLITE_STMTSTATUS_FILTER_HIT,
                    SQLITE_STMTSTATUS_MEMUSED
                };
                
                for (size_t i = 0; i < sizeof(status_ops)/sizeof(status_ops[0]); i++) {
                    int reset_flag = (sql_data[i % sql_size] & 1);
                    int status = sqlite3_stmt_status(stmt, status_ops[i], reset_flag);
                    (void)status; /* Use result to avoid compiler warning */
                }
                
                #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
                /* Test scanstatus if available */
                int idx = 0;
                int64_t nloop = 0;
                rc = sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_NLOOP, &nloop);
                if (rc == SQLITE_OK) {
                    /* Try a few more operations */
                    int64_t nvisit = 0;
                    int64_t est = 0;
                    sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_NVISIT, &nvisit);
                    sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_EST, &est);
                }
                
                /* Reset scanstatus counters */
                sqlite3_stmt_scanstatus_reset(stmt);
                #endif
                
                sqlite3_finalize(stmt);
            }
        }
    } else {
        /* UTF-8 preparation */
        rc = sqlite3_prepare_v3(db, (const char*)sql_data, (int)sql_size, 
                               prep_flags, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            /* Test the prepared statement */
            const char *norm_sql = sqlite3_normalized_sql(stmt);
            (void)norm_sql; /* Use result to avoid compiler warning */
            
            /* Test various status operations */
            int status_ops[] = {
                SQLITE_STMTSTATUS_FULLSCAN_STEP,
                SQLITE_STMTSTATUS_SORT,
                SQLITE_STMTSTATUS_AUTOINDEX,
                SQLITE_STMTSTATUS_VM_STEP,
                SQLITE_STMTSTATUS_REPREPARE,
                SQLITE_STMTSTATUS_RUN,
                SQLITE_STMTSTATUS_FILTER_MISS,
                SQLITE_STMTSTATUS_FILTER_HIT,
                SQLITE_STMTSTATUS_MEMUSED
            };
            
            for (size_t i = 0; i < sizeof(status_ops)/sizeof(status_ops[0]); i++) {
                int reset_flag = (sql_data[i % sql_size] & 1);
                int status = sqlite3_stmt_status(stmt, status_ops[i], reset_flag);
                (void)status; /* Use result to avoid compiler warning */
            }
            
            #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
            /* Test scanstatus if available */
            int idx = 0;
            int64_t nloop = 0;
            rc = sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_NLOOP, &nloop);
            if (rc == SQLITE_OK) {
                /* Try a few more operations */
                int64_t nvisit = 0;
                int64_t est = 0;
                sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_NVISIT, &nvisit);
                sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_EST, &est);
            }
            
            /* Reset scanstatus counters */
            sqlite3_stmt_scanstatus_reset(stmt);
            #endif
            
            sqlite3_finalize(stmt);
        }
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static const char *dummy_file = "./dummy_file";
    
    /* Initialize database if first run */
    if (!initialized) {
        /* Create a dummy file for database */
        FILE *fp = fopen(dummy_file, "w");
        if (fp) {
            fclose(fp);
        }
        initialized = 1;
    }
    
    if (Size < 1) {
        return 0;
    }
    
    /* Open an in-memory database */
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK || db == NULL) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }
    
    /* Create a simple table for testing */
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT OR IGNORE INTO test VALUES(1, 'test1');", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT OR IGNORE INTO test VALUES(2, 'test2');", NULL, NULL, NULL);
    
    /* Extract preparation flags from first byte */
    unsigned int prep_flags = 0;
    if (Size > 0) {
        prep_flags = Data[0];
    }
    
    /* Determine if we should use UTF-16 based on second byte */
    int use_utf16 = 0;
    if (Size > 1) {
        use_utf16 = Data[1] & 1;
    }
    
    /* Use remaining data as SQL input */
    const uint8_t *sql_data = Data;
    size_t sql_size = Size;
    
    if (Size > 2) {
        sql_data = Data + 2;
        sql_size = Size - 2;
    }
    
    /* Test with various preparation scenarios */
    fuzz_prepare_and_test(db, sql_data, sql_size, prep_flags, use_utf16);
    
    /* Test with different flags */
    unsigned int test_flags[] = {
        0,
        SQLITE_PREPARE_PERSISTENT,
        SQLITE_PREPARE_NORMALIZE,
        SQLITE_PREPARE_NO_VTAB
    };
    
    for (size_t i = 0; i < sizeof(test_flags)/sizeof(test_flags[0]); i++) {
        if (sql_size > 0) {
            fuzz_prepare_and_test(db, sql_data, sql_size, test_flags[i], 0);
        }
    }
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}