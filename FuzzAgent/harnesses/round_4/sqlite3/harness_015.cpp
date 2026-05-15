#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <fuzzer/FuzzedDataProvider.h>
#include "sqlite3.h"

namespace harness_015 {
/*
 * Fuzzing harness for SQLite library - targeting User-Defined Function (UDF) Result APIs
 * as identified by coverage analysis.
 * 
 * This harness targets SQLite UDF result APIs with 0% coverage despite high complexity:
 * - sqlite3_result_blob64 (453 undiscovered branches)
 * - sqlite3_result_text16be (447 undiscovered branches)
 * - sqlite3_result_text16le (447 undiscovered branches)
 * - sqlite3_result_value (445 undiscovered branches)
 * - sqlite3_result_zeroblob64 (445 undiscovered branches)
 * - sqlite3_result_error16 (439 undiscovered branches)
 * 
 * Required setup APIs:
 * - sqlite3_open_v2 - database initialization
 * - sqlite3_create_function_v2 - UDF registration
 * - sqlite3_exec or sqlite3_prepare_v2 + sqlite3_step - SQL execution
 * 
 * Cleanup APIs:
 * - sqlite3_close - database cleanup
 * 
 * Following the exact invocation sequence:
 * 1. sqlite3_open_v2 - create database connection
 * 2. sqlite3_create_function_v2 - register UDF with custom callback
 * 3. In callback: use various sqlite3_result_* APIs based on fuzzer input
 * 4. sqlite3_exec - execute SQL to invoke the UDF
 * 5. sqlite3_close - close database connection
 * 
 * These UDF result functions represent ~2,700+ points of undiscovered accumulated branch complexity.
 * They form the return value mechanism for user-defined SQL functions, called from within
 * UDF callbacks to return results to SQLite.
 * 
 * Semantic differentiation from existing harnesses (000-014):
 * - First harness specifically targeting UDF result APIs (sqlite3_result_* family)
 * - Tests returning different data types from UDF callbacks
 * - Exercises UTF-16 text encoding variants (BE/LE)
 * - Tests 64-bit variants for BLOBs and zero BLOBs
 * - Covers error reporting from UDFs with sqlite3_result_error16
 * - Tests copying values with sqlite3_result_value
 * - Different from harness_014 which focuses on statement binding operations
 * - Different from harness_013 which focuses on extension loading
 */


// Structure to hold test data for UDF callbacks
typedef struct {
    uint8_t result_type;
    int int_value;
    double double_value;
    sqlite3_int64 int64_value;
    char* text_data;
    size_t text_len;
    uint8_t* blob_data;
    size_t blob_size;
    char* utf16_text;
    size_t utf16_len;
    char* error_msg;
    size_t error_len;
    sqlite3_uint64 zeroblob_size;
} udf_test_data_t;

// Forward declaration for UDF callback
static void udf_test_function(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv
);

// Destructor for test data
static void udf_test_data_destroy(void* data) {
    udf_test_data_t* test_data = (udf_test_data_t*)data;
    if (test_data) {
        free(test_data->text_data);
        free(test_data->blob_data);
        free(test_data->utf16_text);
        free(test_data->error_msg);
        free(test_data);
    }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum input for meaningful testing of UDF result APIs
    // Minimum: result_type + data for various result types
    if (size < 32) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Step 1: Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Step 2: Open in-memory database
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
    
    // Step 3: Allocate and populate test data structure
    udf_test_data_t* test_data = (udf_test_data_t*)malloc(sizeof(udf_test_data_t));
    if (!test_data) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    memset(test_data, 0, sizeof(udf_test_data_t));
    
    // Consume result type selection
    test_data->result_type = fdp.ConsumeIntegral<uint8_t>() % 7;
    
    // Consume data for various result types
    test_data->int_value = fdp.ConsumeIntegral<int>();
    test_data->double_value = fdp.ConsumeFloatingPoint<double>();
    test_data->int64_value = fdp.ConsumeIntegral<sqlite3_int64>();
    
    // Consume text data (UTF-8)
    std::string text_str = fdp.ConsumeRandomLengthString(256);
    test_data->text_len = text_str.size();
    test_data->text_data = (char*)malloc(test_data->text_len + 1);
    if (test_data->text_data && test_data->text_len > 0) {
        memcpy(test_data->text_data, text_str.c_str(), test_data->text_len);
        test_data->text_data[test_data->text_len] = '\0';
    }
    
    // Consume blob data
    size_t blob_size = fdp.ConsumeIntegralInRange<size_t>(0, std::min(fdp.remaining_bytes(), (size_t)1024));
    test_data->blob_size = blob_size;
    test_data->blob_data = (uint8_t*)malloc(blob_size);
    if (test_data->blob_data && blob_size > 0) {
        std::vector<uint8_t> blob_vec = fdp.ConsumeBytes<uint8_t>(blob_size);
        memcpy(test_data->blob_data, blob_vec.data(), blob_size);
    }
    
    // Consume UTF-16 text data
    std::string utf16_str = fdp.ConsumeRandomLengthString(128);
    test_data->utf16_len = utf16_str.size();
    test_data->utf16_text = (char*)malloc(test_data->utf16_len + 1);
    if (test_data->utf16_text && test_data->utf16_len > 0) {
        memcpy(test_data->utf16_text, utf16_str.c_str(), test_data->utf16_len);
        test_data->utf16_text[test_data->utf16_len] = '\0';
    }
    
    // Consume error message
    std::string error_str = fdp.ConsumeRandomLengthString(200);
    test_data->error_len = error_str.size();
    test_data->error_msg = (char*)malloc(test_data->error_len + 1);
    if (test_data->error_msg && test_data->error_len > 0) {
        memcpy(test_data->error_msg, error_str.c_str(), test_data->error_len);
        test_data->error_msg[test_data->error_len] = '\0';
    }
    
    test_data->zeroblob_size = fdp.ConsumeIntegral<sqlite3_uint64>() % 65536;
    
    // Step 4: Register test UDF with test_data as user data
    rc = sqlite3_create_function_v2(
        db,
        "test_udf",
        0,  // Number of arguments (our test UDF takes no arguments)
        SQLITE_UTF8,
        test_data,  // User data pointer - passed to callback
        udf_test_function,
        nullptr,  // xStep (not an aggregate)
        nullptr,  // xFinal (not an aggregate)
        udf_test_data_destroy   // xDestroy - clean up test data
    );
    
    if (rc != SQLITE_OK) {
        udf_test_data_destroy(test_data);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Also register variant that takes arguments for sqlite3_result_value testing
    // We'll create a separate test data copy for this function
    udf_test_data_t* test_data2 = (udf_test_data_t*)malloc(sizeof(udf_test_data_t));
    if (test_data2) {
        memcpy(test_data2, test_data, sizeof(udf_test_data_t));
        // Don't duplicate string data, just point to same buffers
        // We'll need to handle this in destruction carefully
        test_data2->text_data = nullptr;  // Won't free in destructor
        test_data2->blob_data = nullptr;
        test_data2->utf16_text = nullptr;
        test_data2->error_msg = nullptr;
        
        rc = sqlite3_create_function_v2(
            db,
            "test_udf_with_args",
            2,  // Takes 2 arguments for testing sqlite3_result_value
            SQLITE_UTF8,
            test_data2,
            udf_test_function,
            nullptr,
            nullptr,
            udf_test_data_destroy
        );
    }
    
    // Step 5: Create a simple table for testing
    const char* create_sql = "CREATE TABLE udf_test (id INTEGER, data TEXT);";
    char* errmsg = nullptr;
    sqlite3_exec(db, create_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Insert some test data
    sqlite3_exec(db, "INSERT INTO udf_test VALUES (1, 'test1'), (2, 'test2');", 
                 nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Step 6: Execute SQL to invoke the UDF multiple times with different queries
    // This exercises different result types from the callback
    
    // Test 1: Simple UDF call returning different result types
    const char* test_sql = "SELECT test_udf();";
    sqlite3_exec(db, test_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Test 2: UDF in WHERE clause
    test_sql = "SELECT * FROM udf_test WHERE test_udf();";
    sqlite3_exec(db, test_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Test 3: UDF with arguments for sqlite3_result_value testing
    test_sql = "SELECT test_udf_with_args(id, data) FROM udf_test;";
    sqlite3_exec(db, test_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Test 4: UDF in expression
    test_sql = "SELECT id, data, test_udf() FROM udf_test;";
    sqlite3_exec(db, test_sql, nullptr, nullptr, &errmsg);
    if (errmsg != nullptr) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Step 7: Clean up
    // Note: test_data and test_data2 will be freed by sqlite3_create_function_v2's destructor
    // when the database is closed or when the functions are redefined
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}

// UDF callback implementation that uses various sqlite3_result_* APIs
static void udf_test_function(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv
) {
    // Get test data from user_data
    udf_test_data_t* test_data = (udf_test_data_t*)sqlite3_user_data(context);
    if (!test_data) {
        sqlite3_result_null(context);
        return;
    }
    
    // Determine which result API to use based on test_data.result_type
    switch (test_data->result_type) {
        case 0: {
            // Test sqlite3_result_blob64
            if (test_data->blob_data && test_data->blob_size > 0) {
                sqlite3_result_blob64(context, 
                                     test_data->blob_data,
                                     test_data->blob_size,
                                     SQLITE_TRANSIENT);
            } else {
                sqlite3_result_null(context);
            }
            break;
        }
        
        case 1: {
            // Test sqlite3_result_text16be (UTF-16 Big Endian)
            if (test_data->utf16_text && test_data->utf16_len > 0) {
                sqlite3_result_text16be(context,
                                       test_data->utf16_text,
                                       test_data->utf16_len,
                                       SQLITE_TRANSIENT);
            } else {
                sqlite3_result_text16be(context, "test", 4, SQLITE_TRANSIENT);
            }
            break;
        }
        
        case 2: {
            // Test sqlite3_result_text16le (UTF-16 Little Endian)
            if (test_data->utf16_text && test_data->utf16_len > 0) {
                sqlite3_result_text16le(context,
                                       test_data->utf16_text,
                                       test_data->utf16_len,
                                       SQLITE_TRANSIENT);
            } else {
                sqlite3_result_text16le(context, "test", 4, SQLITE_TRANSIENT);
            }
            break;
        }
        
        case 3: {
            // Test sqlite3_result_value - copy one of the arguments
            if (argc >= 2) {
                // Copy the second argument
                sqlite3_result_value(context, argv[1]);
            } else if (argc >= 1) {
                // Copy the first argument
                sqlite3_result_value(context, argv[0]);
            } else {
                // No arguments, return null
                sqlite3_result_null(context);
            }
            break;
        }
        
        case 4: {
            // Test sqlite3_result_zeroblob64
            sqlite3_result_zeroblob64(context, test_data->zeroblob_size);
            break;
        }
        
        case 5: {
            // Test sqlite3_result_error16
            if (test_data->error_msg && test_data->error_len > 0) {
                sqlite3_result_error16(context,
                                      test_data->error_msg,
                                      test_data->error_len);
            } else {
                sqlite3_result_error16(context, "UDF error", 9);
            }
            break;
        }
        
        case 6: {
            // Test other result types for completeness
            uint8_t subtype = test_data->result_type;
            if (subtype % 3 == 0) {
                sqlite3_result_int(context, test_data->int_value);
            } else if (subtype % 3 == 1) {
                sqlite3_result_double(context, test_data->double_value);
            } else {
                sqlite3_result_int64(context, test_data->int64_value);
            }
            break;
        }
        
        default: {
            sqlite3_result_null(context);
            break;
        }
    }
}

} // namespace harness_015
