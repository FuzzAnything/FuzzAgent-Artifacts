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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 3
//<Quality> {"density":21,"unique_branches":{"sqlite3_finalize":[[89557,8,89557,26,3,0,4,0]],"sqlite3_value_dup":[[89818,7,89818,37,0,0,4,0],[89821,9,89821,53,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_compileoption_used","sqlite3_create_filename","sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_db_filename","sqlite3_profile","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_create_window_function","sqlite3_exec","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_value_blob","sqlite3_value_bytes","sqlite3_value_free","sqlite3_finalize","sqlite3_profile","sqlite3_close","sqlite3_free_filename","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_compileoption_used","sqlite3_create_filename","sqlite3_hard_heap_limit64","sqlite3_open","sqlite3_db_filename","sqlite3_profile","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_int64","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_create_window_function","sqlite3_exec","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_value","sqlite3_value_dup","sqlite3_value_blob","sqlite3_value_bytes","sqlite3_value_free","sqlite3_finalize","sqlite3_profile","sqlite3_close","sqlite3_free_filename","sqlite3_shutdown"],"visited":0}
/**/


// Window function callbacks
static void  id_000004_windowStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Do nothing in step
}

static void  id_000004_windowFinal(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 42);  // Use sqlite3_result_int
}

static void  id_000004_windowValue(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 24);  // Use sqlite3_result_int
}

static void  id_000004_windowInverse(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // Do nothing in inverse
}

static void  id_000004_windowDestroy(void *p) {
    // Cleanup if needed
}

// Profile callback
static void  id_000004_profileCallback(void *pCtx, const char *sql, sqlite3_uint64 ns) {
    // Do nothing in profile callback
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Create temporary file with input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Get file descriptors
    int in_fd = fuzz_fileno(in_file);
    int out_fd = fuzz_fileno(out_file);
    (void)in_fd; (void)out_fd; // Suppress unused warnings
    
    // Initialize SQLite
    sqlite3_initialize();
    
    // Check compile option
    int compile_used = sqlite3_compileoption_used("ENABLE_COLUMN_METADATA");
    (void)compile_used;
    
    // Create database filename
    sqlite3_filename filename = sqlite3_create_filename(
        "test.db", "test.db-journal", "test.db-wal", 0, nullptr);
    
    // Set hard heap limit
    sqlite3_int64 heap_limit = sqlite3_hard_heap_limit64(1024 * 1024); // 1MB
    
    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    
    if (db && rc == SQLITE_OK) {
        // Get database filename (should be ":memory:" for in-memory db)
        sqlite3_filename db_file = sqlite3_db_filename(db, "main");
        (void)db_file;
        
        // Set profile callback
        void *old_profile = sqlite3_profile(db,  id_000004_profileCallback, nullptr);
        (void)old_profile;
        
        // Create a simple table and insert data from input
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB);", nullptr, nullptr, nullptr);
        
        // Prepare statement to insert data
        sqlite3_stmt *stmt = nullptr;
        const char *insert_sql = "INSERT INTO test VALUES (?, ?);";
        rc = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // Bind parameters using input data
            sqlite3_bind_int64(stmt, 1, size);
            if (size > 0) {
                sqlite3_bind_blob(stmt, 2, data, static_cast<int>(size > 1024 ? 1024 : size), SQLITE_TRANSIENT);
            }
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        // Create window function
        rc = sqlite3_create_window_function(
            db,
            "fuzz_window",
            1,  // nArg
            SQLITE_UTF8,
            nullptr,  // pApp
             id_000004_windowStep,
             id_000004_windowFinal,
             id_000004_windowValue,
             id_000004_windowInverse,
             id_000004_windowDestroy
        );
        
        // Query to use the window function
        sqlite3_exec(db, 
            "SELECT fuzz_window(id) OVER (ORDER BY id) FROM test;", 
            nullptr, nullptr, nullptr);
        
        // Get a value from the database to duplicate
        sqlite3_stmt *select_stmt = nullptr;
        rc = sqlite3_prepare_v3(db, "SELECT data FROM test LIMIT 1;", -1, 0, &select_stmt, nullptr);
        
        if (rc == SQLITE_OK && select_stmt) {
            if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                sqlite3_value *value = sqlite3_column_value(select_stmt, 0);
                if (value) {
                    // Duplicate the value
                    sqlite3_value *dup_value = sqlite3_value_dup(value);
                    if (dup_value) {
                        // Write duplicated value to output file
                        const void *blob_data = sqlite3_value_blob(dup_value);
                        int blob_size = sqlite3_value_bytes(dup_value);
                        if (blob_data && blob_size > 0) {
                            fwrite(blob_data, 1, blob_size, out_file);
                        }
                        sqlite3_value_free(dup_value);
                    }
                }
            }
            sqlite3_finalize(select_stmt);
        }
        
        // Remove profile callback
        sqlite3_profile(db, nullptr, nullptr);
        
        // Close database
        sqlite3_close(db);
    }
    
    // Clean up
    if (filename) {
        sqlite3_free_filename(filename);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_fd_closed(out_fd);
	assert_fd_closed(in_fd);
	return 0;
}