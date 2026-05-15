// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void initialize_database_and_statement(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    // Create a simple table and insert some data
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test (name) VALUES ('test1'), ('test2'), ('test3');";
    
    rc = sqlite3_exec(*db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return;
    }
    
    // Prepare a simple statement
    const char *select_sql = "SELECT * FROM test WHERE id = ?;";
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        *stmt = NULL;
    }
}

static void cleanup_database_and_statement(sqlite3 *db, sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Initialize database and prepared statement
    initialize_database_and_statement(&db, &stmt);
    if (!db || !stmt) {
        cleanup_database_and_statement(db, stmt);
        return 0;
    }
    
    // Use the fuzzer input to determine which functions to call and with what parameters
    if (Size < 1) {
        cleanup_database_and_statement(db, stmt);
        return 0;
    }
    
    uint8_t selector = Data[0];
    size_t data_index = 1;
    
    // Test sqlite3_stmt_explain with different modes
    if (selector & 0x01) {
        int eMode = 0;
        if (Size - data_index >= 1) {
            eMode = Data[data_index] % 3;  // 0, 1, or 2
            data_index++;
        }
        
        // Reset the statement first to ensure it's not active
        sqlite3_reset(stmt);
        sqlite3_stmt_explain(stmt, eMode);
    }
    
    // Test sqlite3_stmt_scanstatus and sqlite3_stmt_scanstatus_v2
    if (selector & 0x02) {
        int idx = -1;
        int iScanStatusOp = 0;
        int flags = 0;
        
        if (Size - data_index >= 3) {
            idx = (int8_t)Data[data_index];  // Can be negative
            data_index++;
            iScanStatusOp = Data[data_index] % 10;  // Use modulo for op selection
            data_index++;
            flags = Data[data_index] & 0x01;  // Only use the first flag bit
            data_index++;
        }
        
        // Declare variables to hold output
        sqlite3_int64 out_int64 = 0;
        double out_double = 0.0;
        const char *out_str = NULL;
        int out_int = 0;
        
        // Try different output types based on operation
        void *pOut = NULL;
        if (iScanStatusOp % 4 == 0) {
            pOut = &out_int64;
        } else if (iScanStatusOp % 4 == 1) {
            pOut = &out_double;
        } else if (iScanStatusOp % 4 == 2) {
            pOut = &out_str;
        } else {
            pOut = &out_int;
        }
        
        // Execute the statement once to potentially populate scan status
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1, 1);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Call both scanstatus functions
        sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, pOut);
        sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, pOut);
    }
    
    // Test sqlite3_stmt_status
    if (selector & 0x04) {
        int op = 0;
        int resetFlg = 0;
        
        if (Size - data_index >= 2) {
            op = Data[data_index] % 20;  // SQLITE_STMTSTATUS counters
            data_index++;
            resetFlg = Data[data_index] & 0x01;
            data_index++;
        }
        
        sqlite3_stmt_status(stmt, op, resetFlg);
    }
    
    // Test sqlite3_stmt_busy
    if (selector & 0x08) {
        // First, potentially make the statement busy
        if (Size - data_index >= 1 && (Data[data_index] & 0x01)) {
            sqlite3_reset(stmt);
            sqlite3_bind_int(stmt, 1, 2);
            sqlite3_step(stmt);  // Step but don't finish
            // Don't reset - leave it busy
        }
        
        int busy = sqlite3_stmt_busy(stmt);
        (void)busy;  // Use the result to avoid unused variable warning
    }
    
    // Test sqlite3_expired
    if (selector & 0x10) {
        // Try to expire the statement by schema change
        if (Size - data_index >= 1 && (Data[data_index] & 0x01)) {
            const char *alter_sql = "ALTER TABLE test ADD COLUMN newcol TEXT;";
            sqlite3_exec(db, alter_sql, NULL, NULL, NULL);
        }
        
        int expired = sqlite3_expired(stmt);
        (void)expired;  // Use the result to avoid unused variable warning
    }
    
    // Execute the statement to explore more states
    if (selector & 0x20) {
        sqlite3_reset(stmt);
        
        // Bind a parameter if we have data
        if (Size - data_index >= 1) {
            int bind_value = Data[data_index] % 10 + 1;  // Values 1-10
            sqlite3_bind_int(stmt, 1, bind_value);
            data_index++;
        }
        
        // Step through results
        int step_result = sqlite3_step(stmt);
        while (step_result == SQLITE_ROW) {
            // Access column data
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char *name = sqlite3_column_text(stmt, 1);
            (void)id;
            (void)name;  // Use variables to avoid warnings
            step_result = sqlite3_step(stmt);
        }
        
        sqlite3_reset(stmt);
    }
    
    cleanup_database_and_statement(db, stmt);
    return 0;
}