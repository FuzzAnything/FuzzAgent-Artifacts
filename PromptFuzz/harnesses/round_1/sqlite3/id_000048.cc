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
//<ID> 302
//<Prompt> []
/*<Combination>: [
*/
//<score> 36, nr_unique_branch: 1
//<Quality> {"density":18,"unique_branches":{"bindText":[[91145,9,91145,17,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_close","sqlite3_shutdown","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_int","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_sleep","sqlite3_thread_cleanup","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_column_int","sqlite3_column_value","sqlite3_value_pointer","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_sleep","sqlite3_thread_cleanup","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite library
    sqlite3_initialize();
    
    // Open a database in memory
    sqlite3* db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    // Create a test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test ("
        "id INTEGER PRIMARY KEY, "
        "value_int INTEGER, "
        "value_blob BLOB);";
    
    sqlite3_stmt* create_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &create_stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(create_stmt);
        sqlite3_finalize(create_stmt);
    }
    
    // Prepare an insert statement
    const char* insert_sql = 
        "INSERT INTO test (value_int, value_blob) VALUES (?, ?);";
    
    sqlite3_stmt* insert_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind integer value from first 4 bytes of input
        int int_value = 0;
        if (size >= sizeof(int)) {
            memcpy(&int_value, data, sizeof(int));
        }
        sqlite3_bind_int(insert_stmt, 1, int_value);
        
        // Bind blob value from remaining data
        const void* blob_data = (size > sizeof(int)) ? data + sizeof(int) : nullptr;
        int blob_size = (size > sizeof(int)) ? (int)(size - sizeof(int)) : 0;
        sqlite3_bind_blob(insert_stmt, 2, blob_data, blob_size, SQLITE_STATIC);
        
        // Execute insert
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Query the data back
    const char* select_sql = "SELECT id, value_int, value_blob FROM test;";
    sqlite3_stmt* select_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // Get column values using various APIs
            int id = sqlite3_column_int(select_stmt, 0);
            int retrieved_int = sqlite3_column_int(select_stmt, 1);
            
            // Get column value as sqlite3_value
            sqlite3_value* col_value = sqlite3_column_value(select_stmt, 2);
            if (col_value) {
                // Try to get as pointer (will likely return null for blob)
                void* ptr = sqlite3_value_pointer(col_value, "blob");
                (void)ptr; // Unused
            }
        }
        sqlite3_finalize(select_stmt);
    }
    
    // String comparison using input data
    if (size >= 20) {
        const char* str1 = reinterpret_cast<const char*>(data);
        const char* str2 = reinterpret_cast<const char*>(data + 10);
        int cmp_result = sqlite3_strnicmp(str1, str2, 10);
        (void)cmp_result; // Unused
    }
    
    // Check memory usage
    sqlite3_int64 highwater = sqlite3_memory_highwater(1); // Reset flag
    
    // Use sleep API (minimal delay)
    sqlite3_sleep(0);
    
    // Clean up thread-specific data
    sqlite3_thread_cleanup();
    
    // Clean up database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}