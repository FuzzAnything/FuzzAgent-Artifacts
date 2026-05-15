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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 18, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["sqlite3_open_v2","sqlite3_close","sqlite3_trace_v2","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_hard_heap_limit64","sqlite3_release_memory","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_db_filename","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_blob_open","sqlite3_blob_close","sqlite3_finalize","sqlite3_next_stmt","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_trace_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_reset","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_double","sqlite3_finalize","sqlite3_finalize","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_hard_heap_limit64","sqlite3_release_memory","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_db_filename","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_blob_open","sqlite3_blob_close","sqlite3_finalize","sqlite3_next_stmt","sqlite3_close","sqlite3_close"],"visited":0}
/**/


// Trace callback function
static int  id_000040_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    if (size == 0) {
	return 0;
	}
	
    
    sqlite3* db = nullptr;
    sqlite3* dest_db = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* blob = nullptr;
    char* errmsg = nullptr;
    int rc;
    
    // Create input file from the fuzzer data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open the input database file
    rc = sqlite3_open_v2("input_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        if (db) sqlite3_close(db);
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Set up tracing
    sqlite3_trace_v2(db, SQLITE_TRACE_STMT,  id_000040_trace_callback, nullptr);
    
    // Write fuzzer data to the input database file
    FILE* db_file = fopen("input_file", "wb");
    if (db_file) {
        int fd = fuzz_fileno(in_file);
        rewind(in_file);
        uint8_t buffer[4096];
        size_t bytes_read;
        while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
            fwrite(buffer, 1, bytes_read, db_file);
        }
        assert_file_closed(&db_file);;
        assert_file_closed(&in_file);;
    } else {
        assert_file_closed(&in_file);;
        sqlite3_close(db);
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Execute some SQL using sqlite3_exec
    const char* sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT, value REAL);";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    if (errmsg) sqlite3_free(errmsg);
    
    // Prepare a statement
    const char* insert_sql = "INSERT INTO test VALUES(?, ?, ?)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind parameters
        sqlite3_bind_int(stmt, 1, 1);
        sqlite3_bind_text(stmt, 2, "test", -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, 3.14);
        
        // Execute the statement
        rc = sqlite3_step(stmt);
        
        // Get statement status and data count
        sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        int data_count = sqlite3_data_count(stmt);
        
        // Reset the statement for potential reuse
        sqlite3_reset(stmt);
        
        // Now prepare a SELECT statement to get data back
        const char* select_sql = "SELECT * FROM test";
        sqlite3_stmt* select_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
        
        if (rc == SQLITE_OK && select_stmt) {
            rc = sqlite3_step(select_stmt);
            if (rc == SQLITE_ROW) {
                // Get column data
                double col_value = sqlite3_column_double(select_stmt, 2);
            }
            sqlite3_finalize(select_stmt);
        }
        
        sqlite3_finalize(stmt);
    }
    
    // String comparison
    sqlite3_strnicmp("test1", "test2", 5);
    
    // Memory management
    sqlite3_int64 highwater = sqlite3_memory_highwater(1);
    sqlite3_int64 limit = sqlite3_hard_heap_limit64(1000000);
    sqlite3_release_memory(100);
    
    // Open destination database for backup
    rc = sqlite3_open_v2("output_file", &dest_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    
    if (rc == SQLITE_OK && dest_db) {
        // Initialize backup
        backup = sqlite3_backup_init(dest_db, "main", db, "main");
        if (backup) {
            // Perform backup
            sqlite3_backup_step(backup, 5); // Backup 5 pages at a time
            sqlite3_backup_finish(backup); // Note: sqlite3_backup_finish is used instead of sqlite3_backup_step with negative
            
            // Get database filename
            sqlite3_filename filename = sqlite3_db_filename(dest_db, "main");
        }
        
        // Create a table with BLOB for blob API testing
        const char* blob_sql = "CREATE TABLE IF NOT EXISTS blob_test(data BLOB)";
        sqlite3_exec(dest_db, blob_sql, nullptr, nullptr, nullptr);
        
        // Insert a blob
        const char* insert_blob_sql = "INSERT INTO blob_test VALUES(?)";
        sqlite3_stmt* blob_stmt = nullptr;
        rc = sqlite3_prepare_v2(dest_db, insert_blob_sql, -1, &blob_stmt, nullptr);
        
        if (rc == SQLITE_OK && blob_stmt) {
            // Bind some data as blob
            uint8_t blob_data[] = {0x01, 0x02, 0x03, 0x04};
            sqlite3_bind_blob(blob_stmt, 1, blob_data, sizeof(blob_data), SQLITE_STATIC);
            sqlite3_step(blob_stmt);
            sqlite3_finalize(blob_stmt);
            
            // Try to open the blob (requires rowid)
            const char* get_rowid_sql = "SELECT rowid FROM blob_test LIMIT 1";
            sqlite3_stmt* rowid_stmt = nullptr;
            rc = sqlite3_prepare_v2(dest_db, get_rowid_sql, -1, &rowid_stmt, nullptr);
            
            if (rc == SQLITE_OK && rowid_stmt) {
                if (sqlite3_step(rowid_stmt) == SQLITE_ROW) {
                    sqlite3_int64 rowid = sqlite3_column_int64(rowid_stmt, 0);
                    // Try to open blob (this might fail if table doesn't have proper structure)
                    rc = sqlite3_blob_open(dest_db, "main", "blob_test", "data", rowid, 0, &blob);
                    if (rc == SQLITE_OK && blob) {
                        sqlite3_blob_close(blob);
                    }
                }
                sqlite3_finalize(rowid_stmt);
            }
        }
        
        // Iterate through all statements
        sqlite3_stmt* next_stmt = nullptr;
        while ((next_stmt = sqlite3_next_stmt(dest_db, next_stmt)) != nullptr) {
            // Just iterate, no action needed
        }
        
        sqlite3_close(dest_db);
    }
    
    // Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}