#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_003 {
/*
 * Fuzzing harness for SQLite UTF-16/Unicode APIs
 * Targets: sqlite3_open16, sqlite3_prepare16_v2, sqlite3_prepare16_v3, 
 *          sqlite3_bind_text16, sqlite3_column_text16, sqlite3_column_name16,
 *          sqlite3_value_text16, sqlite3_value_text16le, sqlite3_value_text16be,
 *          sqlite3_errmsg16, sqlite3_create_collation16, sqlite3_complete16
 * 
 * This harness specifically targets UTF-16/Unicode APIs which have 500+ 
 * accumulated branches across 20+ APIs at 0% coverage according to coverage analysis.
 * Implementation follows exact sequence: 1) sqlite3_initialize, 2) sqlite3_open16 
 * with UTF-16 filename, 3) sqlite3_prepare16_v2 with UTF-16 SQL string, 
 * 4) sqlite3_bind_text16 with UTF-16 parameters, 5) sqlite3_step, 
 * 6) sqlite3_column_text16/column_name16 for results, 7) sqlite3_value_text16 
 * family via custom functions, 8) sqlite3_errmsg16 for errors, 
 * 9) sqlite3_finalize, sqlite3_close, sqlite3_shutdown.
 * Includes collation testing with sqlite3_create_collation16.
 * 
 * Semantic differentiation from existing harnesses:
 * - harness_000: Core UTF-8 APIs
 * - harness_001: Database file parsing  
 * - harness_002: BLOB operations
 * - harness_003: UTF-16/Unicode specific APIs
 */



// Custom collation function for UTF-16 collation testing
static int utf16_collation_func(void* pArg, int n1, const void* p1, int n2, const void* p2) {
    // Simple UTF-16 collation: compare raw UTF-16 data
    int min_len = n1 < n2 ? n1 : n2;
    int cmp = memcmp(p1, p2, min_len);
    if (cmp != 0) return cmp;
    return n1 - n2;
}

