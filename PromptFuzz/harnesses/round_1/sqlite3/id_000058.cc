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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 7
//<Quality> {"density":12,"unique_branches":{"sqlite3_stmt_scanstatus_v2":[[91875,16,91875,28,0,0,4,1],[91883,7,91883,20,0,0,4,0]],"blobSeekToRow":[[101363,7,101363,14,0,0,4,0]],"sqlite3_blob_reopen":[[101784,7,101784,11,0,0,4,1],[101788,7,101788,18,0,0,4,1],[101797,9,101797,22,0,0,4,0],[101798,36,101798,40,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_exec","sqlite3_close","sqlite3_compileoption_get","sqlite3_prepare_v3","sqlite3_bind_zeroblob","sqlite3_reset","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_close","sqlite3_stmt_scanstatus","sqlite3_vfs_find","sqlite3_vfs_unregister","sqlite3_vfs_register","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_compileoption_get","sqlite3_prepare_v3","sqlite3_bind_zeroblob","sqlite3_reset","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_blob_open","sqlite3_blob_reopen","sqlite3_blob_close","sqlite3_stmt_scanstatus","sqlite3_vfs_find","sqlite3_vfs_unregister","sqlite3_vfs_register","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t* f_data, size_t f_size) {
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
    sqlite3_blob *blob = nullptr;
    sqlite3_vfs *vfs = nullptr;
    
    // Create a temporary database in memory
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
	return 0;
	}
	
    
    // Create a table with BLOB column for blob operations
    const char *create_table = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test (data) VALUES (zeroblob(100));";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. Call sqlite3_compileoption_get with input-derived index
    int compile_idx = static_cast<int>(data[0]) % 50;  // Ensure valid range
    const char *compile_opt = sqlite3_compileoption_get(compile_idx);
    // Option exists check is not needed for fuzzing
    
    // 2. Call sqlite3_bind_zeroblob in a prepared statement
    const char *insert_sql = "INSERT INTO test (data) VALUES (?)";
    rc = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        int blob_size = static_cast<int>(size % 1024) + 1;  // Non-zero size
        rc = sqlite3_bind_zeroblob(stmt, 1, blob_size);
        sqlite3_reset(stmt);
    }
    
    // 3. Call sqlite3_filename_wal (requires filename from db)
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
    if (db_filename) {
        const char *wal_name = sqlite3_filename_wal(db_filename);
        // wal_name may be nullptr if no WAL file exists
    }
    
    // 4. Call sqlite3_blob_reopen - open blob and try to reopen
    rc = sqlite3_blob_open(db, "main", "test", "data", 1, 1, &blob);
    if (rc == SQLITE_OK && blob) {
        // Try to reopen to different row (rowid 2 may not exist)
        sqlite3_blob_reopen(blob, 2);
        sqlite3_blob_close(blob);
    }
    
    // 5. Call sqlite3_stmt_scanstatus if statement was prepared
    if (stmt) {
        void *scan_out = nullptr;
        // Use small index 0 and SQLITE_SCANSTAT_NLOOP operation (value 3)
        sqlite3_stmt_scanstatus(stmt, 0, 3, &scan_out);
    }
    
    // 6. Call sqlite3_vfs_unregister - try to unregister current VFS
    vfs = sqlite3_vfs_find(nullptr);  // Get default VFS
    if (vfs) {
        // Note: Unregistering default VFS may fail but we attempt it
        sqlite3_vfs_unregister(vfs);
        // Re-register for cleanup (not strictly needed for fuzzing)
        sqlite3_vfs_register(vfs, 1);
    }
    
    // Clean up
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}