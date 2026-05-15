// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
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

static int dummy_busy_handler(void *data, int count) {
    return 0;
}

static int dummy_progress_handler(void *data) {
    return 0;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Open an in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Set some handlers to avoid hangs
    sqlite3_busy_handler(db, dummy_busy_handler, NULL);
    sqlite3_progress_handler(db, 1000, dummy_progress_handler, NULL);

    // Create a simple table for testing
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);
    
    // Insert some dummy data
    sqlite3_exec(db, "INSERT INTO test (data) VALUES ('test1'), ('test2'), ('test3');", NULL, NULL, NULL);

    // Prepare statements using both UTF-8 and UTF-16 APIs
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const void *tail16 = NULL;
    
    // Use fuzzer data for SQL queries (limit to reasonable size)
    size_t sql_size = Size > 1024 ? 1024 : Size;
    char *sql = (char *)malloc(sql_size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, sql_size);
    sql[sql_size] = '\0';

    // Test sqlite3_prepare_v3 with various flags
    unsigned int prep_flags = Data[0] % 8;  // Use first byte for flags
    rc = sqlite3_prepare_v3(db, sql, -1, prep_flags, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt != NULL) {
        // Test sqlite3_normalized_sql
        const char *normalized = sqlite3_normalized_sql(stmt);
        (void)normalized;  // Use result to avoid compiler warning
        
        // Test sqlite3_stmt_status with various operations
        // SQLITE_STMTSTATUS_FULLSCAN_STEP = 1, SQLITE_STMTSTATUS_SORT = 2, etc.
        int stmt_status_op = (Data[1] % 10) + 1;  // Use second byte for op
        int reset_flag = Data[2] % 2;  // Use third byte for reset flag
        int status_value = sqlite3_stmt_status(stmt, stmt_status_op, reset_flag);
        (void)status_value;  // Use result to avoid compiler warning
        
        // Test sqlite3_stmt_scanstatus if compiled with support
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        {
            int idx = Data[3] % 10;  // Use fourth byte for idx
            int scan_op = Data[4] % 5;  // Use fifth byte for scan op
            int out_value = 0;
            
            // Try different scan status operations
            switch (scan_op) {
                case 0:
                    sqlite3_stmt_scanstatus(stmt, idx, 0, &out_value);  // SQLITE_SCANSTAT_NLOOP
                    break;
                case 1:
                    sqlite3_stmt_scanstatus(stmt, idx, 1, &out_value);  // SQLITE_SCANSTAT_NSELECT
                    break;
                case 2:
                    sqlite3_stmt_scanstatus(stmt, idx, 2, &out_value);  // SQLITE_SCANSTAT_NCYCLE
                    break;
                case 3:
                    sqlite3_stmt_scanstatus(stmt, idx, 3, &out_value);  // SQLITE_SCANSTAT_COMPLEX
                    break;
                default:
                    sqlite3_stmt_scanstatus(stmt, idx, 4, &out_value);  // SQLITE_SCANSTAT_NVISIT
                    break;
            }
            
            // Reset scan status counters
            sqlite3_stmt_scanstatus_reset(stmt);
        }
        #endif
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // Test sqlite3_prepare16_v2 with UTF-16 encoded SQL
    // Convert first part of input to UTF-16 (simple conversion for fuzzing)
    size_t utf16_size = sql_size * 2;
    void *sql16 = malloc(utf16_size + 2);
    if (sql16) {
        // Simple ASCII to UTF-16LE conversion for fuzzing
        uint8_t *ptr = (uint8_t *)sql16;
        for (size_t i = 0; i < sql_size; i++) {
            ptr[i * 2] = sql[i];
            ptr[i * 2 + 1] = 0;
        }
        // Null terminate UTF-16 string
        ptr[sql_size * 2] = 0;
        ptr[sql_size * 2 + 1] = 0;
        
        rc = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &tail16);
        
        if (rc == SQLITE_OK && stmt != NULL) {
            // Execute the statement once to populate statistics
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            
            // Test sqlite3_stmt_status again
            int stmt_status_op2 = (Data[5] % 10) + 1;  // Use sixth byte for op
            int reset_flag2 = Data[6] % 2;  // Use seventh byte for reset flag
            int status_value2 = sqlite3_stmt_status(stmt, stmt_status_op2, reset_flag2);
            (void)status_value2;  // Use result to avoid compiler warning
            
            sqlite3_finalize(stmt);
        }
        
        free(sql16);
    }

    // Clean up
    free(sql);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}