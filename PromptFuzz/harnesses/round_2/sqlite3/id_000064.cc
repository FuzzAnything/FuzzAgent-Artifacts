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
//<ID> 442
//<Prompt> []
/*<Combination>: [
*/
//<score> 58, nr_unique_branch: 4
//<Quality> {"density":29,"unique_branches":{"btreeBeginTrans":[[73352,45,73352,51,0,0,4,0],[73416,11,73416,45,0,0,4,0]],"verifyDbFile":[[39382,7,39382,26,0,0,4,0]],"sqlite3InitOne":[[140533,11,140533,49,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_value_text16","sqlite3_result_text16","sqlite3_value_text16","sqlite3_result_text16","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_text","sqlite3_prepare16_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_reset","sqlite3_bind_int","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_value_free","sqlite3_filename_database","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_result_text16","sqlite3_value_text16","sqlite3_result_text16","sqlite3_value_text16","sqlite3_create_function16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_prepare16_v2","sqlite3_step","sqlite3_finalize","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_reset","sqlite3_bind_int","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_value_free","sqlite3_filename_database","sqlite3_stricmp","sqlite3_total_changes64","sqlite3_memory_used","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt *stmt16 = nullptr;
    sqlite3_str *str_obj = nullptr;
    sqlite3_value *value_dup = nullptr;
    char *err_msg = nullptr;
    char *str_result = nullptr;
    int rc = SQLITE_OK;
    
    // Write input data to a file to simulate database file
    FILE *in_file = fopen("input_file", "wb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    fwrite(data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    // Open database from the file we just created
    rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK || !db) {
        remove("input_file");
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Custom function implementation
    auto xFunc = [](sqlite3_context *context, int argc, sqlite3_value **argv) {
        if (argc > 0) {
            const void *text = sqlite3_value_text16(argv[0]);
            if (text) {
                sqlite3_result_text16(context, text, -1, SQLITE_TRANSIENT);
            }
        }
    };
    
    // Create custom function
    sqlite3_create_function16(db, L"myfunc", 1, SQLITE_UTF16, nullptr, 
                             xFunc, nullptr, nullptr);
    
    // Create a table using sqlite3_exec
    const char *create_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, create_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Prepare INSERT statement
    const char *insert_sql = 
        "INSERT INTO test(id, name, data) VALUES(?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind parameters
        int id_val = (size > 0) ? data[0] : 1;
        sqlite3_bind_int(stmt, 1, id_val);
        
        char name_buf[64];
        snprintf(name_buf, sizeof(name_buf), "test_%zu", size);
        sqlite3_bind_text(stmt, 2, name_buf, -1, SQLITE_TRANSIENT);
        
        // Bind blob from input data
        if (size > 0) {
            sqlite3_bind_blob(stmt, 3, data, 
                             (size > 1000) ? 1000 : (int)size, 
                             SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
        
        // Execute INSERT
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Prepare SELECT statement
        const char *select_sql = "SELECT * FROM test WHERE id = ?;";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_bind_int(stmt, 1, id_val);
            
            // Execute SELECT
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int col_count = sqlite3_column_count(stmt);
                for (int i = 0; i < col_count; i++) {
                    int col_type = sqlite3_column_type(stmt, i);
                    switch (col_type) {
                        case SQLITE_BLOB: {
                            const void *blob = sqlite3_column_blob(stmt, i);
                            int blob_size = sqlite3_column_bytes(stmt, i);
                            // Use the blob data
                            break;
                        }
                        case SQLITE_TEXT: {
                            const unsigned char *text = sqlite3_column_text(stmt, i);
                            // Use the text data
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Prepare UTF-16 statement
    const void *select16_sql = L"SELECT myfunc(name) FROM test;";
    rc = sqlite3_prepare16_v2(db, select16_sql, -1, &stmt16, nullptr);
    if (rc == SQLITE_OK && stmt16) {
        while (sqlite3_step(stmt16) == SQLITE_ROW) {
            // Process row
        }
        sqlite3_finalize(stmt16);
    }
    
    // Create and use sqlite3_str
    str_obj = sqlite3_str_new(db);
    if (str_obj) {
        sqlite3_str_append(str_obj, "Test string", 11);
        sqlite3_str_append(str_obj, " appended", 9);
        str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    // Use sqlite3_value_dup
    if (stmt) {
        sqlite3_reset(stmt);
        sqlite3_bind_int(stmt, 1, 1);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            if (val) {
                value_dup = sqlite3_value_dup(val);
                if (value_dup) {
                    sqlite3_value_free(value_dup);
                }
            }
        }
    }
    
    // Use sqlite3_filename_database
    const char *filename = "input_file";
    const char *db_name = sqlite3_filename_database((sqlite3_filename)filename);
    
    // Use sqlite3_stricmp
    int cmp_result = sqlite3_stricmp("test1", "test2");
    
    // Get statistics
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    sqlite3_int64 memory_used = sqlite3_memory_used();
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    // Remove temporary file
    remove("input_file");
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}