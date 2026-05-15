#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_010 {
/*
** SQLite Advanced UTF-16/Unicode Operations Fuzzing Harness
**
** This harness specifically targets advanced SQLite Unicode/UTF-16 operations
** identified as the most significant coverage gap with over 5000 undiscovered
** branch points. Focus on advanced Unicode features beyond basic UTF-16 string
** operations covered in harness_006:
** - UTF-16 collation creation and handling (sqlite3_create_collation16)
** - UTF-16 collation needed callbacks (sqlite3_collation_needed16)
** - UTF-16 result text functions (sqlite3_result_text16, sqlite3_result_text16le, sqlite3_result_text16be)
** - Different UTF-16 encodings (SQLITE_UTF16, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16_ALIGNED)
** - Unicode normalization and encoding conversion patterns
**
** Differentiation from existing harnesses:
** - harness_000: Basic database operations (open, exec, prepare, step) - UTF-8 only
** - harness_001: Advanced operations with progress handlers and authorizers - UTF-8 only
** - harness_002: BLOB-specific incremental I/O operations - UTF-8 only
** - harness_003: Virtual table creation, configuration, and operations - UTF-8 only
** - harness_004: Database backup operations between databases - UTF-8 only
** - harness_005: Database serialization/deserialization with WAL checkpoints - UTF-8 only
** - harness_006: Basic UTF-16 specific APIs (open16, prepare16, bind_text16, column_text16)
** - harness_007: [TBD]
** - harness_008: [TBD]
** - harness_009: [TBD]
** - harness_010: Advanced UTF-16 collation, result, and encoding operations (this harness)
**
** Advanced UTF-16 operation lifecycle:
**   Database setup → Register UTF-16 collation functions → 
**   Set collation needed callback → Execute queries using collations →
**   Test UTF-16 result text functions → Cleanup
**
** Key design considerations:
** 1. UTF-16 collation function implementation with proper encoding handling
** 2. Collation needed callback for dynamic collation registration
** 3. Different UTF-16 byte order handling (LE, BE, native)
** 4. Unicode character validation and surrogate pair handling
** 5. Proper memory management for UTF-16 strings
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

// UTF-16 collation comparison function
// Simple case-insensitive comparison for UTF-16 strings
static int utf16_collation_func(void* pArg, int n1, const void* s1, int n2, const void* s2) {
    (void)pArg; // Unused parameter
    
    const uint16_t* str1 = static_cast<const uint16_t*>(s1);
    const uint16_t* str2 = static_cast<const uint16_t*>(s2);
    
    // Handle NULL pointers or zero lengths
    if (str1 == nullptr || str2 == nullptr) {
        if (str1 == str2) return 0;
        return (str1 == nullptr) ? -1 : 1;
    }
    
    // Calculate actual lengths if not provided (null-terminated strings)
    if (n1 < 0) {
        n1 = 0;
        while (str1[n1] != 0) n1++;
        n1 *= sizeof(uint16_t);
    }
    if (n2 < 0) {
        n2 = 0;
        while (str2[n2] != 0) n2++;
        n2 *= sizeof(uint16_t);
    }
    
    // Compare byte by byte (simple binary comparison)
    int len1 = n1 / sizeof(uint16_t);
    int len2 = n2 / sizeof(uint16_t);
    int min_len = (len1 < len2) ? len1 : len2;
    
    for (int i = 0; i < min_len; i++) {
        uint16_t c1 = str1[i];
        uint16_t c2 = str2[i];
        
        // Simple case folding for ASCII
        if (c1 >= 'A' && c1 <= 'Z') c1 += ('a' - 'A');
        if (c2 >= 'A' && c2 <= 'Z') c2 += ('a' - 'A');
        
        if (c1 != c2) {
            return (c1 < c2) ? -1 : 1;
        }
    }
    
    // Strings are equal up to min_len
    if (len1 == len2) return 0;
    return (len1 < len2) ? -1 : 1;
}

// UTF-16 collation needed callback (for UTF-8 names)
static void collation_needed_callback(void* pArg, sqlite3* db, int eTextRep, const char* zName) {
    (void)pArg; // Unused parameter
    (void)db;   // Unused in this simple implementation
    (void)eTextRep; // Unused
    (void)zName; // Unused
    
    // In a real implementation, we would register the needed collation here
    // For fuzzing, we just record that the callback was invoked
}

// UTF-16 collation needed callback for UTF-16 names
static void collation_needed16_callback(void* pArg, sqlite3* db, int eTextRep, const void* zName) {
    (void)pArg; // Unused parameter
    
    // Try to register a simple collation for the requested name
    if (zName != nullptr) {
        // Register a simple binary collation
        sqlite3_create_collation16(db, zName, eTextRep, nullptr, utf16_collation_func);
    }
}

// Custom scalar function that returns UTF-16 text
static void utf16_result_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    (void)argc; // Unused
    
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(sqlite3_user_data(context));
    if (fdp == nullptr || fdp->remaining_bytes() < 10) {
        sqlite3_result_null(context);
        return;
    }
    
    // Generate UTF-16 string from fuzzer input
    auto [utf16_data, bytes] = generate_utf16_string(*fdp, 100);
    
    if (!utf16_data.empty()) {
        // Choose which UTF-16 result function to use based on fuzzer input
        uint8_t func_type = fdp->ConsumeIntegral<uint8_t>() % 4;
        
        switch (func_type) {
            case 0:
                // sqlite3_result_text16 - native byte order
                sqlite3_result_text16(context, utf16_data.data(), 
                                     static_cast<int>(bytes), SQLITE_TRANSIENT);
                break;
            case 1:
                // sqlite3_result_text16le - little endian
                sqlite3_result_text16le(context, utf16_data.data(),
                                       static_cast<int>(bytes), SQLITE_TRANSIENT);
                break;
            case 2:
                // sqlite3_result_text16be - big endian
                sqlite3_result_text16be(context, utf16_data.data(),
                                       static_cast<int>(bytes), SQLITE_TRANSIENT);
                break;
            case 3:
                // sqlite3_result_text16 with transient (alternative)
                sqlite3_result_text16(context, utf16_data.data(),
                                     static_cast<int>(bytes), SQLITE_TRANSIENT);
                break;
        }
    } else {
        sqlite3_result_null(context);
    }
}

// Operation types for the advanced UTF-16 harness
enum AdvancedUtf16Operation {
    UTF16_OP_COLLATION = 0,
    UTF16_OP_COLLATION_NEEDED = 1,
    UTF16_OP_RESULT_FUNCTIONS = 2,
    UTF16_OP_MIXED_ENCODINGS = 3,
    UTF16_OP_COMPREHENSIVE = 4
};

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum input size check
    const size_t MIN_SIZE = sizeof(int) * 8 + sizeof(size_t) * 4 + 256;
    if (size < MIN_SIZE) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    char* err_msg = nullptr;
    
    // Choose operation type
    AdvancedUtf16Operation operation = static_cast<AdvancedUtf16Operation>(
        fdp.ConsumeIntegralInRange<uint8_t>(0, 4));
    
    // Open in-memory database
    int rc = sqlite3_open_v2(":memory:", &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY, 
                            nullptr);
    
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set limits to prevent excessive resource usage
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, fdp.ConsumeIntegralInRange<int>(1000, 50000));
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, fdp.ConsumeIntegralInRange<int>(1000, 100000));
    
    // Create basic table for testing
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS utf16_advanced_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT COLLATE utf16_ci, "
        "description TEXT, "
        "value REAL);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert some test data
    if (fdp.remaining_bytes() > 100) {
        const char* insert_sql = 
            "INSERT INTO utf16_advanced_test (name, description, value) VALUES "
            "('Test1', 'Description 1', 1.1), "
            "('Test2', 'Description 2', 2.2), "
            "('Test3', 'Description 3', 3.3);";
        
        rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, &err_msg);
        if (rc != SQLITE_OK && err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
    }
    
    // Main advanced UTF-16 testing based on operation type
    switch (operation) {
        case UTF16_OP_COLLATION: {
            // Test UTF-16 collation creation with different encodings
            
            // Generate UTF-16 collation name
            auto [collation_name_utf16, name_bytes] = generate_utf16_string(fdp, 50);
            
            if (!collation_name_utf16.empty()) {
                // Choose encoding type
                int encoding = fdp.ConsumeIntegralInRange<int>(0, 3);
                int text_rep = SQLITE_UTF8;
                
                switch (encoding) {
                    case 0: text_rep = SQLITE_UTF16; break;
                    case 1: text_rep = SQLITE_UTF16LE; break;
                    case 2: text_rep = SQLITE_UTF16BE; break;
                    case 3: text_rep = SQLITE_UTF16_ALIGNED; break;
                }
                
                // Register UTF-16 collation
                rc = sqlite3_create_collation16(db, collation_name_utf16.data(),
                                               text_rep, nullptr, utf16_collation_func);
                
                // Test the collation in a query if registration succeeded
                if (rc == SQLITE_OK && fdp.remaining_bytes() > 50) {
                    // Create UTF-16 SQL query using the collation
                    std::string utf8_sql = "SELECT * FROM utf16_advanced_test WHERE name COLLATE ";
                    
                    // Convert collation name to UTF-8 for query (simplified)
                    std::string collation_name_utf8;
                    for (size_t i = 0; i < collation_name_utf16.size() && collation_name_utf16[i] != 0; i++) {
                        uint16_t ch = collation_name_utf16[i];
                        if (ch < 128) {
                            collation_name_utf8.push_back(static_cast<char>(ch));
                        }
                    }
                    
                    if (!collation_name_utf8.empty()) {
                        utf8_sql += collation_name_utf8 + " = 'Test1'";
                        
                        sqlite3_stmt* stmt = nullptr;
                        rc = sqlite3_prepare_v2(db, utf8_sql.c_str(), -1, &stmt, nullptr);
                        
                        if (rc == SQLITE_OK && stmt != nullptr) {
                            sqlite3_step(stmt);
                            sqlite3_finalize(stmt);
                        }
                    }
                }
                
                // Also test sqlite3_create_collation_v2 with UTF-16
                if (fdp.ConsumeBool() && fdp.remaining_bytes() > 100) {
                    auto [collation_name2_utf16, name2_bytes] = generate_utf16_string(fdp, 50);
                    
                    if (!collation_name2_utf16.empty()) {
                        // Simple destroy callback
                        auto destroy_func = [](void* p) { (void)p; };
                        
                        // Test with UTF-16 aligned encoding
                        rc = sqlite3_create_collation_v2(db, "test_collation_v2",
                                                        SQLITE_UTF16_ALIGNED,
                                                        nullptr,
                                                        utf16_collation_func,
                                                        destroy_func);
                    }
                }
            }
            break;
        }
        
        case UTF16_OP_COLLATION_NEEDED: {
            // Test collation needed callbacks
            
            // Set UTF-16 collation needed callback
            rc = sqlite3_collation_needed16(db, &fdp, collation_needed16_callback);
            
            // Also set UTF-8 collation needed callback for comparison
            if (fdp.ConsumeBool()) {
                sqlite3_collation_needed(db, &fdp, collation_needed_callback);
            }
            
            // Try to use a non-existent collation to trigger the callback
            if (rc == SQLITE_OK && fdp.remaining_bytes() > 100) {
                // Generate random collation name
                auto [missing_collation_name, missing_name_bytes] = generate_utf16_string(fdp, 30);
                
                if (!missing_collation_name.empty()) {
                    // Try to create a query with the missing collation
                    // We'll use UTF-8 query with collation name converted from UTF-16
                    std::string query = "SELECT * FROM utf16_advanced_test ORDER BY name COLLATE ";
                    
                    std::string collation_name_utf8;
                    for (size_t i = 0; i < missing_collation_name.size() && missing_collation_name[i] != 0; i++) {
                        uint16_t ch = missing_collation_name[i];
                        if (ch < 128) {
                            collation_name_utf8.push_back(static_cast<char>(ch));
                        } else {
                            collation_name_utf8.push_back('X'); // Placeholder
                        }
                    }
                    
                    if (!collation_name_utf8.empty()) {
                        query += collation_name_utf8;
                        
                        sqlite3_stmt* stmt = nullptr;
                        rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
                        
                        if (rc == SQLITE_OK && stmt != nullptr) {
                            sqlite3_step(stmt);
                            sqlite3_finalize(stmt);
                        }
                    }
                }
            }
            break;
        }
        
        case UTF16_OP_RESULT_FUNCTIONS: {
            // Test UTF-16 result text functions
            
            // Register a custom scalar function that returns UTF-16 text
            rc = sqlite3_create_function_v2(db, "utf16_func", 0, SQLITE_UTF8,
                                           &fdp, utf16_result_func,
                                           nullptr, nullptr, nullptr);
            
            // Also register with UTF-16 encoding
            if (fdp.ConsumeBool()) {
                sqlite3_create_function_v2(db, "utf16_func16", 0, SQLITE_UTF16,
                                          &fdp, utf16_result_func,
                                          nullptr, nullptr, nullptr);
            }
            
            // Test the function in a query
            if (rc == SQLITE_OK && fdp.remaining_bytes() > 50) {
                const char* test_sql = "SELECT utf16_func(), utf16_func() || ' suffix'";
                
                sqlite3_stmt* stmt = nullptr;
                rc = sqlite3_prepare_v2(db, test_sql, -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    rc = sqlite3_step(stmt);
                    if (rc == SQLITE_ROW) {
                        // Get the result as UTF-16 text
                        const void* utf16_text = sqlite3_column_text16(stmt, 0);
                        if (utf16_text) {
                            int bytes = sqlite3_column_bytes16(stmt, 0);
                            (void)bytes; // Use to avoid warning
                        }
                        
                        // Also test column_text16 on concatenated result
                        const void* concat_text = sqlite3_column_text16(stmt, 1);
                        if (concat_text) {
                            int concat_bytes = sqlite3_column_bytes16(stmt, 1);
                            (void)concat_bytes; // Use to avoid warning
                        }
                    }
                    sqlite3_finalize(stmt);
                }
            }
            
            // Test additional UTF-16 functionality
            if (fdp.remaining_bytes() > 100) {
                // Create another function that returns UTF-16 text with different encodings
                auto additional_utf16_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
                    (void)argc; (void)argv;
                    
                    // Generate simple UTF-16 data
                    uint16_t utf16_data[] = {'H', 'e', 'l', 'l', 'o', 0};
                    
                    // Use different encoding based on a simple condition
                    sqlite3_result_text16le(ctx, utf16_data, 10, SQLITE_TRANSIENT);
                };
                
                sqlite3_create_function_v2(db, "additional_utf16_test", 0, SQLITE_UTF8,
                                          nullptr, additional_utf16_func,
                                          nullptr, nullptr, nullptr);
                
                // Execute it
                const char* exec_sql = "SELECT additional_utf16_test()";
                rc = sqlite3_exec(db, exec_sql, nullptr, nullptr, &err_msg);
                if (rc != SQLITE_OK && err_msg) {
                    sqlite3_free(err_msg);
                    err_msg = nullptr;
                }
            }
        }
        
        case UTF16_OP_MIXED_ENCODINGS: {
            // Test mixing different UTF-16 encodings and UTF-8
            
            // Register collations with different encodings
            int encodings[] = {SQLITE_UTF16, SQLITE_UTF16LE, SQLITE_UTF16BE, SQLITE_UTF16_ALIGNED};
            
            for (int i = 0; i < 4 && fdp.remaining_bytes() > 50; i++) {
                std::string collation_name = "mixed_collation_" + std::to_string(i);
                std::vector<uint16_t> utf16_name = utf8_to_utf16(collation_name);
                
                if (!utf16_name.empty()) {
                    sqlite3_create_collation16(db, utf16_name.data(),
                                              encodings[i], nullptr, utf16_collation_func);
                }
            }
            
            // Create a query that uses mixed collations
            if (fdp.remaining_bytes() > 100) {
                // Complex query with ORDER BY using collation
                const char* mixed_sql = 
                    "SELECT name, description FROM utf16_advanced_test "
                    "WHERE name COLLATE NOCASE = 'test1' "
                    "ORDER BY description COLLATE BINARY";
                
                sqlite3_stmt* stmt = nullptr;
                rc = sqlite3_prepare_v2(db, mixed_sql, -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        // Get results in different encodings
                        const unsigned char* utf8_text = sqlite3_column_text(stmt, 0);
                        const void* utf16_text = sqlite3_column_text16(stmt, 1);
                        
                        (void)utf8_text;
                        (void)utf16_text;
                    }
                    sqlite3_finalize(stmt);
                }
            }
            
            // Test error handling with invalid UTF-16
            if (fdp.ConsumeBool()) {
                // Try to create collation with invalid UTF-16 data
                uint16_t invalid_utf16[] = {0xD800, 0x0041, 0}; // Lone high surrogate
                sqlite3_create_collation16(db, invalid_utf16, SQLITE_UTF16,
                                          nullptr, utf16_collation_func);
            }
            break;
        }
        
        case UTF16_OP_COMPREHENSIVE: {
            // Comprehensive test combining all features
            
            // 1. Register UTF-16 collation
            auto [comp_collation_name, comp_name_bytes] = generate_utf16_string(fdp, 30);
            if (!comp_collation_name.empty()) {
                sqlite3_create_collation16(db, comp_collation_name.data(),
                                          SQLITE_UTF16, nullptr, utf16_collation_func);
            }
            
            // 2. Set collation needed callback
            sqlite3_collation_needed16(db, &fdp, collation_needed16_callback);
            
            // 3. Register UTF-16 result function
            sqlite3_create_function_v2(db, "comprehensive_utf16_func", 0, SQLITE_UTF16,
                                      &fdp, utf16_result_func,
                                      nullptr, nullptr, nullptr);
            
            // 4. Create and execute comprehensive query
            if (fdp.remaining_bytes() > 200) {
                // Build dynamic query
                std::string comprehensive_sql = 
                    "SELECT comprehensive_utf16_func() as utf16_result, "
                    "name, description, value "
                    "FROM utf16_advanced_test "
                    "WHERE name LIKE '%Test%' ";
                
                // Add ORDER BY with collation if we have one
                if (!comp_collation_name.empty()) {
                    // Convert first few chars of UTF-16 name to UTF-8 for query
                    std::string order_collation;
                    for (size_t i = 0; i < 10 && i < comp_collation_name.size() && comp_collation_name[i] != 0; i++) {
                        uint16_t ch = comp_collation_name[i];
                        if (ch < 128) {
                            order_collation.push_back(static_cast<char>(ch));
                        }
                    }
                    
                    if (!order_collation.empty()) {
                        comprehensive_sql += "ORDER BY name COLLATE " + order_collation;
                    }
                }
                
                sqlite3_stmt* stmt = nullptr;
                rc = sqlite3_prepare_v2(db, comprehensive_sql.c_str(), -1, &stmt, nullptr);
                
                if (rc == SQLITE_OK && stmt != nullptr) {
                    rc = sqlite3_step(stmt);
                    while (rc == SQLITE_ROW) {
                        // Test all column access methods
                        for (int col = 0; col < sqlite3_column_count(stmt); col++) {
                            const void* utf16_val = sqlite3_column_text16(stmt, col);
                            const unsigned char* utf8_val = sqlite3_column_text(stmt, col);
                            int utf16_bytes = sqlite3_column_bytes16(stmt, col);
                            int utf8_bytes = sqlite3_column_bytes(stmt, col);
                            
                            (void)utf16_val;
                            (void)utf8_val;
                            (void)utf16_bytes;
                            (void)utf8_bytes;
                        }
                        rc = sqlite3_step(stmt);
                    }
                    sqlite3_finalize(stmt);
                }
            }
            
            // 5. Test error messages in UTF-16
            const void* utf16_err = sqlite3_errmsg16(db);
            (void)utf16_err;
            
            break;
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

} // namespace harness_010
