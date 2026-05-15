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
//<ID> 329
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 3
//<Quality> {"density":20,"unique_branches":{"vdbeCompareMemString":[[88370,7,88370,29,0,0,4,1],[88385,10,88385,15,0,0,4,1],[88385,19,88385,24,0,0,4,1]]},"library_calls":["sqlite3_os_init","sqlite3_open16","sqlite3_close_v2","sqlite3_busy_timeout","sqlite3_rollback_hook","sqlite3_wal_autocheckpoint","sqlite3_collation_needed","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_strglob","sqlite3_next_stmt","sqlite3_close_v2"],"critical_calls":["sqlite3_os_init","sqlite3_open16","sqlite3_busy_timeout","sqlite3_rollback_hook","sqlite3_wal_autocheckpoint","sqlite3_collation_needed","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_strglob","sqlite3_next_stmt","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
	return 0;
	}
	

    // Initialize OS interface
    sqlite3_os_init();

    // Open database from memory
    sqlite3 *db = nullptr;
    const char16_t *db_name = u":memory:";
    int rc = sqlite3_open16(db_name, &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }

    // Set busy timeout
    sqlite3_busy_timeout(db, 100);

    // Set up rollback hook
    static int rollback_count = 0;
    auto rollback_callback = [](void *) { rollback_count++; };
    sqlite3_rollback_hook(db, rollback_callback, nullptr);

    // Set WAL autocheckpoint
    sqlite3_wal_autocheckpoint(db, 1000);

    // Set up collation needed callback
    auto collation_needed_callback = [](void *, sqlite3 *, int, const char *) {};
    sqlite3_collation_needed(db, nullptr, collation_needed_callback);

    // Create table
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT)";
    sqlite3_stmt *create_stmt = nullptr;
    const char *tail = nullptr;
    rc = sqlite3_prepare_v2(db, create_sql, -1, &create_stmt, &tail);
    if (rc == SQLITE_OK && create_stmt) {
        sqlite3_step(create_stmt);
        sqlite3_finalize(create_stmt);
    }

    // Prepare insert statement with parameters
    const char *insert_sql = "INSERT INTO test(data) VALUES(?)";
    sqlite3_stmt *insert_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, &tail);
    
    if (rc == SQLITE_OK && insert_stmt) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(insert_stmt);
        
        // Bind data from fuzzer input
        if (param_count >= 1) {
            // Convert input data to string for binding
            size_t str_len = size > 1024 ? 1024 : size;
            char *str_data = static_cast<char*>(malloc(str_len + 1));
            if (str_data) {
                memcpy(str_data, data, str_len);
                str_data[str_len] = '\0';
                
                // Get parameter name (for demonstration)
                const char *param_name = sqlite3_bind_parameter_name(insert_stmt, 1);
                
                // Bind integer from first byte
                if (size >= 1) {
                    sqlite3_bind_int(insert_stmt, 1, static_cast<int>(data[0]));
                }
                
                free(str_data);
            }
        }
        
        // Execute statement
        rc = sqlite3_step(insert_stmt);
        
        // Finalize statement
        sqlite3_finalize(insert_stmt);
    }

    // Prepare select statement
    const char *select_sql = "SELECT * FROM test";
    sqlite3_stmt *select_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, &tail);
    
    if (rc == SQLITE_OK && select_stmt) {
        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // Process rows if needed
        }
        sqlite3_finalize(select_stmt);
    }

    // Use strglob with fuzzer data
    if (size >= 2) {
        // Create null-terminated strings for comparison
        char *pattern = static_cast<char*>(malloc(size/2 + 1));
        char *str = static_cast<char*>(malloc(size/2 + 1));
        
        if (pattern && str) {
            size_t half = size/2;
            memcpy(pattern, data, half);
            pattern[half] = '\0';
            memcpy(str, data + half, half);
            str[half] = '\0';
            
            sqlite3_strglob(pattern, str);
            
            free(pattern);
            free(str);
        }
    }

    // Iterate through all statements
    sqlite3_stmt *stmt = nullptr;
    while ((stmt = sqlite3_next_stmt(db, stmt)) != nullptr) {
        // Process each statement
    }

    // Cleanup
    sqlite3_close_v2(db);
    
    return 0;
}