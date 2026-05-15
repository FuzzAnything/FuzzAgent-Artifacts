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
//<ID> 290
//<Prompt> []
/*<Combination>: [
*/
//<score> 80, nr_unique_branch: 4
//<Quality> {"density":20,"unique_branches":{"sqlite3VdbeExec":[[93039,7,93039,20,0,0,4,0]],"sqlite3_str_errcode":[[31829,10,31829,11,0,0,4,0]],"sqlite3_progress_handler":[[179387,7,179387,32,0,0,4,1],[179393,7,179393,13,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_finalize","sqlite3_close","sqlite3_str_finish","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_errcode","sqlite3_progress_handler","sqlite3_user_data","sqlite3_result_int","sqlite3_result_null","sqlite3_user_data","sqlite3_result_int","sqlite3_result_null","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_finish","sqlite3_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_errcode","sqlite3_progress_handler","sqlite3_result_null","sqlite3_result_int","sqlite3_user_data","sqlite3_result_null","sqlite3_result_int","sqlite3_user_data","sqlite3_create_function_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_finish","sqlite3_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t* f_data, size_t f_size) {
	if(f_size<86) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
	//fuzzer shim end}




    // Early return if insufficient data
    if (size < 1) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_str *str_obj = nullptr;
    FILE *in_file = nullptr;
    FILE *out_file = nullptr;
    int rc = SQLITE_OK;
    
    // 1. Create FILE* for input data using fmemopen
    in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Create FILE* for output
    out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        cleanup: // Label for cleanup
        if (in_file) assert_file_closed(&in_file);;
        if (out_file) assert_file_closed(&out_file);;
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        if (str_obj) sqlite3_str_finish(str_obj);
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 4. Create a table and insert data using input
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) goto cleanup;
    
    // 5. Prepare INSERT statement with parameters
    const char *insert_sql = "INSERT INTO test(data) VALUES(?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) goto cleanup;
    
    // 6. Use sqlite3_bind_parameter_count() - API #1
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Bind the input data as blob if there's at least one parameter
    if (param_count > 0) {
        // Use first 1024 bytes or entire input if smaller
        size_t bind_size = size > 1024 ? 1024 : size;
        rc = sqlite3_bind_blob(stmt, fuzz_int32_t_2, data, bind_size, SQLITE_STATIC);
        if (rc != SQLITE_OK) goto cleanup;
        
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) goto cleanup;
        
        sqlite3_reset(stmt);
    }
    
    // 7. Use sqlite3_stmt_explain() - API #2 (if available)
    // Note: This is an internal API, so we'll simulate its usage
    // by preparing an EXPLAIN query
    const char *explain_sql = "EXPLAIN SELECT * FROM test;";
    sqlite3_stmt *explain_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, explain_sql, -1, &explain_stmt, nullptr);
    if (rc == SQLITE_OK && explain_stmt) {
        // Process explain output
        while (sqlite3_step(explain_stmt) == SQLITE_ROW) {
            // Just consume the rows
        }
        sqlite3_finalize(explain_stmt);
    }
    
    // 8. Create and use sqlite3_str object - APIs #3 and #4
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        // Build a string from input data (use printable chars only)
        for (size_t i = 0; i < size && i < 100; i++) {
            if (data[i] >= 32 && data[i] <= 126) {
                sqlite3_str_appendchar(str_obj, fuzz_int32_t_3, (char)data[i]);
            }
        }
        
        // Check error code using sqlite3_str_errcode() - API #3
        int str_err = sqlite3_str_errcode(str_obj);
        (void)str_err; // Use the result to avoid unused variable warning
    }
    
    // 9. Use sqlite3_progress_handler() - API #4
    // Set progress handler that checks every 10 operations
    sqlite3_progress_handler(db, fuzz_int32_t_4, [](void* user_data) -> int {
        // Return non-zero to interrupt operation
        return 0; // Don't interrupt
    }, nullptr);
    
    // 10. Use sqlite3_user_data() - API #5
    // Create a scalar function to demonstrate sqlite3_user_data
    struct UserData { int value; };
    UserData ud = {42};
    
    auto scalar_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        // Get user data
        UserData* ud_ptr = (UserData*)sqlite3_user_data(ctx);
        if (ud_ptr) {
            sqlite3_result_int(ctx, ud_ptr->value);
        } else {
            sqlite3_result_null(ctx);
        }
    };
    
    rc = sqlite3_create_function_v2(db, fuzz_str_5, fuzz_int32_t_6, fuzz_int32_t_7, &ud,
                                     scalar_func, nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        // Call the function
        const char *func_sql = "SELECT myfunc();";
        sqlite3_stmt *func_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, nullptr);
        if (rc == SQLITE_OK && func_stmt) {
            if (sqlite3_step(func_stmt) == SQLITE_ROW) {
                // Result consumed
            }
            sqlite3_finalize(func_stmt);
        }
    }
    
    // 11. Use sqlite3_database_file_object() - API #6 (if available)
    // This is typically an internal API, but we'll attempt to use it
    // with the main database name
    const char* main_db = "main";
    // Note: sqlite3_database_file_object is not in the provided API list,
    // so we'll comment this out to avoid compilation errors
    // sqlite3_file* db_file = sqlite3_database_file_object(main_db);
    
    // 12. Use file descriptors if needed
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; // Use to avoid unused variable warning
    (void)out_fd;
    
    // 13. Write some output using the output file
    const char* output_msg = "Fuzzing completed\n";
    fwrite(output_msg, 1, strlen(output_msg), out_file);
    
    // 14. Cleanup
    if (str_obj) {
        char* str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            // Write string result to output file
            fwrite(str_result, 1, strlen(str_result), out_file);
            sqlite3_free(str_result);
        }
        str_obj = nullptr;
    }
    
    if (in_file) assert_file_closed(&in_file);;
    if (out_file) assert_file_closed(&out_file);;
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}