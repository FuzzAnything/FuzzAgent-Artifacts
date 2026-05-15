#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_000 {
/*
 * Fuzzing harness for SQLite library - focusing on prepared statement APIs
 * This harness targets sqlite3_prepare_v2, sqlite3_step, sqlite3_finalize
 * and related APIs to complement the existing ossfuzz.c which uses sqlite3_exec
 */


int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    if (size < 16) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db,
                             SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                             nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits to prevent timeouts and excessive memory usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 50000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 25000);
    sqlite3_limit(db, SQLITE_LIMIT_LIKE_PATTERN_LENGTH, 250);
    
    // Consume operation type from fuzzer input
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Consume SQL statement - use random length string up to remaining bytes
    // but leave some bytes for other parameters if needed
    size_t remaining = fdp.remaining_bytes();
    size_t sql_len = remaining > 0 ? fdp.ConsumeIntegralInRange<size_t>(0, remaining) : 0;
    std::string sql = fdp.ConsumeBytesAsString(sql_len);
    
    // If no SQL provided, use a simple parameterized query
    if (sql.empty()) {
        sql = "SELECT ?1, ?2, ?3, ?4, ?5;";
    }
    
    switch (operation) {
        case 0: {
            // Test prepared statement with sqlite3_prepare_v2 and sqlite3_step
            sqlite3_stmt* stmt = nullptr;
            const char* tail = nullptr;
            
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Step through the statement multiple times based on input
                // Use remaining input for step count
                int max_steps = fdp.ConsumeIntegralInRange<int>(0, 10);
                for (int i = 0; i < max_steps; i++) {
                    rc = sqlite3_step(stmt);
                    if (rc != SQLITE_ROW && rc != SQLITE_DONE) {
                        break;
                    }
                }
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case 1: {
            // Test sqlite3_exec for comparison with prepared statements
            char* errmsg = nullptr;
            sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errmsg);
            if (errmsg != nullptr) {
                sqlite3_free(errmsg);
            }
            break;
        }
        
        case 2: {
            // Test multiple prepared statements with column access
            sqlite3_stmt* stmt = nullptr;
            const char* tail = nullptr;
            
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Get column count
                int col_count = sqlite3_column_count(stmt);
                
                // Step once to get some data
                rc = sqlite3_step(stmt);
                if (rc == SQLITE_ROW) {
                    // Access some column data
                    for (int i = 0; i < col_count && i < 5; i++) {
                        // Get column type and name
                        int col_type = sqlite3_column_type(stmt, i);
                        const char* col_name = sqlite3_column_name(stmt, i);
                        
                        // Access column data based on type
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
                            case SQLITE_NULL:
                                // Nothing to do for NULL
                                break;
                        }
                    }
                }
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case 3: {
            // Test transaction handling with prepared statements and parameter binding
            sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr);
            
            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Bind some parameters if the statement has placeholders
                int param_count = sqlite3_bind_parameter_count(stmt);
                for (int i = 1; i <= param_count && i <= 5; i++) {
                    // Use fuzzer input to determine binding type and value
                    if (fdp.remaining_bytes() > 0) {
                        uint8_t bind_type = fdp.ConsumeIntegral<uint8_t>() % 5;
                        switch (bind_type) {
                            case 0: {
                                // Bind integer
                                int int_val = fdp.ConsumeIntegral<int>();
                                sqlite3_bind_int(stmt, i, int_val);
                                break;
                            }
                            case 1: {
                                // Bind double
                                double double_val = fdp.ConsumeFloatingPoint<double>();
                                sqlite3_bind_double(stmt, i, double_val);
                                break;
                            }
                            case 2: {
                                // Bind text
                                if (fdp.remaining_bytes() > 0) {
                                    size_t text_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)100));
                                    std::string text_val = fdp.ConsumeBytesAsString(text_len);
                                    sqlite3_bind_text(stmt, i, text_val.c_str(), -1, SQLITE_TRANSIENT);
                                } else {
                                    sqlite3_bind_null(stmt, i);
                                }
                                break;
                            }
                            case 3: {
                                // Bind blob
                                if (fdp.remaining_bytes() > 0) {
                                    size_t blob_len = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)100));
                                    std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_len);
                                    sqlite3_bind_blob(stmt, i, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
                                } else {
                                    sqlite3_bind_null(stmt, i);
                                }
                                break;
                            }
                            case 4:
                            default:
                                // Bind null
                                sqlite3_bind_null(stmt, i);
                                break;
                        }
                    } else {
                        sqlite3_bind_null(stmt, i);
                    }
                }
                
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
            
            sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr);
            break;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_000
