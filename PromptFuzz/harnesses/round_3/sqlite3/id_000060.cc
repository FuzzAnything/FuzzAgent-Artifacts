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
//<ID> 373
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"openDatabase":[[180841,13,180841,40,0,0,4,0],[180881,7,180881,19,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_open_v2","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_extended_errcode","sqlite3_error_offset","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_reset","sqlite3_db_status","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_vtab_collation","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_open_v2","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_extended_errcode","sqlite3_error_offset","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_reset","sqlite3_db_status","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_vtab_collation","sqlite3_finalize","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t* f_data, size_t f_size) {
	if(f_size<157) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_18, fdp);
	//fuzzer shim end}




    // Create a file from the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for writing
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_snapshot *snapshot = nullptr;
    char *errmsg = nullptr;
    int rc;
    
    // 1. Open database with various flags
    rc = sqlite3_open_v2(fuzz_str_1, &db, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | 
                        SQLITE_OPEN_MEMORY | SQLITE_OPEN_NOMUTEX,
                        nullptr);
    
    if (rc != SQLITE_OK) {
        // If opening fails, try with different flags
        sqlite3_open_v2(fuzz_str_2, &db, fuzz_int32_t_3, nullptr);
    }
    
    if (!db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Set various limits
    sqlite3_limit(db, fuzz_int32_t_4, fuzz_int32_t_5);
    sqlite3_limit(db, fuzz_int32_t_6, fuzz_int32_t_7);
    sqlite3_limit(db, fuzz_int32_t_8, fuzz_int32_t_9);
    
    // 3. Create a table and insert data using exec
    const char *create_sql = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, fuzz_str_10, nullptr, nullptr, &errmsg);
    
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // 4. Prepare a statement for inserting data
    const char *insert_sql = "INSERT INTO test(data) VALUES(?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 5. Bind blob data from input
        if (size > 0) {
            // Use first part of data for blob binding
            size_t bind_size = size > 1000 ? 1000 : size;
            rc = sqlite3_bind_blob(stmt, fuzz_int32_t_11, data, bind_size, SQLITE_STATIC);
            
            if (rc == SQLITE_OK) {
                // 6. Execute the statement
                rc = sqlite3_step(stmt);
                
                // 7. Get extended error code
                int extended_err = sqlite3_extended_errcode(db);
                (void)extended_err; // Use variable
                
                // 8. Get error offset if any
                int err_offset = sqlite3_error_offset(db);
                (void)err_offset; // Use variable
            }
            
            // 9. Reset statement for potential reuse
            sqlite3_reset(stmt);
        }
    }
    
    // 10. Prepare a SELECT statement
    const char *select_sql = "SELECT * FROM test;";
    sqlite3_finalize(stmt);
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 11. Execute SELECT and process results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 12. Get column value
            sqlite3_value *col_value = sqlite3_column_value(stmt, fuzz_int32_t_12);
            if (col_value) {
                // 13. Get subtype of value
                unsigned int subtype = sqlite3_value_subtype(col_value);
                (void)subtype; // Use variable
            }
        }
        sqlite3_reset(stmt);
    }
    
    // 14. Get database status
    int cur = 0, hiwtr = 0;
    sqlite3_db_status(db, fuzz_int32_t_13, &cur, &hiwtr, fuzz_int32_t_14);
    sqlite3_db_status(db, fuzz_int32_t_15, &cur, &hiwtr, fuzz_int32_t_16);
    
    // 15. Try to get snapshot (may fail if not in WAL mode)
    sqlite3_snapshot_get(db, fuzz_str_17, &snapshot);
    
    // 16. Simulate vtab_collation usage (we need sqlite3_index_info)
    // Since we don't have a virtual table, we'll create a minimal structure
    // to demonstrate the API call
    struct DummyIndexInfo {
        int nConstraint;
        int nOrderBy;
        void *aConstraintUsage;
    } dummy_info = {0, 0, nullptr};
    
    // Cast to required type and call (this is for demonstration only)
    const char *collation = sqlite3_vtab_collation(
        reinterpret_cast<sqlite3_index_info*>(&dummy_info), fuzz_int32_t_18);
    (void)collation; // Use variable
    
    // Cleanup
    if (snapshot) {
        // Note: sqlite3_snapshot_free is not in the list, so we just nullify
        snapshot = nullptr;
    }
    
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // 17. Close database
    sqlite3_close_v2(db);
    
    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}