// Custom scalar function to test sqlite3_value_text16 family
static void utf16_value_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc == 1) {
        const void* text16 = sqlite3_value_text16(argv[0]);
        const void* text16le = sqlite3_value_text16le(argv[0]);
        const void* text16be = sqlite3_value_text16be(argv[0]);
        
        // Return the length of UTF-16 string as result
        int len = sqlite3_value_bytes16(argv[0]);
        sqlite3_result_int(context, len);
    } else {
        sqlite3_result_null(context);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size needed for meaningful UTF-16 testing
    // Need enough for UTF-16 strings (2 bytes per char minimum)
    if (size < 64) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume configuration parameters from fuzzer input
    bool use_prepare16_v3 = fdp.ConsumeBool();
    bool enable_collation = fdp.ConsumeBool();
    bool use_custom_func = fdp.ConsumeBool();
    bool test_error_paths = fdp.ConsumeBool();
    int max_utf16_str_len = fdp.ConsumeIntegralInRange<int>(1, 512);
    int num_bind_params = fdp.ConsumeIntegralInRange<int>(0, 10);
    
    // Initialize SQLite library
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    
    // Generate UTF-16 filename from fuzzer input
    std::string utf8_filename = fdp.ConsumeRandomLengthString(max_utf16_str_len);
    // Convert to UTF-16 (simplified - just duplicate bytes for testing)
    // In real fuzzing, we would need proper UTF-8 to UTF-16 conversion
    std::vector<uint16_t> utf16_filename;
    for (size_t i = 0; i < utf8_filename.size(); i++) {
        utf16_filename.push_back(static_cast<uint8_t>(utf8_filename[i]));
    }
    // Add null terminator for UTF-16
    utf16_filename.push_back(0);
    
    // Step 1: Open database with sqlite3_open16 using UTF-16 filename
    int rc = sqlite3_open16(utf16_filename.data(), &db);
    if (rc != SQLITE_OK || db == nullptr) {
        // Try with in-memory database as fallback
        const void* memory_db_name = u":memory:";
        rc = sqlite3_open16(memory_db_name, &db);
        if (rc != SQLITE_OK || db == nullptr) {
            sqlite3_shutdown();
            return 0;
        }
    }
    
    // Set safety limits
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
    sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 50);
    
    // Register custom function if enabled
    if (use_custom_func) {
        rc = sqlite3_create_function(db, "utf16_len", 1, SQLITE_UTF16, nullptr, 
                                      utf16_value_func, nullptr, nullptr);
    }
    // Register UTF-16 collation if enabled
    if (enable_collation) {
        static const char16_t utf16_collation_name[] = u"UTF16_CUSTOM";
        rc = sqlite3_create_collation16(db, (const void*)utf16_collation_name, SQLITE_UTF16, nullptr, 
                                         utf16_collation_func);
    }
    
    // Create test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS utf16_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value TEXT);";
    
    char* err_msg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg != nullptr) {
        sqlite3_free(err_msg);
    }
    
    // Clear any existing data
    sqlite3_exec(db, "DELETE FROM utf16_test;", nullptr, nullptr, nullptr);
    
    // Generate UTF-16 SQL statement from fuzzer input
    std::string utf8_sql_base = "INSERT INTO utf16_test (id, name, value) VALUES (?, ?, ?)";
    std::vector<uint16_t> utf16_sql;
    for (size_t i = 0; i < utf8_sql_base.size(); i++) {
        utf16_sql.push_back(static_cast<uint8_t>(utf8_sql_base[i]));
    }
    utf16_sql.push_back(0);  // Null terminator
    
    sqlite3_stmt* stmt = nullptr;
    const void* tail = nullptr;
    
    // Step 2: Prepare statement with UTF-16 SQL
    if (use_prepare16_v3) {
        // Use sqlite3_prepare16_v3 with prepFlags
        unsigned int prepFlags = fdp.ConsumeIntegral<unsigned int>();
        rc = sqlite3_prepare16_v3(db, utf16_sql.data(), -1, prepFlags, &stmt, &tail);
    } else {
        // Use sqlite3_prepare16_v2
        rc = sqlite3_prepare16_v2(db, utf16_sql.data(), -1, &stmt, &tail);
    }
    
    if (rc != SQLITE_OK || stmt == nullptr) {
        // Test sqlite3_complete16 on partial input
        if (fdp.remaining_bytes() > 0) {
            std::string partial_sql = fdp.ConsumeRandomLengthString(20);
            std::vector<uint16_t> partial_utf16;
            for (char c : partial_sql) {
                partial_utf16.push_back(static_cast<uint8_t>(c));
            }
            partial_utf16.push_back(0);
            int complete = sqlite3_complete16(partial_utf16.data());
        }
        
        // Test error message retrieval
        if (test_error_paths) {
            const void* errmsg = sqlite3_errmsg16(db);
            // Just access the error message, don't use it
            (void)errmsg;
        }
        
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 3: Bind UTF-16 parameters
    for (int i = 1; i <= num_bind_params && i <= 3 && fdp.remaining_bytes() > 0; i++) {
        std::string param_str = fdp.ConsumeRandomLengthString(max_utf16_str_len);
        std::vector<uint16_t> utf16_param;
        for (char c : param_str) {
            utf16_param.push_back(static_cast<uint8_t>(c));
        }
        utf16_param.push_back(0);
        
        // Use SQLITE_TRANSIENT so SQLite makes its own copy
        rc = sqlite3_bind_text16(stmt, i, utf16_param.data(), -1, SQLITE_TRANSIENT);
    }
    
    // Bind remaining parameters if we didn't bind all 3
    for (int i = num_bind_params + 1; i <= 3; i++) {
        if (i == 1) {
            sqlite3_bind_int(stmt, i, fdp.ConsumeIntegral<int>());
        } else {
            // Use remaining fuzzer input for default values
            if (fdp.remaining_bytes() > 0) {
                std::string param_str = fdp.ConsumeRandomLengthString(max_utf16_str_len);
                std::vector<uint16_t> param_utf16;
                for (char c : param_str) {
                    param_utf16.push_back(static_cast<uint8_t>(c));
                }
                param_utf16.push_back(0);
                sqlite3_bind_text16(stmt, i, param_utf16.data(), -1, SQLITE_TRANSIENT);
            } else {
                // If no input left, bind null
                sqlite3_bind_null(stmt, i);
            }
        }
    }
    
    // Step 4: Execute the statement
    rc = sqlite3_step(stmt);
    
    // Reset statement for potential reuse or to test finalize
    sqlite3_reset(stmt);
    
    // Now test SELECT to exercise column text APIs
    std::string select_sql = "SELECT id, name, value FROM utf16_test";
    std::vector<uint16_t> utf16_select;
    for (char c : select_sql) {
        utf16_select.push_back(static_cast<uint8_t>(c));
    }
    utf16_select.push_back(0);
    
    sqlite3_stmt* select_stmt = nullptr;
    rc = sqlite3_prepare16_v2(db, utf16_select.data(), -1, &select_stmt, nullptr);
    
    if (rc == SQLITE_OK && select_stmt != nullptr) {
        rc = sqlite3_step(select_stmt);
        if (rc == SQLITE_ROW) {
            // Step 5: Test column text and name APIs
            int col_count = sqlite3_column_count(select_stmt);
            for (int i = 0; i < col_count; i++) {
                // Test sqlite3_column_name16
                const void* col_name = sqlite3_column_name16(select_stmt, i);
                (void)col_name;  // Use to avoid unused variable warning
                
                // Test sqlite3_column_text16 based on column type
                int col_type = sqlite3_column_type(select_stmt, i);
                if (col_type == SQLITE_TEXT) {
                    const void* col_text = sqlite3_column_text16(select_stmt, i);
                    (void)col_text;
                    int bytes16 = sqlite3_column_bytes16(select_stmt, i);
                    (void)bytes16;
                }
            }
        }
        sqlite3_finalize(select_stmt);
    }
    
    // Test custom function if registered
    if (use_custom_func) {
        std::string func_sql = "SELECT utf16_len(name) FROM utf16_test";
        std::vector<uint16_t> utf16_func;
        for (char c : func_sql) {
            utf16_func.push_back(static_cast<uint8_t>(c));
        }
        utf16_func.push_back(0);
        
        sqlite3_stmt* func_stmt = nullptr;
        rc = sqlite3_prepare16_v2(db, utf16_func.data(), -1, &func_stmt, nullptr);
        if (rc == SQLITE_OK && func_stmt != nullptr) {
            sqlite3_step(func_stmt);
            sqlite3_finalize(func_stmt);
        }
    }
    
    // Test error paths if enabled
    if (test_error_paths) {
        // Intentionally create an error by preparing invalid SQL
        std::string invalid_sql = "INVALID SQL STATEMENT";
        std::vector<uint16_t> utf16_invalid;
        for (char c : invalid_sql) {
            utf16_invalid.push_back(static_cast<uint8_t>(c));
        }
        utf16_invalid.push_back(0);
        
        sqlite3_stmt* invalid_stmt = nullptr;
        rc = sqlite3_prepare16_v2(db, utf16_invalid.data(), -1, &invalid_stmt, nullptr);
        if (rc != SQLITE_OK) {
            const void* errmsg = sqlite3_errmsg16(db);
            (void)errmsg;
        }
        if (invalid_stmt != nullptr) {
            sqlite3_finalize(invalid_stmt);
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_003
