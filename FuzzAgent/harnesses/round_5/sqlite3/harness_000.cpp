#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_000 {
/*
** SQLite basic database operations fuzzing harness
** 
** This harness tests core SQLite APIs:
** - sqlite3_open_v2: Database creation/opening
** - sqlite3_exec: SQL execution with callback
** - sqlite3_prepare_v2: SQL statement preparation
** - sqlite3_step: Statement execution
** - sqlite3_finalize: Statement cleanup
**
** The harness uses FuzzedDataProvider to split fuzzer input into:
** - Database open flags
** - SQL query strings
** - Operation type selection
** - Various integer parameters
*/



// Simple callback function for sqlite3_exec
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    // Just count the rows - don't process data to keep it simple
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    char* err_msg = nullptr;
    
    // Consume database open flags from fuzzer input
    int open_flags = 0;
    
    // Set basic flags for in-memory database
    open_flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    
    // Add some optional flags based on fuzzer input
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_NOMUTEX;
    }
    if (fdp.ConsumeBool()) {
        open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    
    // Open in-memory database
    int rc = sqlite3_open_v2(":memory:", &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits based on fuzzer input to prevent timeouts
    int max_opcodes = fdp.ConsumeIntegralInRange<int>(1000, 50000);
    int max_length = fdp.ConsumeIntegralInRange<int>(1000, 100000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, max_opcodes);
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, max_length);
    
    // Determine which operations to test based on fuzzer input
    uint8_t operation_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // Test 1: sqlite3_exec with various SQL statements
    if (operation_mask & 0x01) {
        // Consume SQL strings from fuzzer input
        std::string sql1 = fdp.ConsumeRandomLengthString(512);
        std::string sql2 = fdp.ConsumeRandomLengthString(512);
        std::string sql3 = fdp.ConsumeRandomLengthString(512);
        
        // Execute CREATE TABLE if we have enough input
        if (!sql1.empty()) {
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);", 
                        nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Execute INSERT statements using fuzzer-generated SQL
        if (!sql2.empty()) {
            sqlite3_exec(db, sql2.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Execute SELECT or other operations
        if (!sql3.empty()) {
            sqlite3_exec(db, sql3.c_str(), exec_callback, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
    }
    
    // Test 2: Prepared statement workflow (prepare_v2, step, finalize)
    if (operation_mask & 0x02) {
        // Consume SQL for prepared statement
        std::string prep_sql = fdp.ConsumeRandomLengthString(1024);
        
        if (!prep_sql.empty()) {
            sqlite3_stmt* stmt = nullptr;
            const char* tail = nullptr;
            
            // Prepare the statement
            rc = sqlite3_prepare_v2(db, prep_sql.c_str(), -1, &stmt, &tail);
            
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Step through the statement multiple times based on fuzzer input
                int max_steps = fdp.ConsumeIntegralInRange<int>(1, 100);
                
                for (int i = 0; i < max_steps; i++) {
                    rc = sqlite3_step(stmt);
                    
                    // Stop if we're done or hit an error
                    if (rc == SQLITE_DONE || rc == SQLITE_ERROR || rc == SQLITE_MISUSE) {
                        break;
                    }
                    
                    // If we got a row, we could potentially extract column data
                    // but we'll keep it simple for the first harness
                    if (rc == SQLITE_ROW) {
                        // Could consume column data here, but skip for simplicity
                        // int col_count = sqlite3_column_count(stmt);
                    }
                }
                
                // Finalize the statement
                sqlite3_finalize(stmt);
            }
        }
    }
    
    // Test 3: Multiple prepared statements with different SQL operations
    if (operation_mask & 0x04 && fdp.remaining_bytes() > 100) {
        // Create a table for more complex operations
        sqlite3_exec(db, 
            "CREATE TABLE IF NOT EXISTS fuzz_data ("
            "id INTEGER PRIMARY KEY, "
            "text_data TEXT, "
            "int_data INTEGER, "
            "real_data REAL);", 
            nullptr, nullptr, nullptr);
        
        // Prepare INSERT statement with parameters
        sqlite3_stmt* insert_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, 
            "INSERT INTO fuzz_data (text_data, int_data, real_data) VALUES (?, ?, ?);",
            -1, &insert_stmt, nullptr);
        
        if (rc == SQLITE_OK && insert_stmt != nullptr) {
            // Bind parameters from fuzzer input
            std::string text_data = fdp.ConsumeRandomLengthString(256);
            int int_data = fdp.ConsumeIntegral<int>();
            double real_data = fdp.ConsumeFloatingPoint<double>();
            
            sqlite3_bind_text(insert_stmt, 1, text_data.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(insert_stmt, 2, int_data);
            sqlite3_bind_double(insert_stmt, 3, real_data);
            
            // Execute the insert
            sqlite3_step(insert_stmt);
            sqlite3_finalize(insert_stmt);
        }
        
        // Prepare SELECT statement
        sqlite3_stmt* select_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, "SELECT * FROM fuzz_data;", -1, &select_stmt, nullptr);
        
        if (rc == SQLITE_OK && select_stmt != nullptr) {
            // Step through all rows
            while (sqlite3_step(select_stmt) == SQLITE_ROW) {
                // Row data available but not processed
            }
            sqlite3_finalize(select_stmt);
        }
    }
    
    // Test 4: Transaction operations
    if (operation_mask & 0x08) {
        // Begin transaction
        sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
        
        // Perform some operations within transaction
        std::string trans_sql = fdp.ConsumeRandomLengthString(512);
        if (!trans_sql.empty()) {
            sqlite3_exec(db, trans_sql.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Commit or rollback based on fuzzer input
        if (fdp.ConsumeBool()) {
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
        } else {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
    }
    
    // Clean up
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    sqlite3_shutdown();
    return 0;
}

} // namespace harness_000
