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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 210, nr_unique_branch: 8
//<Quality> {"density":35,"unique_branches":{"sqlite3FindFunction":[[126292,21,126292,49,0,0,4,1]],"sqlite3CreateFunc":[[179481,8,179481,17,0,0,4,1],[179580,15,179580,21,0,0,4,1]],"fillInCell":[[14098,27,14098,28,8,0,4,0],[14098,27,14098,28,9,0,4,0]],"sqlite3_stmt_scanstatus_v2":[[91842,15,91842,22,0,0,4,0]],"cellSizePtrTableLeaf":[[14098,27,14098,28,3,0,4,0],[14098,27,14098,28,4,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_text","sqlite3_finalize","sqlite3_close","sqlite3_bind_blob","sqlite3_finalize","sqlite3_close","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_name16","sqlite3_stmt_scanstatus","sqlite3_column_value","sqlite3_value_bytes16","sqlite3_value_dup","sqlite3_value_bytes16","sqlite3_finalize","sqlite3_get_autocommit","sqlite3_mprintf","sqlite3_free","sqlite3_create_function","sqlite3_user_data","sqlite3_result_int","sqlite3_user_data","sqlite3_result_int","sqlite3_prepare_v2","sqlite3_step","sqlite3_context_db_handle","sqlite3_finalize","sqlite3_create_function","sqlite3_value_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_name16","sqlite3_stmt_scanstatus","sqlite3_column_value","sqlite3_value_bytes16","sqlite3_value_dup","sqlite3_value_bytes16","sqlite3_finalize","sqlite3_get_autocommit","sqlite3_mprintf","sqlite3_free","sqlite3_create_function","sqlite3_result_int","sqlite3_user_data","sqlite3_result_int","sqlite3_user_data","sqlite3_prepare_v2","sqlite3_step","sqlite3_context_db_handle","sqlite3_finalize","sqlite3_create_function","sqlite3_value_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // 1. Create input file from memory buffer
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // 2. Create output file for database operations
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Mark as used
    (void)out_fd; // Mark as used

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_value* value = nullptr;
    int rc = 0;

    // 3. Open database in memory for fuzzing
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 4. Create a simple table and insert some data
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "data BLOB);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 5. Prepare a statement for insertion using input data
    const char* insert_sql = "INSERT INTO fuzz_test (name, data) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 6. Bind parameters using the input data
    // Use first part of data as name, rest as blob
    size_t name_len = size > 100 ? 100 : size / 2;
    const char* name_data = reinterpret_cast<const char*>(data);
    
    rc = sqlite3_bind_text(stmt, 1, name_data, name_len, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    size_t blob_len = size - name_len;
    const void* blob_data = data + name_len;
    rc = sqlite3_bind_blob(stmt, 2, blob_data, blob_len, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 7. Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }

    // 8. Reset statement for reuse
    sqlite3_reset(stmt);

    // 9. Prepare a SELECT statement
    const char* select_sql = "SELECT * FROM fuzz_test";
    sqlite3_stmt* select_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
    if (rc == SQLITE_OK && select_stmt) {
        // 10. Execute SELECT and use required APIs
        rc = sqlite3_step(select_stmt);
        if (rc == SQLITE_ROW) {
            // Use sqlite3_column_name16 - API #3 from list
            const void* column_name = sqlite3_column_name16(select_stmt, 0);
            (void)column_name; // Mark as used
            
            // Use sqlite3_stmt_scanstatus if available
            // Note: This requires special build options in SQLite
            // We'll call it but it may not do anything in standard builds
            void* scan_status_out = nullptr;
            sqlite3_stmt_scanstatus(select_stmt, 0, 0, scan_status_out);
            
            // Get column value and use sqlite3_value_bytes16
            sqlite3_value* col_value = sqlite3_column_value(select_stmt, 2);
            if (col_value) {
                // Use sqlite3_value_bytes16 - API #6 from list
                int bytes_len = sqlite3_value_bytes16(col_value);
                (void)bytes_len; // Mark as used
                
                // Create a duplicate value for testing
                value = sqlite3_value_dup(col_value);
                if (value) {
                    // Use it again
                    int dup_bytes_len = sqlite3_value_bytes16(value);
                    (void)dup_bytes_len; // Mark as used
                }
            }
        }
        sqlite3_finalize(select_stmt);
    }

    // 11. Use sqlite3_get_autocommit - API #4 from list
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit; // Mark as used

    // 12. Create a sqlite3_str for testing (if supported)
    // Note: sqlite3_str APIs might not be available in all builds
    // We'll use what's available from the provided list
    sqlite3_str* str = nullptr;
    // Create string using mprintf
    char* test_str = sqlite3_mprintf("Test %d", autocommit);
    if (test_str) {
        // Write string to output file
        fwrite(test_str, strlen(test_str), 1, out_file);
        fflush(out_file);
        sqlite3_free(test_str);
    }

    // 13. Get database handle from context (simulated)
    // We need a sqlite3_context for sqlite3_context_db_handle
    // Create a simple scalar function to get context
    struct ContextWrapper {
        sqlite3* db;
        sqlite3_context* ctx;
    } wrapper = {db, nullptr};
    
    // Register a simple function
    rc = sqlite3_create_function(
        db, 
        "fuzz_func", 
        1, 
        SQLITE_UTF8, 
        &wrapper,
        [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            // Store context in wrapper
            ContextWrapper* w = static_cast<ContextWrapper*>(sqlite3_user_data(ctx));
            if (w) w->ctx = ctx;
            
            // Return a simple value
            sqlite3_result_int(ctx, 42);
        },
        nullptr,
        nullptr
    );
    
    if (rc == SQLITE_OK) {
        // Execute the function to get context
        const char* func_sql = "SELECT fuzz_func(1)";
        sqlite3_stmt* func_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, nullptr);
        if (rc == SQLITE_OK && func_stmt) {
            if (sqlite3_step(func_stmt) == SQLITE_ROW) {
                // Now wrapper.ctx should be set
                if (wrapper.ctx) {
                    // Use sqlite3_context_db_handle - API #2 from list
                    sqlite3* db_from_ctx = sqlite3_context_db_handle(wrapper.ctx);
                    (void)db_from_ctx; // Mark as used
                }
            }
            sqlite3_finalize(func_stmt);
        }
        
        // Remove the function
        sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, nullptr, nullptr, nullptr, nullptr);
    }

    // 14. Cleanup
    if (value) {
        sqlite3_value_free(value);
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Clean up output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}