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
//<ID> 371
//<Prompt> []
/*<Combination>: [
*/
//<score> 161, nr_unique_branch: 13
//<Quality> {"density":23,"unique_branches":{"sqlite3_limit":[[180528,15,180528,25,0,0,4,0],[180528,29,180528,57,0,0,4,0]],"sqlite3StrAccumEnlarge":[[31663,7,31663,18,0,0,4,0],[31674,9,31674,35,0,0,4,1],[31679,9,31679,27,0,0,4,0],[14098,27,14098,28,1,0,4,0],[14098,27,14098,28,2,0,4,1]],"sqlite3_str_reset":[[19944,24,19944,72,1,0,4,0]],"sqlite3VdbeMemSetStr":[[83139,17,83139,39,0,0,4,1],[83140,11,83140,31,0,0,4,0]],"sqlite3ErrorToParser":[[34710,16,34710,40,0,0,4,1]],"sqlite3NestedParse":[[120450,7,120450,14,0,0,4,0],[120454,9,120454,26,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_limit","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_extended_errcode","sqlite3_error_offset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_finalize","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_snapshot_cmp","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_limit","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_extended_errcode","sqlite3_error_offset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_finalize","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_snapshot_cmp","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
	if(f_size<172) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_snapshot *snapshot = nullptr;
    char *err_msg = nullptr;
    int rc;
    
    // 1. Open database with various flags
    rc = sqlite3_open_v2(fuzz_str_1, &db, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
                        nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Set some limits based on input data
    if (size >= sizeof(int)) {
        int limit_val = *(reinterpret_cast<const int*>(data));
        sqlite3_limit(db, fuzz_int32_t_2, limit_val & 0xFFFF);
    }
    
    // 3. Create a table and insert data from input
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_3, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 4. Prepare INSERT statement
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 5. Bind blob from input data
        if (size > 0) {
            // Use first part of data for binding
            size_t bind_size = size > 100 ? 100 : size;
            sqlite3_bind_blob(stmt, fuzz_int32_t_4, data, static_cast<int>(bind_size), SQLITE_STATIC);
            
            // 6. Execute statement
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE || rc == SQLITE_ROW) {
                // 7. Reset for potential reuse
                sqlite3_reset(stmt);
            }
            
            // 8. Get extended error code
            int extended_err = sqlite3_extended_errcode(db);
            
            // 9. Get error offset if any
            int err_offset = sqlite3_error_offset(db);
        }
        
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 10. Prepare SELECT statement
    const char *select_sql = "SELECT data FROM test ORDER BY id DESC LIMIT 1;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 11. Execute SELECT
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            // 12. Get column value
            sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_5);
            if (value) {
                // 13. Get subtype if available
                unsigned int subtype = sqlite3_value_subtype(value);
                (void)subtype; // Use to avoid unused variable warning
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 14. Get database status
    int current = 0, highwater = 0;
    sqlite3_db_status(db, fuzz_int32_t_6, &current, &highwater, fuzz_int32_t_7);
    
    // 15. Try to get snapshot (may fail for in-memory DB, but we try)
    sqlite3_snapshot_get(db, fuzz_str_8, &snapshot);
    if (snapshot) {
        // Compare with itself (trivial comparison)
        int cmp = sqlite3_snapshot_cmp(snapshot, snapshot);
        (void)cmp;
        sqlite3_free(snapshot);
        snapshot = nullptr;
    }
    
    // 16. Clean up
    assert_file_closed(&in_file);;
    
    // Execute another SQL statement that might trigger virtual table collation
    // Note: We can't easily create a virtual table, but we can prepare for it
    const char *vtab_sql = "CREATE VIRTUAL TABLE IF NOT EXISTS vtab USING fts5(content);";
    rc = sqlite3_exec(db, fuzz_str_9, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 17. Close database
    sqlite3_close_v2(db);
    
    assert_file_closed(&in_file);
	return 0;
}