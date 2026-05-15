#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_012 {
/*
 * Fuzzing harness for SQLite library - targeting SQLite parameter binding operations
 * with 64-bit and special data types as identified by coverage analysis.
 * 
 * This harness targets SQLite parameter binding APIs with focus on 64-bit variants
 * and special data types:
 * - sqlite3_bind_int64 - bind 64-bit integers
 * - sqlite3_bind_double - bind floating-point values
 * - sqlite3_bind_blob64 - bind large BLOBs (up to 2^64 bytes)
 * - sqlite3_bind_text64 - bind large text (up to 2^64 bytes) with encoding
 * - sqlite3_bind_zeroblob64 - bind zero-filled BLOBs of specified size
 * - sqlite3_bind_pointer - bind pointer values with type information
 * - sqlite3_bind_null - bind NULL values
 * 
 * Plus parameter management functions:
 * - sqlite3_bind_parameter_count - get number of parameters in statement
 * - sqlite3_bind_parameter_index - get index of named parameter
 * - sqlite3_bind_parameter_name - get name of parameter at index
 * 
 * Following the invocation sequence:
 * 1. Initialize SQLite and open in-memory database
 * 2. Create test table with various column types
 * 3. Prepare parameterized INSERT statement with multiple parameters
 * 4. Test parameter management functions (count, index, name)
 * 5. Bind different data types using 64-bit and special binding APIs
 * 6. Execute statement and verify binding
 * 7. Test edge cases: out-of-range indices, large values, special encodings
 * 8. Clean up resources
 * 
 * Semantic differentiation from existing harnesses (000-011):
 * - Focuses specifically on 64-bit binding variants (blob64, text64, zeroblob64)
 * - Tests pointer binding with destructor callbacks
 * - Exercises parameter management functions not covered in other harnesses
 * - Tests SQLITE_TRANSIENT and custom destructors (SQLITE_STATIC not used with temporary data for safety)
 * - Tests different text encodings (UTF-8, UTF-16LE, UTF-16BE) with text64
 * - Targets 460+ accumulated undiscovered branches with 0% coverage
 */


// Destructor callback for pointer binding tests
static void pointer_destructor(void* ptr) {
    // Free allocated integer memory
    delete static_cast<int*>(ptr);
}

