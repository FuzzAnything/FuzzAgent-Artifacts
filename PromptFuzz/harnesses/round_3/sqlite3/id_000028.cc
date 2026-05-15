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
//<ID> 248
//<Prompt> []
/*<Combination>: [
*/
//<score> 75, nr_unique_branch: 2
//<Quality> {"density":25,"unique_branches":{"sqlite3_result_error_nomem":[[90160,7,90160,14,0,0,4,1]],"sqlite3OomFault":[[30651,9,30651,19,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare16","sqlite3_stmt_scanstatus_v2","sqlite3_column_database_name16","sqlite3_table_column_metadata","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_value","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_step","sqlite3_step","sqlite3_result_error_nomem","sqlite3_result_error_nomem","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_finish","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare16","sqlite3_stmt_scanstatus_v2","sqlite3_column_database_name16","sqlite3_table_column_metadata","sqlite3_str_new","sqlite3_str_appendf","sqlite3_str_value","sqlite3_prepare_v2","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_transfer_bindings","sqlite3_finalize","sqlite3_step","sqlite3_step","sqlite3_result_error_nomem","sqlite3_result_error_nomem","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_finish","sqlite3_finalize","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<110) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}




    // Create a temporary file to store the input data as a SQLite database
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for any potential output
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt1 = nullptr;
    sqlite3_stmt *stmt2 = nullptr;
    sqlite3_str *str_obj = nullptr;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Try to read the input as a SQLite database file
    // We'll attempt to use the input data as database content
    if (size > 0) {
        // Create a table using input data as table/column names
        char create_table[512];
        snprintf(create_table, sizeof(create_table),
                "CREATE TABLE IF NOT EXISTS test_table ("
                "col1 BLOB, "
                "col2 TEXT);");
        rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
        
        // Insert some data using the input
        if (size > 10) {
            sqlite3_stmt *insert_stmt;
            const char *insert_sql = "INSERT INTO test_table VALUES (?, ?)";
            rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr);
            if (rc == SQLITE_OK) {
                // Use first part of input as blob
                size_t blob_size = size > 100 ? 100 : size;
                rc = sqlite3_bind_blob(insert_stmt, fuzz_int32_t_1, data, blob_size, SQLITE_STATIC);
                
                // Use remaining as text (truncated to safe length)
                size_t text_len = size > 200 ? 200 : size;
                char *text_data = (char *)malloc(text_len + 1);
                if (text_data) {
                    memcpy(text_data, data, text_len > size ? size : text_len);
                    text_data[text_len] = '\0';
                    rc = sqlite3_bind_text(insert_stmt, fuzz_int32_t_2, text_data, -1, SQLITE_TRANSIENT);
                    free(text_data);
                }
                
                sqlite3_step(insert_stmt);
                sqlite3_finalize(insert_stmt);
            }
        }
    }
    
    // 1. Use sqlite3_prepare16 to prepare a statement with UTF-16 SQL
    const wchar_t *utf16_sql = L"SELECT * FROM test_table";
    rc = sqlite3_prepare16(db, fuzz_str_3, static_cast<int>(fuzz_str_sz_3), &stmt1, nullptr);
    
    if (rc == SQLITE_OK && stmt1) {
        // 2. Use sqlite3_stmt_scanstatus_v2
        int scan_status;
        rc = sqlite3_stmt_scanstatus_v2(stmt1, fuzz_int32_t_4, fuzz_int32_t_5, fuzz_int32_t_6, &scan_status);
        
        // 3. Use sqlite3_column_database_name16
        const void *db_name = sqlite3_column_database_name16(stmt1, fuzz_int32_t_7);
        
        // 4. Use sqlite3_table_column_metadata
        const char *data_type = nullptr;
        const char *coll_seq = nullptr;
        int not_null = 0, primary_key = 0, autoinc = 0;
        rc = sqlite3_table_column_metadata(db, fuzz_str_8, fuzz_str_9, fuzz_str_10,
                                          &data_type, &coll_seq, 
                                          &not_null, &primary_key, &autoinc);
        
        // 5. Create and use sqlite3_str
        str_obj = sqlite3_str_new(db);
        if (str_obj) {
            sqlite3_str_appendf(str_obj, "Test string with data: %zu bytes", size);
            // 6. Use sqlite3_str_value
            char *str_value = sqlite3_str_value(str_obj);
            if (str_value && out_file) {
                fwrite(str_value, 1, strlen(str_value), out_file);
            }
        }
        
        // Prepare another statement for transfer_bindings
        const char *sql2 = "SELECT * FROM test_table WHERE col1 IS NOT NULL";
        rc = sqlite3_prepare_v2(db, sql2, -1, &stmt2, nullptr);
        
        if (stmt2) {
            // 7. Use sqlite3_transfer_bindings (if we had bindings)
            // Since we don't have bindings in this example, we'll create some
            sqlite3_stmt *stmt_with_bindings = nullptr;
            const char *sql_bind = "SELECT ?";
            rc = sqlite3_prepare_v2(db, sql_bind, -1, &stmt_with_bindings, nullptr);
            if (stmt_with_bindings) {
                sqlite3_bind_int(stmt_with_bindings, fuzz_int32_t_11, fuzz_int32_t_12);
                // Now transfer to stmt2 (though stmt2 doesn't expect this binding)
                // This will likely fail but we're calling the API as requested
                rc = sqlite3_transfer_bindings(stmt_with_bindings, stmt2);
                sqlite3_finalize(stmt_with_bindings);
            }
            
            // Execute the statements
            while (sqlite3_step(stmt1) == SQLITE_ROW) {
                // Process row data if needed
            }
            
            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                // Process row data if needed
            }
        }
    }
    
    // 8. Use sqlite3_result_error_nomem in a user-defined function context
    // We need to create a custom function to use this
    struct UserData {
        int dummy;
    };
    
    auto xFunc = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        // Simulate out of memory condition
        sqlite3_result_error_nomem(ctx);
    };
    
    rc = sqlite3_create_function(db, fuzz_str_13, fuzz_int32_t_14, fuzz_int32_t_15, 
                                 nullptr, xFunc, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        sqlite3_stmt *test_stmt;
        rc = sqlite3_prepare_v2(db, "SELECT test_nomem()", -1, &test_stmt, nullptr);
        if (rc == SQLITE_OK) {
            sqlite3_step(test_stmt);
            sqlite3_finalize(test_stmt);
        }
    }
    
    // Cleanup
    if (str_obj) {
        sqlite3_str_finish(str_obj);
    }
    
    if (stmt1) {
        sqlite3_finalize(stmt1);
    }
    
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove the output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}