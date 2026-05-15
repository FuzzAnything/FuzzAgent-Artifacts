#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_018 {
/*
 * Fuzzing harness for SQLite library - targeting SQL statement completion validation APIs cluster
 * as identified by coverage analysis with 741+ undiscovered branches.
 * 
 * This harness specifically targets SQL Completion Validation APIs with high undiscovered branches:
 * - sqlite3_complete16 (Primary Target - UTF-16): 637 undiscovered branches, 0 hits
 * - sqlite3_complete (Secondary Target - UTF-8): 104 undiscovered branches, 0 hits
 * 
 * Supporting APIs for comprehensive testing:
 * - sqlite3_initialize / sqlite3_shutdown - Library initialization and cleanup
 * - sqlite3_malloc / sqlite3_free - Memory allocation for test strings
 * 
 * Invocation Sequence as specified by coverage guidance:
 * 1. Setup: sqlite3_initialize() for library initialization
 * 2. Test SQL Generation: Generate various SQL strings (complete/incomplete statements) from fuzzer input
 * 3. UTF-16 Testing: Convert test strings to UTF-16, call sqlite3_complete16() with various inputs
 * 4. UTF-8 Testing: Call sqlite3_complete() with UTF-8 strings
 * 5. Cleanup: sqlite3_shutdown() for cleanup
 * 
 * Test Scenarios derived from fuzzer input:
 * - Complete SQL statements (ending with semicolon)
 * - Incomplete SQL statements (missing semicolon)
 * - SQL with various syntax patterns
 * - Edge cases (NULL pointers, empty strings, very long strings)
 * - Strings with special characters and encoding variations
 * 
 * Error Path Coverage:
 * - NULL pointer inputs (handled via empty strings)
 * - Zero-length strings
 * - Strings with unterminated literals/comments
 * - Strings with mismatched parentheses/quotes
 * - Strings exceeding internal buffers
 * 
 * Semantic differentiation from existing harnesses (000-017):
 * - Specifically targets SQL statement completion validation (0% coverage currently)
 * - Focuses on UTF-16 encoding with sqlite3_complete16() 
 * - Tests edge cases for SQL syntax validation
 * - Different from harness_000 which focuses on prepared statements
 * - Different from harness_017 which focuses on SQL preparation APIs
 * - Provides comprehensive SQL validation testing independent of database operations
 * 
 * These functions represent 741+ undiscovered branches in the SQL validation cluster.
 */


