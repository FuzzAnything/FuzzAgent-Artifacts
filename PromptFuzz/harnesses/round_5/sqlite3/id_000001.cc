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
//<ID> 1
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"sqlite3_serialize":[[52993,7,52993,9,0,0,4,0],[52995,7,52995,21,0,0,4,0],[53004,11,53004,15,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_close","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_open","sqlite3_exec","sqlite3_serialize","sqlite3_close","sqlite3_strglob","sqlite3_global_recover","sqlite3_rollback_hook","sqlite3_finalize","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_bind_text","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_open","sqlite3_exec","sqlite3_serialize","sqlite3_close","sqlite3_strglob","sqlite3_global_recover","sqlite3_rollback_hook","sqlite3_finalize","sqlite3_free","sqlite3_close"],"visited":0}
/**/


// Rollback hook callback
static void  id_000001_rollback_hook_callback(void *pData) {
    // Empty callback - just to demonstrate the hook
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Create a table for testing
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Prepare a statement with parameters
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test (name, data) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close(db);
        return 0;
    }

    // Use sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // Use sqlite3_bind_parameter_name for the first parameter if available
    const char *param_name = nullptr;
    if (param_count > 0) {
        param_name = sqlite3_bind_parameter_name(stmt, 1);
    }

    // Bind parameters to the statement
    if (size > 0) {
        // Bind name parameter
        sqlite3_bind_text(stmt, 1, "test_name", -1, SQLITE_STATIC);
        
        // Bind data parameter using the input
        if (size > 0) {
            sqlite3_bind_blob(stmt, 2, data, static_cast<int>(size), SQLITE_STATIC);
        }
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        
        // Reset statement for potential reuse
        sqlite3_reset(stmt);
    }

    // Use sqlite3_serialize
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized_data = nullptr;
    
    // Create a temporary database on disk to serialize
    sqlite3 *temp_db = nullptr;
    rc = sqlite3_open("temp_db", &temp_db);
    if (rc == SQLITE_OK && temp_db != nullptr) {
        // Create a table in temp database
        sqlite3_exec(temp_db, create_table_sql, nullptr, nullptr, nullptr);
        
        // Serialize the database
        serialized_data = sqlite3_serialize(temp_db, "main", &serialized_size, 0);
        
        sqlite3_close(temp_db);
        remove("temp_db");
    }

    // Use sqlite3_strglob
    int strglob_result = 0;
    if (size > 0) {
        // Create a pattern from input data (use first 8 bytes or less if size < 8)
        size_t pattern_len = size < 8 ? size : 8;
        char *pattern = (char *)malloc(pattern_len + 1);
        if (pattern) {
            memcpy(pattern, data, pattern_len);
            pattern[pattern_len] = '\0';
            
            // Test string to match against
            const char *test_str = "test_string";
            strglob_result = sqlite3_strglob(pattern, test_str);
            
            free(pattern);
        }
    }

    // Use sqlite3_global_recover
    int recover_result = sqlite3_global_recover();

    // Use sqlite3_rollback_hook
    void *old_hook_data = sqlite3_rollback_hook(db,  id_000001_rollback_hook_callback, nullptr);

    // Clean up statement
    sqlite3_finalize(stmt);

    // Free serialized data if allocated
    if (serialized_data) {
        sqlite3_free(serialized_data);
    }

    // Clean up database
    sqlite3_close(db);

    return 0;
}