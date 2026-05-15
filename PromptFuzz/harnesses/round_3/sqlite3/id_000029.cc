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
//<score> 42.5, nr_unique_branch: 4
//<Quality> {"density":17,"unique_branches":{"unixLockSharedMemory":[[42412,11,42412,24,0,0,4,1]],"sqlite3AbsInt32":[[36062,7,36062,25,0,0,4,0]],"pcache1Cachesize":[[55302,9,55302,57,0,0,4,0]],"sqlite3_collation_needed16":[[181328,7,181328,32,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_libversion","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_libversion","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close"],"visited":1}
/**/


// Dummy callback for sqlite3_exec
static int  id_000029_exec_callback(void* data, int argc, char** argv, char** col_names) {
    return 0;
}

// Dummy callback for sqlite3_collation_needed16
static void  id_000029_collation_callback(void* data, sqlite3* db, int enc, const void* name) {
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* f_data, size_t f_size) {
	if(f_size<94) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
	//fuzzer shim end}




    // 1. Write input data to a temporary file (simulating sqlite3 output data)
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a temporary database file
    const char* db_filename = "fuzz_temp.db";
    FILE* db_file = fopen(db_filename, "wb");
    if (!db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Copy input data to database file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, db_file);
    }
    assert_file_closed(&in_file);;
    assert_file_closed(&db_file);;
    
    // 2. Open the database
    sqlite3* db = nullptr;
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        remove(db_filename);
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Get library version
    const char* version = sqlite3_libversion();
    
    // 4. Set some limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 5. Register collation callback
    sqlite3_collation_needed16(db, nullptr,  id_000029_collation_callback);
    
    // 6. Execute some SQL
    char* errmsg = nullptr;
    const char* sql1 = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    sqlite3_exec(db, fuzz_str_7,  id_000029_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Insert some data using the input
    char sql_insert[256];
    snprintf(sql_insert, sizeof(sql_insert), 
             "INSERT INTO test VALUES (1, 'test');");
    sqlite3_exec(db, sql_insert,  id_000029_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // 7. Prepare and execute a SELECT statement
    sqlite3_stmt* stmt = nullptr;
    const char* sql_select = "SELECT * FROM test;";
    const char* tail = nullptr;
    
    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                // Get column value
                sqlite3_value* col_value = sqlite3_column_value(stmt, fuzz_int32_t_8);
                if (col_value) {
                    // Get subtype (if any)
                    unsigned int subtype = sqlite3_value_subtype(col_value);
                    (void)subtype; // Use to avoid unused warning
                }
            }
        }
        
        // 8. Get a snapshot (if supported)
        sqlite3_snapshot* snapshot = nullptr;
        sqlite3_snapshot_get(db, fuzz_str_9, &snapshot);
        // Note: Cannot free snapshot without sqlite3_snapshot_free
        
        // 9. Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // 10. Try to get sqlite3_str value (simulated - we don't have actual sqlite3_str)
    // This is a workaround since we can't create sqlite3_str directly
    const char* dummy_str_value = "dummy";
    
    // 11. Clean up
    sqlite3_close(db);
    remove(db_filename);
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	return 0;
}