// Simple UTF-8 to UTF-16 conversion helper
// This is a basic conversion for ASCII/compatible characters
static std::vector<uint16_t> utf8_to_utf16(const std::string& utf8_str) {
    std::vector<uint16_t> utf16_str;
    utf16_str.reserve(utf8_str.size() + 1); // Reserve space plus null terminator
    
    for (size_t i = 0; i < utf8_str.size(); i++) {
        unsigned char c = utf8_str[i];
        
        // Simple ASCII conversion (valid for basic SQL statements)
        if (c < 0x80) {
            utf16_str.push_back(static_cast<uint16_t>(c));
        } else {
            // For non-ASCII, use replacement character
            utf16_str.push_back(0xFFFD); // Unicode replacement character
        }
    }
    
    // Add null terminator for UTF-16 string
    utf16_str.push_back(0);
    return utf16_str;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // We need enough for: operation type, SQL strings, edge case testing
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Step 1: Consume operation type from fuzzer input
    // 0 = Test sqlite3_complete16 with UTF-16 strings
    // 1 = Test sqlite3_complete with UTF-8 strings  
    // 2 = Test both functions with various SQL statements
    // 3 = Test error paths (NULL pointers, empty strings, etc.)
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 4;
    
    // Step 2: Generate SQL test strings from fuzzer input
    std::vector<std::string> test_sql_statements;
    
    // Consume number of SQL test cases to generate (1-8)
    size_t num_test_cases = fdp.ConsumeIntegralInRange<size_t>(1, 8);
    
    for (size_t i = 0; i < num_test_cases; i++) {
        // Consume SQL string length from fuzzer input
        size_t max_sql_len = std::min(fdp.remaining_bytes(), (size_t)4096);
        if (max_sql_len == 0) break;
        
        size_t sql_len = fdp.ConsumeIntegralInRange<size_t>(1, max_sql_len);
        std::string sql = fdp.ConsumeBytesAsString(sql_len);
        
        // Based on operation type, optionally modify the SQL string
        if (operation == 0) {
            // For UTF-16 testing, ensure we have some complete statements
            // Randomly add semicolon to make some statements complete
            if (fdp.ConsumeBool() && !sql.empty()) {
                sql += ";";
            }
        } else if (operation == 1) {
            // For UTF-8 testing, ensure we have some incomplete statements
            // Randomly remove trailing semicolon if present
            if (!sql.empty() && sql.back() == ';' && fdp.ConsumeBool()) {
                sql.pop_back();
            }
        } else if (operation == 2) {
            // Mixed testing - randomly add or remove semicolon
            if (!sql.empty()) {
                if (sql.back() == ';' && fdp.ConsumeBool()) {
                    sql.pop_back();  // Remove semicolon
                } else if (fdp.ConsumeBool()) {
                    sql += ";";  // Add semicolon
                }
            }
        } else if (operation == 3) {
            // Error path testing - modify SQL to create edge cases
            if (!sql.empty()) {
                uint8_t error_type = fdp.ConsumeIntegral<uint8_t>() % 8;
                switch (error_type) {
                    case 0:
                        // Empty string (already handled)
                        break;
                    case 1:
                        // Just semicolon
                        sql = ";";
                        break;
                    case 2:
                        // Add unterminated string literal using fuzzer input
                        if (fdp.remaining_bytes() > 10 && sql.find('\'') == std::string::npos) {
                            size_t literal_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)50));
                            std::string literal = fdp.ConsumeBytesAsString(literal_len);
                            sql += " '" + literal;  // No closing quote
                        }
                        break;
                    case 3:
                        // Add unterminated comment using fuzzer input
                        if (fdp.remaining_bytes() > 10 && sql.find("/*") == std::string::npos) {
                            size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)50));
                            std::string comment = fdp.ConsumeBytesAsString(comment_len);
                            sql += " /* " + comment;  // No closing comment
                        }
                        break;
                    case 4:
                        // Add unmatched parenthesis
                        sql += " (";
                        if (fdp.remaining_bytes() > 10) {
                            size_t paren_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)50));
                            std::string paren_content = fdp.ConsumeBytesAsString(paren_len);
                            sql += paren_content;
                        }
                        break;
                    case 5:
                        // CREATE TRIGGER special case using fuzzer input
                        if (fdp.remaining_bytes() > 10) {
                            size_t trigger_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)100));
                            std::string trigger_body = fdp.ConsumeBytesAsString(trigger_len);
                            sql = "CREATE TRIGGER test BEGIN " + trigger_body + ";";
                        }
                        break;
                    case 6:
                        // Add double quotes using fuzzer input
                        if (fdp.remaining_bytes() > 10) {
                            size_t quote_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)50));
                            std::string quote_content = fdp.ConsumeBytesAsString(quote_len);
                            sql += " \"" + quote_content + "\"";
                        }
                        break;
                    case 7:
                        // Add backticks using fuzzer input
                        if (fdp.remaining_bytes() > 10) {
                            size_t backtick_len = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)50));
                            std::string backtick_content = fdp.ConsumeBytesAsString(backtick_len);
                            sql += " `" + backtick_content + "`";
                        }
                        break;
                }
            }
        }
        
        test_sql_statements.push_back(sql);
    }
    
    // Step 3: Test sqlite3_complete16 (UTF-16)
    if (operation == 0 || operation == 2) {
        for (const auto& sql : test_sql_statements) {
            // Convert to UTF-16
            std::vector<uint16_t> utf16_sql = utf8_to_utf16(sql);
            
            // Test with valid UTF-16 string
            int result = sqlite3_complete16(utf16_sql.data());
            // Don't check result - we want to test all return codes
            
            // Test edge cases
            if (!sql.empty()) {
                // Test with NULL pointer (should handle gracefully)
                // sqlite3_complete16(NULL); // This might crash, so we need to be careful
                // Instead test with empty string
                std::vector<uint16_t> empty_utf16 = {0};
                result = sqlite3_complete16(empty_utf16.data());
                
                // Test with very long string (if we have enough input)
                if (fdp.remaining_bytes() > 100) {
                    size_t long_len = fdp.ConsumeIntegralInRange<size_t>(100, std::min(fdp.remaining_bytes(), (size_t)10000));
                    std::string long_sql = fdp.ConsumeBytesAsString(long_len);
                    std::vector<uint16_t> long_utf16 = utf8_to_utf16(long_sql);
                    result = sqlite3_complete16(long_utf16.data());
                }
            }
        }
    }
    
    // Step 4: Test sqlite3_complete (UTF-8)
    if (operation == 1 || operation == 2 || operation == 3) {
        for (const auto& sql : test_sql_statements) {
            // Test with valid UTF-8 string
            int result = sqlite3_complete(sql.c_str());
            // Don't check result - we want to test all return codes
            
            // Test edge cases for UTF-8
            if (!sql.empty() && fdp.remaining_bytes() > 10) {
                // Test with empty string
                result = sqlite3_complete("");
                
                // Test with string containing only null terminator
                const char* null_str = "\0";
                result = sqlite3_complete(null_str);
                
                // Generate comment SQL from fuzzer input
                if (fdp.remaining_bytes() > 20) {
                    size_t comment_len = fdp.ConsumeIntegralInRange<size_t>(5, std::min(fdp.remaining_bytes(), (size_t)100));
                    std::string comment_sql = fdp.ConsumeBytesAsString(comment_len);
                    comment_sql += "; -- comment";
                    result = sqlite3_complete(comment_sql.c_str());
                }
                
                // Generate block comment SQL from fuzzer input
                if (fdp.remaining_bytes() > 20) {
                    size_t block_len = fdp.ConsumeIntegralInRange<size_t>(5, std::min(fdp.remaining_bytes(), (size_t)100));
                    std::string block_sql = fdp.ConsumeBytesAsString(block_len);
                    block_sql = "SELECT /* " + block_sql + " */ 1;";
                    result = sqlite3_complete(block_sql.c_str());
                }
                
                // Test CREATE TRIGGER special case with fuzzer input
                if (fdp.remaining_bytes() > 20) {
                    size_t trigger_len = fdp.ConsumeIntegralInRange<size_t>(5, std::min(fdp.remaining_bytes(), (size_t)100));
                    std::string trigger_body = fdp.ConsumeBytesAsString(trigger_len);
                    std::string trigger_sql = "CREATE TRIGGER test BEGIN " + trigger_body + ";";
                    result = sqlite3_complete(trigger_sql.c_str());
                }
            }
        }
    }
    
    // Step 5: Test error path combinations
    if (operation == 3) {
        // Test with SQLite allocated memory
        char* sqlite_allocated = nullptr;
        if (fdp.remaining_bytes() > 10) {
            size_t alloc_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)100));
            sqlite_allocated = (char*)sqlite3_malloc(alloc_size);
            if (sqlite_allocated != nullptr) {
                // Fill with some data from fuzzer
                size_t fill_size = std::min(alloc_size - 1, fdp.remaining_bytes());
                if (fill_size > 0) {
                    std::vector<uint8_t> fill_data = fdp.ConsumeBytes<uint8_t>(fill_size);
                    memcpy(sqlite_allocated, fill_data.data(), fill_size);
                    sqlite_allocated[fill_size] = '\0';
                    
                    // Test with sqlite3_complete
                    int result = sqlite3_complete(sqlite_allocated);
                    
                    // Convert to UTF-16 and test
                    std::vector<uint16_t> utf16_allocated = utf8_to_utf16(std::string(sqlite_allocated));
                    result = sqlite3_complete16(utf16_allocated.data());
                }
                sqlite3_free(sqlite_allocated);
            }
        }
        
        // Test with strings containing special characters
        if (fdp.remaining_bytes() > 20) {
            std::string special_sql = fdp.ConsumeRandomLengthString(100);
            // Inject various special characters
            special_sql += ";";
            int result = sqlite3_complete(special_sql.c_str());
            
            // Test UTF-16 version
            std::vector<uint16_t> special_utf16 = utf8_to_utf16(special_sql);
            result = sqlite3_complete16(special_utf16.data());
        }
    }
    
    // Step 6: Cleanup
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_018
