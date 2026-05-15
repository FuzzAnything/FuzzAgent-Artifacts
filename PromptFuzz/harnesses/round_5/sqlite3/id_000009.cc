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
//<ID> 15
//<Prompt> []
/*<Combination>: [
*/
//<score> 66, nr_unique_branch: 7
//<Quality> {"density":11,"unique_branches":{"insertCellFast":[[77094,9,77094,11,0,0,4,0]],"sqlite3BtreeInsert":[[79179,9,79179,11,0,0,4,0]],"allocateSpace":[[71582,9,71582,15,0,0,4,1]],"btreeComputeFreeSpace":[[71897,27,71897,43,0,0,4,0]],"sqlite3VdbeExec":[[100061,7,100061,34,0,0,4,0],[100062,8,100062,37,0,0,4,1],[100063,11,100063,44,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_db_readonly","sqlite3_malloc","sqlite3_msize","sqlite3_free","sqlite3_sourceid","sqlite3_prepare_v2","sqlite3_db_handle","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_db_readonly","sqlite3_malloc","sqlite3_msize","sqlite3_free","sqlite3_sourceid","sqlite3_prepare_v2","sqlite3_db_handle","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_exec","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Use fmemopen to create a FILE* from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }

    // Create a temporary database file to work with
    const char *temp_db_name = "temp_fuzz.db";
    FILE *temp_db_file = fopen(temp_db_name, "wb");
    if (!temp_db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&temp_db_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // Copy input data to temporary database file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, temp_db_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&temp_db_file);;

    // Open the database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(temp_db_name, &db);
    if (rc != SQLITE_OK || !db) {
        remove(temp_db_name);
        assert_file_closed(&temp_db_file);
	assert_file_closed(&in_file);
	return 0;
    }

    // API 1: Check if database is read-only
    int is_readonly = sqlite3_db_readonly(db, "main");
    
    // API 2: Use sqlite3_msize on allocated memory
    void *test_mem = sqlite3_malloc(256);
    if (test_mem) {
        sqlite3_uint64 mem_size = sqlite3_msize(test_mem);
        (void)mem_size; // Use variable to avoid compiler warning
        sqlite3_free(test_mem);
    }

    // API 3: Get SQLite source ID
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Use variable

    // API 4: Get database handle from a statement
    sqlite3_stmt *stmt = nullptr;
    const char *simple_sql = "SELECT 1";
    rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
        (void)db_from_stmt; // Use variable
        
        // Execute the statement once
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    // API 5: Try to use sqlite3_vtab_collation
    // First create a virtual table if possible
    const char *create_vtab_sql = 
        "CREATE VIRTUAL TABLE IF NOT EXISTS test_vtab USING fts4(content);";
    sqlite3_exec(db, create_vtab_sql, nullptr, nullptr, nullptr);
    
    // Even if virtual table creation fails, we can still try to get collation
    // by preparing a query that might use a virtual table
    sqlite3_stmt *vtab_stmt = nullptr;
    const char *vtab_sql = "SELECT * FROM test_vtab WHERE content MATCH 'test';";
    sqlite3_prepare_v2(db, vtab_sql, -1, &vtab_stmt, nullptr);
    
    // While we can't easily get sqlite3_index_info* in normal usage,
    // we can at least demonstrate the API exists and the program compiles
    // In real usage, sqlite3_vtab_collation would be called from within
    // a virtual table implementation's xBestIndex method
    
    if (vtab_stmt) {
        sqlite3_finalize(vtab_stmt);
    }

    // Clean up: drop the virtual table if it exists
    sqlite3_exec(db, "DROP TABLE IF EXISTS test_vtab", nullptr, nullptr, nullptr);

    // Close database
    sqlite3_close(db);
    
    // Remove temporary file
    remove(temp_db_name);

    assert_file_closed(&temp_db_file);
	assert_file_closed(&in_file);
	return 0;
}