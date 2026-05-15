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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 1
//<Quality> {"density":9,"unique_branches":{"sqlite3_error_offset":[[180260,47,180260,58,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_error_offset","sqlite3_error_offset","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_stmt_status","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_error_offset","sqlite3_error_offset","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_stmt_status","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_finalize","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 2. Open a database connection
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Use error offset if available (check after potential errors)
    int error_offset = 0;
    if (sqlite3_error_offset(db) > 0) {
        error_offset = sqlite3_error_offset(db);
    }
    
    // 4. Create a table and prepare a statement
    const char* create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO test VALUES (?, 'test');";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (stmt && rc == SQLITE_OK) {
        // 5. Use sqlite3_bind_int with data from input
        int bind_value = 0;
        if (size > 0) {
            bind_value = static_cast<int>(data[0]);
        }
        sqlite3_bind_int(stmt, 1, bind_value);
        
        // 6. Execute the statement
        rc = sqlite3_step(stmt);
        
        // 7. Use sqlite3_stmt_status
        int stmt_status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        (void)stmt_status; // Use variable to avoid unused warning
    }
    
    // 8. Use sqlite3_strnicmp for string comparison
    const char* str1 = "SQLite";
    const char* str2 = "sqlite";
    int compare_result = sqlite3_strnicmp(str1, str2, 6);
    (void)compare_result; // Use variable to avoid unused warning
    
    // 9. Use sqlite3_memory_highwater
    sqlite3_int64 memory_highwater = sqlite3_memory_highwater(0);
    (void)memory_highwater; // Use variable to avoid unused warning
    
    // 10. Read some data from the input file
    if (size > 0) {
        uint8_t buffer[256];
        size_t to_read = size < sizeof(buffer) ? size : sizeof(buffer);
        fread(buffer, 1, to_read, in_file);
    }
    
    // 11. Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Reset error offset usage
    (void)error_offset; // Use variable to avoid unused warning
    
    // Close database
    sqlite3_close_v2(db);
    
    // Close file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	return 0;
}