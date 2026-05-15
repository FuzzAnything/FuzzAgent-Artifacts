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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 3
//<Quality> {"density":10,"unique_branches":{"sqlite3VdbeSetSql":[[84047,7,84047,46,0,0,4,0]],"sqlite3VdbeMemValidStrRep":[[82124,7,82124,26,0,0,4,0],[82126,9,82126,31,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_memory_alarm","sqlite3_limit","sqlite3_exec","sqlite3_prepare","sqlite3_step","sqlite3_column_count","sqlite3_column_database_name16","sqlite3_column_bytes16","sqlite3_finalize","sqlite3_mutex_held","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_memory_alarm","sqlite3_limit","sqlite3_exec","sqlite3_prepare","sqlite3_step","sqlite3_column_count","sqlite3_column_database_name16","sqlite3_column_bytes16","sqlite3_finalize","sqlite3_mutex_held","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 10) {
	return 0;
	}
	  // Need minimum data for operations

    // Create memory file from input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	

    // Get file descriptor for input
    int in_fd = fuzz_fileno(in_file);
    if (in_fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Open SQLite database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
    }

    // Set memory alarm callback
    sqlite3_int64 threshold = 1000000;  // 1MB threshold
    sqlite3_memory_alarm([](void *pArg, sqlite3_int64 used, int) {
        // Simple callback that does nothing but satisfies API usage
        (void)pArg;
        (void)used;
    }, nullptr, threshold);

    // Set various limits using input data values
    // Use data bytes to determine limit values (modulo for safe ranges)
    for (int i = 0; i < 5 && i * 2 + 1 < (int)size; i++) {
        int limit_id = data[i * 2] % 10;  // Limit IDs 0-9
        int new_val = data[i * 2 + 1] % 1000;
        sqlite3_limit(db, limit_id, new_val);
    }

    // Create a simple table and query it
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);"
        "INSERT INTO test VALUES (1, 'test1');"
        "INSERT INTO test VALUES (2, 'test2');";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);

    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *query = "SELECT * FROM test;";
    rc = sqlite3_prepare(db, query, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            
            for (int i = 0; i < col_count; i++) {
                // Call sqlite3_column_database_name16
                const void *db_name = sqlite3_column_database_name16(stmt, i);
                (void)db_name;  // Use variable to avoid unused warning
                
                // Call sqlite3_column_bytes16
                int bytes = sqlite3_column_bytes16(stmt, i);
                (void)bytes;  // Use variable to avoid unused warning
            }
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }

    // Check mutex status (using NULL pointer as allowed by documentation)
    int mutex_held = sqlite3_mutex_held(nullptr);
    (void)mutex_held;  // Use variable to avoid unused warning

    // Read some data from input file to ensure data consumption
    char buffer[256];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer), in_file);
    (void)bytes_read;  // Use variable to avoid unused warning

    // Close input file
    assert_file_closed(&in_file);;

    // Close database
    sqlite3_close(db);

    assert_file_closed(&in_file);
	assert_fd_closed(in_fd);
	return 0;
}