// Custom destructor for testing SQLITE_TRANSIENT and custom destructors
static void custom_destructor(void* ptr) {
    // Free allocated character array memory
    delete[] static_cast<char*>(ptr);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing
    // Minimum: operation type + basic parameters
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize SQLite
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
    
    // Set limits to prevent excessive resource usage
    sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 100000);
    sqlite3_limit(db, SQLITE_LIMIT_VDBE_OP, 50000);
    
    // Step 2: Create test table with various column types
    const char* create_table_sql = 
        "CREATE TABLE bind_test ("
        "  id INTEGER PRIMARY KEY,"
        "  int_val INTEGER,"
        "  int64_val INTEGER,"
        "  real_val REAL,"
        "  text_val TEXT,"
        "  blob_val BLOB,"
        "  null_val ANY"
        ");";
    
    char* errmsg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Step 3: Prepare parameterized INSERT statement
    // Use named parameters to test parameter_index and parameter_name functions
    const char* insert_sql = 
        "INSERT INTO bind_test (int_val, int64_val, real_val, text_val, blob_val, null_val) "
        "VALUES (:int, :int64, :real, :text, :blob, :null);";
    
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, &tail);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Step 4: Test parameter management functions
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Test parameter_index for named parameters
    int idx_int = sqlite3_bind_parameter_index(stmt, ":int");
    int idx_int64 = sqlite3_bind_parameter_index(stmt, ":int64");
    int idx_real = sqlite3_bind_parameter_index(stmt, ":real");
    int idx_text = sqlite3_bind_parameter_index(stmt, ":text");
    int idx_blob = sqlite3_bind_parameter_index(stmt, ":blob");
    int idx_null = sqlite3_bind_parameter_index(stmt, ":null");
    
    // Test parameter_name for each index
    const char* name1 = sqlite3_bind_parameter_name(stmt, 1);
    const char* name2 = sqlite3_bind_parameter_name(stmt, 2);
    const char* name3 = sqlite3_bind_parameter_name(stmt, 3);
    const char* name4 = sqlite3_bind_parameter_name(stmt, 4);
    const char* name5 = sqlite3_bind_parameter_name(stmt, 5);
    const char* name6 = sqlite3_bind_parameter_name(stmt, 6);
    
    // Also test out-of-range indices
    const char* name_out_of_range = sqlite3_bind_parameter_name(stmt, 0);
    const char* name_too_large = sqlite3_bind_parameter_name(stmt, param_count + 1);
    
    // Step 5: Bind different data types using fuzzer input
    
    // Consume operation type to determine binding strategy
    uint8_t operation = fdp.ConsumeIntegral<uint8_t>() % 8;
    
    // Bind parameters based on operation type
    switch (operation) {
        case 0: {
            // Test basic bindings with various data types
            
            // Bind 32-bit integer
            int int_val = fdp.ConsumeIntegral<int>();
            rc = sqlite3_bind_int(stmt, idx_int, int_val);
            
            // Bind 64-bit integer
            sqlite3_int64 int64_val = fdp.ConsumeIntegral<sqlite3_int64>();
            rc = sqlite3_bind_int64(stmt, idx_int64, int64_val);
            
            // Bind double value
            double double_val = fdp.ConsumeFloatingPoint<double>();
            rc = sqlite3_bind_double(stmt, idx_real, double_val);
            
            // Bind text (regular text binding)
            std::string text_val = fdp.ConsumeRandomLengthString(256);
            rc = sqlite3_bind_text(stmt, idx_text, text_val.c_str(), -1, SQLITE_TRANSIENT);
            // Bind blob (regular blob binding)
            if (fdp.remaining_bytes() < 1) break;
            size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)1024));
            std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
            rc = sqlite3_bind_blob(stmt, idx_blob, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
            // Bind NULL
            rc = sqlite3_bind_null(stmt, idx_null);
            
            break;
        }
        
        case 1: {
            // Test 64-bit binding variants
            
            // Bind large 64-bit integer
            sqlite3_int64 large_int64 = fdp.ConsumeIntegral<sqlite3_int64>();
            rc = sqlite3_bind_int64(stmt, idx_int64, large_int64);
            
            // Bind text64 with UTF-8 encoding
            std::string text64_val = fdp.ConsumeRandomLengthString(512);
            rc = sqlite3_bind_text64(stmt, idx_text, text64_val.c_str(), 
                                     text64_val.size(), SQLITE_TRANSIENT, SQLITE_UTF8);
            
            // Bind blob64
            if (fdp.remaining_bytes() < 1) break;
            size_t blob64_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)2048));
            std::vector<uint8_t> blob64_data = fdp.ConsumeBytes<uint8_t>(blob64_size);
            rc = sqlite3_bind_blob64(stmt, idx_blob, blob64_data.data(), 
                                     blob64_data.size(), SQLITE_TRANSIENT);
            
            // Bind zeroblob64
            sqlite3_uint64 zeroblob_size = fdp.ConsumeIntegralInRange<sqlite3_uint64>(0, 1024);
            rc = sqlite3_bind_zeroblob64(stmt, idx_blob, zeroblob_size);
            
            break;
        }
        
        case 2: {
            // Test pointer binding and special destructors
            
            // Create test data for pointer binding
            int* test_ptr = new int(fdp.ConsumeIntegral<int>());
            
            // Bind pointer with destructor
            rc = sqlite3_bind_pointer(stmt, idx_int, test_ptr, "int*", pointer_destructor);
            
            // Bind NULL pointer
            rc = sqlite3_bind_pointer(stmt, idx_null, nullptr, "void*", nullptr);
            
            // Test SQLITE_TRANSIENT (SQLite makes a copy of the data)
            std::string static_text = fdp.ConsumeRandomLengthString(128);
            rc = sqlite3_bind_text(stmt, idx_text, static_text.c_str(), -1, SQLITE_TRANSIENT);
            
            // Test custom destructor
            char* custom_data = new char[64];
            std::memset(custom_data, 0, 64);
            rc = sqlite3_bind_blob(stmt, idx_blob, custom_data, 64, custom_destructor);
            
            // Clean up pointer (destructor should be called by SQLite)
            // Note: SQLite will call the destructor when done
            break;
        }
        
        case 3: {
            // Test different text encodings with text64
            
            // UTF-8 encoding (default)
            std::string utf8_text = fdp.ConsumeRandomLengthString(256);
            rc = sqlite3_bind_text64(stmt, idx_text, utf8_text.c_str(), 
                                     utf8_text.size(), SQLITE_TRANSIENT, SQLITE_UTF8);
            
            // UTF-16LE encoding
            rc = sqlite3_bind_text64(stmt, idx_text, utf8_text.c_str(), 
                                     utf8_text.size(), SQLITE_TRANSIENT, SQLITE_UTF16LE);
            
            // UTF-16BE encoding
            rc = sqlite3_bind_text64(stmt, idx_text, utf8_text.c_str(), 
                                     utf8_text.size(), SQLITE_TRANSIENT, SQLITE_UTF16BE);
            
            // UTF-16 native
            rc = sqlite3_bind_text64(stmt, idx_text, utf8_text.c_str(), 
                                     utf8_text.size(), SQLITE_TRANSIENT, SQLITE_UTF16);
            
            break;
        }
        
        case 4: {
            // Test edge cases: out-of-range indices, large values
            
            // Test binding with out-of-range index (should return SQLITE_RANGE)
            int test_val1 = fdp.ConsumeIntegral<int>();
            rc = sqlite3_bind_int(stmt, 0, test_val1);  // Index 0 is invalid
            int test_val2 = fdp.ConsumeIntegral<int>();
            rc = sqlite3_bind_int(stmt, param_count + 1, test_val2);  // Index beyond count
            // Test binding with negative size (for text/blob)
            std::string text = fdp.ConsumeRandomLengthString(100);
            rc = sqlite3_bind_text(stmt, idx_text, text.c_str(), -1, SQLITE_TRANSIENT);
            
            // Test zeroblob with zero size
            rc = sqlite3_bind_zeroblob(stmt, idx_blob, 0);
            
            // Test zeroblob with negative size (should create zero-length BLOB)
            rc = sqlite3_bind_zeroblob(stmt, idx_blob, -1);
            
            // Test zeroblob64 with zero size
            rc = sqlite3_bind_zeroblob64(stmt, idx_blob, 0);
            
            break;
        }
        
        case 5: {
            // Test binding after statement has been stepped
            
            // First bind and step once
            rc = sqlite3_bind_int(stmt, idx_int, fdp.ConsumeIntegral<int>());
            rc = sqlite3_step(stmt);
            
            // Reset statement to allow rebinding
            rc = sqlite3_reset(stmt);
            
            // Now bind new values
            rc = sqlite3_bind_int64(stmt, idx_int64, fdp.ConsumeIntegral<sqlite3_int64>());
            rc = sqlite3_bind_double(stmt, idx_real, fdp.ConsumeFloatingPoint<double>());
            
            break;
        }
        
        case 6: {
            // Test multiple bindings and rebindings
            
            // Bind all parameters
            for (int i = 1; i <= param_count; i++) {
                if (i == idx_int) {
                    rc = sqlite3_bind_int(stmt, i, fdp.ConsumeIntegral<int>());
                } else if (i == idx_int64) {
                    rc = sqlite3_bind_int64(stmt, i, fdp.ConsumeIntegral<sqlite3_int64>());
                } else if (i == idx_real) {
                    rc = sqlite3_bind_double(stmt, i, fdp.ConsumeFloatingPoint<double>());
                } else if (i == idx_text) {
                    std::string text = fdp.ConsumeRandomLengthString(100);
                    rc = sqlite3_bind_text(stmt, i, text.c_str(), -1, SQLITE_TRANSIENT);
                } else if (i == idx_blob) {
                    if (fdp.remaining_bytes() < 1) continue;
                    size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)256));
                    std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
                    rc = sqlite3_bind_blob(stmt, i, blob_data.data(), blob_data.size(), SQLITE_TRANSIENT);
                } else if (i == idx_null) {
                    rc = sqlite3_bind_null(stmt, i);
                }
            }
            
            // Clear bindings
            
            // Rebind with different values
            rc = sqlite3_bind_int(stmt, idx_int, fdp.ConsumeIntegral<int>());
            rc = sqlite3_bind_null(stmt, idx_null);
            
            break;
        }
        
        case 7: {
            // Test mixed binding strategies
            
            // Mix regular and 64-bit bindings
            rc = sqlite3_bind_int(stmt, idx_int, fdp.ConsumeIntegral<int>());
            rc = sqlite3_bind_int64(stmt, idx_int64, fdp.ConsumeIntegral<sqlite3_int64>());
            
            // Mix text and text64
            std::string text1 = fdp.ConsumeRandomLengthString(100);
            rc = sqlite3_bind_text(stmt, idx_text, text1.c_str(), -1, SQLITE_TRANSIENT);
            
            std::string text2 = fdp.ConsumeRandomLengthString(200);
            rc = sqlite3_bind_text64(stmt, idx_text, text2.c_str(), 
                                     text2.size(), SQLITE_TRANSIENT, SQLITE_UTF8);
            // Mix blob and blob64
            if (fdp.remaining_bytes() < 1) break;
            size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(1, std::min(fdp.remaining_bytes(), (size_t)512));
            std::vector<uint8_t> blob_data = fdp.ConsumeBytes<uint8_t>(blob_size);
            rc = sqlite3_bind_blob64(stmt, idx_blob, blob_data.data(), 
                                     blob_data.size(), SQLITE_TRANSIENT);
            // Test NULL binding
            rc = sqlite3_bind_null(stmt, idx_null);
            
            break;
        }
    }
    
    // Step 6: Execute the statement if all parameters are bound
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        
        // Handle possible results
        if (rc == SQLITE_ROW) {
            // Should not happen for INSERT, but handle anyway
        } else if (rc == SQLITE_DONE) {
            // INSERT successful
        }
    }
    
    // Step 7: Test additional edge cases with remaining input
    
    // Test binding with different statement (new prepared statement)
    const char* select_sql = "SELECT ?1, ?2, ?3;";
    sqlite3_stmt* stmt2 = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
    if (rc == SQLITE_OK && stmt2 != nullptr) {
        // Bind parameters to new statement
        rc = sqlite3_bind_int64(stmt2, 1, fdp.ConsumeIntegral<sqlite3_int64>());
        rc = sqlite3_bind_double(stmt2, 2, fdp.ConsumeFloatingPoint<double>());
        
        std::string text = fdp.ConsumeRandomLengthString(100);
        rc = sqlite3_bind_text64(stmt2, 3, text.c_str(), text.size(), 
                                 SQLITE_TRANSIENT, SQLITE_UTF8);
        
        // Execute select
        rc = sqlite3_step(stmt2);
        while (rc == SQLITE_ROW) {
            rc = sqlite3_step(stmt2);
        }
        
        sqlite3_finalize(stmt2);
    }
    
    // Step 8: Clean up resources
    
    // Finalize main statement
    sqlite3_finalize(stmt);
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}

} // namespace harness_012
