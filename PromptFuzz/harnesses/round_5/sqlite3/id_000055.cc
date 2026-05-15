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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 1
//<Quality> {"density":17,"unique_branches":{"sqlite3CreateFunc":[[179481,21,179481,30,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_str_new","sqlite3_close","sqlite3_shutdown","sqlite3_str_length","sqlite3_create_function","sqlite3_memory_alarm","sqlite3_db_status","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_str_finish","sqlite3_free","sqlite3_close","sqlite3_free"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_str_new","sqlite3_str_length","sqlite3_create_function","sqlite3_memory_alarm","sqlite3_db_status","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_str_finish","sqlite3_free","sqlite3_close","sqlite3_free"],"visited":0}
/**/


// Custom function callbacks
static void  id_000055_custom_func(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        const void* text = sqlite3_value_text(argv[0]);
        if (text) {
            sqlite3_result_text(context, static_cast<const char*>(text), -1, SQLITE_TRANSIENT);
        } else {
            sqlite3_result_null(context);
        }
    } else {
        sqlite3_result_int64(context, 42);
    }
}

static void  id_000055_custom_step(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Aggregation step function - does nothing for this example
}

static void  id_000055_custom_final(sqlite3_context* context) {
    sqlite3_result_int64(context, 100);
}

// Memory alarm callback
static void  id_000055_memory_alarm_callback(void* data, sqlite3_int64 nBytes, int flags) {
    // Do nothing for fuzzing
}

// Exec callback
static int  id_000055_exec_callback(void* data, int argc, char** argv, char** colNames) {
    return 0; // Continue processing
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        sqlite3_shutdown();
        assert_file_closed(&out_file);
	return 0;
    }
    
    // Write input data to a temporary file for database
    const char* db_filename = "fuzz_temp.db";
    FILE* db_file = fopen(db_filename, "wb");
    if (db_file) {
        fwrite(data, 1, size, db_file);
        assert_file_closed(&db_file);;
    }
    
    sqlite3* db = nullptr;
    int rc;
    
    // Open database
    rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&out_file);;
        remove(db_filename);
        sqlite3_shutdown();
        assert_file_closed(&db_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    // Create string builder
    sqlite3_str* str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        assert_file_closed(&out_file);;
        remove(db_filename);
        sqlite3_shutdown();
        assert_file_closed(&db_file);
	assert_file_closed(&out_file);
	return 0;
    }
    
    // Get string length (will be 0 initially)
    int str_len = sqlite3_str_length(str);
    
    // Create custom function
    rc = sqlite3_create_function(
        db, 
        "fuzz_func", 
        1, 
        SQLITE_UTF8, 
        nullptr, 
         id_000055_custom_func, 
         id_000055_custom_step, 
         id_000055_custom_final
    );
    
    // Set memory alarm
    sqlite3_memory_alarm( id_000055_memory_alarm_callback, nullptr, 1000);
    
    // Get database status
    int current = 0, highwater = 0;
    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current, &highwater, 0);
    
    // Prepare a simple statement
    sqlite3_stmt* stmt = nullptr;
    const char* tail = nullptr;
    rc = sqlite3_prepare_v2(
        db, 
        "SELECT fuzz_func(?), name FROM sqlite_master WHERE type='table'", 
        -1, 
        &stmt, 
        &tail
    );
    
    if (rc == SQLITE_OK && stmt) {
        // Bind parameter
        sqlite3_bind_int64(stmt, 1, static_cast<sqlite3_int64>(size));
        
        // Execute statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column value
            sqlite3_value* col_value = sqlite3_column_value(stmt, 0);
            if (col_value) {
                // Try to get as pointer (will likely fail, but we try)
                void* ptr = sqlite3_value_pointer(col_value, "test");
                // Write to output file if we got something
                if (ptr) {
                    fwrite(&ptr, sizeof(ptr), 1, out_file);
                }
            }
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Execute SQL using sqlite3_exec
    char* errmsg = nullptr;
    rc = sqlite3_exec(
        db, 
        "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER, data BLOB);"
        "INSERT INTO fuzz_test VALUES (1, zeroblob(100));"
        "SELECT * FROM fuzz_test;",
         id_000055_exec_callback,
        nullptr,
        &errmsg
    );
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Finish and free the string builder
    char* str_result = sqlite3_str_finish(str);
    if (str_result) {
        // Write string result to output file
        fwrite(str_result, 1, strlen(str_result), out_file);
        sqlite3_free(str_result);
    }
    
    // Close database
    sqlite3_close(db);
    
    // Clean up
    assert_file_closed(&out_file);;
    remove(db_filename);
    
    // Free any remaining allocations
    sqlite3_free(nullptr); // Safe to call with nullptr
    
    // Don't call shutdown here as it might interfere with fuzzer
    // sqlite3_shutdown();
    
    assert_file_closed(&db_file);
	assert_file_closed(&out_file);
	return 0;
}