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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 150, nr_unique_branch: 67
//<Quality> {"density":25,"unique_branches":{"sqlite3Insert":[[132577,18,132577,32,0,0,4,1],[132857,11,132857,33,0,0,4,0]],"sqlite3CreateFunc":[[179482,7,179482,32,0,0,4,0]],"sqlite3_str_value":[[31839,7,31839,11,0,0,4,1],[31839,15,31839,26,0,0,4,1]],"sqlite3Pragma":[[139154,15,139154,31,0,0,4,1],[139184,15,139184,28,0,0,4,0],[139189,17,139189,21,0,0,4,1],[139205,17,139205,28,0,0,4,0]],"sqlite3_str_vappendf":[[30983,19,30983,27,0,0,4,0],[30986,18,30986,35,0,0,4,0],[30986,39,30986,45,0,0,4,1],[30992,15,30992,50,0,0,4,1],[30996,15,30996,21,0,0,4,0],[30996,25,30996,31,0,0,4,0],[31138,13,31138,25,0,0,4,0],[31138,29,31138,56,0,0,4,0],[31171,16,31171,32,0,0,4,0],[14098,27,14098,28,3,0,4,1]],"sqlite3VdbeAssertMayAbort":[[84764,30,84764,51,0,0,4,0]],"sqlite3GenerateConstraintChecks":[[133389,7,133389,37,0,0,4,0],[133394,16,133394,22,0,0,4,0],[133394,16,133394,22,0,0,4,1],[133399,13,133399,29,0,0,4,0],[133399,13,133399,29,0,0,4,1],[133400,13,133400,27,0,0,4,1],[133404,13,133404,24,0,0,4,1],[133408,13,133408,19,0,0,4,1],[133412,13,133412,38,0,0,4,1],[133414,19,133414,38,0,0,4,0],[133417,13,133417,32,0,0,4,1],[133428,19,133428,27,0,0,4,1],[133436,11,133436,26,0,0,4,1],[133446,11,133446,24,0,0,4,0],[133449,11,133449,27,0,0,4,1],[133450,11,133450,23,0,0,4,1],[133461,11,133461,18,0,0,4,1],[133469,11,133469,24,0,0,4,0],[133469,28,133469,43,0,0,4,0],[14098,27,14098,28,13,0,4,1],[14098,27,14098,28,19,0,4,1]],"sqlite3_table_column_metadata":[[181515,7,181515,32,0,0,4,1],[181515,36,181515,49,0,0,4,1],[181524,7,181524,20,0,0,4,1],[181530,7,181530,12,0,0,4,1],[181536,7,181536,21,0,0,4,1],[181539,17,181539,32,0,0,4,0],[181541,11,181541,56,0,0,4,0],[181541,11,181541,56,0,0,4,1],[181545,9,181545,25,0,0,4,1],[181566,7,181566,11,0,0,4,0],[181571,15,181571,32,0,0,4,1],[181576,7,181576,16,0,0,4,0],[181587,7,181587,17,0,0,4,0],[181588,7,181588,16,0,0,4,0],[181589,7,181589,15,0,0,4,0],[181590,7,181590,18,0,0,4,0],[181591,7,181591,15,0,0,4,0],[181593,7,181593,20,0,0,4,0],[181593,24,181593,29,0,0,4,1],[181599,32,181599,39,0,0,4,1],[18322,29,18322,57,3,0,4,1]],"sqlite3VdbeExec":[[93404,1,93404,19,0,0,4,0],[93407,7,93407,24,0,0,4,0],[93409,7,93409,34,0,0,4,0]],"sqlite3Prepare16":[[141229,17,141229,23,0,0,4,1]],"sqlite3MisuseError":[[14098,27,14098,28,1,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_exec","sqlite3_table_column_metadata","sqlite3_prepare16","sqlite3_prepare16","sqlite3_bind_text","sqlite3_transfer_bindings","sqlite3_step","sqlite3_column_count","sqlite3_column_database_name16","sqlite3_reset","sqlite3_reset","sqlite3_stmt_scanstatus_v2","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_appendf","sqlite3_str_value","sqlite3_str_finish","sqlite3_free","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_bind_pointer","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_exec","sqlite3_table_column_metadata","sqlite3_prepare16","sqlite3_prepare16","sqlite3_bind_text","sqlite3_transfer_bindings","sqlite3_step","sqlite3_column_count","sqlite3_column_database_name16","sqlite3_reset","sqlite3_reset","sqlite3_stmt_scanstatus_v2","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_appendf","sqlite3_str_value","sqlite3_str_finish","sqlite3_free","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_bind_pointer","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":1}
/*Looking at the provided APIs, I'll create a fuzzer that simulates database operations involving column metadata retrieval, statement preparation with 16-bit SQL, binding transfer, and various result operations.

*/


