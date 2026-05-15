// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const char *norm_sql = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    // Use first byte to determine flags and modes
    uint8_t mode = Data[0];
    size_t sql_len = Size > 1 ? Size - 1 : 0;
    const char *sql = sql_len > 0 ? (const char*)(Data + 1) : "";
    
    // Try to prepare a statement with sqlite3_prepare_v3
    unsigned int prep_flags = 0;
    if (mode & 0x01) prep_flags |= SQLITE_PREPARE_PERSISTENT;
    if (mode & 0x02) prep_flags |= SQLITE_PREPARE_NORMALIZE;
    if (mode & 0x04) prep_flags |= SQLITE_PREPARE_NO_VTAB;
    
    rc = sqlite3_prepare_v3(db, sql, (int)sql_len, prep_flags, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // Test sqlite3_normalized_sql
        norm_sql = sqlite3_normalized_sql(stmt);
        
        // Test sqlite3_stmt_status with various operations
        int reset_flg = (mode >> 3) & 0x01;
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, reset_flg);
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_SORT, reset_flg);
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_AUTOINDEX, reset_flg);
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_VM_STEP, reset_flg);
        
        // Test sqlite3_stmt_scanstatus if available
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        if (sql_len > 0) {
            sqlite3_int64 scanstat_out;
            int idx = (mode >> 4) & 0x0F;
            int scan_op = (mode >> 8) & 0x0F;
            
            // Try different scan status operations
            if (scan_op == 0) {
                sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_NLOOP, &scanstat_out);
            } else if (scan_op == 1) {
                sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_NVISIT, &scanstat_out);
            } else if (scan_op == 2) {
                sqlite3_stmt_scanstatus(stmt, idx, SQLITE_SCANSTAT_EST, &scanstat_out);
            }
            
            // Reset scan status
            sqlite3_stmt_scanstatus_reset(stmt);
        }
        #endif
        
        // Try to step through the statement if it's a SELECT
        if (sql_len > 0 && sql[0] == 'S') {
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    // Try sqlite3_prepare16_v2 with UTF-16 encoding
    if (Size > 2) {
        // Create a simple UTF-16 SQL string (simplified - just duplicate bytes)
        size_t utf16_len = (Size - 1) * 2;
        void *utf16_sql = malloc(utf16_len);
        if (utf16_sql) {
            // Simple byte duplication for UTF-16LE
            for (size_t i = 0; i < Size - 1; i++) {
                ((uint8_t*)utf16_sql)[i*2] = Data[i+1];
                ((uint8_t*)utf16_sql)[i*2 + 1] = 0;
            }
            
            rc = sqlite3_prepare16_v2(db, utf16_sql, (int)utf16_len, &stmt, NULL);
            if (rc == SQLITE_OK && stmt) {
                sqlite3_finalize(stmt);
            }
            free(utf16_sql);
        }
    }
    
    // Test with file-based operations
    dummy_file_write(Data, Size);
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}