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
//<ID> 197
//<Prompt> []
/*<Combination>: [
*/
//<score> 38, nr_unique_branch: 2
//<Quality> {"density":19,"unique_branches":{"sqlite3Realloc":[[30351,11,30351,27,0,0,4,0],[30351,31,30351,62,0,0,4,1]]},"library_calls":["sqlite3_soft_heap_limit","sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_bytes16","sqlite3_column_origin_name16","sqlite3_column_type","sqlite3_column_text16","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_get_autocommit","sqlite3_vfs_find","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_close","sqlite3_errmsg","sqlite3_close"],"critical_calls":["sqlite3_soft_heap_limit","sqlite3_open","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_bytes16","sqlite3_column_origin_name16","sqlite3_column_type","sqlite3_column_text16","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_get_autocommit","sqlite3_vfs_find","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_close","sqlite3_errmsg","sqlite3_close"],"visited":0}
/**/


// Custom callback function to use sqlite3_result_text64
static void  id_000033_test_callback(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv
) {
    if (argc > 0) {
        const char *text = (const char*)sqlite3_value_text(argv[0]);
        if (text) {
            sqlite3_result_text64(
                context,
                text,
                sqlite3_value_bytes(argv[0]),
                SQLITE_TRANSIENT,
                SQLITE_UTF8
            );
        } else {
            sqlite3_result_null(context);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Set soft heap limit using a portion of input data
    if (size >= sizeof(int)) {
        int heap_limit = *(int*)data % 1000000; // Use first 4 bytes for heap limit
        sqlite3_soft_heap_limit(heap_limit > 0 ? heap_limit : 1000);
    }
    
    // Initialize database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table and insert some data
    const char *create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, data BLOB);"
        "INSERT INTO test_table (name, data) VALUES ('test1', x'010203');"
        "INSERT INTO test_table (name, data) VALUES ('test2', x'040506');";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Register custom function to use sqlite3_result_text64
    sqlite3_create_function(
        db,
        "test_func",
        1,
        SQLITE_UTF8,
        nullptr,
         id_000033_test_callback,
        nullptr,
        nullptr
    );
    
    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *select_sql = "SELECT id, name, data FROM test_table";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_data_count(stmt);
            
            // Use various column APIs
            for (int i = 0; i < col_count; i++) {
                // Use sqlite3_column_bytes16
                int bytes16 = sqlite3_column_bytes16(stmt, i);
                
                // Use sqlite3_column_origin_name16 (returns NULL for non-column expressions)
                const void *origin_name = sqlite3_column_origin_name16(stmt, i);
                
                // Get column value as text16le if possible
                if (sqlite3_column_type(stmt, i) == SQLITE_TEXT) {
                    const void *text16le = sqlite3_column_text16(stmt, i);
                    // Note: sqlite3_value_text16le needs a sqlite3_value*, not column text
                }
            }
        }
        
        // Reset statement for potential reuse
        sqlite3_reset(stmt);
        
        // Call our custom function
        const char *func_sql = "SELECT test_func(name) FROM test_table";
        sqlite3_stmt *func_stmt = nullptr;
        if (sqlite3_prepare_v2(db, func_sql, -1, &func_stmt, nullptr) == SQLITE_OK) {
            while (sqlite3_step(func_stmt) == SQLITE_ROW) {
                // Process results
            }
            sqlite3_finalize(func_stmt);
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Test autocommit mode
    int autocommit = sqlite3_get_autocommit(db);
    
    // Find VFS
    sqlite3_vfs *vfs = sqlite3_vfs_find(nullptr); // Default VFS
    
    // Use test control (note: sqlite3_test_control is not in the provided list,
    // but we'll simulate its usage with available APIs)
    // We'll use sqlite3_aggregate_count as an alternative test API
    
    // Process input data as a pseudo-database file
    if (size > 0) {
        // Create an in-memory database from input data
        sqlite3 *db2 = nullptr;
        rc = sqlite3_open(":memory:", &db2);
        
        if (rc == SQLITE_OK && db2) {
            // Create a table and insert input data
            sqlite3_exec(db2, "CREATE TABLE input_data (content BLOB);", nullptr, nullptr, nullptr);
            
            sqlite3_stmt *insert_stmt = nullptr;
            const char *insert_sql = "INSERT INTO input_data (content) VALUES (?)";
            
            if (sqlite3_prepare_v2(db2, insert_sql, -1, &insert_stmt, nullptr) == SQLITE_OK) {
                sqlite3_bind_blob(insert_stmt, 1, data, size, SQLITE_STATIC);
                sqlite3_step(insert_stmt);
                sqlite3_finalize(insert_stmt);
            }
            
            // Try to read the data back
            sqlite3_stmt *select_stmt = nullptr;
            const char *select_data_sql = "SELECT content FROM input_data";
            
            if (sqlite3_prepare_v2(db2, select_data_sql, -1, &select_stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                    const void *blob_data = sqlite3_column_blob(select_stmt, 0);
                    int blob_size = sqlite3_column_bytes(select_stmt, 0);
                    // Process blob data
                }
                sqlite3_finalize(select_stmt);
            }
            
            sqlite3_close(db2);
        }
    }
    
    // Get error message (even if no error, this returns "not an error")
    const char *err_msg = sqlite3_errmsg(db);
    
    // Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}