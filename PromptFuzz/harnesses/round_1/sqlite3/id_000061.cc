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
//<ID> 395
//<Prompt> []
/*<Combination>: [
*/
//<score> 60, nr_unique_branch: 2
//<Quality> {"density":20,"unique_branches":{"sqlite3_vtab_nochange":[[90457,7,90457,11,0,0,4,1]],"sqlite3_result_error_nomem":[[90160,7,90160,14,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_create_function","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_exec","sqlite3_vfs_find","sqlite3_vfs_unregister","sqlite3_create_function","sqlite3_value_type","sqlite3_value_int","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_value_type","sqlite3_value_int","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_exec","sqlite3_exec","sqlite3_next_stmt","sqlite3_strglob","sqlite3_thread_cleanup","sqlite3_strlike","sqlite3_create_collation16","sqlite3_exec","sqlite3_exec","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_create_function","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_exec","sqlite3_vfs_find","sqlite3_vfs_unregister","sqlite3_create_function","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_value_int","sqlite3_value_type","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_value_int","sqlite3_value_type","sqlite3_exec","sqlite3_exec","sqlite3_next_stmt","sqlite3_strglob","sqlite3_thread_cleanup","sqlite3_strlike","sqlite3_create_collation16","sqlite3_exec","sqlite3_exec","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 10) {
	return 0;
	}
	
    
    // Create a temporary database in memory
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table
    const char* create_table_sql = "CREATE TABLE test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Prepare a statement to insert data
    const char* insert_sql = "INSERT INTO test VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Use first 4 bytes for integer parameter
        int32_t int_val = 0;
        if (size >= 4) {
            memcpy(&int_val, data, 4);
        }
        
        // Use remaining bytes for text parameter (ensuring null termination)
        size_t text_len = size - 4;
        char* text_data = nullptr;
        if (text_len > 0 && text_len < 1000) {  // Reasonable size limit
            text_data = (char*)malloc(text_len + 1);
            if (text_data) {
                memcpy(text_data, data + 4, text_len);
                text_data[text_len] = '\0';
                
                // Bind parameters
                sqlite3_bind_int(stmt, 1, int_val);
                sqlite3_bind_text(stmt, 2, text_data, -1, SQLITE_TRANSIENT);
                
                // Execute the statement
                sqlite3_step(stmt);
                sqlite3_reset(stmt);
            }
        }
        
        // Free allocated text data
        if (text_data) free(text_data);
    }
    
    // 1. Use sqlite3_value_type - need a value to examine
    // Create a value using bind/text then column functions
    if (stmt) {
        sqlite3_bind_text(stmt, 2, "test", 4, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    
    // 2. Use sqlite3_vtab_nochange - need a context
    // We'll create a simple user-defined function to get a context
    sqlite3_create_function(
        db,
        "test_func",
        1,
        SQLITE_UTF8,
        nullptr,
        [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            // Use sqlite3_vtab_nochange
            int nochange = sqlite3_vtab_nochange(ctx);
            (void)nochange;  // Use the value
            sqlite3_result_int(ctx, 42);
        },
        nullptr,
        nullptr
    );
    
    // Call the function to trigger the callback
    sqlite3_exec(db, "SELECT test_func(1);", nullptr, nullptr, nullptr);
    
    // 3. Use sqlite3_vfs_unregister
    // Find a VFS to unregister (will likely fail, but we're just calling it)
    sqlite3_vfs* vfs = sqlite3_vfs_find(nullptr);
    if (vfs) {
        sqlite3_vfs_unregister(vfs);
    }
    
    // 4. Use sqlite3_result_error_nomem - already in our function above
    // Modify the function to also use this
    sqlite3_create_function(
        db,
        "test_func2",
        1,
        SQLITE_UTF8,
        nullptr,
        [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            // Use sqlite3_value_type
            int val_type = sqlite3_value_type(argv[0]);
            (void)val_type;
            
            // Use sqlite3_result_error_nomem under certain condition
            if (argc > 0 && sqlite3_value_int(argv[0]) == 999) {
                sqlite3_result_error_nomem(ctx);
            } else {
                sqlite3_result_int(ctx, 100);
            }
        },
        nullptr,
        nullptr
    );
    
    // Call with different values
    sqlite3_exec(db, "SELECT test_func2(999);", nullptr, nullptr, nullptr);
    sqlite3_exec(db, "SELECT test_func2(1);", nullptr, nullptr, nullptr);
    
    // 5. Use sqlite3_next_stmt
    // Get the next statement (should be null since we only have one prepared)
    sqlite3_stmt* next_stmt = sqlite3_next_stmt(db, stmt);
    (void)next_stmt;  // Use the value
    
    // 6. Use sqlite3_strglob
    const char* glob_pattern = "*test*";
    const char* test_string = "this is a test string";
    int glob_result = sqlite3_strglob(glob_pattern, test_string);
    (void)glob_result;
    
    // 7. Use sqlite3_thread_cleanup
    sqlite3_thread_cleanup();
    
    // 8. Use sqlite3_strlike
    const char* like_pattern = "%test%";
    const char* like_string = "this is a test string";
    unsigned int escape_char = '\\';
    int like_result = sqlite3_strlike(like_pattern, like_string, escape_char);
    (void)like_result;
    
    // 9. Use sqlite3_create_collation16
    // Create a simple collation
    rc = sqlite3_create_collation16(
        db,
        (const void*)u"test_collation",
        SQLITE_UTF16,
        nullptr,
        [](void* pArg, int n1, const void* s1, int n2, const void* s2) -> int {
            // Simple binary comparison
            int min_len = n1 < n2 ? n1 : n2;
            int cmp = memcmp(s1, s2, min_len);
            if (cmp == 0) {
                return n1 - n2;
            }
            return cmp;
        }
    );
    
    // Create a table with our collation and insert some data
    const void* create_collation_table_sql = u"CREATE TABLE collation_test (text COLLATE test_collation);";
    sqlite3_exec(db, (const char*)create_collation_table_sql, nullptr, nullptr, nullptr);
    
    // Insert using the input data
    if (size > 20) {
        char insert_buf[256];
        snprintf(insert_buf, sizeof(insert_buf), 
                 "INSERT INTO collation_test VALUES (x'%02x%02x%02x%02x');",
                 data[0], data[1], data[2], data[3]);
        sqlite3_exec(db, insert_buf, nullptr, nullptr, nullptr);
    }
    
    // Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}