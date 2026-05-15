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
//<ID> 117
//<Prompt> []
/*<Combination>: [
*/
//<score> 204, nr_unique_branch: 75
//<Quality> {"density":24,"unique_branches":{"checkRef":[[80275,7,80275,28,0,0,4,0]],"unixLock":[[39734,11,39734,26,0,0,4,0]],"checkTreePage":[[80516,7,80516,45,0,0,4,0],[80699,23,80699,41,0,0,4,0]],"sqliteErrorFromPosixError":[[38758,3,38758,14,0,0,4,1],[38770,3,38770,10,0,0,4,0]],"sqlite3PagerCheckpoint":[[63923,7,63923,22,0,0,4,0],[63923,7,63923,22,0,0,4,1],[63923,26,63923,68,0,0,4,1],[63935,7,63935,19,0,0,4,0],[63935,7,63935,19,0,0,4,1],[63937,10,63937,42,0,0,4,0]],"sqlite3_uri_key":[[182345,47,182345,54,0,0,4,1],[182349,10,182349,22,0,0,4,0]],"checkAppendMsg":[[80225,7,80225,21,0,0,4,1],[80229,7,80229,27,0,0,4,0],[80229,7,80229,27,0,0,4,1],[80232,7,80232,19,0,0,4,0],[80232,7,80232,19,0,0,4,1],[80238,7,80238,44,0,0,4,1]],"sqlite3WalCheckpoint":[[68459,7,68459,20,0,0,4,1],[68486,9,68486,22,0,0,4,1],[68496,9,68496,22,0,0,4,1],[68505,13,68505,18,0,0,4,0],[68507,13,68507,19,0,0,4,0],[68528,7,68528,21,0,0,4,1],[68536,11,68536,24,0,0,4,1],[14098,27,14098,28,4,0,4,0],[14098,27,14098,28,5,0,4,0]],"sqlite3_uri_boolean":[[182358,10,182358,11,0,0,4,0]],"sqlite3_wal_checkpoint_v2":[[180088,7,180088,32,0,0,4,1],[180092,7,180092,12,0,0,4,0],[180093,7,180093,13,0,0,4,0],[180099,7,180099,38,0,0,4,1],[180099,42,180099,74,0,0,4,1],[180106,7,180106,10,0,0,4,0],[180106,14,180106,20,0,0,4,0],[180111,7,180111,12,0,0,4,1],[180123,7,180123,25,0,0,4,0]],"sqlite3BtreeIntegrityCheck":[[80808,9,80808,24,0,0,4,0],[80811,16,80811,28,0,0,4,0],[80811,16,80811,28,0,0,4,1],[80811,39,80811,50,0,0,4,0],[80813,11,80813,22,0,0,4,0],[80819,15,80819,51,0,0,4,0],[80832,28,80832,38,0,0,4,1],[80853,11,80853,43,0,0,4,0],[80854,11,80854,35,0,0,4,0],[80854,11,80854,35,0,0,4,1],[80854,39,80854,55,0,0,4,1],[80857,11,80857,43,0,0,4,1],[80871,7,80871,21,0,0,4,1]],"checkList":[[80334,9,80334,32,0,0,4,0],[80380,7,80380,8,0,0,4,0],[80380,12,80380,37,0,0,4,1]],"sqlite3BtreeCheckpoint":[[80929,7,80929,8,0,0,4,0],[80932,9,80932,39,0,0,4,1]],"uriParameter":[[180804,9,180804,13,0,0,4,0]],"sqlite3VdbeExec":[[93977,7,93977,17,0,0,4,1],[99327,7,99327,14,0,0,4,1],[99329,13,99329,15,0,0,4,1],[14098,27,14098,28,123,0,4,0]],"sqlite3Checkpoint":[[180176,12,180176,21,0,0,4,0],[180176,12,180176,21,0,0,4,1],[180176,25,180176,38,0,0,4,0],[180177,9,180177,15,0,0,4,0],[180177,9,180177,15,0,0,4,1],[180177,19,180177,37,0,0,4,1],[180181,11,180181,26,0,0,4,0],[180181,11,180181,26,0,0,4,1],[180188,11,180188,24,0,0,4,0],[180188,28,180188,33,0,0,4,0],[180188,28,180188,33,0,0,4,1],[14098,27,14098,28,2,0,4,1],[14098,27,14098,28,3,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_close","sqlite3_db_filename","sqlite3_uri_key","sqlite3_uri_boolean","sqlite3_enable_load_extension","sqlite3_create_module","sqlite3_exec","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_expired","sqlite3_stmt_scanstatus_v2","sqlite3_step","sqlite3_vtab_in","sqlite3_reset","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_db_filename","sqlite3_uri_key","sqlite3_uri_boolean","sqlite3_enable_load_extension","sqlite3_create_module","sqlite3_exec","sqlite3_prepare_v2","sqlite3_stmt_readonly","sqlite3_expired","sqlite3_stmt_scanstatus_v2","sqlite3_step","sqlite3_vtab_in","sqlite3_reset","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int","sqlite3_finalize","sqlite3_close"],"visited":1}
/*Here's a C++ fuzzer that uses the specified SQLite3 APIs in a coherent scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_23(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    const char *db_filename = "input_file";
    
    // Write input data to file for database creation
    FILE *db_file = fopen(db_filename, "wb");
    if (!db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Copy fuzzer data to database file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, db_file);
    }
    assert_file_closed(&db_file);;
    assert_file_closed(&in_file);;
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;
    
    // Open database with URI parameters
    const char *uri = "file:input_file?cache=shared&mode=ro";
    rc = sqlite3_open_v2(uri, &db, SQLITE_OPEN_URI | SQLITE_OPEN_READONLY, nullptr);
    
    if (rc != SQLITE_OK || !db) {
        sqlite3_close(db);
        remove(db_filename);
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Test URI parameter functions
    sqlite3_filename db_path = sqlite3_db_filename(db, "main");
    if (db_path) {
        // Test uri_key function - get URI parameter keys
        for (int i = 0; i < 5; i++) {
            const char *key = sqlite3_uri_key(db_path, i);
            if (key) {
                // Test uri_boolean function
                int bool_val = sqlite3_uri_boolean(db_path, key, 0);
                (void)bool_val; // Use result
            }
        }
    }
    
    // Enable load extension
    sqlite3_enable_load_extension(db, 1);
    
    // Create and use a virtual table module
    struct DummyModule : public sqlite3_module {
        DummyModule() {
            memset(this, 0, sizeof(sqlite3_module));
            iVersion = 3;
        }
    };
    
    DummyModule dummy_module;
    
    // Create virtual table module
    rc = sqlite3_create_module(db, "fuzz_module", &dummy_module, nullptr);
    
    // Create virtual table using the module
    const char *create_vtab = "CREATE VIRTUAL TABLE IF NOT EXISTS test_vtab USING fuzz_module;";
    rc = sqlite3_exec(db, create_vtab, nullptr, nullptr, nullptr);
    
    // Prepare a statement
    const char *select_sql = "SELECT * FROM sqlite_master;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    
    if (stmt) {
        // Check if statement is read-only
        int is_readonly = sqlite3_stmt_readonly(stmt);
        (void)is_readonly;
        
        // Check if statement is expired
        int is_expired = sqlite3_expired(stmt);
        (void)is_expired;
        
        // Try to get scan status (if supported)
        typedef struct {
            int iScanStatusOp;
            void *pOut;
        } ScanStatusInfo;
        
        ScanStatusInfo scan_info;
        scan_info.iScanStatusOp = 0;
        scan_info.pOut = nullptr;
        
        // This might fail if statement hasn't been executed, but we call it anyway
        sqlite3_stmt_scanstatus_v2(stmt, 0, 0, 0, &scan_info);
        
        // Step through the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Just step through results
        }
        
        // Test vtab_in function with index info
        sqlite3_index_info idx_info;
        memset(&idx_info, 0, sizeof(sqlite3_index_info));
        
        // Try to use vtab_in API
        if (idx_info.nConstraint > 0) {
            sqlite3_vtab_in(&idx_info, 0, 1);
        }
        
        // Reset statement
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Perform WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);
    
    // Test more operations
    const char *pragma_sql = "PRAGMA integrity_check;";
    sqlite3_stmt *pragma_stmt = nullptr;
    if (sqlite3_prepare_v2(db, pragma_sql, -1, &pragma_stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(pragma_stmt);
        sqlite3_finalize(pragma_stmt);
    }
    
    // Test with another statement
    const char *count_sql = "SELECT count(*) FROM sqlite_master;";
    sqlite3_stmt *count_stmt = nullptr;
    if (sqlite3_prepare_v2(db, count_sql, -1, &count_stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(count_stmt) == SQLITE_ROW) {
            // Get the count value
            int count = sqlite3_column_int(count_stmt, 0);
            (void)count;
        }
        sqlite3_finalize(count_stmt);
    }
    
    // Cleanup
    sqlite3_close(db);
    
    // Remove temporary file
    remove(db_filename);
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}