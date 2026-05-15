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
//<ID> 374
//<Prompt> []
/*<Combination>: [
*/
//<score> 93.333336, nr_unique_branch: 9
//<Quality> {"density":28,"unique_branches":{"sqlite3BitvecCreate":[[53247,7,53247,8,0,0,4,1]],"writeJournalHdr":[[57986,17,57986,30,0,0,4,1]],"sqlite3_backup_step":[[81731,9,81731,31,0,0,4,0]],"sqlite3Pragma":[[139404,11,139404,14,0,0,4,1]],"pager_open_journal":[[62251,9,62251,30,0,0,4,0]],"sqlite3_errmsg":[[180241,7,180241,23,0,0,4,0]],"sqlite3PagerRollback":[[63190,37,63190,72,0,0,4,0]],"sqlite3VdbeAddOpList":[[85114,37,85114,56,0,0,4,0]],"dbReallocFinish":[[30539,11,30539,15,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_hard_heap_limit64","sqlite3_trace_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_double","sqlite3_strnicmp","sqlite3_finalize","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_hard_heap_limit64","sqlite3_trace_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_data_count","sqlite3_column_double","sqlite3_strnicmp","sqlite3_finalize","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_memory_highwater","sqlite3_release_memory","sqlite3_close"],"visited":2}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 64) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3 *db2 = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    sqlite3_blob *blob = nullptr;
    char *errmsg = nullptr;
    int rc;
    
    // Create a FILE* from input data for potential file operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open first database (in-memory)
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Set hard heap limit
    sqlite3_hard_heap_limit64(static_cast<sqlite3_int64>(size * 2));
    
    // Set up trace callback (simplified version)
    sqlite3_trace_v2(db, SQLITE_TRACE_STMT, 
        [](unsigned int mask, void* ctx, void* p, void* x) {
            return 0;
        }, nullptr);
    
    // Create a table using sqlite3_exec
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "CREATE TABLE IF NOT EXISTS blob_test(data BLOB);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }
    
    // Insert data using prepared statements
    const char *insert_sql = "INSERT INTO test(name, value) VALUES(?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind text parameter (using first part of input)
        size_t text_len = size > 100 ? 50 : size/2;
        sqlite3_bind_text(stmt, 1, reinterpret_cast<const char*>(data), 
                         static_cast<int>(text_len), SQLITE_TRANSIENT);
        
        // Bind integer parameter
        sqlite3_bind_int(stmt, 2, static_cast<int>(size % 1000));
        
        // Step to execute
        sqlite3_step(stmt);
        
        // Reset for potential reuse
        sqlite3_reset(stmt);
        
        // Bind double parameter
        sqlite3_bind_double(stmt, 2, static_cast<double>(size) / 1000.0);
        sqlite3_step(stmt);
        
        // Get statement status
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        
        // Get data count
        sqlite3_data_count(stmt);
        
        // Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Select and process data
    const char *select_sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Access column data
            if (sqlite3_data_count(stmt) > 2) {
                double val = sqlite3_column_double(stmt, 2);
                // Compare strings
                sqlite3_strnicmp("test", "TEST", 4);
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Insert blob data
    sqlite3_exec(db, "INSERT INTO blob_test(data) VALUES(zeroblob(100))", nullptr, nullptr, nullptr);
    rc = sqlite3_prepare_v2(db, "SELECT last_insert_rowid()", -1, &stmt, nullptr);
    sqlite3_int64 last_rowid = 0;
    if (rc == SQLITE_OK && stmt && sqlite3_step(stmt) == SQLITE_ROW) {
        last_rowid = sqlite3_column_int64(stmt, 0);
    }
    if (stmt) sqlite3_finalize(stmt);
    
    // Open blob for reading/writing
    if (last_rowid > 0) {
        rc = sqlite3_blob_open(db, "main", "blob_test", "data", last_rowid, 1, &blob);
        if (rc == SQLITE_OK && blob) {
            uint8_t buffer[256];
            size_t read_size = size > sizeof(buffer) ? sizeof(buffer) : size;
            sqlite3_blob_read(blob, buffer, static_cast<int>(read_size), 0);
            sqlite3_blob_close(blob);
            blob = nullptr;
        }
    }
    
    // Open second database for backup
    rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc == SQLITE_OK && db2) {
        // Initialize backup
        backup = sqlite3_backup_init(db2, "main", db, "main");
        if (backup) {
            // Perform backup steps
            for (int i = 0; i < 5; i++) {
                rc = sqlite3_backup_step(backup, 1);
                if (rc != SQLITE_OK && rc != SQLITE_BUSY && rc != SQLITE_LOCKED) {
                    break;
                }
            }
            // Finish backup
            sqlite3_backup_finish(backup);
            backup = nullptr;
        }
        sqlite3_close(db2);
        db2 = nullptr;
    }
    
    // Get memory highwater mark
    sqlite3_memory_highwater(0);
    
    // Release memory
    sqlite3_release_memory(static_cast<int>(size / 1024));
    
    // Cleanup
    assert_file_closed(&in_file);;
    if (db) {
        sqlite3_close(db);
    }
    
    assert_file_closed(&in_file);
	return 0;
}