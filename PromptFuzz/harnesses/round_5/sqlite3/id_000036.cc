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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 200, nr_unique_branch: 13
//<Quality> {"density":20,"unique_branches":{"sqlite3AuthRead":[[120046,13,120046,27,0,0,4,0]],"sqlite3Pragma":[[137689,7,137689,13,0,0,4,0],[137689,17,137689,48,0,0,4,1]],"yy_reduce":[[174034,7,174034,14,0,0,4,0]],"sqlite3StartTable":[[121372,25,121372,31,0,0,4,0],[121372,35,121372,46,0,0,4,1],[121378,25,121378,31,0,0,4,0]],"sqlite3VdbeExec":[[96246,31,96246,46,0,0,4,0]],"vdbeCommit":[[86899,11,86899,57,0,0,4,1]],"bindText":[[91157,13,91157,32,0,0,4,1]],"releaseAllSavepoints":[[58226,13,58226,34,0,0,4,0]],"downgradeAllSharedCacheTableLocks":[[70259,7,70259,22,0,0,4,0],[70263,27,70263,32,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_vfs_find","sqlite3_vfs_register","sqlite3_open","sqlite3_vfs_unregister","sqlite3_shutdown","sqlite3_set_authorizer","sqlite3_create_function","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_count","sqlite3_column_table_name16","sqlite3_column_type","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_finalize","sqlite3_vfs_unregister","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_vfs_find","sqlite3_vfs_register","sqlite3_open","sqlite3_set_authorizer","sqlite3_create_function","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_count","sqlite3_column_table_name16","sqlite3_column_type","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_finalize","sqlite3_vfs_unregister","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom VFS structure for registration
static sqlite3_vfs  id_000036_custom_vfs = {0};

// Authorizer callback function
static int  id_000036_authorizer_callback(
    void* pUserData,
    int actionCode,
    const char* detail1,
    const char* detail2,
    const char* detail3,
    const char* detail4) {
    // Simple authorizer that allows everything
    return SQLITE_OK;
}

// Custom function implementation
static void  id_000036_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0) {
        // Use sqlite3_value_int on first argument
        int int_value = sqlite3_value_int(argv[0]);
        
        // For demonstration, return error for negative values, zeroblob for 0, echo for positive
        if (int_value < 0) {
            // Use sqlite3_result_error16
            static const char16_t error_msg[] = u"Negative value error";
            sqlite3_result_error16(context, error_msg, sizeof(error_msg));
        } else if (int_value == 0) {
            // Use sqlite3_result_zeroblob
            sqlite3_result_zeroblob(context, 10); // 10-byte zeroblob
        } else {
            // Return the integer value
            sqlite3_result_int(context, int_value);
        }
    } else {
        sqlite3_result_int(context, 0);
    }
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
        return 0;
    }

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    FILE* in_file = nullptr;
    int rc;
    
    // Create a temporary file with the input data
    in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptor
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Close the file as we'll use the data directly
    assert_file_closed(&in_file);;
    
    // Initialize SQLite
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Register custom VFS (using sqlite3_vfs_register)
    // First, get the default VFS to copy its methods
    sqlite3_vfs* default_vfs = sqlite3_vfs_find(nullptr);
    if (default_vfs) {
        memcpy(& id_000036_custom_vfs, default_vfs, sizeof(sqlite3_vfs));
         id_000036_custom_vfs.zName = "custom_fuzz_vfs";
         id_000036_custom_vfs.szOsFile = sizeof(FILE*);
        rc = sqlite3_vfs_register(& id_000036_custom_vfs, 0);
        // Note: We'll unregister it later
    }
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        sqlite3_vfs_unregister(& id_000036_custom_vfs);
        sqlite3_shutdown();
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Set authorizer (using sqlite3_set_authorizer)
    rc = sqlite3_set_authorizer(db,  id_000036_authorizer_callback, nullptr);
    
    // Create a custom scalar function
    rc = sqlite3_create_function(
        db,
        "fuzz_func",
        1,  // Number of arguments
        SQLITE_UTF8,
        nullptr,
         id_000036_custom_function,
        nullptr,
        nullptr
    );
    
    // Create a test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value INTEGER);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Insert some test data using the input
    char insert_sql[256];
    // Use first byte as integer value for demonstration
    int input_value = data[0];
    snprintf(insert_sql, sizeof(insert_sql),
             "INSERT INTO test_table (name, value) VALUES ('test', %d);",
             input_value);
    rc = sqlite3_exec(db, insert_sql, nullptr, nullptr, nullptr);
    
    // Prepare a statement that uses our custom function
    const char* query_sql = 
        "SELECT fuzz_func(value), name FROM test_table;";
    
    rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Check if statement is an EXPLAIN statement (using sqlite3_stmt_isexplain)
        int is_explain = sqlite3_stmt_isexplain(stmt);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        
        if (rc == SQLITE_ROW) {
            // Get column count
            int col_count = sqlite3_column_count(stmt);
            
            if (col_count > 0) {
                // Try to get table name for second column (using sqlite3_column_table_name16)
                // Note: This will likely return NULL for our simple query
                const void* table_name = sqlite3_column_table_name16(stmt, 1);
                
                // Process first column (result of our custom function)
                int col_type = sqlite3_column_type(stmt, 0);
                
                // For demonstration, also try to use the input data in another way
                // Create a temporary table with blob data from input
                if (size > 0) {
                    sqlite3_stmt* blob_stmt = nullptr;
                    const char* blob_sql = 
                        "CREATE TEMPORARY TABLE temp_blob (data BLOB);"
                        "INSERT INTO temp_blob (data) VALUES (?);";
                    
                    rc = sqlite3_prepare_v2(db, blob_sql, -1, &blob_stmt, nullptr);
                    if (rc == SQLITE_OK && blob_stmt) {
                        // Bind the input data as blob
                        sqlite3_bind_blob(blob_stmt, 1, data, 
                                         (size > 1000) ? 1000 : (int)size, 
                                         SQLITE_STATIC);
                        rc = sqlite3_step(blob_stmt);
                        sqlite3_finalize(blob_stmt);
                    }
                }
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Test with an EXPLAIN statement
    const char* explain_sql = "EXPLAIN SELECT * FROM test_table;";
    rc = sqlite3_prepare_v2(db, explain_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        int is_explain = sqlite3_stmt_isexplain(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Unregister the custom VFS (using sqlite3_vfs_unregister)
    sqlite3_vfs_unregister(& id_000036_custom_vfs);
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}