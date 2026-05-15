#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include "sqlite3.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace harness_003 {
/*
 * Fuzzing harness for SQLite user-defined functions and collation sequences.
 * Targets advanced APIs not covered in previous harnesses:
 * - sqlite3_create_function (user-defined scalar functions)
 * - sqlite3_create_collation (custom collation sequences)
 * 
 * This complements harness_000 (basic SQL), harness_001 (binding/column APIs),
 * and harness_002 (transaction/schema/blob) by targeting function/collation APIs.
 */



/*
 * Simple scalar function for testing.
 */
static void test_scalar_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        // Return the first argument
        sqlite3_result_value(context, argv[0]);
    } else {
        sqlite3_result_null(context);
    }
}

/*
 * Simple collation function.
 */
static int test_collation(void* pArg, int n1, const void* p1, int n2, const void* p2) {
    // Simple binary collation
    int len = n1 < n2 ? n1 : n2;
    int cmp = memcmp(p1, p2, len);
    if (cmp == 0) {
        return n1 - n2;
    }
    return cmp;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Minimum size check
    if (size < 8) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Open in-memory database (use simple open for compatibility)
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return 0;
    }
    
    // Test user-defined functions
    std::string func_name = fdp.ConsumeRandomLengthString(32);
    if (func_name.empty()) {
        func_name = "test_func";
    }
    
    // Register user-defined scalar function
    rc = sqlite3_create_function(
        db,
        func_name.c_str(),
        fdp.ConsumeIntegralInRange<int>(-1, 5),  // -1 means variable arguments
        SQLITE_UTF8,
        nullptr,
        test_scalar_func,
        nullptr,
        nullptr
    );
    
    // Test the function if registration succeeded
    if (rc == SQLITE_OK) {
        // Create a test table
        const char* create_sql = "CREATE TABLE test_data(id INTEGER, value TEXT);";
        char* err_msg = nullptr;
        sqlite3_exec(db, create_sql, nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Insert some test data using fuzzer input
        std::string test_value = fdp.ConsumeRandomLengthString(64);
        if (!test_value.empty()) {
            std::string insert_sql = "INSERT INTO test_data VALUES(1, '" + test_value + "');";
            sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Use the registered function
        std::string select_sql = "SELECT " + func_name + "(value) FROM test_data;";
        sqlite3_exec(db, select_sql.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Clean up
        sqlite3_exec(db, "DROP TABLE test_data;", nullptr, nullptr, nullptr);
    }
    
    // Test collation sequences
    std::string collation_name = fdp.ConsumeRandomLengthString(32);
    if (collation_name.empty()) {
        collation_name = "test_collation";
    }
    
    rc = sqlite3_create_collation(
        db,
        collation_name.c_str(),
        SQLITE_UTF8,
        nullptr,
        test_collation
    );
    
    if (rc == SQLITE_OK) {
        // Create table with custom collation
        char* err_msg = nullptr;
        std::string create_sql = "CREATE TABLE collation_test(text_val TEXT COLLATE " + 
                                collation_name + ");";
        sqlite3_exec(db, create_sql.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Insert test data using fuzzer input
        std::string text1 = fdp.ConsumeRandomLengthString(128);
        if (!text1.empty()) {
            std::string insert_sql = "INSERT INTO collation_test VALUES('" + text1 + "');";
            sqlite3_exec(db, insert_sql.c_str(), nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = nullptr;
            }
        }
        
        // Test ordering with custom collation
        std::string order_sql = "SELECT text_val FROM collation_test ORDER BY text_val COLLATE " + 
                               collation_name + ";";
        sqlite3_exec(db, order_sql.c_str(), nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = nullptr;
        }
        
        // Clean up
        sqlite3_exec(db, "DROP TABLE collation_test;", nullptr, nullptr, nullptr);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}

} // namespace harness_003
