// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:80267:16 in sqlite3.h
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

static void dummy_file_write(const uint8_t *Data, size_t Size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare statement using fuzzer input as SQL
    // Use Size as nByte, but ensure null termination for safety
    char *sql = malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = 0;
    
    rc = sqlite3_prepare_v2(db, sql, (int)Size, &stmt, &tail);
    
    // Test the APIs if statement was prepared successfully
    if (rc == SQLITE_OK && stmt) {
        // Test sqlite3_stmt_isexplain
        int explain_type = sqlite3_stmt_isexplain(stmt);
        (void)explain_type; // Use result to avoid unused variable warning
        
        // Test sqlite3_column_count
        int col_count = sqlite3_column_count(stmt);
        (void)col_count;
        
        // Test sqlite3_expired
        int expired = sqlite3_expired(stmt);
        (void)expired;
        
        // Test sqlite3_reset
        rc = sqlite3_reset(stmt);
        (void)rc;
        
        // Test sqlite3_stmt_scanstatus_v2 with various parameters
        // We need to try different idx and iScanStatusOp values
        for (int idx = 0; idx < 2; idx++) {
            for (int op = 0; op < 4; op++) {
                int flags = 0;
                void *pOut = NULL;
                // Allocate some memory for output
                int out_val = 0;
                pOut = &out_val;
                
                // Try different flag combinations
                for (int f = 0; f < 2; f++) {
                    flags = f;
                    sqlite3_stmt_scanstatus_v2(stmt, idx, op, flags, pOut);
                }
            }
        }
        
        // Prepare another statement with different SQL to test more states
        sqlite3_stmt *stmt2 = NULL;
        const char *simple_sql = "SELECT 1;";
        if (sqlite3_prepare_v2(db, simple_sql, -1, &stmt2, NULL) == SQLITE_OK) {
            sqlite3_stmt_isexplain(stmt2);
            sqlite3_column_count(stmt2);
            sqlite3_expired(stmt2);
            sqlite3_reset(stmt2);
            sqlite3_finalize(stmt2);
        }
        
        // Test with EXPLAIN statements
        sqlite3_stmt *explain_stmt = NULL;
        const char *explain_sql = "EXPLAIN SELECT 1;";
        if (sqlite3_prepare_v2(db, explain_sql, -1, &explain_stmt, NULL) == SQLITE_OK) {
            int explain_result = sqlite3_stmt_isexplain(explain_stmt);
            (void)explain_result;
            sqlite3_finalize(explain_stmt);
        }
        
        // Test with EXPLAIN QUERY PLAN
        sqlite3_stmt *explain_qp_stmt = NULL;
        const char *explain_qp_sql = "EXPLAIN QUERY PLAN SELECT 1;";
        if (sqlite3_prepare_v2(db, explain_qp_sql, -1, &explain_qp_stmt, NULL) == SQLITE_OK) {
            int explain_qp_result = sqlite3_stmt_isexplain(explain_qp_stmt);
            (void)explain_qp_result;
            sqlite3_finalize(explain_qp_stmt);
        }
    }
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    free(sql);
    
    // Test with file-based database as well
    dummy_file_write(Data, Size);
    sqlite3 *file_db = NULL;
    if (sqlite3_open("./dummy_file", &file_db) == SQLITE_OK) {
        sqlite3_stmt *file_stmt = NULL;
        const char *test_sql = "CREATE TABLE IF NOT EXISTS test(x);";
        if (sqlite3_prepare_v2(file_db, test_sql, -1, &file_stmt, NULL) == SQLITE_OK) {
            sqlite3_step(file_stmt);
            sqlite3_reset(file_stmt);
            sqlite3_column_count(file_stmt);
            sqlite3_finalize(file_stmt);
        }
        sqlite3_close(file_db);
    }
    
    // Final cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}