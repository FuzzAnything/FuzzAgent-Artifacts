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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 72, nr_unique_branch: 3
//<Quality> {"density":24,"unique_branches":{"sqlite3LookasideUsed":[[23810,7,23810,17,0,0,4,0]],"sqlite3_db_status":[[23832,5,23832,40,0,0,4,0],[23834,11,23834,20,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_str_new","sqlite3_close","sqlite3_shutdown","sqlite3_str_appendchar","sqlite3_str_length","sqlite3_str_finish","sqlite3_create_function","sqlite3_free","sqlite3_close","sqlite3_shutdown","sqlite3_memory_alarm","sqlite3_db_status","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_length","sqlite3_str_finish","sqlite3_create_function","sqlite3_memory_alarm","sqlite3_db_status","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_free","sqlite3_free","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to create a custom function, process input data, and perform database operations:

*/


// Custom function callback for sqlite3_create_function
static void  id_000058_custom_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc >= 1) {
        const void* ptr = sqlite3_value_pointer(argv[0], "custom_type");
        if (ptr) {
            sqlite3_result_pointer(context, (void*)ptr, "custom_type", nullptr);
        } else {
            sqlite3_result_int64(context, sqlite3_value_int64(argv[0]));
        }
    }
}

// Callback for sqlite3_exec
static int  id_000058_exec_callback(void* data, int argc, char** argv, char** col_names) {
    (void)data; (void)argc; (void)argv; (void)col_names;
    return 0;
}

// Memory alarm callback
static void  id_000058_memory_alarm_callback(void* data, sqlite3_int64 used, int threshold) {
    (void)data; (void)used; (void)threshold;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize SQLite3
    if (sqlite3_initialize() != SQLITE_OK) {
	return 0;
	}
	
    
    // Create input file from data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Write data to a file that SQLite can open
    const char* filename = "input_file";
    FILE* out_file = fopen(filename, "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Copy data from in_file to out_file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_str* str_obj = nullptr;
    char* str_result = nullptr;
    char* err_msg = nullptr;
    
    // 2. Open database
    if (sqlite3_open(filename, &db) != SQLITE_OK) {
        remove(filename);
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 3. Create sqlite3_str object
    str_obj = sqlite3_str_new(db);
    if (!str_obj) {
        sqlite3_close(db);
        remove(filename);
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Use some data to build string
    size_t str_len = size < 100 ? size : 100;
    for (size_t i = 0; i < str_len; i++) {
        sqlite3_str_appendchar(str_obj, 1, data[i] ? data[i] : ' ');
    }
    
    // 4. Get string length
    int length = sqlite3_str_length(str_obj);
    (void)length; // Use length to avoid unused variable warning
    
    // 5. Finish string
    str_result = sqlite3_str_finish(str_obj);
    
    // 6. Create custom function
    if (sqlite3_create_function(db, " id_000058_custom_func", 1, SQLITE_UTF8, nullptr,
                                 id_000058_custom_func, nullptr, nullptr) != SQLITE_OK) {
        if (str_result) sqlite3_free(str_result);
        sqlite3_close(db);
        remove(filename);
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 7. Set memory alarm
    sqlite3_int64 alarm_threshold = (sqlite3_int64)(size * 2);
    sqlite3_memory_alarm( id_000058_memory_alarm_callback, nullptr, alarm_threshold);
    
    // 8. Get database status
    int current = 0, highwater = 0;
    sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current, &highwater, 0);
    
    // 9. Execute SQL using sqlite3_exec
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_table_sql,  id_000058_exec_callback, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Insert data using prepared statement
    const char* insert_sql = "INSERT INTO test(data) VALUES(?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind data as integer (using first byte)
        sqlite3_int64 bind_value = size > 0 ? data[0] : 0;
        sqlite3_bind_int64(stmt, 1, bind_value);
        
        // Execute statement
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Query using custom function
    const char* query_sql = "SELECT  id_000058_custom_func(id) FROM test;";
    if (sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column value
            sqlite3_value* col_value = sqlite3_column_value(stmt, 0);
            if (col_value) {
                // Try to get as pointer
                void* ptr = sqlite3_value_pointer(col_value, "custom_type");
                (void)ptr; // Use ptr to avoid unused variable warning
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    if (str_result) sqlite3_free(str_result);
    if (err_msg) sqlite3_free(err_msg);
    
    // 10. Close database
    sqlite3_close(db);
    
    // Remove temporary file
    remove(filename);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}