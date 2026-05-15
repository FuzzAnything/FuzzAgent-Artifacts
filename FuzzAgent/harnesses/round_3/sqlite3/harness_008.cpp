#include <fuzzer/FuzzedDataProvider.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include "sqlite3.h"

namespace harness_008 {


// Simple helper to create UTF-16 strings from UTF-8 for fuzzing
// This creates a basic UTF-16 string by treating each byte as a 16-bit character
// For fuzzing purposes, this is sufficient to test the UTF-16 APIs
std::vector<unsigned short> create_simple_utf16(const std::string& utf8_str) {
    std::vector<unsigned short> utf16_result;
    utf16_result.reserve(utf8_str.size() + 1);
    
    for (unsigned char c : utf8_str) {
        utf16_result.push_back(static_cast<unsigned short>(c));
    }
    
    utf16_result.push_back(0); // Null terminator
    return utf16_result;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check for UTF-16 SQL operations
    if (size < 16) {
        return 0;
    }

    FuzzedDataProvider fdp(data, size);

    // Consume fixed-size configuration parameters
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;  // Which prepare function to use
    uint8_t prep_flags = fdp.ConsumeIntegral<uint8_t>();
    bool test_complete16 = fdp.ConsumeBool();
    bool test_stmt_explain = fdp.ConsumeBool();
    bool use_memory_db = fdp.ConsumeBool();
    uint8_t explain_mode = fdp.ConsumeIntegral<uint8_t>() % 3;  // 0: normal, 1: explain, 2: explain query plan
    
    // Consume variable-size data for SQL statements
    std::string sql_utf8 = fdp.ConsumeRandomLengthString(1024);
    std::string remaining_input = fdp.ConsumeRemainingBytesAsString();
    
    // Create UTF-16 encoded SQL strings
    std::vector<unsigned short> sql_utf16 = create_simple_utf16(sql_utf8);
    std::vector<unsigned short> remaining_utf16 = create_simple_utf16(remaining_input);
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    const void* tail = nullptr;
    int rc;

    // Initialize SQLite (sqlite3_initialize is called automatically by sqlite3_open16 if needed)
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Open database with UTF-16 filename or in-memory database
    const void* db_name_utf16 = nullptr;
    std::vector<unsigned short> db_name;
    
    if (use_memory_db) {
        // Use in-memory database with UTF-16 encoding
        db_name = create_simple_utf16(":memory:");
        db_name_utf16 = db_name.data();
    } else {
        // Use temporary file with UTF-16 name  
        std::string temp_name = "fuzz_temp.db";
        db_name = create_simple_utf16(temp_name);
        db_name_utf16 = db_name.data();
    }
    
    // Open UTF-16 database - PRIMARY TARGET: sqlite3_open16
    rc = sqlite3_open16(db_name_utf16, &db);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }

    // Set memory limits to prevent excessive memory usage
    sqlite3_hard_heap_limit64(50000000); // 50MB limit

    // Test sqlite3_complete16 to check if SQL statement is complete - PRIMARY TARGET: sqlite3_complete16
    if (test_complete16 && sql_utf16.size() > 1) {
        int is_complete = sqlite3_complete16(sql_utf16.data());
        // Result is 0 if incomplete, non-zero if complete
        // Just calling the API exercises the code paths
    }

    // Prepare UTF-16 SQL statement using different prepare functions - PRIMARY TARGETS: prepare16 family
    switch (operation) {
        case 0:
            // Use sqlite3_prepare16 (legacy interface)
            rc = sqlite3_prepare16(db, sql_utf16.data(), -1, &stmt, &tail);
            break;
        case 1:
            // Use sqlite3_prepare16_v2 (recommended interface)
            rc = sqlite3_prepare16_v2(db, sql_utf16.data(), -1, &stmt, &tail);
            break;
        case 2:
            // Use sqlite3_prepare16_v3 with prepare flags
            // SQLITE_PREPARE_PERSISTENT flag value is 0x01 based on SQLite docs
            rc = sqlite3_prepare16_v3(db, sql_utf16.data(), -1, prep_flags & 0x01, &stmt, &tail);
            break;
        case 3:
        default:
            // Try multiple prepare functions
            rc = sqlite3_prepare16(db, sql_utf16.data(), 
                                  static_cast<int>((sql_utf16.size() - 1) * sizeof(unsigned short)), 
                                  &stmt, &tail);
            if (rc != SQLITE_OK || stmt == nullptr) {
                rc = sqlite3_prepare16_v2(db, sql_utf16.data(), -1, &stmt, &tail);
            }
            break;
    }

    // If statement was successfully prepared, test additional functionality
    if (rc == SQLITE_OK && stmt != nullptr) {
        // Test sqlite3_stmt_explain if requested - PRIMARY TARGET: sqlite3_stmt_explain
        if (test_stmt_explain) {
            // explain_mode: 0 = normal, 1 = explain, 2 = explain query plan
            int explain_rc = sqlite3_stmt_explain(stmt, explain_mode);
            // Return value indicates success or failure
        }
        
        // Step through the statement to execute it (limit steps to prevent infinite loops)
        int step_count = 0;
        while (step_count < 10 && (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            step_count++;
            
            // Get column information without accessing data
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                // Just get column type without accessing data
                int col_type = sqlite3_column_type(stmt, i);
                
                // Test UTF-16 column name function (core function that should be available)
                const void* col_name16 = sqlite3_column_name16(stmt, i);
                
                // Note: Some UTF-16 column info functions might not be available in this build
                // We'll stick to core functions to ensure compilation succeeds
            }
        }
        
        // Reset the statement
        sqlite3_reset(stmt);
        
        // Test statement SQL retrieval
        const char* sql_utf8_retrieved = sqlite3_sql(stmt);
        
        // Test expanded SQL
        char* expanded_sql = sqlite3_expanded_sql(stmt);
        if (expanded_sql != nullptr) {
            sqlite3_free(expanded_sql);
        }
        
        // Finalize the statement - REQUIRED HELPER: sqlite3_finalize
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Test with remaining input as another SQL statement
    if (remaining_utf16.size() > 1) {
        rc = sqlite3_prepare16_v2(db, remaining_utf16.data(), -1, &stmt, &tail);
        if (rc == SQLITE_OK && stmt != nullptr) {
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }

    // Close the database - REQUIRED HELPER: sqlite3_close / sqlite3_close_v2
    if (db != nullptr) {
        // Randomly choose between close and close_v2
        if (fdp.ConsumeBool()) {
            rc = sqlite3_close(db);
        } else {
            rc = sqlite3_close_v2(db);
        }
    }

    // Shutdown SQLite
    sqlite3_shutdown();

    return 0;
}

} // namespace harness_008
