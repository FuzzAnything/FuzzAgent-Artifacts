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
//<ID> 199
//<Prompt> []
/*<Combination>: [
*/
//<score> 99, nr_unique_branch: 36
//<Quality> {"density":9,"unique_branches":{"vfsUnlink":[[26027,13,26027,26,0,0,4,0]],"sqlite3VdbeMemValidStrRep":[[82106,7,82106,12,0,0,4,1]],"invokeProfileCallback":[[89543,7,89543,19,0,0,4,0],[89547,7,89547,40,0,0,4,1]],"sqlite3Step":[[90253,11,90253,73,0,0,4,0],[90254,14,90254,28,0,0,4,0],[90254,14,90254,28,0,0,4,1],[90254,32,90254,39,0,0,4,0],[89557,8,89557,26,15,0,4,0]],"sqlite3_complete16":[[177465,7,177465,9,0,0,4,1],[177470,7,177470,12,0,0,4,0]],"sqlite3_vfs_unregister":[[26077,7,26077,9,0,0,4,1]],"sqlite3_table_column_metadata":[[181515,7,181515,32,0,0,4,1],[181515,36,181515,49,0,0,4,1],[181524,7,181524,20,0,0,4,1],[181530,7,181530,12,0,0,4,1],[181536,7,181536,21,0,0,4,1],[181539,17,181539,32,0,0,4,0],[181541,11,181541,56,0,0,4,0],[181545,9,181545,25,0,0,4,1],[181566,7,181566,11,0,0,4,0],[181571,15,181571,32,0,0,4,1],[181576,7,181576,16,0,0,4,0],[181587,7,181587,17,0,0,4,0],[181588,7,181588,16,0,0,4,0],[181589,7,181589,15,0,0,4,0],[181590,7,181590,18,0,0,4,0],[181591,7,181591,15,0,0,4,0],[181593,7,181593,20,0,0,4,0],[181593,24,181593,29,0,0,4,1],[181599,32,181599,39,0,0,4,1],[18322,29,18322,57,3,0,4,1]],"sqlite3VdbeMemSetStr":[[83119,7,83119,15,0,0,4,1]],"sqlite3VdbeMemHandleBom":[[34292,7,34292,16,0,0,4,1]],"sqlite3_profile":[[179845,7,179845,32,0,0,4,1],[179855,7,179855,19,0,0,4,0]]},"library_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_busy_timeout","sqlite3_profile","sqlite3_prepare_v3","sqlite3_step","sqlite3_reset","sqlite3_complete16","sqlite3_value_dup","sqlite3_value_text16be","sqlite3_value_free","sqlite3_table_column_metadata","sqlite3_vfs_find","sqlite3_vfs_unregister","sqlite3_vfs_register","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_busy_timeout","sqlite3_profile","sqlite3_prepare_v3","sqlite3_step","sqlite3_reset","sqlite3_complete16","sqlite3_value_dup","sqlite3_value_text16be","sqlite3_value_free","sqlite3_table_column_metadata","sqlite3_vfs_find","sqlite3_vfs_unregister","sqlite3_vfs_register","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

// Profile callback function
static void  id_000027_profile_callback(void *userData, const char *sql, sqlite3_uint64 elapsed) {
    // Do nothing in fuzzing - just satisfy API requirement
    (void)userData;
    (void)sql;
    (void)elapsed;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimal data
    
    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd;  // Mark as used
    (void)out_fd; // Mark as used
    
    // Check if library is thread-safe
    int is_threadsafe = sqlite3_threadsafe();
    (void)is_threadsafe;
    
    // Create in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // Set busy timeout
    sqlite3_busy_timeout(db, 1000);  // 1 second timeout
    
    // Set up profile callback
    sqlite3_profile(db,  id_000027_profile_callback, nullptr);
    
    // Create a simple table using input data
    char create_table_sql[256];
    size_t sql_len = size < 200 ? size : 200;
    memcpy(create_table_sql, data, sql_len);
    create_table_sql[sql_len] = '\0';
    
    // Add SQL syntax to make it a valid CREATE statement
    // We'll use a fixed table name since we can't trust the input
    char *fixed_sql = (char*)"CREATE TABLE IF NOT EXISTS test_table (id INTEGER, data BLOB)";
    
    // Prepare statement
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    int rc = sqlite3_prepare_v3(db, fixed_sql, -1, 0, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Do nothing - just consume rows if any
        }
        
        // Reset statement for reuse
        sqlite3_reset(stmt);
        
        // Test if input might be complete SQL (for sqlite3_complete16)
        // Convert a portion of data to UTF-16 for testing
        size_t test_len = size < 100 ? size : 100;
        void *utf16_data = malloc(test_len * 2 + 2);
        if (utf16_data) {
            // Simple byte-to-wide char conversion (not proper UTF-8 to UTF-16)
            uint16_t *dest = (uint16_t*)utf16_data;
            for (size_t i = 0; i < test_len; i++) {
                dest[i] = data[i];
            }
            dest[test_len] = 0;  // Null terminate
            
            // Check if it's complete SQL
            int is_complete = sqlite3_complete16(utf16_data);
            (void)is_complete;
            
            free(utf16_data);
        }
        
        // Test sqlite3_value_text16be
        // Create a test value
        sqlite3_value *test_value = sqlite3_value_dup(nullptr);
        if (test_value) {
            const void *text16be = sqlite3_value_text16be(test_value);
            (void)text16be;
            sqlite3_value_free(test_value);
        }
        
        // Get table metadata (for the table we just created)
        const char *dataType = nullptr;
        const char *collSeq = nullptr;
        int notNull = 0;
        int primaryKey = 0;
        int autoInc = 0;
        
        sqlite3_table_column_metadata(
            db, 
            "main", 
            "test_table", 
            "id", 
            &dataType, 
            &collSeq, 
            &notNull, 
            &primaryKey, 
            &autoInc
        );
        
        // VFS operations
        // First get the default VFS
        sqlite3_vfs *vfs = sqlite3_vfs_find(nullptr);
        if (vfs) {
            // Try to unregister and re-register it
            sqlite3_vfs_unregister(vfs);
            sqlite3_vfs_register(vfs, 1);  // Re-register as default
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}