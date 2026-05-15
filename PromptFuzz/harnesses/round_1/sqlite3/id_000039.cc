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
//<ID> 250
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 9
//<Quality> {"density":25,"unique_branches":{"sqlite3_bind_double":[[91193,7,91193,20,0,0,4,1]],"vdbeUnbind":[[91102,7,91102,31,0,0,4,0]],"sqlite3_hard_heap_limit64":[[29960,7,29960,11,0,0,4,1],[29962,9,29962,30,0,0,4,1],[29962,34,29962,56,0,0,4,1]],"sqlite3VdbeExec":[[94492,11,94492,36,0,0,4,1]],"sqlite3MallocAlarm":[[30033,7,30033,29,0,0,4,0]],"mallocWithAlarm":[[30057,7,30057,28,0,0,4,1]],"sqlite3Realloc":[[30351,11,30351,27,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_blob_close","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_free","sqlite3_trace_v2","sqlite3_hard_heap_limit64","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_double","sqlite3_step","sqlite3_data_count","sqlite3_column_double","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_blob_open","sqlite3_blob_read","sqlite3_release_memory","sqlite3_db_filename","sqlite3_next_stmt"],"critical_calls":["sqlite3_open_v2","sqlite3_trace_v2","sqlite3_hard_heap_limit64","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_bind_int","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_status","sqlite3_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_double","sqlite3_step","sqlite3_data_count","sqlite3_column_double","sqlite3_strnicmp","sqlite3_memory_highwater","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_blob_open","sqlite3_blob_read","sqlite3_release_memory","sqlite3_db_filename","sqlite3_next_stmt","sqlite3_blob_close","sqlite3_finalize","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_free"],"visited":1}
/**/


extern "C" {
}

// Trace callback for sqlite3_trace_v2
static int  id_000039_trace_callback(unsigned int mask, void* context, void* p, void* x) {
    return 0;
}

// Callback for sqlite3_exec
static int  id_000039_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open memory-mapped file for input
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    
    sqlite3* db = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_backup* backup = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_stmt* next_stmt = nullptr;
    sqlite3_blob* blob = nullptr;
    char* err_msg = nullptr;
    int rc;
    
    // 1. Open first database (in-memory)
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        cleanup:
        if (blob) sqlite3_blob_close(blob);
        if (stmt) sqlite3_finalize(stmt);
        if (backup) sqlite3_backup_finish(backup);
        if (db2) sqlite3_close(db2);
        if (db) sqlite3_close(db);
        if (err_msg) sqlite3_free(err_msg);
        assert_file_closed(&out_file);;
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
    }
    
    // 2. Set trace
    sqlite3_trace_v2(db, SQLITE_TRACE_STMT,  id_000039_trace_callback, nullptr);
    
    // 3. Set hard heap limit (use first 8 bytes as limit)
    sqlite3_int64 heap_limit = size >= 8 ? *(reinterpret_cast<const sqlite3_int64*>(data)) : 1000000;
    sqlite3_hard_heap_limit64(heap_limit);
    
    // 4. Create table and insert data using exec
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "INSERT INTO test(name, value) VALUES('test1', 1.5);"
        "INSERT INTO test(name, value) VALUES('test2', 2.5);";
    
    rc = sqlite3_exec(db, create_table,  id_000039_exec_callback, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // 5. Prepare statement (use input data as SQL if possible)
    const char* sql = "INSERT INTO test(name, value) VALUES(?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 6. Bind parameters using input data
        if (size > 0) {
            // Use part of input as text parameter
            size_t text_len = size > 100 ? 100 : size;
            char* text_buf = reinterpret_cast<char*>(malloc(text_len + 1));
            if (text_buf) {
                memcpy(text_buf, data, text_len);
                text_buf[text_len] = 0;
                sqlite3_bind_text(stmt, 1, text_buf, -1, SQLITE_TRANSIENT);
                free(text_buf);
            }
            
            // Use integer from data
            int int_val = size > 4 ? *(reinterpret_cast<const int*>(data)) : 42;
            sqlite3_bind_int(stmt, 2, int_val);
            
            // Also bind double
            double double_val = size > 12 ? *(reinterpret_cast<const double*>(data + 4)) : 3.14;
            sqlite3_bind_double(stmt, 3, double_val);
        }
        
        // 7. Execute statement
        rc = sqlite3_step(stmt);
        
        // 8. Get statement status
        int status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        
        // 9. Reset statement
        sqlite3_reset(stmt);
        
        // 10. Prepare select statement to use data_count and column_double
        sqlite3_finalize(stmt);
        stmt = nullptr;
        
        const char* select_sql = "SELECT id, name, value FROM test WHERE value > ?";
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK && stmt) {
            sqlite3_bind_double(stmt, 1, 1.0);
            
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // 11. Get data count
                int col_count = sqlite3_data_count(stmt);
                
                // 12. Get column value as double
                if (col_count >= 3) {
                    double col_val = sqlite3_column_double(stmt, 2);
                    // Write to output file
                    fwrite(&col_val, sizeof(double), 1, out_file);
                }
            }
        }
    }
    
    // 13. String comparison using input data
    if (size >= 10) {
        const char* str1 = reinterpret_cast<const char*>(data);
        const char* str2 = reinterpret_cast<const char*>(data + 5);
        int cmp_result = sqlite3_strnicmp(str1, str2, 5);
    }
    
    // 14. Get memory highwater mark
    sqlite3_int64 highwater = sqlite3_memory_highwater(1);
    
    // 15. Open second database for backup
    rc = sqlite3_open_v2("backup.db", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc == SQLITE_OK && db2) {
        // 16. Initialize backup
        backup = sqlite3_backup_init(db2, "main", db, "main");
        if (backup) {
            // 17. Perform backup step
            rc = sqlite3_backup_step(backup, 5); // Backup 5 pages at a time
        }
    }
    
    // 18. Try to open a blob (requires a table with ROWID)
    rc = sqlite3_blob_open(db, "main", "test", "id", 1, 0, &blob);
    if (rc == SQLITE_OK && blob) {
        // Read blob data if available
        char buffer[100];
        if (size > 0) {
            size_t read_size = size > 100 ? 100 : size;
            rc = sqlite3_blob_read(blob, buffer, read_size, 0);
        }
    }
    
    // 19. Release memory
    sqlite3_release_memory(1000);
    
    // 20. Get database filename
    sqlite3_filename db_filename = sqlite3_db_filename(db, "main");
    
    // 21. Iterate through statements
    while ((next_stmt = sqlite3_next_stmt(db, stmt)) != nullptr) {
        stmt = next_stmt;
        // Do something with statement
    }
    
    goto cleanup;
}