// Custom user data structure for demonstration
struct TestData {
    int value;
    char name[32];
};

// Callback for pointer cleanup
static void  id_000026_cleanup_pointer(void *ptr) {
    // Simple cleanup function
    if (ptr) {
        free(ptr);
    }
}

// User-defined function context
static void  id_000026_test_aggregate_context(sqlite3_context *ctx) {
    // Use the aggregate context API
    void *agg_ctx = sqlite3_aggregate_context(ctx, 100);
    if (!agg_ctx) {
        sqlite3_result_error_nomem(ctx);
        return;
    }
    
    // Do something with the context
    memset(agg_ctx, 0, 100);
    sqlite3_result_int(ctx, 42);
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<424) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    // Open an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Create a test table
    const char *create_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL, "
        "value REAL, "
        "data BLOB"
        ");";
    rc = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    // Insert some test data
    const char *insert_sql = 
        "INSERT INTO test_table (name, value, data) VALUES "
        "('test1', 1.5, x'010203'), "
        "('test2', 2.5, x'040506'), "
        "('test3', 3.5, x'070809');";
    rc = sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
    
    // 1. Use sqlite3_table_column_metadata
    const char *data_type = nullptr;
    const char *coll_seq = nullptr;
    int not_null = 0;
    int primary_key = 0;
    int autoinc = 0;
    
    rc = sqlite3_table_column_metadata(
        db, fuzz_str_3, fuzz_str_4, fuzz_str_5,
        &data_type, &coll_seq, &not_null, &primary_key, &autoinc
    );
    
    // 2. Prepare two statements with 16-bit SQL (UTF-16)
    sqlite3_stmt *stmt1 = nullptr;
    sqlite3_stmt *stmt2 = nullptr;
    
    // Convert input data to UTF-16 for testing
    // We'll use a simple SELECT statement in UTF-16
    const wchar_t utf16_sql[] = L"SELECT * FROM test_table WHERE name LIKE ?";
    
    rc = sqlite3_prepare16(
        db,
        fuzz_str_6,
        static_cast<int>(fuzz_str_sz_6),
        &stmt1,
        nullptr
    );
    
    if (rc == SQLITE_OK && stmt1) {
        // Prepare another statement
        rc = sqlite3_prepare16(
            db,
            fuzz_str_7,
            static_cast<int>(fuzz_str_sz_7),
            &stmt2,
            nullptr
        );
        
        if (rc == SQLITE_OK && stmt2 && stmt1) {
            // 3. Use sqlite3_transfer_bindings
            // First bind some values to stmt1
            if (size > 0) {
                // Use first byte as a pattern length, but limit it
                int pattern_len = data[0] % 32;
                if (pattern_len > 0 && pattern_len < size) {
                    char pattern[33] = {0};
                    memcpy(pattern, data + 1, pattern_len);
                    pattern[pattern_len] = '\0';
                    
                    rc = sqlite3_bind_text(stmt1, fuzz_int32_t_8, pattern, -1, SQLITE_TRANSIENT);
                    
                    // Now transfer bindings from stmt1 to stmt2
                    rc = sqlite3_transfer_bindings(stmt1, stmt2);
                }
            }
            
            // Execute the statements
            while (sqlite3_step(stmt1) == SQLITE_ROW) {
                // Get column information
                int col_count = sqlite3_column_count(stmt1);
                
                // 4. Use sqlite3_column_database_name16
                for (int i = 0; i < col_count; i++) {
                    const void *db_name = sqlite3_column_database_name16(stmt1, fuzz_int32_t_9);
                    // Can write to output file if needed
                    if (db_name && out_file) {
                        fwrite(db_name, 1, 32, out_file);
                    }
                }
            }
            
            // Reset statements for reuse
            sqlite3_reset(stmt1);
            sqlite3_reset(stmt2);
        }
    }
    
    // 5. Use sqlite3_stmt_scanstatus_v2
    if (stmt1) {
        int64_t scan_info = 0;
        rc = sqlite3_stmt_scanstatus_v2(
            stmt1,
            fuzz_int32_t_10,  // idx
            fuzz_int32_t_11,  // iScanStatusOp (SQLITE_SCANSTAT_NLOOP)
            fuzz_int32_t_12,  // flags
            &scan_info
        );
    }
    
    // 6. Create and use sqlite3_str
    sqlite3_str *str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // Append some data from input
        size_t append_size = size > 100 ? 100 : size;
        if (append_size > 0) {
            sqlite3_str_appendf(str_obj, "Data: ");
            for (size_t i = 0; i < append_size; i++) {
                sqlite3_str_appendf(str_obj, "%02x", data[i]);
            }
        }
        
        // 7. Use sqlite3_str_value
        char *str_value = sqlite3_str_value(str_obj);
        if (str_value && out_file) {
            fwrite(str_value, 1, strlen(str_value), out_file);
        }
        
        // Clean up the string object
        // Note: sqlite3_str_finish returns the string and frees the object
        char *finished_str = sqlite3_str_finish(str_obj);
        if (finished_str) {
            sqlite3_free(finished_str);
        }
    }
    
    // 8. Test sqlite3_result_error_nomem by creating a user-defined function
    // We'll create a simple aggregate function that might run out of memory
    rc = sqlite3_create_function(
        db,
        fuzz_str_13,
        fuzz_int32_t_14,  // number of arguments
        fuzz_int32_t_15,
        nullptr,
        nullptr,  // scalar function
        nullptr,  // step function  
         id_000026_test_aggregate_context  // final function
    );
    
    if (rc == SQLITE_OK) {
        // Execute a query that uses our function
        sqlite3_stmt *agg_stmt = nullptr;
        const char *agg_sql = "SELECT test_agg(id) FROM test_table";
        rc = sqlite3_prepare_v2(db, agg_sql, -1, &agg_stmt, nullptr);
        
        if (rc == SQLITE_OK && agg_stmt) {
            sqlite3_step(agg_stmt);
            sqlite3_finalize(agg_stmt);
        }
        
        // Remove the function
        sqlite3_create_function(db, fuzz_str_16, fuzz_int32_t_17, fuzz_int32_t_18, nullptr, nullptr, nullptr, nullptr);
    }
    
    // Test pointer binding
    if (stmt1) {
        TestData *test_ptr = (TestData*)malloc(sizeof(TestData));
        if (test_ptr) {
            test_ptr->value = 42;
            strncpy(test_ptr->name, "Test Pointer", sizeof(test_ptr->name)-1);
            
            rc = sqlite3_bind_pointer(
                stmt1,
                fuzz_int32_t_20,
                test_ptr,
                fuzz_str_19,
                 id_000026_cleanup_pointer
            );
            
            // Execute to trigger cleanup
            sqlite3_step(stmt1);
            sqlite3_reset(stmt1);
        }
    }
    
    // Clean up
    if (stmt1) {
        sqlite3_finalize(stmt1);
    }
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    
    // Close database
    sqlite3_close(db);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}