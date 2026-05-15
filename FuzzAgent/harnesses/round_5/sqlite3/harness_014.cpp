#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_014 {
/*
** SQLite Advanced UTF-16 API Fuzzing Harness - Critical Coverage Gaps
**
** This harness specifically targets critical UTF-16 SQLite APIs identified
** as completely uncovered coverage gaps:
** - sqlite3_complete16: Determine if UTF-16 SQL string is complete
** - sqlite3_prepare16_v3: Prepare UTF-16 SQL statements with flags
** - sqlite3_bind_text64: Bind UTF-8/UTF-16 text with 64-bit length
** - sqlite3_result_text16be: Set UTF-16BE text result in custom functions
** - sqlite3_result_text16le: Set UTF-16LE text result in custom functions
**
** Differentiation from existing harnesses:
** - harness_006: Basic UTF-16 operations (open16, prepare16, bind_text16, column_text16)
** - harness_010: Advanced UTF-16 collation, result, and encoding operations
** - harness_014: Critical uncovered UTF-16 APIs with complete lifecycle (this harness)
**
** Advanced UTF-16 operation lifecycle:
**   Database setup with create_function_v2 → SQL completion checking →
**   UTF-16 statement preparation with flags → Binding with 64-bit lengths →
**   Custom function execution with different UTF-16 encodings → Cleanup
**
** Key design considerations:
** 1. Different UTF-16 encodings (BE, LE, native)
** 2. 64-bit length binding for large text values
** 3. SQL completion checking for UTF-16 strings
** 4. Custom function implementation with create_function_v2
** 5. Edge cases for large text values and encoding conversions
*/





// Helper function to generate UTF-16 string from fuzzer input
// Returns a pair: (UTF-16 data, byte length)
static std::pair<std::vector<uint16_t>, size_t> generate_utf16_string(FuzzedDataProvider& fdp, size_t max_chars) {
    // Consume length for UTF-16 string (in characters, not bytes)
    size_t length = fdp.ConsumeIntegralInRange<size_t>(0, max_chars);
    
    if (length == 0) {
        return {std::vector<uint16_t>(), 0};
    }
    
    std::vector<uint16_t> utf16_chars;
    utf16_chars.reserve(length);
    
    // Generate random UTF-16 code points (including surrogate pairs)
    for (size_t i = 0; i < length && fdp.remaining_bytes() > 0; i++) {
        // Generate basic multilingual plane (BMP) characters most of the time
        // but occasionally generate surrogate pairs for astral plane coverage
        if (fdp.ConsumeProbability<double>() < 0.05 && i + 1 < length) {
            // Generate a surrogate pair (U+10000 to U+10FFFF)
            uint16_t high_surrogate = 0xD800 + fdp.ConsumeIntegralInRange<uint16_t>(0, 0x3FF);
            uint16_t low_surrogate = 0xDC00 + fdp.ConsumeIntegralInRange<uint16_t>(0, 0x3FF);
            utf16_chars.push_back(high_surrogate);
            utf16_chars.push_back(low_surrogate);
            i++; // Consume an extra position
        } else {
            // Generate BMP character (U+0000 to U+FFFF, excluding surrogate range)
            uint16_t ch = fdp.ConsumeIntegral<uint16_t>();
            // Exclude surrogate range (0xD800-0xDFFF)
            if (ch >= 0xD800 && ch <= 0xDFFF) {
                ch = 0x0041; // Fallback to 'A'
            }
            utf16_chars.push_back(ch);
        }
    }
    
    // Add null terminator if we have characters
    if (!utf16_chars.empty()) {
        utf16_chars.push_back(0);
    }
    
    return {utf16_chars, utf16_chars.size() * sizeof(uint16_t)};
}

// Helper function to convert UTF-8 to UTF-16 for testing mixed encodings
static std::vector<uint16_t> utf8_to_utf16(const std::string& utf8_str) {
    // Simple conversion: ASCII to UTF-16 (for testing purposes)
    std::vector<uint16_t> utf16_chars;
    for (char c : utf8_str) {
        utf16_chars.push_back(static_cast<uint16_t>(c & 0xFF));
    }
    if (!utf16_chars.empty()) {
        utf16_chars.push_back(0);
    }
    return utf16_chars;
}

