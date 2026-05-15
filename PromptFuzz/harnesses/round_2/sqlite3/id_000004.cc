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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 238, nr_unique_branch: 43
//<Quality> {"density":17,"unique_branches":{"apiHandleError":[[30684,7,30684,23,0,0,4,1],[30684,27,30684,49,0,0,4,1]],"sqlite3SrcListLookup":[[126418,7,126418,11,0,0,4,1]],"sqlite3VdbeSetSql":[[84045,7,84045,11,0,0,4,0]],"sqlite3LockAndPrepare":[[141077,9,141077,22,0,0,4,1],[141077,26,141077,42,0,0,4,1],[141078,12,141078,34,0,0,4,1],[141079,12,141079,29,0,0,4,1]],"sqlite3Prepare":[[141015,31,141015,53,0,0,4,0],[141016,9,141016,27,0,0,4,0],[141016,31,141016,47,0,0,4,0],[141019,9,141019,21,0,0,4,1],[141024,9,141024,23,0,0,4,0]],"sqlite3RunParser":[[177025,7,177025,22,0,0,4,0],[177026,9,177026,27,0,0,4,1]],"sqlite3ErrorMsg":[[34687,7,34687,27,0,0,4,0],[34688,7,34688,22,0,0,4,1]],"schemaIsValid":[[140720,14,140720,25,0,0,4,0],[140720,14,140720,25,0,0,4,1],[140723,9,140723,15,0,0,4,0],[140723,9,140723,15,0,0,4,1],[140728,9,140728,51,0,0,4,0],[140730,11,140730,27,0,0,4,1],[140730,31,140730,53,0,0,4,1],[140734,11,140734,24,0,0,4,1],[140743,9,140743,52,0,0,4,1],[140749,9,140749,26,0,0,4,0]],"sqlite3DeleteTable":[[121010,7,121010,14,0,0,4,0]],"sqlite3LocateTable":[[120587,7,120587,11,0,0,4,0],[120592,9,120592,56,0,0,4,0],[120592,60,120592,76,0,0,4,0],[120594,11,120594,18,0,0,4,0],[120594,22,120594,62,0,0,4,1],[120597,11,120597,15,0,0,4,1],[120603,9,120603,29,0,0,4,1],[120609,7,120609,11,0,0,4,0],[120610,24,120610,43,0,0,4,1],[120611,9,120611,15,0,0,4,1]],"sqlite3ApiExit":[[30711,27,30711,29,0,0,4,0]],"sqlite3Insert":[[132409,7,132409,14,0,0,4,0],[133089,7,133089,14,0,0,4,1]],"sqlite3FinishCoding":[[120287,7,120287,19,0,0,4,0],[120288,9,120288,25,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_backup_finish","sqlite3_close","sqlite3_db_name","sqlite3_txn_state","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_bind_blob64","sqlite3_bind_null","sqlite3_step","sqlite3_finalize","sqlite3_release_memory","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_open","sqlite3_backup_init","sqlite3_backup_remaining","sqlite3_backup_finish","sqlite3_close","sqlite3_db_name","sqlite3_txn_state","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_null","sqlite3_bind_blob64","sqlite3_step","sqlite3_finalize","sqlite3_release_memory","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    int rc;
    
    // Open main database from input file
    int fd = fuzz_fileno(in_file);
    if (fd < 0) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Try to open database in read-only mode
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }
    
    // Import data into in-memory database using backup API (simulating restore)
    // Create a temporary on-disk database for backup operations
    sqlite3 *temp_db = nullptr;
    rc = sqlite3_open(":memory:", &temp_db);
    if (rc == SQLITE_OK && temp_db) {
        // Use backup API (even though we can't initialize it directly, we can simulate usage)
        backup = sqlite3_backup_init(db, "main", temp_db, "main");
        if (backup) {
            // Get remaining pages in backup
            int remaining = sqlite3_backup_remaining(backup);
            // Finish backup
            sqlite3_backup_finish(backup);
        }
        sqlite3_close(temp_db);
    }
    
    // Get database name
    const char *db_name = sqlite3_db_name(db, 0);
    
    // Check transaction state
    int txn_state = sqlite3_txn_state(db, db_name ? db_name : "main");
    
    // Prepare a simple statement
    const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Prepare another statement for binding
    const char *insert_sql = "INSERT INTO test VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind null to first parameter
        sqlite3_bind_null(stmt, 1);
        
        // Bind blob from input data to second parameter
        if (size > 0) {
            // Use first min(size, 100) bytes for blob
            size_t blob_size = size > 100 ? 100 : size;
            sqlite3_bind_blob64(stmt, 2, data, blob_size, SQLITE_STATIC);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
        
        // Execute the statement
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Attempt to release memory
    sqlite3_release_memory(100);
    
    // Cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    // Close the input file
    assert_file_closed(&in_file);;
    
    assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
}