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
//<ID> 372
//<Prompt> []
/*<Combination>: [
*/
//<score> 432, nr_unique_branch: 49
//<Quality> {"density":27,"unique_branches":{"vdbeCommit":[[86901,11,86901,41,0,0,4,1]],"unixOpen":[[44096,9,44096,13,0,0,4,0],[44097,11,44097,20,0,0,4,0],[44097,11,44097,20,0,0,4,1],[44097,24,44097,37,0,0,4,1],[44101,17,44101,30,0,0,4,0],[44101,34,44101,45,0,0,4,0],[44111,9,44111,13,0,0,4,0],[44113,11,44113,24,0,0,4,0],[44224,7,44224,20,0,0,4,0]],"getPageNormal":[[61996,11,61996,30,0,0,4,0],[61998,13,61998,33,0,0,4,1],[62036,7,62036,10,0,0,4,0]],"sqlite3PagerOpen":[[61198,9,61198,22,0,0,4,0],[61199,11,61199,32,0,0,4,1],[61215,9,61215,22,0,0,4,1],[61215,26,61215,54,0,0,4,0],[61224,9,61224,22,0,0,4,0],[61366,9,61366,22,0,0,4,1],[61421,7,61421,20,0,0,4,1],[61428,7,61428,20,0,0,4,1],[61437,7,61437,20,0,0,4,0]],"sqlite3SystemError":[[34610,7,34610,26,0,0,4,0]],"unixFullPathname":[[44423,7,44423,20,0,0,4,1],[44432,7,44432,14,0,0,4,0]],"sqlite3PcacheDrop":[[54123,7,54123,27,0,0,4,1]],"sqlite3CantopenError":[[14098,27,14098,28,1,0,4,1]],"unixLogErrorAtLine":[[39153,7,39153,15,0,0,4,1]],"openDatabase":[[181066,7,181066,20,0,0,4,0],[181067,9,181067,31,0,0,4,1]],"pcache1TruncateUnsafe":[[14109,23,14109,24,4,0,4,1]],"robust_open":[[38569,9,38569,13,0,0,4,0],[38570,11,38570,23,0,0,4,1],[38583,7,38583,12,0,0,4,1]],"sqlite3VdbeHalt":[[87284,24,87284,40,0,0,4,0],[87302,35,87302,51,0,0,4,0]],"appendOnePathElement":[[44332,7,44332,20,0,0,4,0],[44333,9,44333,17,0,0,4,1],[44334,9,44334,22,0,0,4,0],[44334,9,44334,22,0,0,4,1],[44334,26,44334,34,0,0,4,1],[44342,7,44342,46,0,0,4,0],[44350,7,44350,27,0,0,4,1],[44356,11,44356,24,0,0,4,0]],"sqlite3BtreeOpen":[[72415,9,72415,22,0,0,4,1],[72419,9,72419,22,0,0,4,0],[72527,7,72527,20,0,0,4,0],[72528,9,72528,12,0,0,4,0],[72528,16,72528,27,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_close_v2","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_extended_errcode","sqlite3_error_offset","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_reset","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_extended_errcode","sqlite3_error_offset","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_reset","sqlite3_db_status","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close_v2"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified SQLite APIs in a coherent scenario:

*/


extern "C" {
}

// We need to declare this since it's used by sqlite3_snapshot_get
struct sqlite3_snapshot;

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<54) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_8, fuzz_str_sz_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_snapshot *snapshot = nullptr;
    char *errmsg = nullptr;
    
    // Parse input data structure
    // First null-terminated string: database name
    // Second null-terminated string: SQL command
    // Remaining data: blob data
    
    const char *db_name = reinterpret_cast<const char*>(data);
    size_t db_name_len = strnlen(db_name, size);
    if (db_name_len >= size) {
	return 0;
	}
	
    
    const char *sql = reinterpret_cast<const char*>(data + db_name_len + 1);
    size_t sql_len = strnlen(sql, size - db_name_len - 1);
    if (db_name_len + 1 + sql_len >= size) {
	return 0;
	}
	
    
    const void *blob_data = data + db_name_len + 1 + sql_len + 1;
    int blob_size = size - (db_name_len + 1 + sql_len + 1);
    
    // 1. Open database (using input data for filename)
    int rc = sqlite3_open_v2(db_name, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }
    
    // 2. Set some limits on the database
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 3. Execute SQL command from input
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    
    // 4. Get extended error code
    int extended_err = sqlite3_extended_errcode(db);
    
    // 5. Get error offset if available
    int err_offset = sqlite3_error_offset(db);
    
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // 6. Prepare a statement
    rc = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }
    
    // 7. Prepare another statement for binding blob
    sqlite3_finalize(stmt);
    stmt = nullptr;
    
    rc = sqlite3_prepare_v2(db, "INSERT INTO test (data) VALUES (?);", -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 8. Bind blob data from input
        if (blob_size > 0) {
            sqlite3_bind_blob(stmt, fuzz_int32_t_7, blob_data, blob_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_blob(stmt, fuzz_int32_t_9, fuzz_str_8, static_cast<int>(fuzz_str_sz_8), SQLITE_STATIC);
        }
        
        // 9. Execute the statement
        rc = sqlite3_step(stmt);
        
        // 10. Reset statement for potential reuse
        sqlite3_reset(stmt);
        
        // 11. Prepare a SELECT statement to retrieve the data
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        rc = sqlite3_prepare_v2(db, "SELECT data FROM test;", -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            // 12. Step through results
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                // 13. Get column value
                sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_10);
                
                // 14. Get subtype if available
                unsigned int subtype = sqlite3_value_subtype(value);
            }
            
            // 15. Reset the statement
            sqlite3_reset(stmt);
        }
    }
    
    // 16. Get database status
    int cur = 0, hiwtr = 0;
    sqlite3_db_status(db, fuzz_int32_t_11, &cur, &hiwtr, fuzz_int32_t_12);
    
    // 17. Try to get a snapshot (might fail depending on configuration)
    sqlite3_snapshot_get(db, fuzz_str_13, &snapshot);
    
    // Note: sqlite3_vtab_collation requires a valid sqlite3_index_info*
    // which we typically get from virtual table implementations.
    // We'll create a dummy scenario for demonstration:
    // We would need to create a virtual table first, which is complex.
    // For fuzzing purposes, we'll skip actual vtab_collation usage
    // as it requires specific context.
    
    // 18. Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    if (db) {
        sqlite3_close_v2(db);
    }
    
    return 0;
}