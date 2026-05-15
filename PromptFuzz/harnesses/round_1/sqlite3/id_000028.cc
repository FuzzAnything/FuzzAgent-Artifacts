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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 11, nr_unique_branch: 0
//<Quality> {"density":11,"unique_branches":{},"library_calls":["sqlite3_libversion","sqlite3_keyword_count","sqlite3_global_recover","sqlite3_vfs_find","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_finish","sqlite3_free","sqlite3_keyword_name","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_keyword_count","sqlite3_global_recover","sqlite3_vfs_find","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_finish","sqlite3_free","sqlite3_keyword_name","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified SQLite APIs while consuming the input data:

*/


extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 2) {
	return 0;
	}
	  // Need minimum data for processing

    // 1. Get libversion using sqlite3_libversion()
    const char* lib_version = sqlite3_libversion();
    
    // 2. Get keyword count using sqlite3_keyword_count()
    int keyword_count = sqlite3_keyword_count();
    
    // 3. Attempt global recovery using sqlite3_global_recover()
    int recovery_result = sqlite3_global_recover();
    
    // 4. Find VFS - extract VFS name from input data
    char* vfs_name = nullptr;
    // Use first null-terminated string from input as VFS name
    size_t vfs_name_len = 0;
    while (vfs_name_len < size && data[vfs_name_len] != 0) {
        vfs_name_len++;
    }
    
    if (vfs_name_len > 0 && vfs_name_len < size) {
        vfs_name = (char*)malloc(vfs_name_len + 1);
        if (vfs_name) {
            memcpy(vfs_name, data, vfs_name_len);
            vfs_name[vfs_name_len] = '\0';
            sqlite3_vfs* vfs = sqlite3_vfs_find(vfs_name);
            // VFS found, we don't need to use it further for this test
            free(vfs_name);
        }
    }
    
    // 5. Create and use string builder with sqlite3_str_appendchar
    sqlite3_str* str_builder = sqlite3_str_new(nullptr);
    if (str_builder) {
        // Append some characters based on input data
        size_t chars_to_append = size > 100 ? 100 : size;
        for (size_t i = 0; i < chars_to_append; i++) {
            char c = (char)((data[i] % 26) + 'a');  // Convert to letter a-z
            sqlite3_str_appendchar(str_builder, 1, c);
        }
        
        // Free the string builder
        char* built_str = sqlite3_str_finish(str_builder);
        if (built_str) {
            sqlite3_free(built_str);
        }
    }
    
    // 6. Process keywords using sqlite3_keyword_name()
    for (int i = 0; i < keyword_count && i < 10; i++) {  // Limit to first 10 keywords
        const char* keyword;
        int keyword_len;
        if (sqlite3_keyword_name(i, &keyword, &keyword_len) == SQLITE_OK) {
            // Keyword processed, can be used if needed
        }
    }
    
    // Additional test: Try to create a temporary database and use the input data
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc == SQLITE_OK && db) {
        // Create a simple table
        const char* create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);";
        sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
        
        // Prepare a statement to insert data
        sqlite3_stmt* stmt = nullptr;
        const char* insert_sql = "INSERT INTO test (id, data) VALUES (?, ?);";
        
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            // Bind data from input
            sqlite3_bind_int(stmt, 1, 1);
            
            // Use input data as blob
            if (size > 0) {
                sqlite3_bind_blob(stmt, 2, data, 
                                 (int)(size > 1000 ? 1000 : size),  // Limit size
                                 SQLITE_TRANSIENT);
            }
            
            // Execute the statement
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        // Try to query the data back
        const char* select_sql = "SELECT data FROM test WHERE id = 1;";
        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                // Data retrieved, but we don't need to use it for fuzzing
            }
            sqlite3_finalize(stmt);
        }
        
        // Create a file from input data using fmemopen
        if (size > 0) {
            FILE* in_file = fmemopen((void *)data, size, "rb");
            if (in_file) {
                // Use the FILE* - read a few bytes to ensure it's working
                char buffer[16];
                size_t read_bytes = fread(buffer, 1, 
                                         size > 16 ? 16 : size, 
                                         in_file);
                assert_file_closed(&in_file);;
            }
        }
        
        // Create an output file
        FILE* out_file = fopen("output_file", "wb");
        if (out_file) {
            // Write version info to output file
            fprintf(out_file, "SQLite version: %s\n", lib_version);
            fprintf(out_file, "Keywords count: %d\n", keyword_count);
            fprintf(out_file, "Recovery result: %d\n", recovery_result);
            
            // Get file descriptor and use it
            int fd = fuzz_fileno(out_file);
            if (fd != -1) {
                // File descriptor obtained, could be used for other operations
            }
            
            assert_file_closed(&out_file);;
            
            // Clean up output file
            remove("output_file");
        }
        
        // Close database
        sqlite3_close(db);
    }
    
    return 0;
}