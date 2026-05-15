#include "FDSan.h"
#include "FuzzedDataProvider.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
extern "C" {
#include <sqlite3.h>
}
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 150, nr_unique_branch: 12
//<Quality> {"density":30,"unique_branches":{"sqlite3_bind_zeroblob64":[[91326,7,91326,11,0,0,4,1],[91329,7,91329,48,0,0,4,1]],"sqlite3VdbeExec":[[95763,13,95763,18,0,0,4,1],[95816,7,95816,12,0,0,4,0],[95874,11,95874,18,0,0,4,1],[97906,7,97906,30,0,0,4,0]],"sqlite3_transfer_bindings":[[91407,7,91407,29,0,0,4,0]],"fillInCell":[[76807,9,76807,16,0,0,4,1],[76809,15,76809,21,0,0,4,0],[76809,15,76809,21,0,0,4,1],[76821,9,76821,21,0,0,4,1],[14098,27,14098,28,6,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_extended_result_codes","sqlite3_keyword_count","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_close","sqlite3_bind_text","sqlite3_bind_zeroblob64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_transfer_bindings","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_errstr","sqlite3_finalize","sqlite3_value_type","sqlite3_value_int64","sqlite3_result_int64","sqlite3_result_error","sqlite3_value_type","sqlite3_value_int64","sqlite3_result_int64","sqlite3_result_error","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_finalize","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_extended_result_codes","sqlite3_keyword_count","sqlite3_exec","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_zeroblob64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_transfer_bindings","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_errstr","sqlite3_finalize","sqlite3_result_error","sqlite3_result_int64","sqlite3_value_int64","sqlite3_value_type","sqlite3_result_error","sqlite3_result_int64","sqlite3_value_int64","sqlite3_value_type","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_finalize","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":0}
/*Here's a fuzz driver that uses all the specified SQLite3 APIs in a meaningful way:

*/


extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open an in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) {
            sqlite3_close(db);
        }
        return 0;
    }

    // Enable extended result codes for better error handling
    sqlite3_extended_result_codes(db, 1);

    // Get keyword count - this doesn't use input data but is required
    int keyword_count = sqlite3_keyword_count();

    // Create a simple table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "blob_data BLOB, "
        "value INTEGER"
        ")";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare an INSERT statement
    sqlite3_stmt* stmt1 = nullptr;
    const char* insert_sql = 
        "INSERT INTO test_table (name, blob_data, value) VALUES (?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, nullptr);
    
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare another statement for testing transfer_bindings
    sqlite3_stmt* stmt2 = nullptr;
    const char* update_sql = 
        "UPDATE test_table SET name = ?, blob_data = ?, value = ? WHERE id = ?";
    rc = sqlite3_prepare_v2(db, update_sql, -1, &stmt2, nullptr);
    
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }

    // Use input data for binding
    if (size > 0) {
        // Split the input data into parts for different bindings
        size_t split_point = size / 3;
        
        // For stmt1: bind a text value using input data
        if (split_point > 0) {
            // Create a null-terminated string from first part of input
            std::vector<char> name_str(data, data + split_point);
            if (name_str.size() > 0) {
                name_str.back() = '\0';  // Ensure null termination
                sqlite3_bind_text(stmt1, 1, name_str.data(), -1, SQLITE_TRANSIENT);
            }
        }
        
        // For stmt1: bind a zeroblob using keyword_count as size
        // Use keyword_count modulo 1000 to avoid huge allocations
        sqlite3_int64 blob_size = keyword_count % 1000;
        sqlite3_bind_zeroblob64(stmt1, 2, blob_size);
        
        // For stmt1: bind an integer value from input data
        if (size > split_point + 1) {
            int int_value = static_cast<int>(data[split_point]);
            sqlite3_bind_int(stmt1, 3, int_value);
        }
        
        // Execute stmt1
        rc = sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
        
        // Test transfer_bindings from stmt1 to stmt2
        if (sqlite3_transfer_bindings(stmt1, stmt2) == SQLITE_OK) {
            // Bind an additional parameter for the WHERE clause
            sqlite3_bind_int(stmt2, 4, 1);  // Update row with id=1
            
            // Execute stmt2
            sqlite3_step(stmt2);
            sqlite3_reset(stmt2);
        }
        
        // Test sqlite3_errstr by deliberately causing an error
        // Use a bad SQL statement that should fail
        sqlite3_stmt* bad_stmt = nullptr;
        const char* bad_sql = "BAD SQL STATEMENT";
        int bad_rc = sqlite3_prepare_v2(db, bad_sql, -1, &bad_stmt, nullptr);
        if (bad_rc != SQLITE_OK) {
            // Use sqlite3_errstr to get the error message
            const char* err_str = sqlite3_errstr(bad_rc);
            // err_str can be used for logging or other purposes
            // For fuzzing, we just consume it
            (void)err_str;
        }
        if (bad_stmt) {
            sqlite3_finalize(bad_stmt);
        }
    }

    // Create a custom function that uses sqlite3_result_int64
    struct FuncContext {
        sqlite3_int64 multiplier;
    };
    
    static auto custom_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        if (argc == 1 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
            sqlite3_int64 input_val = sqlite3_value_int64(argv[0]);
            sqlite3_int64 result = input_val * 2;  // Simple transformation
            sqlite3_result_int64(ctx, result);
        } else {
            sqlite3_result_error(ctx, "Invalid arguments", -1);
        }
    };
    
    // Register the custom function
    rc = sqlite3_create_function(
        db, 
        "custom_double", 
        1, 
        SQLITE_UTF8, 
        nullptr, 
        custom_func, 
        nullptr, 
        nullptr
    );
    
    if (rc == SQLITE_OK) {
        // Test the custom function with some input data
        if (size > 0) {
            // Use a value from input data
            int test_value = static_cast<int>(data[0] % 100);
            char test_sql[100];
            snprintf(test_sql, sizeof(test_sql), 
                    "SELECT custom_double(%d)", test_value);
            
            sqlite3_stmt* test_stmt = nullptr;
            if (sqlite3_prepare_v2(db, test_sql, -1, &test_stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(test_stmt) == SQLITE_ROW) {
                    // Result is available, consume it
                    sqlite3_int64 result = sqlite3_column_int64(test_stmt, 0);
                    (void)result;  // Use result to avoid unused variable warning
                }
                sqlite3_finalize(test_stmt);
            }
        }
    }

    // Create a FILE* from input data and read from it
    if (size > 0) {
        FILE* in_file = fmemopen((void*)data, size, "rb");
        if (in_file) {
            // Read a small amount to consume the FILE
            char buffer[256];
            fread(buffer, 1, sizeof(buffer) < size ? sizeof(buffer) : size, in_file);
            assert_file_closed(&in_file);;
        }
    }

    // Create an output file
    FILE* out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write something to the file
        fwrite("test", 1, 4, out_file);
        assert_file_closed(&out_file);;
    }

    // Clean up
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);

    assert_file_closed(&out_file);
	return 0;
}