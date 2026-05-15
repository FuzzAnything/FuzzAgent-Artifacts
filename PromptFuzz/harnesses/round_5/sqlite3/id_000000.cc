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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 18
//<Quality> {"density":15,"unique_branches":{"memdbFromDbSchema":[[52936,7,52936,42,0,0,4,0]],"sqlite3_serialize":[[52964,7,52964,32,0,0,4,1],[52970,7,52970,17,0,0,4,1],[52973,7,52973,13,0,0,4,0],[52974,7,52974,12,0,0,4,1],[52975,7,52975,8,0,0,4,1],[52988,7,52988,13,0,0,4,1],[52991,8,52991,12,0,0,4,0],[52993,7,52993,9,0,0,4,1],[52995,7,52995,21,0,0,4,1],[52999,9,52999,15,0,0,4,0],[53000,9,53000,41,0,0,4,0],[53000,9,53000,41,0,0,4,1],[53004,11,53004,15,0,0,4,0],[53008,21,53008,32,0,0,4,0],[53008,21,53008,32,0,0,4,1],[53012,15,53012,28,0,0,4,0]],"sqlite3VdbeExec":[[100730,1,100730,18,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_close_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_text","sqlite3_step","sqlite3_bind_parameter_name","sqlite3_finalize","sqlite3_rollback_hook","sqlite3_strglob","sqlite3_serialize","sqlite3_free","sqlite3_global_recover","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_bind_parameter_name","sqlite3_finalize","sqlite3_rollback_hook","sqlite3_strglob","sqlite3_serialize","sqlite3_free","sqlite3_global_recover","sqlite3_close_v2"],"visited":1}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_0(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;
    unsigned char *serialized_data = nullptr;
    sqlite3_int64 serialized_size = 0;
    const char *param_name = nullptr;
    
    // Open an in-memory database
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }
    
    // Create a table and insert some data using input
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_sql, nullptr, nullptr, nullptr);
    
    // Prepare an INSERT statement with parameters
    const char *insert_sql = "INSERT INTO test VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Bind parameters using input data
        if (param_count >= 1) {
            // Use first byte as integer ID
            int id = (data[0] % 100) + 1;
            sqlite3_bind_int(stmt, 1, id);
            
            // Use remaining data as text (with bounds checking)
            if (size > 1) {
                size_t text_len = size - 1;
                if (text_len > 100) text_len = 100; // Limit size
                const char *text_data = reinterpret_cast<const char*>(data + 1);
                sqlite3_bind_text(stmt, 2, text_data, text_len, SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_text(stmt, 2, "default", 7, SQLITE_STATIC);
            }
            
            // Execute the statement
            sqlite3_step(stmt);
        }
        
        // Get parameter name (for demonstration)
        if (param_count >= 1) {
            param_name = sqlite3_bind_parameter_name(stmt, 1);
        }
        
        sqlite3_finalize(stmt);
    }
    
    // Set a rollback hook
    static int rollback_count = 0;
    sqlite3_rollback_hook(db, [](void *pCount) {
        int *count = static_cast<int*>(pCount);
        if (count) (*count)++;
    }, &rollback_count);
    
    // Perform string glob matching using input data
    if (size >= 3) {
        // Use first 2 bytes as pattern and string for strglob
        char pattern[3] = {static_cast<char>(data[0]), static_cast<char>(data[1]), 0};
        char match_str[3] = {static_cast<char>(data[1]), static_cast<char>(data[2]), 0};
        
        int glob_result = sqlite3_strglob(pattern, match_str);
        (void)glob_result; // Use result to avoid unused variable warning
    }
    
    // Serialize the database
    // Use the first 4 bytes to determine flags if enough data
    unsigned int flags = 0;
    if (size >= 4) {
        flags = *reinterpret_cast<const unsigned int*>(data) & 0x3; // Use only 2 bits
    }
    
    serialized_data = sqlite3_serialize(db, "main", &serialized_size, flags);
    
    // Clean up serialized data if allocated
    if (serialized_data) {
        sqlite3_free(serialized_data);
    }
    
    // Attempt global recovery (this may not always succeed)
    int recovery_result = sqlite3_global_recover();
    (void)recovery_result; // Use result to avoid unused variable warning
    
    // Cleanup
    if (db) {
        sqlite3_close_v2(db);
    }
    
    return 0;
}