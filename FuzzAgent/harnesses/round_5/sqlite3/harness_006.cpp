#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_006 {
/*
** SQLite UTF-16 API cluster fuzzing harness
**
** This harness specifically targets SQLite UTF-16 APIs identified as completely
** uncovered coverage gaps across all 6 existing fuzzers:
** - sqlite3_open16: Open database with UTF-16 filename
** - sqlite3_prepare16: Prepare SQL statement from UTF-16 text (legacy)
** - sqlite3_prepare16_v2: Prepare SQL statement from UTF-16 text
** - sqlite3_prepare16_v3: Prepare SQL statement from UTF-16 text with flags
** - sqlite3_bind_text16: Bind UTF-16 text to prepared statement parameters
** - sqlite3_column_text16: Retrieve UTF-16 text from query results
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare, step) - UTF-8 only
** - harness_001: Advanced operations with progress handlers and authorizers - UTF-8 only
** - harness_002: BLOB-specific incremental I/O operations - UTF-8 only
** - harness_003: Virtual table creation, configuration, and operations - UTF-8 only
** - harness_004: Database backup operations between databases - UTF-8 only
** - harness_005: Database serialization/deserialization with WAL checkpoints - UTF-8 only
** - harness_006: UTF-16 specific APIs and Unicode handling (this harness)
**
** UTF-16 operation lifecycle: 
**   Database open with UTF-16 filename → Prepare UTF-16 statements → 
**   Bind UTF-16 parameters → Execute → Retrieve UTF-16 results → Cleanup
**
** Key design considerations:
** 1. Native UTF-16 byte order handling (platform-dependent)
** 2. Proper conversion between UTF-8 and UTF-16 where necessary
** 3. Unicode surrogate pair handling for complete coverage
** 4. Byte order marks (BOM) handling for UTF-16 text
*/



// Simple callback for sqlite3_exec (minimal implementation)
static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName) {
    (void)NotUsed;
    (void)argc;
    (void)argv;
    (void)azColName;
    return 0;
}

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

// Operation types for the UTF-16 harness
enum Utf16Operation {
    UTF16_OP_BASIC = 0,
    UTF16_OP_MULTIPLE_PREPARE = 1,
    UTF16_OP_BIND_RETRIEVE = 2,
    UTF16_OP_MIXED_ENCODINGS = 3
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check: need enough for UTF-16 operations
    // sizeof(int) for config + sizeof(size_t) for string lengths + at least 128 bytes for UTF-16 data
    const size_t MIN_SIZE = sizeof(int) * 4 + sizeof(size_t) * 2 + 128;
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
    