// Custom scalar function that uses different UTF-16 result functions
static void utf16_advanced_result_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    (void)argc; // Unused
    (void)argv; // Unused
    
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(sqlite3_user_data(context));
    if (fdp == nullptr || fdp->remaining_bytes() < 10) {
        sqlite3_result_null(context);
        return;
    }
    
    // Generate UTF-16 string from fuzzer input
    auto [utf16_data, bytes] = generate_utf16_string(*fdp, 200);
    
    if (!utf16_data.empty()) {
        // Choose which UTF-16 result function to use based on fuzzer input
        uint8_t func_type = fdp->ConsumeIntegral<uint8_t>() % 4;
        
        switch (func_type) {
            case 0:
                // sqlite3_result_text16 - native byte order
                sqlite3_result_text16(context, utf16_data.data(), 
                                    static_cast<int>(bytes),
                                    SQLITE_TRANSIENT);
                break;
            case 1:
                // sqlite3_result_text16le - little-endian UTF-16
                sqlite3_result_text16le(context, utf16_data.data(), 
                                      static_cast<int>(bytes),
                                      SQLITE_TRANSIENT);
                break;
            case 2:
                // sqlite3_result_text16be - big-endian UTF-16
                sqlite3_result_text16be(context, utf16_data.data(), 
                                      static_cast<int>(bytes),
                                      SQLITE_TRANSIENT);
                break;
            case 3:
                // sqlite3_result_text16 with explicit encoding
                sqlite3_result_text16(context, utf16_data.data(), 
                                    static_cast<int>(bytes),
                                    SQLITE_TRANSIENT);
                break;
        }
    } else {
        sqlite3_result_null(context);
    }
}

// Custom aggregate function step (minimal implementation)
static void utf16_aggregate_step(sqlite3_context* context, int argc, sqlite3_value** argv) {
    (void)context;
    (void)argc;
    (void)argv;
    // For fuzzing, just a stub implementation
}

// Custom aggregate function finalize
static void utf16_aggregate_finalize(sqlite3_context* context) {
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(sqlite3_user_data(context));
    if (fdp == nullptr || fdp->remaining_bytes() < 10) {
        sqlite3_result_null(context);
        return;
    }
    
    // Generate UTF-16 string for aggregate result
    auto [utf16_data, bytes] = generate_utf16_string(*fdp, 100);
    
    if (!utf16_data.empty()) {
        // Use big-endian UTF-16 for aggregate result
        sqlite3_result_text16be(context, utf16_data.data(), 
                              static_cast<int>(bytes),
                              SQLITE_TRANSIENT);
    } else {
        sqlite3_result_null(context);
    }
}

// Operation types for the advanced UTF-16 harness
enum AdvancedUtf16Operation {
    ADV_UTF16_OP_COMPLETE = 0,
    ADV_UTF16_OP_PREPARE_V3 = 1,
    ADV_UTF16_OP_BIND_TEXT64 = 2,
    ADV_UTF16_OP_RESULT_FUNCTIONS = 3,
    ADV_UTF16_OP_FULL_LIFECYCLE = 4
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for advanced UTF-16 operations
    const size_t MIN_SIZE = sizeof(int) * 8 + sizeof(size_t) * 4 + 256;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    char* err_msg = nullptr;
    
    // Generate UTF-16 database filename (for sqlite3_open16)
    auto [utf16_filename, filename_bytes] = generate_utf16_string(fdp, 256);
    
