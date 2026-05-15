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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 160, nr_unique_branch: 13
//<Quality> {"density":32,"unique_branches":{"sqlite3_filename_journal":[[182392,7,182392,19,0,0,4,0]],"sqlite3_error_offset":[[180260,7,180260,9,0,0,4,0],[180260,13,180260,43,0,0,4,0],[180260,47,180260,58,0,0,4,1]],"sqlite3_drop_modules":[[153788,7,153788,32,0,0,4,1],[153790,44,153790,49,0,0,4,0],[153790,44,153790,49,0,0,4,1],[153793,9,153793,16,0,0,4,0],[153795,17,153795,31,0,0,4,1],[153796,11,153796,25,0,0,4,1]],"sqlite3VtabCreateModule":[[153694,7,153694,17,0,0,4,0],[153714,7,153714,11,0,0,4,0],[153715,9,153715,19,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_keyword_count","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_clear_bindings","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_dup","sqlite3_bind_value","sqlite3_value_free","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_finalize","sqlite3_value_int64","sqlite3_result_int64","sqlite3_result_error_nomem","sqlite3_value_int64","sqlite3_result_int64","sqlite3_result_error_nomem","sqlite3_create_function_v2","sqlite3_collation_needed","sqlite3_error_offset","sqlite3_errstr","sqlite3_filename_journal","sqlite3_create_module","sqlite3_drop_modules","sqlite3_exec","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_keyword_count","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_clear_bindings","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_dup","sqlite3_bind_value","sqlite3_value_free","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_finalize","sqlite3_result_error_nomem","sqlite3_result_int64","sqlite3_value_int64","sqlite3_result_error_nomem","sqlite3_result_int64","sqlite3_value_int64","sqlite3_create_function_v2","sqlite3_collation_needed","sqlite3_error_offset","sqlite3_errstr","sqlite3_filename_journal","sqlite3_create_module","sqlite3_drop_modules","sqlite3_exec","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Extract data from input file for various uses
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *file_content = nullptr;
    if (file_size > 0) {
        file_content = (char *)malloc(file_size + 1);
        if (file_content) {
            fread(file_content, 1, file_size, in_file);
            file_content[file_size] = '\0';
        }
    }
    assert_file_closed(&in_file);;
    
    // Initialize SQLite
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (file_content) free(file_content);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get keyword count
    int keyword_count = sqlite3_keyword_count();
    
    // Create a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "data BLOB, "
        "value INTEGER);";
    
    char *errmsg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Prepare a statement for insertion
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = 
        "INSERT INTO test_table (name, data, value) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt && file_content && file_size > 0) {
        // Bind parameters using various binding functions
        // Bind text (use first part of data as string)
        int text_len = (file_size > 100) ? 100 : file_size;
        sqlite3_bind_text(stmt, 1, file_content, text_len, SQLITE_TRANSIENT);
        
        // Bind blob (use middle part of data)
        if (file_size > 200) {
            int blob_offset = 100;
            int blob_len = (file_size - blob_offset > 100) ? 100 : file_size - blob_offset;
            sqlite3_bind_blob(stmt, 2, file_content + blob_offset, blob_len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        
        // Bind integer (use size as value)
        sqlite3_bind_int(stmt, 3, file_size);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
            // Reset statement for potential reuse
            sqlite3_reset(stmt);
            
            // Bind zeroblob64 for large blob
            sqlite3_bind_zeroblob64(stmt, 2, 1024);
            
            // Execute again
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
        
        // Clear bindings
        sqlite3_clear_bindings(stmt);
        
        // Prepare another statement for selection
        sqlite3_stmt *stmt2 = nullptr;
        const char *select_sql = "SELECT * FROM test_table;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
        
        if (rc == SQLITE_OK && stmt2) {
            // Step through results
            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                // Get column value
                if (sqlite3_column_count(stmt2) > 2) {
                    sqlite3_value *col_value = sqlite3_column_value(stmt2, 2);
                    if (col_value) {
                        // Duplicate the value
                        sqlite3_value *dup_value = sqlite3_value_dup(col_value);
                        if (dup_value) {
                            // Bind the duplicated value to first statement
                            sqlite3_bind_value(stmt, 1, dup_value);
                            // Free the duplicated value
                            sqlite3_value_free(dup_value);
                        }
                    }
                }
            }
            
            // Transfer bindings from stmt to stmt2
            sqlite3_transfer_bindings(stmt, stmt2);
            
            // Finalize the second statement
            sqlite3_finalize(stmt2);
        }
        
        // Finalize the first statement
        sqlite3_finalize(stmt);
    }
    
    // Create a custom function
    auto custom_func = [](sqlite3_context *context, int argc, sqlite3_value **argv) {
        if (argc > 0) {
            sqlite3_int64 val = sqlite3_value_int64(argv[0]);
            sqlite3_result_int64(context, val * 2);
        } else {
            sqlite3_result_error_nomem(context);
        }
    };
    
    rc = sqlite3_create_function_v2(
        db, "custom_double", 1, SQLITE_UTF8, 
        nullptr, custom_func, nullptr, nullptr, nullptr
    );
    
    // Create a collation needed callback
    auto collation_needed_cb = [](void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
        // Callback implementation
    };
    
    sqlite3_collation_needed(db, nullptr, collation_needed_cb);
    
    // Get error offset if there's an error
    int error_offset = sqlite3_error_offset(db);
    
    // Get error string for a sample error code
    const char *err_str = sqlite3_errstr(SQLITE_ERROR);
    
    // Get filename journal (though in-memory db won't have one)
    const char *journal_name = sqlite3_filename_journal(nullptr);
    
    // Create a module (virtual table)
    static sqlite3_module dummy_module = {};
    rc = sqlite3_create_module(db, "dummy_module", &dummy_module, nullptr);
    
    // Drop all modules except core ones
    const char *keep_modules[] = {nullptr};
    sqlite3_drop_modules(db, keep_modules);
    
    // Execute a query using the custom function
    const char *test_sql = "SELECT custom_double(5);";
    rc = sqlite3_exec(db, test_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Test aggregate context (though not in our function)
    // This would normally be used in an aggregate function
    
    // Clean up
    if (file_content) {
        free(file_content);
    }
    
    // Close database
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}