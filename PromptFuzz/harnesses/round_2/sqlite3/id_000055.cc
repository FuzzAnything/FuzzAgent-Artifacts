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
//<ID> 424
//<Prompt> []
/*<Combination>: [
*/
//<score> 135, nr_unique_branch: 51
//<Quality> {"density":15,"unique_branches":{"sqlite3_filename_journal":[[182392,7,182392,19,0,0,4,1],[182395,31,182395,43,0,0,4,1],[14155,25,14155,46,1,0,4,0],[14155,26,14155,29,1,0,4,0]],"sqlite3_vtab_nochange":[[90457,7,90457,11,0,0,4,1]],"memdbFromDbSchema":[[52936,7,52936,42,0,0,4,0]],"sqlite3VdbeExec":[[100730,1,100730,18,0,0,4,0]],"patternCompare":[[128155,14,128155,48,0,0,4,0],[128156,18,128156,31,0,0,4,0],[128156,35,128156,46,0,0,4,0],[128157,13,128157,24,0,0,4,0],[128157,13,128157,24,0,0,4,1],[128157,28,128157,56,0,0,4,0],[128157,28,128157,56,0,0,4,1],[128163,17,128163,30,0,0,4,0],[128164,13,128164,31,0,0,4,0],[128166,15,128166,19,0,0,4,0],[128166,15,128166,19,0,0,4,1],[128189,11,128189,17,0,0,4,1],[128192,13,128192,19,0,0,4,0],[128202,15,128202,28,0,0,4,0],[128209,16,128209,43,0,0,4,0],[128209,16,128209,43,0,0,4,1],[128210,15,128210,20,0,0,4,0],[128220,13,128220,17,0,0,4,0],[128258,20,128258,53,0,0,4,0],[128258,61,128258,67,0,0,4,0],[128258,61,128258,67,0,0,4,1],[128258,71,128258,78,0,0,4,0],[128261,24,128261,42,0,0,4,1],[128261,46,128261,51,0,0,4,1],[128264,10,128264,21,0,0,4,1],[128082,37,128082,46,2,0,4,1],[128082,37,128082,46,12,0,4,0]],"sqlite3_filename_wal":[[182406,7,182406,16,0,0,4,0]],"sqlite3_serialize":[[52964,7,52964,32,0,0,4,1],[52970,7,52970,17,0,0,4,1],[52973,7,52973,13,0,0,4,0],[52974,7,52974,12,0,0,4,1],[52975,7,52975,8,0,0,4,1],[52988,7,52988,13,0,0,4,1],[52991,8,52991,12,0,0,4,0],[52993,7,52993,9,0,0,4,1],[52995,7,52995,21,0,0,4,1],[52999,9,52999,15,0,0,4,0],[53000,9,53000,41,0,0,4,1],[53004,11,53004,15,0,0,4,0],[53008,21,53008,32,0,0,4,0],[53008,21,53008,32,0,0,4,1],[53012,15,53012,28,0,0,4,0]],"sqlite3_overload_function":[[179769,7,179769,9,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_blob64","sqlite3_step","sqlite3_finalize","sqlite3_serialize","sqlite3_strlike","sqlite3_strlike","sqlite3_create_filename","sqlite3_filename_wal","sqlite3_create_function_v2","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_vfs_find","sqlite3_vfs_find","sqlite3_overload_function","sqlite3_free_filename","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_blob64","sqlite3_step","sqlite3_finalize","sqlite3_serialize","sqlite3_strlike","sqlite3_strlike","sqlite3_create_filename","sqlite3_filename_wal","sqlite3_create_function_v2","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_result_int","sqlite3_vtab_nochange","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_vfs_find","sqlite3_vfs_find","sqlite3_overload_function","sqlite3_free_filename","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create temporary file for input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    int fd = fuzz_fileno(in_file);
    (void)fd; // Use fd to avoid unused variable warning
    
    // Open output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Create in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Prepare a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);";
    char *errmsg = nullptr;
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Insert some data into table
    const char *insert_sql = "INSERT INTO test(data) VALUES(?)";
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_bind_blob64(stmt, 1, data, size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 1. Use sqlite3_serialize to get database serialization
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized_data = sqlite3_serialize(
        db, "main", &serialized_size, 0);
    
    if (serialized_data) {
        // Write serialized data to output file
        fwrite(serialized_data, 1, serialized_size, out_file);
        fflush(out_file);
        
        // 2. Use sqlite3_strlike with serialized data as pattern
        // Convert first few bytes of data to string for pattern matching
        char pattern[256] = {0};
        size_t pattern_len = size < 255 ? size : 255;
        memcpy(pattern, data, pattern_len);
        pattern[pattern_len] = '\0';
        
        // Create a test string to match against
        const char *test_string = "test_pattern";
        
        // Use sqlite3_strlike with different escape characters
        int like_result1 = sqlite3_strlike(pattern, test_string, 0);
        int like_result2 = sqlite3_strlike(pattern, test_string, '\\');
        
        (void)like_result1; // Use results to avoid warnings
        (void)like_result2;
        
        // 3. Use sqlite3_filename_wal
        sqlite3_filename db_filename = sqlite3_create_filename(
            "main", "main-journal", "main-wal", 0, nullptr);
        if (db_filename) {
            const char *wal_name = sqlite3_filename_wal(db_filename);
            (void)wal_name; // Use result
            
            // 4. Use sqlite3_vtab_nochange
            // Create a virtual table context by creating a function
            rc = sqlite3_create_function_v2(
                db, "test_func", 1, SQLITE_UTF8, nullptr,
                [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
                    // Call sqlite3_vtab_nochange if we have a context
                    int nochange = sqlite3_vtab_nochange(ctx);
                    (void)nochange;
                    sqlite3_result_int(ctx, 42);
                },
                nullptr, nullptr, nullptr);
            
            // Call the function to trigger the context
            const char *select_func = "SELECT test_func(5)";
            sqlite3_stmt *func_stmt = nullptr;
            rc = sqlite3_prepare_v3(db, select_func, -1, 0, &func_stmt, nullptr);
            if (rc == SQLITE_OK) {
                sqlite3_step(func_stmt);
                sqlite3_finalize(func_stmt);
            }
            
            // 5. Use sqlite3_vfs_find
            sqlite3_vfs *vfs = sqlite3_vfs_find(nullptr); // Default VFS
            if (vfs) {
                // Try to find a specific VFS
                sqlite3_vfs *unix_vfs = sqlite3_vfs_find("unix");
                (void)unix_vfs;
            }
            
            // 6. Use sqlite3_overload_function
            rc = sqlite3_overload_function(db, "test_func", 1);
            (void)rc;
            
            sqlite3_free_filename(db_filename);
        }
        
        sqlite3_free(serialized_data);
    }
    
    // Cleanup
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    // Close database
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}