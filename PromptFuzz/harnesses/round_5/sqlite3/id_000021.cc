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
//<ID> 102
//<Prompt> []
/*<Combination>: [
*/
//<score> 817, nr_unique_branch: 53
//<Quality> {"density":43,"unique_branches":{"sqlite3BtreePayloadChecked":[[75033,7,75033,33,0,0,4,0]],"accessPayload":[[74842,9,74842,35,0,0,4,1],[74854,24,74854,29,0,0,4,1],[74981,24,74981,29,0,0,4,1]],"sqlite3CodeDropTable":[[123538,7,123538,40,0,0,4,0]],"blobReadWrite":[[101705,9,101705,35,0,0,4,1]],"yy_reduce":[[174166,7,174166,14,0,0,4,0]],"autoIncBegin":[[132066,7,132066,45,0,0,4,0],[132067,7,132067,48,0,0,4,0],[132076,9,132076,19,0,0,4,1],[132077,9,132077,18,0,0,4,1],[132079,9,132079,25,0,0,4,1],[132087,12,132087,17,0,0,4,1],[132088,9,132088,17,0,0,4,0],[132092,11,132092,35,0,0,4,1],[20950,35,20950,49,3,0,4,1],[14156,25,14156,46,5,0,4,1],[14156,26,14156,29,5,0,4,1],[14098,27,14098,28,7,0,4,1]],"autoIncrementEnd":[[132190,26,132190,27,0,0,4,0],[132190,26,132190,27,0,0,4,1],[132210,9,132210,15,0,0,4,1]],"sqlite3_db_mutex":[[178467,7,178467,32,0,0,4,1]],"sqlite3FinishCoding":[[120385,9,120385,22,0,0,4,0]],"sqlite3_blob_bytes":[[101766,11,101766,12,0,0,4,0],[101766,16,101766,24,0,0,4,0]],"sqlite3BtreeDelete":[[79540,28,79540,45,0,0,4,0]],"autoIncStep":[[132172,7,132172,14,0,0,4,0]],"sqlite3EndTable":[[123041,9,123041,44,0,0,4,0],[123041,48,123041,65,0,0,4,0],[123044,11,123044,35,0,0,4,0],[123090,9,123090,47,0,0,4,0]],"sqlite3AutoincrementBegin":[[132123,26,132123,27,0,0,4,0],[132123,26,132123,27,0,0,4,1],[132146,9,132146,15,0,0,4,1],[132159,9,132159,24,0,0,4,1]],"invalidateIncrblobCursors":[[70342,31,70342,32,0,0,4,0],[70342,31,70342,32,0,0,4,1],[70343,9,70343,41,0,0,4,1]],"sqlite3VdbeExec":[[97750,9,97750,16,0,0,4,0],[97753,11,97753,20,0,0,4,1],[97769,11,97769,31,0,0,4,1],[97769,35,97769,53,0,0,4,1],[97773,11,97773,24,0,0,4,1],[99666,1,99666,15,0,0,4,0],[99668,7,99668,16,0,0,4,1],[99678,7,99678,26,0,0,4,1],[92825,34,92825,60,585,0,4,1]],"sqlite3AutoincrementEnd":[[132222,7,132222,20,0,0,4,0]],"sqlite3CreateFunc":[[179548,7,179548,8,0,0,4,0],[179548,12,179548,58,0,0,4,0],[179548,62,179548,75,0,0,4,0],[179549,9,179549,24,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_prepare_v2","sqlite3_finalize","sqlite3_close","sqlite3_bind_blob","sqlite3_finalize","sqlite3_close","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_blob_close","sqlite3_close","sqlite3_total_changes","sqlite3_changes64","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_mutex_leave","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_result_error_code","sqlite3_result_error_code","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_exec","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_last_insert_rowid","sqlite3_blob_open","sqlite3_total_changes","sqlite3_changes64","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_mutex_leave","sqlite3_str_new","sqlite3_str_append","sqlite3_str_append","sqlite3_str_finish","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int64","sqlite3_step","sqlite3_finalize","sqlite3_result_error_code","sqlite3_result_error_code","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_create_function","sqlite3_blob_bytes","sqlite3_blob_read","sqlite3_blob_close","sqlite3_exec","sqlite3_close"],"visited":0}
/*We are going to create a fuzzing driver that uses the specified SQLite APIs in a meaningful way.
 The event we're simulating: 
 1. Open an SQLite database (in memory or file-based)
 2. Create a table with a BLOB column
 3. Insert a row with BLOB data from the fuzzer input
 4. Open a BLOB handle to read/write that BLOB
 5. Use various SQLite APIs to interact with the database and BLOB
 6. Clean up resources

 We'll use the following APIs as required:
   - sqlite3_total_changes
   - sqlite3_blob_open
   - sqlite3_stmt_scanstatus_v2
   - sqlite3_result_error_code
   - sqlite3_changes64
   - sqlite3_str_append
   - sqlite3_db_mutex

 Steps:
 1. Create an in-memory database.
 2. Create a table with a BLOB column.
 3. Insert a row with the fuzzer input as the BLOB data.
 4. Open a BLOB handle to that row.
 5. Use the other APIs in a meaningful way.

 Note: We must consume the fuzzer input (data, size). We'll use the input as the BLOB content.

 We'll also handle errors and clean up resources.

 Let's write the code step by step.</think>*/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit for insufficient data
    if (size < 1) {
	return 0;
	}
	

    // Step 1: Create an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Step 2: Create a table with a BLOB column
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_test ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "blob_data BLOB"
        ");";
    
    char *err_msg = nullptr;
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }

    // Step 3: Insert a row with the fuzzer input as BLOB data
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO fuzz_test (blob_data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        if (stmt) sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // Bind the fuzzer input as a BLOB
    rc = sqlite3_bind_blob(stmt, 1, data, static_cast<int>(size), SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    sqlite3_finalize(stmt);

    // Step 4: Get the rowid of the inserted row
    sqlite3_int64 rowid = sqlite3_last_insert_rowid(db);

    // Step 5: Open a BLOB handle to the inserted row
    sqlite3_blob *blob = nullptr;
    rc = sqlite3_blob_open(db, "main", "fuzz_test", "blob_data", rowid, 0, &blob);
    if (rc != SQLITE_OK) {
        if (blob) sqlite3_blob_close(blob);
        sqlite3_close(db);
        return 0;
    }

    // Step 6: Use sqlite3_total_changes (after insert)
    int total_changes = sqlite3_total_changes(db);

    // Step 7: Use sqlite3_changes64 (should be 1 after insert)
    sqlite3_int64 changes64 = sqlite3_changes64(db);

    // Step 8: Use sqlite3_db_mutex to get database mutex
    sqlite3_mutex *db_mutex = sqlite3_db_mutex(db);
    if (db_mutex) {
        // Try to acquire the mutex
        int try_rc = sqlite3_mutex_try(db_mutex);
        if (try_rc == SQLITE_OK) {
            // We got the mutex, release it
            sqlite3_mutex_leave(db_mutex);
        }
    }

    // Step 9: Use sqlite3_str_append to build an error message
    sqlite3_str *str_obj = sqlite3_str_new(db);
    if (str_obj) {
        const char *prefix = "Fuzz error: ";
        sqlite3_str_append(str_obj, prefix, static_cast<int>(strlen(prefix)));
        
        // Append part of the input data as string (limited to safe length)
        int append_len = static_cast<int>(size > 100 ? 100 : size);
        sqlite3_str_append(str_obj, reinterpret_cast<const char*>(data), append_len);
        
        // Get the string and free the object
        char *built_str = sqlite3_str_finish(str_obj);
        if (built_str) {
            sqlite3_free(built_str);
        }
    }

    // Step 10: Use sqlite3_stmt_scanstatus_v2 on a prepared statement
    sqlite3_stmt *scan_stmt = nullptr;
    const char *select_sql = "SELECT * FROM fuzz_test WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &scan_stmt, nullptr);
    if (rc == SQLITE_OK && scan_stmt) {
        // Bind the rowid
        sqlite3_bind_int64(scan_stmt, 1, rowid);
        
        // Execute once
        sqlite3_step(scan_stmt);
        
        // Use sqlite3_stmt_scanstatus_v2 (requires SQLITE_ENABLE_STMT_SCANSTATUS)
        #ifdef SQLITE_ENABLE_STMT_SCANSTATUS
        int dummy_out;
        sqlite3_stmt_scanstatus_v2(scan_stmt, 0, SQLITE_SCANSTAT_NLOOP, 0, &dummy_out);
        #endif
        
        sqlite3_finalize(scan_stmt);
    }

    // Step 11: Use sqlite3_result_error_code in a user-defined function context
    // We'll create a simple scalar function that may return an error
    struct UserData {
        const uint8_t *data;
        size_t size;
    };
    
    auto xFunc = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        // This function always returns an error for fuzzing
        sqlite3_result_error_code(ctx, SQLITE_ERROR);
    };
    
    rc = sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, nullptr, 
                                 xFunc, nullptr, nullptr);
    if (rc == SQLITE_OK) {
        // Call the function
        sqlite3_stmt *func_stmt = nullptr;
        rc = sqlite3_prepare_v2(db, "SELECT fuzz_func();", -1, &func_stmt, nullptr);
        if (rc == SQLITE_OK && func_stmt) {
            sqlite3_step(func_stmt);  // Will return error
            sqlite3_finalize(func_stmt);
        }
        // Clean up the function
        sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, nullptr, 
                               nullptr, nullptr, nullptr);
    }

    // Step 12: Read some data from the BLOB (if BLOB is not empty)
    if (sqlite3_blob_bytes(blob) > 0) {
        uint8_t read_buf[256];
        int to_read = static_cast<int>(size > sizeof(read_buf) ? sizeof(read_buf) : size);
        rc = sqlite3_blob_read(blob, read_buf, to_read, 0);
        // Ignore result - just exercising the API
    }

    // Step 13: Clean up
    sqlite3_blob_close(blob);
    
    // Try to delete the table to clean up
    sqlite3_exec(db, "DROP TABLE IF EXISTS fuzz_test;", nullptr, nullptr, nullptr);
    
    sqlite3_close(db);

    return 0;
}