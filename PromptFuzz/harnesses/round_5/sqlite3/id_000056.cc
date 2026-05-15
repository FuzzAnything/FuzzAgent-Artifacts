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
//<ID> 317
//<Prompt> []
/*<Combination>: [
*/
//<score> 312, nr_unique_branch: 21
//<Quality> {"density":26,"unique_branches":{"sqlite3DeleteColumnNames":[[120924,9,120924,28,0,0,4,1]],"sqlite3AtoF":[[35114,30,35114,36,0,0,4,1]],"applyAffinity":[[14098,27,14098,28,12,0,4,1],[14098,27,14098,28,13,0,4,0]],"sqlite3DbFreeNN":[[30264,9,30264,25,0,0,4,0]],"sqlite3DeleteTable":[[121011,7,121011,26,0,0,4,1]],"yy_reduce":[[174844,7,174844,15,0,0,4,0],[174853,7,174853,11,0,0,4,1],[174854,7,174854,28,0,0,4,0]],"sqlite3DbNNFreeNN":[[30303,7,30303,23,0,0,4,0]],"sqlite3_db_status":[[23903,5,23903,37,0,0,4,0],[23911,16,23911,25,0,0,4,0],[23911,16,23911,25,0,0,4,1],[23927,54,23927,55,0,0,4,1],[23930,53,23930,54,0,0,4,0],[23930,53,23930,54,0,0,4,1],[14155,25,14155,46,16,0,4,0],[14155,26,14155,29,16,0,4,0]],"sqlite3BinaryCompareCollSeq":[[108362,23,108362,54,0,0,4,0]],"sqlite3Atoi64":[[35269,13,35269,27,0,0,4,1]],"sqlite3_msize":[[30202,10,30202,11,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_str_new","sqlite3_close","sqlite3_shutdown","sqlite3_str_appendchar","sqlite3_str_length","sqlite3_str_finish","sqlite3_close","sqlite3_shutdown","sqlite3_create_function","sqlite3_free","sqlite3_close","sqlite3_shutdown","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_close","sqlite3_shutdown","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_db_status","sqlite3_finalize","sqlite3_memory_alarm","sqlite3_db_status","sqlite3_free","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_length","sqlite3_str_finish","sqlite3_create_function","sqlite3_exec","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_column_value","sqlite3_value_pointer","sqlite3_db_status","sqlite3_finalize","sqlite3_memory_alarm","sqlite3_db_status","sqlite3_free","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom function implementation
static void  id_000056_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Simple function that returns the first argument as integer
    if (argc > 0) {
        sqlite3_result_int64(context, sqlite3_value_int64(argv[0]));
    }
}

// Memory alarm callback
static void  id_000056_memory_alarm_callback(void* data, sqlite3_int64 used, int N) {
    // Simple callback that does nothing for fuzzing
}

// Exec callback
static int  id_000056_exec_callback(void* data, int argc, char** argv, char** colNames) {
    return 0; // Continue processing
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Create a temporary database in memory
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a string builder
    sqlite3_str* str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Append the input data to the string builder (limit to avoid excessive memory)
    size_t append_size = size > 1000 ? 1000 : size;
    for (size_t i = 0; i < append_size; i++) {
        sqlite3_str_appendchar(str, 1, (char)data[i]);
    }
    
    // Get the length of the built string
    int str_length = sqlite3_str_length(str);
    
    // Finish the string
    char* built_string = sqlite3_str_finish(str);
    if (!built_string) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a custom function
    if (sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, nullptr, 
                                 id_000056_custom_function, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_free(built_string);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a table using exec
    const char* create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data TEXT);";
    char* err_msg = nullptr;
    if (sqlite3_exec(db, create_table_sql,  id_000056_exec_callback, nullptr, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_free(built_string);
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Insert data using the built string
    char insert_sql[512];
    snprintf(insert_sql, sizeof(insert_sql), 
             "INSERT INTO test VALUES (%d, '%s');", 
             str_length, built_string);
    
    if (sqlite3_exec(db, insert_sql,  id_000056_exec_callback, nullptr, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    
    // Prepare a statement
    sqlite3_stmt* stmt = nullptr;
    const char* select_sql = "SELECT custom_func(id), data FROM test;";
    const char* tail = nullptr;
    
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Bind a parameter if the statement has parameters
        // (this example doesn't have parameters, but we'll demonstrate the API)
        if (size > 0) {
            // Use first byte as integer for binding
            sqlite3_int64 bind_value = (sqlite3_int64)data[0];
            sqlite3_bind_int64(stmt, 1, bind_value);
        }
        
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column value
            sqlite3_value* col_value = sqlite3_column_value(stmt, 0);
            
            // Try to get pointer from value (will likely return null)
            void* ptr = sqlite3_value_pointer(col_value, "test_type");
            
            // Get database status
            int current = 0, highwater = 0;
            sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current, &highwater, 0);
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Set memory alarm
    sqlite3_memory_alarm( id_000056_memory_alarm_callback, nullptr, 1000000);
    
    // Get database status again
    int current = 0, highwater = 0;
    sqlite3_db_status(db, SQLITE_DBSTATUS_SCHEMA_USED, &current, &highwater, 1);
    
    // Clean up
    sqlite3_free(built_string);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}