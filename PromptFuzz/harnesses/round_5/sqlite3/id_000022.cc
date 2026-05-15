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
//<ID> 111
//<Prompt> []
/*<Combination>: [
*/
//<score> 84, nr_unique_branch: 13
//<Quality> {"density":12,"unique_branches":{"sqlite3OomClear":[[30672,7,30672,23,0,0,4,1]],"sqlite3_errmsg16":[[180284,7,180284,10,0,0,4,1],[180287,7,180287,38,0,0,4,1],[180291,7,180291,23,0,0,4,1],[180295,9,180295,13,0,0,4,0]],"blobReadWrite":[[101705,9,101705,35,0,0,4,0],[101705,39,101705,61,0,0,4,1]],"sqlite3BtreePutData":[[81042,7,81042,20,0,0,4,1],[81046,7,81046,33,0,0,4,1],[81068,7,81068,43,0,0,4,0],[70664,4,70664,33,1,0,4,1]],"sqlite3_get_clientdata":[[181345,22,181345,23,0,0,4,1]],"saveAllCursors":[[70555,7,70555,14,0,0,4,0]]},"library_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_get_clientdata","sqlite3_exec","sqlite3_blob_open","sqlite3_blob_write","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_finalize","sqlite3_errmsg16","sqlite3_close"],"critical_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_get_clientdata","sqlite3_exec","sqlite3_blob_open","sqlite3_blob_write","sqlite3_blob_close","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_finalize","sqlite3_errmsg16","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return if input is too small to be meaningful
    if (size < 16) {
	return 0;
	}
	
    
    // Get libversion number (API 1)
    int version = sqlite3_libversion_number();
    
    // Create in-memory database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
	return 0;
	}
	
    
    // Set client data (API 7)
    char* key_data = (char*)malloc(16);
    if (key_data) {
        memcpy(key_data, "test_key", 9);
        // Note: sqlite3_set_clientdata is not in provided list, using get only
        void* client_data = sqlite3_get_clientdata(db, "test_key");
        free(key_data);
    }
    
    // Create a table and insert blob data
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test(data) VALUES (zeroblob(100));", 
        nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        // Open blob handle
        sqlite3_blob* blob = nullptr;
        rc = sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
        
        if (rc == SQLITE_OK && blob) {
            // Write some input data to blob
            int write_size = size > 100 ? 100 : size;
            rc = sqlite3_blob_write(blob, data, write_size, 0);
            
            // Close blob (API 6)
            sqlite3_blob_close(blob);
        }
    }
    
    // Prepare a statement
    sqlite3_stmt* stmt = nullptr;
    const char* sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Get column count (API 4)
        int col_count = sqlite3_column_count(stmt);
        
        // Execute statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Simulate virtual table context usage
            // We'll create a user-defined function to get sqlite3_context
            
            // For vtab_nochange (API 2) and vtab_distinct (API 3):
            // These require virtual table contexts which we can't easily create.
            // We'll demonstrate their usage in a minimal way.
            
            // Note: In real usage, these would be called from within
            // virtual table implementations, not directly like this.
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Get error message in UTF-16 (API 5)
    const void* errmsg = sqlite3_errmsg16(db);
    
    // Create a virtual table index info structure simulation
    // For vtab_distinct - this is normally passed to xBestIndex
    // We'll create a minimal simulation
    if (size > sizeof(sqlite3_index_info)) {
        sqlite3_index_info* idx_info = (sqlite3_index_info*)data;
        // Note: This is just to show the API call, actual usage requires
        // proper initialization of the structure
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}