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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 153, nr_unique_branch: 127
//<Quality> {"density":17,"unique_branches":{"sqlite3_filename_database":[[182388,7,182388,19,0,0,4,1]],"sqlite3_set_last_insert_rowid":[[178699,7,178699,32,0,0,4,1]],"sqlite3_result_zeroblob":[[90111,35,90111,38,0,0,4,0]],"sqlite3_complete":[[177327,7,177327,14,0,0,4,1],[177333,10,177333,15,0,0,4,0],[177333,10,177333,15,0,0,4,1],[177335,7,177335,15,0,0,4,0],[177335,7,177335,15,0,0,4,1],[177339,7,177339,15,0,0,4,0],[177339,7,177339,15,0,0,4,1],[177340,7,177340,16,0,0,4,0],[177340,7,177340,16,0,0,4,1],[177341,7,177341,16,0,0,4,0],[177341,7,177341,16,0,0,4,1],[177342,7,177342,16,0,0,4,0],[177342,7,177342,16,0,0,4,1],[177343,7,177343,16,0,0,4,0],[177343,7,177343,16,0,0,4,1],[177347,7,177347,15,0,0,4,0],[177347,7,177347,15,0,0,4,1],[177348,13,177348,25,0,0,4,0],[177348,13,177348,25,0,0,4,1],[177353,16,177353,23,0,0,4,0],[177353,16,177353,23,0,0,4,1],[177353,28,177353,40,0,0,4,0],[177353,28,177353,40,0,0,4,1],[177353,44,177353,56,0,0,4,0],[177353,44,177353,56,0,0,4,1],[177354,13,177354,23,0,0,4,0],[177354,13,177354,23,0,0,4,1],[177359,7,177359,15,0,0,4,0],[177359,7,177359,15,0,0,4,1],[177360,13,177360,25,0,0,4,0],[177360,13,177360,25,0,0,4,1],[177364,16,177364,21,0,0,4,0],[177364,16,177364,21,0,0,4,1],[177364,25,177364,36,0,0,4,0],[177364,25,177364,36,0,0,4,1],[177365,13,177365,21,0,0,4,0],[177365,13,177365,21,0,0,4,1],[177369,7,177369,15,0,0,4,0],[177369,7,177369,15,0,0,4,1],[177371,16,177371,21,0,0,4,0],[177371,16,177371,21,0,0,4,1],[177371,25,177371,35,0,0,4,0],[177371,25,177371,35,0,0,4,1],[177372,13,177372,21,0,0,4,0],[177372,13,177372,21,0,0,4,1],[177376,7,177376,15,0,0,4,0],[177376,7,177376,15,0,0,4,1],[177377,7,177377,15,0,0,4,0],[177377,7,177377,15,0,0,4,1],[177378,7,177378,16,0,0,4,0],[177378,7,177378,16,0,0,4,1],[177381,16,177381,21,0,0,4,0],[177381,16,177381,21,0,0,4,1],[177381,25,177381,33,0,0,4,0],[177381,25,177381,33,0,0,4,1],[177382,13,177382,21,0,0,4,0],[177382,13,177382,21,0,0,4,1],[177386,7,177386,14,0,0,4,0],[177386,7,177386,14,0,0,4,1],[177398,13,177398,21,0,0,4,0],[177398,13,177398,21,0,0,4,1],[177398,23,177398,31,0,0,4,0],[177398,23,177398,31,0,0,4,1],[177399,19,177399,25,0,0,4,0],[177399,19,177399,25,0,0,4,1],[177399,29,177399,66,0,0,4,0],[177399,29,177399,66,0,0,4,1],[177406,13,177406,21,0,0,4,0],[177406,13,177406,21,0,0,4,1],[177406,23,177406,31,0,0,4,0],[177406,23,177406,31,0,0,4,1],[177407,19,177407,25,0,0,4,0],[177407,19,177407,25,0,0,4,1],[177407,29,177407,67,0,0,4,0],[177407,29,177407,67,0,0,4,1],[177409,25,177409,31,0,0,4,0],[177409,25,177409,31,0,0,4,1],[177409,35,177409,70,0,0,4,0],[177409,35,177409,70,0,0,4,1],[177411,25,177411,31,0,0,4,0],[177411,25,177411,31,0,0,4,1],[177411,35,177411,75,0,0,4,0],[177411,35,177411,75,0,0,4,1],[177418,13,177418,21,0,0,4,0],[177418,13,177418,21,0,0,4,1],[177418,24,177418,32,0,0,4,0],[177418,24,177418,32,0,0,4,1],[177419,19,177419,25,0,0,4,0],[177419,19,177419,25,0,0,4,1],[177419,29,177419,63,0,0,4,0],[177419,29,177419,63,0,0,4,1],[177423,19,177423,25,0,0,4,0],[177423,19,177423,25,0,0,4,1],[177423,29,177423,67,0,0,4,0],[177423,29,177423,67,0,0,4,1],[177432,13,177432,20,0,0,4,0],[177432,13,177432,20,0,0,4,1],[176500,20,176500,65,10,0,4,0],[176500,20,176500,65,10,0,4,1],[176500,20,176500,65,11,0,4,0],[176500,20,176500,65,11,0,4,1]],"sqlite3_db_status":[[23826,36,23826,47,0,0,4,1],[23826,50,23826,63,0,0,4,1],[23832,5,23832,40,0,0,4,0],[23834,11,23834,20,0,0,4,1],[23855,5,23855,39,0,0,4,1],[23856,5,23856,45,0,0,4,1],[23857,5,23857,45,0,0,4,1],[23876,5,23876,43,0,0,4,1],[23877,5,23877,36,0,0,4,1],[23903,5,23903,37,0,0,4,1],[23949,5,23949,35,0,0,4,1],[23973,5,23973,37,0,0,4,1],[23976,5,23976,35,0,0,4,1],[23977,5,23977,36,0,0,4,1],[23978,5,23978,37,0,0,4,1],[24001,5,24001,38,0,0,4,1],[24007,5,24007,12,0,0,4,1]],"sqlite3LookasideUsed":[[23810,7,23810,17,0,0,4,0]],"sqlite3_bind_text64":[[91257,7,91257,23,0,0,4,1]],"databaseName":[[182253,10,182253,22,0,0,4,0],[182253,26,182253,38,0,0,4,0],[182253,42,182253,54,0,0,4,0],[182253,58,182253,70,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_complete","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_bind_parameter_count","sqlite3_bind_text64","sqlite3_step","sqlite3_last_insert_rowid","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_filename_database","sqlite3_user_data","sqlite3_value_int","sqlite3_result_zeroblob","sqlite3_result_error_toobig","sqlite3_user_data","sqlite3_value_int","sqlite3_result_zeroblob","sqlite3_result_error_toobig","sqlite3_create_function_v2","sqlite3_exec","sqlite3_exec","sqlite3_db_status","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_complete","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_bind_parameter_count","sqlite3_bind_text64","sqlite3_step","sqlite3_last_insert_rowid","sqlite3_finalize","sqlite3_set_last_insert_rowid","sqlite3_filename_database","sqlite3_result_error_toobig","sqlite3_result_zeroblob","sqlite3_value_int","sqlite3_user_data","sqlite3_result_error_toobig","sqlite3_result_zeroblob","sqlite3_value_int","sqlite3_user_data","sqlite3_create_function_v2","sqlite3_exec","sqlite3_exec","sqlite3_db_status","sqlite3_finalize","sqlite3_close_v2"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified SQLite3 APIs to achieve the event of processing SQL statements from input data and performing various database operations:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	
    
    // Open memory stream for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open output file for results
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc;
    sqlite3_int64 last_rowid = 0;
    
    // 1. Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create a test table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT, blob_data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // 3. Check if we have complete SQL statements in input
    // Read first null-terminated string from input
    char sql_buffer[4096];
    size_t sql_len = 0;
    int c;
    while ((c = fgetc(in_file)) != EOF && c != 0 && sql_len < sizeof(sql_buffer) - 1) {
        sql_buffer[sql_len++] = static_cast<char>(c);
    }
    sql_buffer[sql_len] = '\0';
    
    // Use sqlite3_complete to check if SQL is complete
    int is_complete = sqlite3_complete(sql_buffer);
    
    if (is_complete && sql_len > 0) {
        // 4. Prepare and execute the SQL statement
        rc = sqlite3_prepare_v2(db, sql_buffer, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // 5. Use sqlite3_stmt_isexplain
            int is_explain = sqlite3_stmt_isexplain(stmt);
            
            // 6. Bind parameters if needed
            // Check if statement has parameters and bind text from remaining input
            int param_count = sqlite3_bind_parameter_count(stmt);
            if (param_count > 0) {
                // Read another string for binding
                char bind_buffer[2048];
                size_t bind_len = 0;
                while ((c = fgetc(in_file)) != EOF && c != 0 && bind_len < sizeof(bind_buffer) - 1) {
                    bind_buffer[bind_len++] = static_cast<char>(c);
                }
                bind_buffer[bind_len] = '\0';
                
                // Use sqlite3_bind_text64 to bind the text
                rc = sqlite3_bind_text64(stmt, 1, bind_buffer, bind_len, 
                                        SQLITE_TRANSIENT, SQLITE_UTF8);
            }
            
            // Execute the statement
            while (rc == SQLITE_OK && sqlite3_step(stmt) == SQLITE_ROW) {
                // Process row results if needed
            }
            
            // Get the last insert rowid
            last_rowid = sqlite3_last_insert_rowid(db);
            
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 7. Use sqlite3_set_last_insert_rowid
    sqlite3_set_last_insert_rowid(db, last_rowid);
    
    // 8. Simulate using sqlite3_filename_database (for a hypothetical filename)
    const char *filename = "test.db";
    const char *db_name = sqlite3_filename_database(filename);
    
    // 9. Create a custom function that uses sqlite3_result_zeroblob and sqlite3_result_error_toobig
    struct FuncContext {
        int call_count;
    };
    
    auto custom_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        FuncContext *fc = static_cast<FuncContext*>(sqlite3_user_data(ctx));
        fc->call_count++;
        
        if (argc > 0) {
            int data_size = sqlite3_value_int(argv[0]);
            
            // Use sqlite3_result_zeroblob for small sizes
            if (data_size > 0 && data_size <= 1024) {
                sqlite3_result_zeroblob(ctx, data_size);
            }
            // Use sqlite3_result_error_toobig for large sizes
            else if (data_size > 1024) {
                sqlite3_result_error_toobig(ctx);
            }
        }
    };
    
    FuncContext func_ctx = {0};
    rc = sqlite3_create_function_v2(
        db, "custom_func", 1, SQLITE_UTF8, &func_ctx,
        custom_func, nullptr, nullptr,
        [](void *p) { /* cleanup */ }
    );
    
    // Test the custom function
    if (rc == SQLITE_OK) {
        const char *test_sql = "SELECT custom_func(512);";
        sqlite3_exec(db, test_sql, nullptr, nullptr, nullptr);
        
        const char *test_sql2 = "SELECT custom_func(2048);";
        sqlite3_exec(db, test_sql2, nullptr, nullptr, nullptr);
    }
    
    // 10. Use sqlite3_db_status to get database status
    int current_conns = 0;
    int highwater_conns = 0;
    sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current_conns, &highwater_conns, 0);
    
    // Write some debug info to output file
    fprintf(out_file, "SQL Complete: %d\n", is_complete);
    fprintf(out_file, "Last RowID: %lld\n", static_cast<long long>(last_rowid));
    fprintf(out_file, "Current connections: %d\n", current_conns);
    fprintf(out_file, "DB Name from filename: %s\n", db_name ? db_name : "null");
    fprintf(out_file, "Custom function calls: %d\n", func_ctx.call_count);
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Close database
    sqlite3_close_v2(db);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}