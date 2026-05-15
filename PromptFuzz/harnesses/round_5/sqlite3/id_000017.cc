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
//<ID> 84
//<Prompt> []
/*<Combination>: [
*/
//<score> 57, nr_unique_branch: 5
//<Quality> {"density":19,"unique_branches":{"databaseName":[[182253,10,182253,22,0,0,4,0],[182253,26,182253,38,0,0,4,0],[182253,42,182253,54,0,0,4,0],[182253,58,182253,70,0,0,4,0]],"bindText":[[91145,9,91145,17,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_thread_cleanup","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_filename_database","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_db_release_memory","sqlite3_close","sqlite3_thread_cleanup","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_thread_cleanup","sqlite3_open","sqlite3_filename_database","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_db_release_memory","sqlite3_close","sqlite3_thread_cleanup","sqlite3_shutdown"],"visited":0}
/**/


// Custom callback function to handle potential errors
static void  id_000017_dummy_callback(void* param) {
    // Empty callback for blob destruction
}

// User-defined function callback that uses sqlite3_result_error_toobig
static void  id_000017_test_func(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // Check if we have a blob argument
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_BLOB) {
        int blob_size = sqlite3_value_bytes(argv[0]);
        // Trigger error if blob is too large (using arbitrary threshold)
        if (blob_size > 1000000) {
            sqlite3_result_error_toobig(ctx);
        } else {
            sqlite3_result_int(ctx, blob_size);
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    FILE* in_file = nullptr;
    const char* db_filename = "fuzz_test.db";
    
    // Clean up any previous thread-specific resources
    sqlite3_thread_cleanup();
    
    // Create a FILE* from the input data
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get the database filename (for demonstration purposes)
    const char* filename = sqlite3_filename_database(db_filename);
    
    // Create a test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB);";
    
    if (sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr) != SQLITE_OK) {
        // Clean up and continue
    }
    
    // Create a custom function that uses sqlite3_result_error_toobig
    sqlite3_create_function(
        db,
        "test_function",
        1,  // Number of arguments
        SQLITE_UTF8,
        nullptr,
         id_000017_test_func,
        nullptr,
        nullptr
    );
    
    // Prepare a statement to insert data
    const char* insert_sql = "INSERT INTO test_table (data) VALUES (?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind blob from input data
        // Use first part of data as blob, ensure we don't overflow
        size_t blob_size = size > 1024 ? 1024 : size;
        
        if (sqlite3_bind_blob(stmt, 1, data, (int)blob_size,  id_000017_dummy_callback) == SQLITE_OK) {
            // Execute the statement
            sqlite3_step(stmt);
        }
        
        // Reset statement for potential reuse
        sqlite3_reset(stmt);
        
        // Test binding different sizes
        if (size > 0) {
            // Bind a very large blob (potential trigger for error_toobig in custom function)
            int large_size = size > 100 ? 100 : (int)size;
            sqlite3_bind_blob(stmt, 1, data, large_size, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
        }
        
        // Test with zero-length blob
        sqlite3_bind_blob(stmt, 1, nullptr, 0, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        // Clean up statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Test the custom function with a query
    const char* select_sql = "SELECT test_function(data) FROM test_table;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process results if needed
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Try to release memory from database
    sqlite3_db_release_memory(db);
    
    // Clean up
    if (in_file) {
        assert_file_closed(&in_file);;
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    // Clean up thread-specific resources again
    sqlite3_thread_cleanup();
    
    // Final shutdown
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	return 0;
}