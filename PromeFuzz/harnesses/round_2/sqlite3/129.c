// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_interrupt at sqlite3.c:172778:17 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
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

static int fuzz_prepare_stmt(sqlite3 *db, sqlite3_stmt **stmt, const char *sql) {
    return sqlite3_prepare_v2(db, sql, -1, stmt, NULL);
}

static void fuzz_exec_sql(sqlite3 *db, const char *sql) {
    sqlite3_exec(db, sql, NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    sqlite3_stmt *iter_stmt = NULL;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy table and insert some data
    fuzz_exec_sql(db, "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);");
    fuzz_exec_sql(db, "INSERT INTO test(name) VALUES('test1');");
    fuzz_exec_sql(db, "INSERT INTO test(name) VALUES('test2');");
    fuzz_exec_sql(db, "INSERT INTO test(name) VALUES('test3');");
    
    // Prepare some statements
    rc = fuzz_prepare_stmt(db, &stmt1, "SELECT * FROM test;");
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    rc = fuzz_prepare_stmt(db, &stmt2, "SELECT id FROM test WHERE name = ?;");
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    rc = fuzz_prepare_stmt(db, &stmt3, "UPDATE test SET name = 'updated' WHERE id = ?;");
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input to decide which operations to perform
    if (Size > 0) {
        uint8_t op_selector = Data[0];
        size_t data_index = 1;
        
        // Test sqlite3_reset
        if (op_selector & 0x01) {
            // Step through some rows first
            for (int i = 0; i < 2 && sqlite3_step(stmt1) == SQLITE_ROW; i++) {
                // Just step through
            }
            sqlite3_reset(stmt1);
        }
        
        // Test sqlite3_db_handle
        if (op_selector & 0x02) {
            sqlite3 *db_from_stmt = sqlite3_db_handle(stmt2);
            if (db_from_stmt != db) {
                // Should never happen, but check anyway
            }
        }
        
        // Test sqlite3_column_count
        if (op_selector & 0x04) {
            int col_count = sqlite3_column_count(stmt1);
            // Also test on UPDATE statement
            int col_count_update = sqlite3_column_count(stmt3);
            (void)col_count;
            (void)col_count_update;
        }
        
        // Test sqlite3_finalize
        if (op_selector & 0x08) {
            // Create and immediately finalize a statement
            sqlite3_stmt *temp_stmt = NULL;
            if (fuzz_prepare_stmt(db, &temp_stmt, "SELECT count(*) FROM test;") == SQLITE_OK) {
                sqlite3_finalize(temp_stmt);
            }
        }
        
        // Test sqlite3_interrupt
        if (op_selector & 0x10) {
            // Start a long-running operation in another thread would be ideal,
            // but for fuzzing we just call interrupt
            sqlite3_interrupt(db);
        }
        
        // Test sqlite3_next_stmt
        if (op_selector & 0x20) {
            // Iterate through all prepared statements
            iter_stmt = sqlite3_next_stmt(db, NULL);
            while (iter_stmt != NULL) {
                sqlite3_stmt *next = sqlite3_next_stmt(db, iter_stmt);
                iter_stmt = next;
            }
        }
        
        // Use remaining fuzzer data to bind parameters
        if (data_index < Size && stmt2 != NULL) {
            // Bind a parameter to stmt2
            if (Size - data_index >= 1) {
                int param = Data[data_index] % 10;
                sqlite3_bind_int(stmt2, 1, param);
                data_index++;
                
                // Step and reset
                sqlite3_step(stmt2);
                sqlite3_reset(stmt2);
            }
        }
        
        if (data_index < Size && stmt3 != NULL) {
            // Bind a parameter to stmt3
            if (Size - data_index >= 1) {
                int param = Data[data_index] % 10;
                sqlite3_bind_int(stmt3, 1, param);
                data_index++;
                
                // Step and reset
                sqlite3_step(stmt3);
                sqlite3_reset(stmt3);
            }
        }
    }
    
    // Finalize all statements
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_finalize(stmt3);
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}