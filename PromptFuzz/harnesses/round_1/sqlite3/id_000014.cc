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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 51, nr_unique_branch: 5
//<Quality> {"density":17,"unique_branches":{"sqlite3_errmsg16":[[180284,7,180284,10,0,0,4,1],[180287,7,180287,38,0,0,4,1],[180291,7,180291,23,0,0,4,1],[180295,9,180295,13,0,0,4,1]],"sqlite3OomClear":[[30672,7,30672,23,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_close","sqlite3_deserialize","sqlite3_close","sqlite3_update_hook","sqlite3_create_function_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_errmsg16","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_deserialize","sqlite3_update_hook","sqlite3_create_function_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_errmsg16","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to create an event that deserializes a database, sets up update hooks, binds zero blobs, and uses result subtypes:

*/


// Callback for update hook
static void  id_000014_update_hook_callback(void* user_data, int op, const char* db_name, 
                                 const char* table_name, sqlite3_int64 rowid) {
    // Can be used to monitor database updates
    (void)user_data;
    (void)op;
    (void)db_name;
    (void)table_name;
    (void)rowid;
}

// Custom function that uses sqlite3_result_subtype
static void  id_000014_custom_function(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    (void)argc;
    (void)argv;
    // Set result with subtype
    sqlite3_result_text(ctx, "test", 4, SQLITE_TRANSIENT);
    sqlite3_result_subtype(ctx, 123); // Set a subtype
}

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1024) {
	return 0;
	}
	 // Need minimum data
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    
    // 1. Create an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
	return 0;
	}
	
    
    // 2. Deserialize the database from input data
    // Note: We'll use a temporary file for deserialization
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read data into a buffer for deserialization
    unsigned char* db_data = (unsigned char*)malloc(size);
    if (!db_data) {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    
    size_t read_size = fread(db_data, 1, size, in_file);
    assert_file_closed(&in_file);;
    
    if (read_size > 0) {
        // Try to deserialize the data as a database
        sqlite3_deserialize(db, "main", db_data, size, size, 
                           SQLITE_DESERIALIZE_FREEONCLOSE | 
                           SQLITE_DESERIALIZE_RESIZEABLE);
        // Note: db_data is now owned by SQLite, don't free it separately
    } else {
        free(db_data);
        sqlite3_close(db);
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set up update hook
    sqlite3_update_hook(db,  id_000014_update_hook_callback, nullptr);
    
    // 4. Create a custom function that uses sqlite3_result_subtype
    sqlite3_create_function_v2(db, "custom_func", 1, SQLITE_UTF8, nullptr,
                               id_000014_custom_function, nullptr, nullptr, nullptr);
    
    // 5. Create a table and prepare a statement
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        // Prepare an INSERT statement
        const char* insert_sql = "INSERT INTO test (data) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // 6. Bind a zero blob using sqlite3_bind_zeroblob64
            // Use the input size modulo reasonable value for blob size
            sqlite3_uint64 blob_size = size % 10000 + 1;
            rc = sqlite3_bind_zeroblob64(stmt, 1, blob_size);
            
            if (rc == SQLITE_OK) {
                // Execute the statement
                rc = sqlite3_step(stmt);
                
                // Also try to call our custom function
                sqlite3_stmt* func_stmt = nullptr;
                const char* func_sql = "SELECT custom_func(42);";
                if (sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, nullptr) == SQLITE_OK) {
                    sqlite3_step(func_stmt);
                    sqlite3_finalize(func_stmt);
                }
            }
            
            sqlite3_finalize(stmt);
        }
    }
    
    // 7. Get error message if needed
    if (rc != SQLITE_OK) {
        const void* err_msg = sqlite3_errmsg16(db);
        (void)err_msg; // Could use this for debugging
    }
    
    // 8. Clean up
    // The database data will be freed by sqlite3_deserialize with FREEONCLOSE flag
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}