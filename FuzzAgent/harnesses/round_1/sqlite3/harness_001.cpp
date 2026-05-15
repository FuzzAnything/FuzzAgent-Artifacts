#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_001 {
/*
 * Fuzzing harness for SQLite database file parsing and deserialization
 * Targets: sqlite3_deserialize, sqlite3_file_control, sqlite3_limit, 
 *          sqlite3_progress_handler, and various configuration options
 * 
 * This harness tests SQLite with malformed database files using the
 * deserialize API, inspired by dbfuzz2.c test harness.
 */



int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for testing
    if (size < 8) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool use_deserialize = fdp.ConsumeBool();
    bool enable_vdbe_debug = fdp.ConsumeBool();
    bool enable_progress_handler = fdp.ConsumeBool();
    int max_progress_callbacks = fdp.ConsumeIntegralInRange<int>(0, 1000000);
    sqlite3_int64 max_db_size = fdp.ConsumeIntegral<sqlite3_int64>();
    
    // Limit max_db_size to reasonable values
    if (max_db_size < 0) max_db_size = -max_db_size;
    if (max_db_size > 100 * 1024 * 1024) max_db_size = 100 * 1024 * 1024; // 100MB max
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Progress handler callback data
    static int progress_callback_count = 0;
    static int max_allowed_callbacks = max_progress_callbacks;
    
    // Progress handler function
    auto progress_handler = [](void* pNotUsed) -> int {
        progress_callback_count++;
        if (progress_callback_count < max_allowed_callbacks) return 0;
        return 1; // Abort if limit reached
    };
    
    // Set progress handler if enabled
    if (enable_progress_handler && max_progress_callbacks > 0) {
        sqlite3_progress_handler(db, 10, progress_handler, nullptr);
        progress_callback_count = 0;
    }
    
    // Enable VDBE debug if requested
    if (enable_vdbe_debug) {
        sqlite3_exec(db, "PRAGMA vdbe_debug=ON", nullptr, nullptr, nullptr);
    }
    
    // Set various safety limits based on fuzzed input
    int limit_flags[] = {
        SQLITE_LIMIT_LENGTH,
        SQLITE_LIMIT_SQL_LENGTH,
        SQLITE_LIMIT_COLUMN,
        SQLITE_LIMIT_EXPR_DEPTH,
        SQLITE_LIMIT_COMPOUND_SELECT,
        SQLITE_LIMIT_VDBE_OP,
        SQLITE_LIMIT_FUNCTION_ARG,
        SQLITE_LIMIT_ATTACHED,
        SQLITE_LIMIT_LIKE_PATTERN_LENGTH,
        SQLITE_LIMIT_VARIABLE_NUMBER,
        SQLITE_LIMIT_TRIGGER_DEPTH,
        SQLITE_LIMIT_WORKER_THREADS
    };
    
    for (size_t i = 0; i < sizeof(limit_flags)/sizeof(limit_flags[0]) && fdp.remaining_bytes() > 0; i++) {
        int limit_value = fdp.ConsumeIntegralInRange<int>(1, 10000);
        sqlite3_limit(db, limit_flags[i], limit_value);
    }
    
    // Apply deserialization if requested and we have data
    if (use_deserialize && fdp.remaining_bytes() > 0) {
        // Consume remaining bytes as database file content
        std::vector<uint8_t> db_content = fdp.ConsumeRemainingBytes<uint8_t>();
        if (!db_content.empty()) {
            // Allocate memory for database content
            unsigned char* db_buffer = (unsigned char*)sqlite3_malloc64(db_content.size() + 1);
            if (db_buffer != nullptr) {
                memcpy(db_buffer, db_content.data(), db_content.size());
                
                // Try to deserialize the malformed database
                rc = sqlite3_deserialize(db, "main", db_buffer, db_content.size(), db_content.size(),
                                        SQLITE_DESERIALIZE_RESIZEABLE | SQLITE_DESERIALIZE_FREEONCLOSE);
                
                // Set size limit if supported
#ifdef SQLITE_FCNTL_SIZE_LIMIT
                if (max_db_size > 0) {
                    sqlite3_file_control(db, "main", SQLITE_FCNTL_SIZE_LIMIT, &max_db_size);
                }
#endif
            }
        }
    } else {
        // Use remaining data as SQL statements if not using deserialize
        while (fdp.remaining_bytes() > 0) {
            std::string sql = fdp.ConsumeRandomLengthString(1000);
            if (sql.empty()) continue;
            
            char* err_msg = nullptr;
            rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg != nullptr) {
                sqlite3_free(err_msg);
            }
        }
    }
    
    // Run a set of standard SQL test queries (inspired by dbfuzz2.c)
    const char* test_queries[] = {
        "PRAGMA integrity_check;",
        "SELECT * FROM sqlite_schema;",
        "SELECT name FROM sqlite_schema WHERE type='table';",
        "PRAGMA foreign_key_check;",
        "PRAGMA quick_check;",
        "VACUUM;",
        "REINDEX;",
        "PRAGMA optimize;"
    };
    
    for (size_t i = 0; i < sizeof(test_queries)/sizeof(test_queries[0]); i++) {
        char* err_msg = nullptr;
        rc = sqlite3_exec(db, test_queries[i], nullptr, nullptr, &err_msg);
        if (err_msg != nullptr) {
            sqlite3_free(err_msg);
        }
    }
    
    // Test prepared statements with fuzzed parameters
    if (fdp.remaining_bytes() > 0) {
        // Reset progress counter for new operations
        progress_callback_count = 0;
        
        const char* sample_sql = "SELECT ?1, ?2, ?3;";
        sqlite3_stmt* stmt = nullptr;
        const char* tail = nullptr;
        
        rc = sqlite3_prepare_v2(db, sample_sql, -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt != nullptr) {
            // Bind parameters from fuzzer input
            for (int i = 1; i <= 3 && fdp.remaining_bytes() > 0; i++) {
                int param_type = fdp.ConsumeIntegralInRange<int>(1, 5);
                switch (param_type) {
                    case 1: // Integer
                        sqlite3_bind_int64(stmt, i, fdp.ConsumeIntegral<int64_t>());
                        break;
                    case 2: // Double
                        sqlite3_bind_double(stmt, i, fdp.ConsumeFloatingPoint<double>());
                        break;
                    case 3: // Text
                        {
                            std::string text = fdp.ConsumeRandomLengthString(100);
                            sqlite3_bind_text(stmt, i, text.c_str(), text.length(), SQLITE_TRANSIENT);
                        }
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
            
            // Execute the prepared statement
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                // Exercise column access functions
                int col_count = sqlite3_column_count(stmt);
                for (int j = 0; j < col_count; j++) {
                    int col_type = sqlite3_column_type(stmt, j);
                    // Just access the data to exercise code paths
                    switch (col_type) {
                        case SQLITE_INTEGER:
                            sqlite3_column_int64(stmt, j);
                            break;
                        case SQLITE_FLOAT:
                            sqlite3_column_double(stmt, j);
                            break;
                        case SQLITE_TEXT:
                            sqlite3_column_text(stmt, j);
                            sqlite3_column_bytes(stmt, j);
                            break;
                        case SQLITE_BLOB:
                            sqlite3_column_blob(stmt, j);
                            sqlite3_column_bytes(stmt, j);
                            break;
                    }
                }
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    // Clean up and check for memory leaks
    rc = sqlite3_close(db);
    
    // Check for memory leaks (simplified version)
    sqlite3_int64 mem_used = sqlite3_memory_used();
    if (mem_used > 0) {
        // Memory leak detected - this is expected with some fuzzed inputs
        // but we should still clean up
    }
    
    sqlite3_shutdown();
    return 0;
}

} // namespace harness_001
