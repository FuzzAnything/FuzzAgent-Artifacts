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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 144, nr_unique_branch: 12
//<Quality> {"density":24,"unique_branches":{"callCollNeeded":[[125868,7,125868,24,0,0,4,0],[125873,9,125873,18,0,0,4,0]],"unixRead":[[41234,9,41234,38,0,0,4,1]],"sqlite3PagerSetJournalMode":[[63830,13,63830,41,0,0,4,1],[63834,13,63834,26,0,0,4,1],[63837,13,63837,26,0,0,4,1]],"unixMapfile":[[43122,7,43122,28,0,0,4,0],[43127,7,43127,26,0,0,4,1]],"pager_end_transaction":[[58500,15,58500,62,0,0,4,0],[58501,11,58501,32,0,0,4,1],[58505,13,58505,26,0,0,4,0],[58505,30,58505,46,0,0,4,0]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_finalize","sqlite3_snapshot_get","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_subtype","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_finalize","sqlite3_snapshot_get","sqlite3_close"],"visited":0}
/**/


// Collation callback function
static void  id_000030_collation_needed_callback(void* pCtx, sqlite3* db, int eTextRep, const void* name) {
    // This is a dummy callback that does nothing
    (void)pCtx;
    (void)db;
    (void)eTextRep;
    (void)name;
}

// Exec callback function
static int  id_000030_exec_callback(void* data, int argc, char** argv, char** colNames) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)colNames;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* f_data, size_t f_size) {
	if(f_size<113) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
	
	FDPGetRandomInteger(int, fuzz_str_idx_7, fuzz_str_sz_7);//fuzzer shim end}




    // Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output database file
    const char* db_filename = "fuzz_test.db";
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_snapshot* snapshot = nullptr;
    char* errmsg = nullptr;
    
    // 1. Get SQLite library version
    const char* version = sqlite3_libversion();
    (void)version; // Use version to avoid unused variable warning
    
    // 2. Open database
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 3. Set some limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 4. Set up collation needed callback
    sqlite3_collation_needed16(db, nullptr,  id_000030_collation_needed_callback);
    
    // 5. Read SQL commands from input data and execute them
    // We'll use the first part of data as SQL commands
    if (size > 0) {
        // Create a null-terminated string from the data
        char* sql = (char*)malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            
            // Try to execute the SQL
            rc = sqlite3_exec(db, sql,  id_000030_exec_callback, nullptr, &errmsg);
            
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            free(sql);
        }
    }
    
    // 6. Prepare a simple statement
    const char* simple_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_prepare_v2(db, fuzz_str_7, static_cast<int>(fuzz_str_idx_7), &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // 7. Step through the statement
        rc = sqlite3_step(stmt);
        
        // 8. Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 9. Prepare another statement to insert data
    const char* insert_sql = "INSERT INTO test(data) VALUES(?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind the input data as a blob
        if (size > 0) {
            sqlite3_bind_blob(stmt, fuzz_int32_t_8, data, (int)(size > 1000 ? 1000 : size), SQLITE_STATIC);
        }
        
        // Step through the insert
        sqlite3_step(stmt);
        
        // Get column value (though this is an INSERT, not a SELECT)
        // We'll prepare a SELECT statement instead
        sqlite3_finalize(stmt);
    }
    
    // 10. Prepare a SELECT statement to retrieve data
    const char* select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 11. Get column value
            sqlite3_value* value = sqlite3_column_value(stmt, fuzz_int32_t_9);
            if (value) {
                // 12. Get subtype of the value
                unsigned int subtype = sqlite3_value_subtype(value);
                (void)subtype; // Use subtype to avoid unused variable warning
            }
            
            // Try to get other columns if they exist
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_value* col_value = sqlite3_column_value(stmt, fuzz_int32_t_10);
                if (col_value) {
                    unsigned int col_subtype = sqlite3_value_subtype(col_value);
                    (void)col_subtype;
                }
            }
        }
        
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 13. Try to get a snapshot (this may fail if WAL mode is not enabled)
    sqlite3_snapshot_get(db, fuzz_str_11, &snapshot);
    
    // 14. Clean up snapshot if we got one
    if (snapshot) {
        // Note: We would need sqlite3_snapshot_free to properly free it,
        // but it's not in our API list. We'll just leave it as the API
        // documentation says it should be freed with sqlite3_free.
        // However, since we don't have sqlite3_snapshot_free in our list,
        // we'll skip explicit freeing.
    }
    
    // 15. Close the input file
    assert_file_closed(&in_file);;
    
    // 16. Close the database
    sqlite3_close(db);
    
    // 17. Remove the temporary database file
    remove(db_filename);
    
    assert_file_closed(&in_file);
	return 0;
}