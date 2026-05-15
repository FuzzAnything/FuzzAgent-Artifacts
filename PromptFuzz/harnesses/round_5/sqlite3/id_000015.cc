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
//<ID> 82
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 3
//<Quality> {"density":12,"unique_branches":{"pcache1EnforceMaxPage":[[55105,10,55105,45,0,0,4,0],[55106,10,55106,47,0,0,4,0]],"pcache1Shrink":[[55321,7,55321,25,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_thread_cleanup","sqlite3_db_filename","sqlite3_filename_database","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_create_function","sqlite3_exec","sqlite3_db_release_memory","sqlite3_finalize","sqlite3_close","sqlite3_thread_cleanup"],"critical_calls":["sqlite3_open","sqlite3_db_filename","sqlite3_filename_database","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_create_function","sqlite3_exec","sqlite3_db_release_memory","sqlite3_finalize","sqlite3_close","sqlite3_thread_cleanup"],"visited":0}
/*Here's a C++ fuzzer that uses the specified SQLite3 APIs to handle a scenario where we read database data from a byte stream and handle potential errors:

*/


// Custom user-defined function that uses sqlite3_result_error_toobig
static void  id_000015_custom_toobig_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    sqlite3_result_error_toobig(context);
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data to work with
    
    // Step 1: Create a temporary database file and write input data to it
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a temporary output file for database
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy data from memory to temporary database file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, out_file);
    }
    assert_file_closed(&out_file);;
    assert_file_closed(&in_file);;
    
    // Step 2: Open the database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open("output_file", &db);
    if (rc != SQLITE_OK || !db) {
        remove("output_file");
        sqlite3_thread_cleanup();
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
    }
    
    // Step 3: Use sqlite3_filename_database to get main database filename
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
    const char *main_db = sqlite3_filename_database(db_filename);
    (void)main_db;  // Use variable to avoid unused warning
    
    // Step 4: Create a table and prepared statement
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Use sqlite3_bind_blob with part of the input data
        size_t blob_size = size > 1000 ? 1000 : size;
        void *blob_data = malloc(blob_size);
        if (blob_data) {
            memcpy(blob_data, data, blob_size);
            rc = sqlite3_bind_blob(stmt, 1, blob_data, (int)blob_size, free);
            
            if (rc == SQLITE_OK) {
                sqlite3_step(stmt);
            }
            sqlite3_reset(stmt);
        }
    }
    
    // Step 5: Create and register a custom function that uses sqlite3_result_error_toobig
    sqlite3_create_function(db, "toobigfunc", 0, SQLITE_UTF8, nullptr, 
                            id_000015_custom_toobig_function, nullptr, nullptr);
    
    // Execute the custom function
    sqlite3_exec(db, "SELECT toobigfunc();", nullptr, nullptr, nullptr);
    
    // Step 6: Try to release memory
    sqlite3_db_release_memory(db);
    
    // Step 7: Cleanup
    if (stmt) sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    // Remove temporary file
    remove("output_file");
    
    // Step 8: Thread cleanup
    sqlite3_thread_cleanup();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("output_file");
	return 0;
}