    // Try to open database with UTF-16 filename or in-memory
    int rc = SQLITE_OK;
    if (!utf16_filename.empty() && fdp.ConsumeBool()) {
        // Test with null-terminated UTF-16 string from fuzzer input
        rc = sqlite3_open16(utf16_filename.data(), &db);
    } else {
        // Fallback to UTF-8 open for in-memory database
        rc = sqlite3_open_v2(":memory:", &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 
                            nullptr);
    }
    
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set limits to prevent excessive resource usage
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, fdp.ConsumeIntegralInRange<int>(1000, 50000));
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    
    // Create basic table using UTF-8 API
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS advanced_utf16_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "description TEXT, "
        "large_text TEXT);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Register custom scalar function using create_function_v2
    std::vector<uint8_t> func_data = fdp.ConsumeRemainingBytes<uint8_t>();
    FuzzedDataProvider* func_fdp = new FuzzedDataProvider(func_data.data(), func_data.size());
    if (func_fdp->remaining_bytes() > 50) {
        rc = sqlite3_create_function_v2(
            db,
            "utf16_advanced_func",
            1, // Number of arguments
            4, // SQLITE_UTF16 = 4, Text encoding
            func_fdp, // User data (FuzzedDataProvider)
            utf16_advanced_result_func,
            nullptr, // Step function (not an aggregate)
            nullptr, // Finalize function (not an aggregate)
            [](void* p) { delete static_cast<FuzzedDataProvider*>(p); } // Destructor
        );
    }
    
    // Register custom aggregate function
    std::vector<uint8_t> agg_data;
    if (fdp.remaining_bytes() > 0) {
        agg_data = fdp.ConsumeRemainingBytes<uint8_t>();
    }
    FuzzedDataProvider* agg_fdp = new FuzzedDataProvider(agg_data.data(), agg_data.size());
    if (agg_fdp->remaining_bytes() > 50) {
        rc = sqlite3_create_function_v2(
            db,
            "utf16_aggregate",
            1, // Number of arguments
            3, // SQLITE_UTF16BE = 3, Big-endian UTF-16 encoding
            agg_fdp, // User data
            nullptr, // Scalar function (not a scalar)
            utf16_aggregate_step,
            utf16_aggregate_finalize,
            [](void* p) { delete static_cast<FuzzedDataProvider*>(p); } // Destructor
        );
    }
    
    // Determine operation type from fuzzer input
    AdvancedUtf16Operation operation = static_cast<AdvancedUtf16Operation>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 4));
    // Insert some test data for queries
    if (fdp.remaining_bytes() > 100) {
        std::string insert_sql = 
            "INSERT INTO advanced_utf16_test (name, description, large_text) VALUES (?, ?, ?);";
        
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt != nullptr) {
            // Generate test data from fuzzer input
            std::string name = fdp.ConsumeRandomLengthString(50);
            std::string desc = fdp.ConsumeRandomLengthString(200);
            std::string large_text = fdp.ConsumeRandomLengthString(500);
            
            // Bind UTF-8 parameters
            sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, desc.c_str(), -1, SQLITE_TRANSIENT);
            
            // Test sqlite3_bind_text64 with 64-bit length
            if (!large_text.empty()) {
                sqlite3_bind_text64(stmt, 3, large_text.c_str(), 
                                  (sqlite3_uint64)large_text.size(),
                                  SQLITE_TRANSIENT, 1); // SQLITE_UTF8 = 1
            } else {
                sqlite3_bind_null(stmt, 3);
            }
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Main advanced UTF-16 testing based on operation type
    switch (operation) {
        case ADV_UTF16_OP_COMPLETE: {
            // Test sqlite3_complete16 with various UTF-16 SQL strings
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 50; i++) {
                auto [sql_utf16, sql_bytes] = generate_utf16_string(fdp, 512);
                
                if (!sql_utf16.empty()) {
                    // Check if the UTF-16 SQL string is complete
                    int is_complete = sqlite3_complete16(sql_utf16.data());
                    (void)is_complete; // Use result to avoid unused warning
                    
                    // Also test with known complete/incomplete SQL
                    if (i == 0) {
                        // Test with known complete SQL (converted to UTF-16)
                        std::vector<uint16_t> complete_sql = utf8_to_utf16("SELECT 1;");
                        sqlite3_complete16(complete_sql.data());
                    } else if (i == 1) {
                        // Test with known incomplete SQL
                        std::vector<uint16_t> incomplete_sql = utf8_to_utf16("SELECT");
                        sqlite3_complete16(incomplete_sql.data());
                    }
                }
            }
            break;
        }
        
        case ADV_UTF16_OP_PREPARE_V3: {
            // Test sqlite3_prepare16_v3 with different preparation flags
            auto [sql_utf16, sql_bytes] = generate_utf16_string(fdp, 1024);
            
            if (!sql_utf16.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const void* pzTail = nullptr;
                
                // Consume preparation flags from fuzzer input
                unsigned int prepFlags = fdp.ConsumeIntegral<uint32_t>();
                
                // Limit flags to valid combinations
                prepFlags &= (SQLITE_PREPARE_PERSISTENT | SQLITE_PREPARE_NORMALIZE |
                             SQLITE_PREPARE_NO_VTAB | SQLITE_PREPARE_NO_VTAB);
                
                rc = sqlite3_prepare16_v3(db, sql_utf16.data(),
                                         static_cast<int>(sql_bytes),
                                         prepFlags,
                                         &stmt, &pzTail);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    // Execute and process results
                    rc = sqlite3_step(stmt);
                    while (rc == SQLITE_ROW) {
                        int col_count = sqlite3_column_count(stmt);
                        for (int col = 0; col < col_count; col++) {
                            // Try to get UTF-16 text from columns
                            const void* utf16_text = sqlite3_column_text16(stmt, col);
                            if (utf16_text) {
                                int bytes = sqlite3_column_bytes16(stmt, col);
                                (void)bytes;
                            }
                        }
                        rc = sqlite3_step(stmt);
                    }
                    
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case ADV_UTF16_OP_BIND_TEXT64: {
            // Test sqlite3_bind_text64 with UTF-16 text
            const char* insert_sql = "INSERT INTO advanced_utf16_test (name, large_text) VALUES (?, ?)";
            
            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
            
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Generate UTF-8 text for binding (sqlite3_bind_text64 expects UTF-8)
                std::string utf8_text = fdp.ConsumeRandomLengthString(1000);
                
                if (!utf8_text.empty()) {
                    // Test sqlite3_bind_text64 with UTF-8 encoding
                    // Note: The length is in bytes, not characters
                    sqlite3_bind_text64(stmt, 2, utf8_text.c_str(),
                                      (sqlite3_uint64)utf8_text.size(),
                                      SQLITE_TRANSIENT, 1); // SQLITE_UTF8 = 1
                    
                    // Bind a regular UTF-8 name
                    std::string name = fdp.ConsumeRandomLengthString(50);
                    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
                    
                    sqlite3_step(stmt);
                }
                
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case ADV_UTF16_OP_RESULT_FUNCTIONS: {
            // Test custom functions with different UTF-16 result functions
            if (fdp.remaining_bytes() > 100) {
                // Execute scalar function
                const char* scalar_sql = "SELECT utf16_advanced_func(?)";
                
                sqlite3_stmt* stmt = nullptr;
                rc = sqlite3_prepare_v2(db, scalar_sql, -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    // Bind parameter
                    std::string param = fdp.ConsumeRandomLengthString(100);
                    sqlite3_bind_text(stmt, 1, param.c_str(), -1, SQLITE_TRANSIENT);
                    
                    // Execute and get result
                    rc = sqlite3_step(stmt);
                    if (rc == SQLITE_ROW) {
                        // Get the UTF-16 result
                        const void* result = sqlite3_column_text16(stmt, 0);
                        if (result) {
                            int bytes = sqlite3_column_bytes16(stmt, 0);
                            (void)bytes;
                        }
                    }
                    
                    sqlite3_finalize(stmt);
                }
                
                // Test aggregate function
                const char* aggregate_sql = "SELECT utf16_aggregate(name) FROM advanced_utf16_test";
                
                sqlite3_stmt* stmt2 = nullptr;
                rc = sqlite3_prepare_v2(db, aggregate_sql, -1, &stmt2, nullptr);
                
                if (rc == SQLITE_OK && stmt2 != nullptr) {
                    rc = sqlite3_step(stmt2);
                    if (rc == SQLITE_ROW) {
                        // Get the UTF-16BE aggregate result
                        const void* result = sqlite3_column_text16(stmt2, 0);
                        if (result) {
                            int bytes = sqlite3_column_bytes16(stmt2, 0);
                            (void)bytes;
                        }
                    }
                    
                    sqlite3_finalize(stmt2);
                }
            }
            break;
        }
        
        case ADV_UTF16_OP_FULL_LIFECYCLE: {
            // Test full lifecycle of advanced UTF-16 operations
            // 1. SQL completion checking
            auto [sql_utf16, sql_bytes] = generate_utf16_string(fdp, 500);
            if (!sql_utf16.empty()) {
                sqlite3_complete16(sql_utf16.data());
            }
            
            // 2. Prepare with v3 flags
            sqlite3_stmt* stmt = nullptr;
            const void* pzTail = nullptr;
            
            if (!sql_utf16.empty() && fdp.remaining_bytes() > 100) {
                rc = sqlite3_prepare16_v3(db, sql_utf16.data(),
                                         static_cast<int>(sql_bytes),
                                         SQLITE_PREPARE_PERSISTENT,
                                         &stmt, &pzTail);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    // 3. Bind with text64 using UTF-8 text
                    std::string bind_text = fdp.ConsumeRandomLengthString(200);
                    if (!bind_text.empty()) {
                        // Find parameter index
                        int param_count = sqlite3_bind_parameter_count(stmt);
                        if (param_count > 0) {
                            // Bind UTF-8 text with 64-bit length
                            sqlite3_bind_text64(stmt, 1, bind_text.c_str(),
                                              (sqlite3_uint64)bind_text.size(),
                                              SQLITE_TRANSIENT, 1); // SQLITE_UTF8 = 1
                        }
                    }
                    
                    // 4. Execute
                    rc = sqlite3_step(stmt);
                    
                    // 5. Cleanup
                    sqlite3_finalize(stmt);
                }

            }
            
            // 6. Test custom functions
            if (fdp.remaining_bytes() > 50) {
                const char* func_sql = "SELECT utf16_advanced_func('test')";
                sqlite3_stmt* func_stmt = nullptr;
                rc = sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, nullptr);
                
                if (rc == SQLITE_OK && func_stmt != nullptr) {
                    sqlite3_step(func_stmt);
                    sqlite3_finalize(func_stmt);
                }
            }
            break;
        }
    }
    
    // Clean up any remaining FuzzedDataProvider objects
    // (They should be cleaned up by sqlite3_create_function_v2 destructor)
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_014
