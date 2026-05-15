#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_011 {
/*
 * Fuzzing harness for SQLite transaction APIs and less-explored features.
 * Targets: Nested transactions (SAVEPOINT, RELEASE, ROLLBACK TO), 
 * advanced PRAGMA statements, VACUUM, backup APIs variations.
 * 
 * This harness complements existing harnesses by focusing on:
 * - Complex transaction control flows not in harness_002
 * - Different PRAGMA statements than harness_007  
 * - Backup API variations from harness_005
 * - VACUUM/ANALYZE operations from harness_008
 * 
 * Uses FuzzedDataProvider for input-driven testing.
 */



// Simple callback for sqlite3_exec
static int exec_callback(void* NotUsed, int argc, char** argv, char** col_names) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)col_names;
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Open in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return 0;
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    
    char* err_msg = nullptr;
    
    // Create test table
    const char* create_sql = 
        "CREATE TABLE IF NOT EXISTS test_tx ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER"
        ");";
    
    sqlite3_exec(db, create_sql, exec_callback, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert initial data
    std::string name1 = fdp.ConsumeRandomLengthString(16);
    std::string name2 = fdp.ConsumeRandomLengthString(16);
    int value1 = fdp.ConsumeIntegral<int>();
    int value2 = fdp.ConsumeIntegral<int>();
    
    std::string insert1 = "INSERT INTO test_tx(name, value) VALUES('" + name1 + "', " + std::to_string(value1) + ");";
    std::string insert2 = "INSERT INTO test_tx(name, value) VALUES('" + name2 + "', " + std::to_string(value2) + ");";
    
    sqlite3_exec(db, insert1.c_str(), exec_callback, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    sqlite3_exec(db, insert2.c_str(), exec_callback, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Consume operation type
    uint8_t op_type = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    switch (op_type) {
        case 0: {
            // Nested transaction with SAVEPOINT
            sqlite3_exec(db, "BEGIN;", exec_callback, nullptr, &err_msg);
            
            std::string sp_name = "sp_" + fdp.ConsumeRandomLengthString(6);
            std::string savepoint = "SAVEPOINT " + sp_name + ";";
            sqlite3_exec(db, savepoint.c_str(), exec_callback, nullptr, &err_msg);
            
            // Perform operation within savepoint
            int delta = fdp.ConsumeIntegralInRange<int>(-100, 100);
            std::string update = "UPDATE test_tx SET value = value + " + std::to_string(delta) + " WHERE id = 1;";
            sqlite3_exec(db, update.c_str(), exec_callback, nullptr, &err_msg);
            
            // Release or rollback to savepoint
            if (fdp.ConsumeBool()) {
                std::string release = "RELEASE " + sp_name + ";";
                sqlite3_exec(db, release.c_str(), exec_callback, nullptr, &err_msg);
            } else {
                std::string rollback = "ROLLBACK TO " + sp_name + ";";
                sqlite3_exec(db, rollback.c_str(), exec_callback, nullptr, &err_msg);
            }
            
            // Commit or rollback main transaction
            if (fdp.ConsumeBool()) {
                sqlite3_exec(db, "COMMIT;", exec_callback, nullptr, &err_msg);
            } else {
                sqlite3_exec(db, "ROLLBACK;", exec_callback, nullptr, &err_msg);
            }
            break;
        }
        
        case 1: {
            // Test various PRAGMA statements
            uint8_t pragma_idx = fdp.ConsumeIntegral<uint8_t>() % 5;
            const char* pragmas[] = {
                "PRAGMA auto_vacuum = 1;",
                "PRAGMA cache_size = 1000;",
                "PRAGMA synchronous = NORMAL;",
                "PRAGMA foreign_keys = ON;",
                "PRAGMA journal_mode = WAL;"
            };
            
            if (pragma_idx < 5) {
                sqlite3_exec(db, pragmas[pragma_idx], exec_callback, nullptr, &err_msg);
            }
            
            // Read some PRAGMAs
            sqlite3_exec(db, "PRAGMA page_size;", exec_callback, nullptr, &err_msg);
            sqlite3_exec(db, "PRAGMA journal_mode;", exec_callback, nullptr, &err_msg);
            break;
        }
        
        case 2: {
            // Test VACUUM operation
            // Create and drop tables to create fragmentation
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 5; i++) {
                std::string temp_tbl = "temp" + std::to_string(i);
                std::string create_temp = "CREATE TABLE " + temp_tbl + "(x INTEGER);";
                sqlite3_exec(db, create_temp.c_str(), exec_callback, nullptr, &err_msg);
                
                std::string insert_temp = "INSERT INTO " + temp_tbl + " VALUES(1);";
                sqlite3_exec(db, insert_temp.c_str(), exec_callback, nullptr, &err_msg);
                
                std::string drop_temp = "DROP TABLE " + temp_tbl + ";";
                sqlite3_exec(db, drop_temp.c_str(), exec_callback, nullptr, &err_msg);
            }
            
            sqlite3_exec(db, "VACUUM;", exec_callback, nullptr, &err_msg);
            break;
        }
        
        case 3: {
            // Test backup API
            sqlite3* backup_db = nullptr;
            rc = sqlite3_open(":memory:", &backup_db);
            
            if (rc == SQLITE_OK && backup_db != nullptr) {
                sqlite3_backup* pBackup = sqlite3_backup_init(backup_db, "main", db, "main");
                if (pBackup) {
                    // Backup in small steps
                    while (sqlite3_backup_step(pBackup, 5) == SQLITE_OK) {
                        // Continue
                    }
                    sqlite3_backup_finish(pBackup);
                }
                sqlite3_close(backup_db);
            }
            break;
        }
    }
    
    // Clear any error messages
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Test immediate vs deferred transactions
    if (fdp.remaining_bytes() > 5) {
        if (fdp.ConsumeBool()) {
            sqlite3_exec(db, "BEGIN IMMEDIATE;", exec_callback, nullptr, &err_msg);
        } else {
            sqlite3_exec(db, "BEGIN DEFERRED;", exec_callback, nullptr, &err_msg);
        }
        
        std::string update = "UPDATE test_tx SET value = value + 1 WHERE id = 2;";
        sqlite3_exec(db, update.c_str(), exec_callback, nullptr, &err_msg);
        
        if (fdp.ConsumeBool()) {
            sqlite3_exec(db, "COMMIT;", exec_callback, nullptr, &err_msg);
        } else {
            sqlite3_exec(db, "ROLLBACK;", exec_callback, nullptr, &err_msg);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}

} // namespace harness_011
