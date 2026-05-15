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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 43, nr_unique_branch: 0
//<Quality> {"density":43,"unique_branches":{},"library_calls":["sqlite3_open_v2","sqlite3_hard_heap_limit64","sqlite3_trace_v2","sqlite3_strnicmp","sqlite3_create_function_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_int","sqlite3_bind_null","sqlite3_bind_double","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_int64","sqlite3_column_double","sqlite3_column_text","sqlite3_column_blob","sqlite3_reset","sqlite3_step","sqlite3_column_count","sqlite3_column_double","sqlite3_column_int64","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_memory_highwater","sqlite3_release_memory"],"critical_calls":["sqlite3_open_v2","sqlite3_hard_heap_limit64","sqlite3_trace_v2","sqlite3_strnicmp","sqlite3_create_function_v2","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_double","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_bind_int","sqlite3_bind_null","sqlite3_bind_double","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_stmt_status","sqlite3_data_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_int64","sqlite3_reset","sqlite3_step","sqlite3_column_count","sqlite3_column_double","sqlite3_column_int64","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_open_v2","sqlite3_backup_init","sqlite3_backup_step","sqlite3_backup_finish","sqlite3_close","sqlite3_close","sqlite3_memory_highwater","sqlite3_release_memory"],"visited":0}
/**/


// Custom function implementation for sqlite3_create_function_v2
static void  id_000069_nullFunc(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    sqlite3_result_null(ctx);
}

// Trace callback for sqlite3_trace_v2
static int  id_000069_traceCallback(unsigned int mask, void* ctx, void* p, void* x) {
    return 0;
}

// Main fuzzing function
extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // Write input to file for database operations
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    int fd = fuzz_fileno(in_file);
    (void)fd; // Prevent unused warning

    // Create output file for backup operations
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    sqlite3* db = nullptr;
    sqlite3* db2 = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* blob = nullptr;
    sqlite3_backup* backup = nullptr;
    char* err_msg = nullptr;
    int rc = 0;

    // 1. Open main database from input data
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(fd);
	return 0;
    }

    // 2. Set hard heap limit
    sqlite3_hard_heap_limit64(1024 * 1024); // 1MB limit

    // 3. Set trace callback
    sqlite3_trace_v2(db, SQLITE_TRACE_STMT,  id_000069_traceCallback, nullptr);

    // 4. Use string comparison
    const char* str1 = "test1";
    const char* str2 = "test2";
    int cmp_result = sqlite3_strnicmp(str1, str2, 5);

    // 5. Create custom function
    rc = sqlite3_create_function_v2(db, "nullfunc", 1, SQLITE_UTF8, nullptr,
                                     id_000069_nullFunc, nullptr, nullptr, nullptr);

    // Create a test table
    const char* create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT, value REAL, data BLOB);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // Insert some data using prepared statement
    const char* insert_sql = "INSERT INTO test VALUES (?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 6. Bind parameters
        sqlite3_bind_int(stmt, 1, 1);
        sqlite3_bind_text(stmt, 2, "test", -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, 3.14);
        
        // Use input data for blob binding (limit to reasonable size)
        int bind_size = (size > 100) ? 100 : (int)size;
        sqlite3_bind_blob(stmt, 4, data, bind_size, SQLITE_STATIC);
        
        // 7. Execute statement
        rc = sqlite3_step(stmt);
        
        // 8. Reset statement for reuse
        sqlite3_reset(stmt);
        
        // Bind different values
        sqlite3_bind_int(stmt, 1, 2);
        sqlite3_bind_null(stmt, 2);
        sqlite3_bind_double(stmt, 3, 2.71);
        sqlite3_bind_zeroblob(stmt, 4, 50);
        
        // Execute again
        rc = sqlite3_step(stmt);
        
        // 9. Get statement status
        int status = sqlite3_stmt_status(stmt, SQLITE_STMTSTATUS_FULLSCAN_STEP, 1);
        
        // 10. Get data count
        int data_count = sqlite3_data_count(stmt);
        
        // 11. Finalize statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Query data back
    const char* select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 12. Get column values
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                int col_type = sqlite3_column_type(stmt, i);
                switch (col_type) {
                    case SQLITE_INTEGER:
                        sqlite3_column_int64(stmt, i);
                        break;
                    case SQLITE_FLOAT:
                        sqlite3_column_double(stmt, i);
                        break;
                    case SQLITE_TEXT:
                        (void)sqlite3_column_text(stmt, i);
                        break;
                    case SQLITE_BLOB:
                        (void)sqlite3_column_blob(stmt, i);
                        break;
                }
            }
        }
        
        // 13. Get more column data
        sqlite3_reset(stmt);
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get specific column types
            if (sqlite3_column_count(stmt) >= 3) {
                double dval = sqlite3_column_double(stmt, 2);
                sqlite3_int64 ival = sqlite3_column_int64(stmt, 0);
                (void)dval;
                (void)ival;
            }
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // 14. BLOB operations
    rc = sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    if (rc == SQLITE_OK && blob) {
        char buffer[100];
        // 15. Read from blob
        rc = sqlite3_blob_read(blob, buffer, sizeof(buffer), 0);
        // 16. Close blob
        sqlite3_blob_close(blob);
        blob = nullptr;
    }

    // 17. Backup operations
    // Open second database for backup
    rc = sqlite3_open_v2(":memory:", &db2, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc == SQLITE_OK && db2) {
        // 18. Initialize backup
        backup = sqlite3_backup_init(db2, "main", db, "main");
        if (backup) {
            // 19. Perform backup in steps
            do {
                rc = sqlite3_backup_step(backup, 5);
            } while (rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED);
            
            // 20. Finish backup
            sqlite3_backup_finish(backup);
            backup = nullptr;
        }
        
        // 21. Close second database
        sqlite3_close(db2);
        db2 = nullptr;
    }

    // 22. Close main database
    sqlite3_close(db);
    db = nullptr;

    // 23. Memory operations
    sqlite3_int64 highwater = sqlite3_memory_highwater(1);
    int released = sqlite3_release_memory(1024);
    (void)highwater;
    (void)released;

    // Clean up files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");

    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_fd_closed(fd);
	return 0;
}