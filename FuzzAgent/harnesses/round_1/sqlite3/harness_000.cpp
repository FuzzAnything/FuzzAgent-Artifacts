#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_000 {
/*
 * Fuzzing harness for SQLite core APIs
 * Targets: sqlite3_open, sqlite3_exec, sqlite3_prepare_v2, sqlite3_step, 
 *          sqlite3_finalize, sqlite3_close
 * 
 * This harness tests SQLite database operations with fuzzed input as SQL queries.
 * It uses FuzzedDataProvider to split input into different parts for configuration
 * and SQL statements.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful testing
    if (size < 16) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    int open_flags = 0;
    bool use_memory_db = fdp.ConsumeBool();
    bool enable_foreign_keys = fdp.ConsumeBool();
    bool use_prepared_stmts = fdp.ConsumeBool();
    int max_sql_length = fdp.ConsumeIntegralInRange<int>(1, 4096);
    
    // Set up database flags based on consumed parameters
    if (use_memory_db) {
        open_flags |= SQLITE_OPEN_MEMORY;
    }
    open_flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    const char* db_name = use_memory_db ? ":memory:" : "fuzz.db";
    
    // Open database connection
    int rc = sqlite3_open_v2(db_name, &db, open_flags, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Apply configuration based on fuzzed parameters
    if (enable_foreign_keys) {
        sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
    }
    
    // Set some safety limits to prevent timeouts
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 50);
    sqlite3_limit(db, SQLITE_LIMIT_EXPR_DEPTH, 20);
    
    // Consume SQL statements from remaining input
    while (fdp.remaining_bytes() > 0) {
        // Get SQL statement from fuzzer input
        std::string sql = fdp.ConsumeRandomLengthString(max_sql_length);
        if (sql.empty()) continue;
        
        if (use_prepared_stmts && fdp.ConsumeBool()) {
            // Test prepared statement API
            sqlite3_stmt* stmt = nullptr;
            const char* tail = nullptr;
            
            // Prepare statement
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Bind parameters if we have enough remaining data
                int param_count = sqlite3_bind_parameter_count(stmt);
                for (int i = 1; i <= param_count && fdp.remaining_bytes() > 0; i++) {
                    // Consume parameter type and value
                    int param_type = fdp.ConsumeIntegralInRange<int>(1, 5);
                    switch (param_type) {
                        case 1: // Integer
                            sqlite3_bind_int(stmt, i, fdp.ConsumeIntegral<int>());
                            break;
                        case 2: // Double
                            sqlite3_bind_double(stmt, i, fdp.ConsumeFloatingPoint<double>());
                            break;
                        case 3: // Text
                            sqlite3_bind_text(stmt, i, fdp.ConsumeRandomLengthString(100).c_str(), 
                                            -1, SQLITE_TRANSIENT);
                            break;
                        case 4: // Blob
                            {
                                size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, 100);
                                std::vector<uint8_t> blob = fdp.ConsumeBytes<uint8_t>(blob_size);
                                if (!blob.empty()) {
                                    sqlite3_bind_blob(stmt, i, blob.data(), blob.size(), SQLITE_TRANSIENT);
                                }
                            }
                            break;
                        case 5: // Null
                            sqlite3_bind_null(stmt, i);
                            break;
                    }
                }
                
                // Execute prepared statement
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                    // Process row if needed (just consume data to exercise code paths)
                    int col_count = sqlite3_column_count(stmt);
                    for (int i = 0; i < col_count; i++) {
                        // Access column data to exercise more code paths
                        int col_type = sqlite3_column_type(stmt, i);
                        switch (col_type) {
                            case SQLITE_INTEGER:
                                sqlite3_column_int64(stmt, i);
                                break;
                            case SQLITE_FLOAT:
                                sqlite3_column_double(stmt, i);
                                break;
                            case SQLITE_TEXT:
                                sqlite3_column_text(stmt, i);
                                break;
                            case SQLITE_BLOB:
                                sqlite3_column_blob(stmt, i);
                                sqlite3_column_bytes(stmt, i);
                                break;
                        }
                    }
                }
                
                // Finalize statement
                sqlite3_finalize(stmt);
            }
        } else {
            // Test simple exec API
            char* err_msg = nullptr;
            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
            }
        }
        
        // Reset database state if needed
        if (fdp.ConsumeBool() && fdp.remaining_bytes() > 0) {
            sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr);
        }
    }
    
    // Clean up
    sqlite3_exec(db, "PRAGMA temp_store_directory=''", nullptr, nullptr, nullptr);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_000
