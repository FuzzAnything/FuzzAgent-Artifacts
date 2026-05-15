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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 20, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"sqlite3_get_autocommit":[[181420,7,181420,32,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_close_v2","sqlite3_limit","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_close_v2","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_reset","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_get_autocommit","sqlite3_limit","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_limit","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_column_count","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_reset","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_get_autocommit","sqlite3_limit","sqlite3_finalize","sqlite3_close_v2"],"visited":1}
/*Here's a C++ fuzzer that uses the specified SQLite3 APIs to simulate a database operation scenario:

*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t* f_data, size_t f_size) {
	if(f_size<131) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_snapshot *snapshot = nullptr;
    int rc = 0;
    
    // Create a temporary database file
    const char *db_name = "fuzz_test.db";
    
    // Open database
    rc = sqlite3_open_v2(fuzz_str_1, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }
    
    // Set some limits
    int old_limit = sqlite3_limit(db, fuzz_int32_t_2, fuzz_int32_t_3);
    
    // Create a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_4, NULL, NULL, NULL);
    
    // Insert some data using the input
    if (size > 0) {
        sqlite3_stmt *insert_stmt = nullptr;
        const char *insert_sql = "INSERT INTO test_table (data) VALUES (?);";
        
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, NULL);
        if (rc == SQLITE_OK && insert_stmt) {
            // Bind the input data as blob
            rc = sqlite3_bind_blob(insert_stmt, fuzz_int32_t_5, data, 
                                  (size > 1000) ? 1000 : (int)size, SQLITE_STATIC);
            if (rc == SQLITE_OK) {
                sqlite3_step(insert_stmt);
            }
            sqlite3_finalize(insert_stmt);
        }
    }
    
    // Prepare a SQL statement using the input data as SQL text
    // Use a portion of input as SQL (ensuring it's null-terminated)
    size_t sql_len = (size < 100) ? size : 100;
    char *sql_text = (char *)malloc(sql_len + 1);
    if (!sql_text) {
        sqlite3_close_v2(db);
        remove(db_name);
        return 0;
    }
    
    memcpy(sql_text, data, sql_len);
    sql_text[sql_len] = '\0';
    
    // Prepare statement using the pseudo sqlite3_prepare_v3 (using v2 as fallback)
    const char *tail = nullptr;
    rc = sqlite3_prepare_v2(db, sql_text, -1, &stmt, &tail);
    free(sql_text);
    
    // Get column count if statement was prepared successfully
    int column_count = 0;
    if (rc == SQLITE_OK && stmt) {
        column_count = sqlite3_column_count(stmt);
        
        // Try to get a value from the statement (if any)
        if (column_count > 0) {
            // Step once to get values
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_ROW) {
                // Get a value from the first column
                sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_6);
                if (value) {
                    // Try to get subtype (though it may be 0 for most values)
                    unsigned int subtype = sqlite3_value_subtype(value);
                    (void)subtype; // Use to avoid unused variable warning
                }
            }
            sqlite3_reset(stmt);
        }
    }
    
    // Get database status
    int current = 0, highwater = 0;
    rc = sqlite3_db_status(db, fuzz_int32_t_7, &current, &highwater, fuzz_int32_t_8);
    
    // Try to get a snapshot (this may fail if WAL mode not enabled)
    rc = sqlite3_snapshot_get(db, fuzz_str_9, &snapshot);
    // Note: snapshot may be nullptr if WAL mode not enabled
    
    // Check autocommit mode
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit; // Use to avoid unused variable warning
    
    // Use the limit API again
    int new_limit = sqlite3_limit(db, fuzz_int32_t_10, fuzz_int32_t_11);
    (void)new_limit; // Use to avoid unused variable warning
    
    // Finalize statement if created
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Close database
    sqlite3_close_v2(db);
    
    // Clean up temporary file
    remove(db_name);
    
    return 0;
}