    // Determine operation type from fuzzer input
    Utf16Operation operation = static_cast<Utf16Operation>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 3));
    
    // Generate UTF-16 database filename (for sqlite3_open16)
    auto [utf16_filename, filename_bytes] = generate_utf16_string(fdp, 256);
    
    // Try to open database with UTF-16 filename
    int rc = SQLITE_OK;
    if (!utf16_filename.empty()) {
        // Note: For in-memory testing, we still use ":memory:" but convert it to UTF-16
        // to test the sqlite3_open16 API. In real usage, this would be an actual filename.
        const void* filename_for_open16 = nullptr;
        
        if (fdp.ConsumeBool()) {
            // Test with actual UTF-16 filename (converted ":memory:")
            std::vector<uint16_t> memory_filename = utf8_to_utf16(":memory:");
            filename_for_open16 = memory_filename.data();
            rc = sqlite3_open16(filename_for_open16, &db);
        } else {
            // Test with null-terminated UTF-16 string from fuzzer input
            filename_for_open16 = utf16_filename.data();
            rc = sqlite3_open16(filename_for_open16, &db);
        }
    } else {
        // Fallback to UTF-8 open if we couldn't generate UTF-16 filename
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
    
    // Create basic table using UTF-8 API (for compatibility)
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS utf16_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "description TEXT, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Insert some data for testing retrieval using fuzzer-generated strings
    if (fdp.remaining_bytes() > 100) {
        // Generate test data from fuzzer input
        std::string name1 = fdp.ConsumeRandomLengthString(50);
        std::string desc1 = fdp.ConsumeRandomLengthString(100);
        std::string name2 = fdp.ConsumeRandomLengthString(50);
        std::string desc2 = fdp.ConsumeRandomLengthString(100);
        
        // Build dynamic INSERT statement
        std::string insert_sql = 
            "INSERT INTO utf16_test (name, description, data) VALUES (?, ?, X'01020304'), (?, ?, X'05060708');";
        
        sqlite3_stmt* stmt = nullptr;
        rc = sqlite3_prepare_v2(db, insert_sql.c_str(), -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt != nullptr) {
            // Bind fuzzer-generated strings
            sqlite3_bind_text(stmt, 1, name1.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, desc1.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, name2.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, desc2.c_str(), -1, SQLITE_TRANSIENT);
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    // Main UTF-16 testing based on operation type
    switch (operation) {
        case UTF16_OP_BASIC: {
            // Basic UTF-16 prepare and execute
            auto [sql_utf16, sql_bytes] = generate_utf16_string(fdp, 512);
            
            if (!sql_utf16.empty()) {
                sqlite3_stmt* stmt = nullptr;
                const void* pzTail = nullptr;
                
                // Choose which prepare16 function to test
                uint8_t prepare_func = fdp.ConsumeIntegral<uint8_t>() % 3;
                
                switch (prepare_func) {
                    case 0:
                        // sqlite3_prepare16 (legacy)
                        rc = sqlite3_prepare16(db, sql_utf16.data(), 
                                              static_cast<int>(sql_bytes),
                                              &stmt, &pzTail);
                        break;
                    case 1:
                        // sqlite3_prepare16_v2
                        rc = sqlite3_prepare16_v2(db, sql_utf16.data(),
                                                 static_cast<int>(sql_bytes),
                                                 &stmt, &pzTail);
                        break;
                    case 2:
                        // sqlite3_prepare16_v3
                        rc = sqlite3_prepare16_v3(db, sql_utf16.data(),
                                                 static_cast<int>(sql_bytes),
                                                 0, // prepFlags
                                                 &stmt, &pzTail);
                        break;
                }
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    // Execute the statement
                    rc = sqlite3_step(stmt);
                    while (rc == SQLITE_ROW) {
                        // Process row if needed
                        rc = sqlite3_step(stmt);
                    }
                    
                    sqlite3_finalize(stmt);
                }
            }
            break;
        }
        
        case UTF16_OP_MULTIPLE_PREPARE: {
            // Test multiple UTF-16 prepare functions in sequence
            for (int i = 0; i < 3 && fdp.remaining_bytes() > 100; i++) {
                auto [sql_utf16, sql_bytes] = generate_utf16_string(fdp, 256);
                
                if (!sql_utf16.empty()) {
                    sqlite3_stmt* stmt = nullptr;
                    const void* pzTail = nullptr;
                    
                    // Alternate between different prepare functions
                    if (i == 0) {
                        rc = sqlite3_prepare16(db, sql_utf16.data(),
                                              static_cast<int>(sql_bytes),
                                              &stmt, &pzTail);
                    } else if (i == 1) {
                        rc = sqlite3_prepare16_v2(db, sql_utf16.data(),
                                                 static_cast<int>(sql_bytes),
                                                 &stmt, &pzTail);
                    } else {
                        rc = sqlite3_prepare16_v3(db, sql_utf16.data(),
                                                 static_cast<int>(sql_bytes),
                                                 fdp.ConsumeIntegral<uint32_t>() & 0x03, // Limited prepFlags
                                                 &stmt, &pzTail);
                    }
                    
                    if (rc == SQLITE_OK && stmt != nullptr) {
                        sqlite3_step(stmt);
                        sqlite3_finalize(stmt);
                    }
                }
            }
            break;
        }
        
        case UTF16_OP_BIND_RETRIEVE: {
            // Test UTF-16 bind and column retrieval
            // Prepare a simple SELECT statement with UTF-16
            std::vector<uint16_t> select_sql = utf8_to_utf16("SELECT * FROM utf16_test WHERE name LIKE ?");
            
            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare16_v2(db, select_sql.data(),
                                     static_cast<int>(select_sql.size() * sizeof(uint16_t)),
                                     &stmt, nullptr);
            
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Generate UTF-16 parameter for binding
                auto [param_utf16, param_bytes] = generate_utf16_string(fdp, 100);
                
                if (!param_utf16.empty()) {
                    // Bind UTF-16 parameter
                    rc = sqlite3_bind_text16(stmt, 1, param_utf16.data(),
                                           static_cast<int>(param_bytes),
                                           SQLITE_TRANSIENT);
                    
                    if (rc == SQLITE_OK) {
                        // Execute and retrieve UTF-16 columns
                        rc = sqlite3_step(stmt);
                        while (rc == SQLITE_ROW) {
                            // Retrieve UTF-16 text from columns
                            int col_count = sqlite3_column_count(stmt);
                            for (int col = 0; col < col_count; col++) {
                                const char* col_type = sqlite3_column_decltype(stmt, col);
                                if (col_type && strstr(col_type, "TEXT")) {
                                    // Try to get UTF-16 text
                                    const void* utf16_text = sqlite3_column_text16(stmt, col);
                                    if (utf16_text) {
                                        // Get byte length
                                        int bytes = sqlite3_column_bytes16(stmt, col);
                                        (void)bytes; // Use bytes to avoid unused warning
                                    }
                                }
                            }
                            rc = sqlite3_step(stmt);
                        }
                    }
                }
                
                sqlite3_finalize(stmt);
            }
            break;
        }
        
        case UTF16_OP_MIXED_ENCODINGS: {
            // Test mixing UTF-8 and UTF-16 APIs
            // Create a statement with UTF-8 but bind with UTF-16
            const char* utf8_sql = "INSERT INTO utf16_test (name, description) VALUES (?, ?)";
            
            sqlite3_stmt* stmt = nullptr;
            rc = sqlite3_prepare_v2(db, utf8_sql, -1, &stmt, nullptr);
            
            if (rc == SQLITE_OK && stmt != nullptr) {
                // Generate UTF-16 parameters
                auto [name_utf16, name_bytes] = generate_utf16_string(fdp, 50);
                auto [desc_utf16, desc_bytes] = generate_utf16_string(fdp, 200);
                
                if (!name_utf16.empty()) {
                    // Bind UTF-16 to UTF-8 prepared statement
                    rc = sqlite3_bind_text16(stmt, 1, name_utf16.data(),
                                           static_cast<int>(name_bytes),
                                           SQLITE_TRANSIENT);
                    
                    if (rc == SQLITE_OK && !desc_utf16.empty()) {
                        rc = sqlite3_bind_text16(stmt, 2, desc_utf16.data(),
                                               static_cast<int>(desc_bytes),
                                               SQLITE_TRANSIENT);
                        
                        if (rc == SQLITE_OK) {
                            sqlite3_step(stmt);
                        }
                    }
                }
                
                sqlite3_finalize(stmt);
            }
            
            // Also test UTF-16 prepare with UTF-8-like data
            std::string utf8_like_data = fdp.ConsumeRandomLengthString(200);
            std::vector<uint16_t> utf16_from_utf8 = utf8_to_utf16(utf8_like_data);
            
            if (!utf16_from_utf8.empty()) {
                sqlite3_stmt* stmt2 = nullptr;
                rc = sqlite3_prepare16(db, utf16_from_utf8.data(),
                                      static_cast<int>(utf16_from_utf8.size() * sizeof(uint16_t)),
                                      &stmt2, nullptr);
                
                if (rc == SQLITE_OK && stmt2 != nullptr) {
                    sqlite3_step(stmt2);
                    sqlite3_finalize(stmt2);
                }
            }
            break;
        }
    }
    
    // Additional UTF-16 error handling tests
    if (fdp.ConsumeBool()) {
        // Test error messages in UTF-16
        const void* utf16_err = sqlite3_errmsg16(db);
        (void)utf16_err; // Use to avoid unused warning
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

} // namespace harness_006
