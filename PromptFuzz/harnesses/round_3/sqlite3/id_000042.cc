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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 100, nr_unique_branch: 23
//<Quality> {"density":20,"unique_branches":{"sqlite3PcacheMakeDirty":[[54139,9,54139,31,0,0,4,1]],"sqlite3VdbeExec":[[99049,7,99049,9,0,0,4,1]],"createCollation":[[180382,16,180382,27,0,0,4,0],[180383,9,180383,24,0,0,4,1],[180396,9,180396,51,0,0,4,0],[180399,16,180399,19,0,0,4,0],[180399,16,180399,19,0,0,4,1],[180401,13,180401,31,0,0,4,0],[180401,13,180401,31,0,0,4,1],[180402,15,180402,22,0,0,4,1]],"freePage2":[[76475,7,76475,15,0,0,4,1],[76558,13,76558,18,0,0,4,1],[76574,7,76574,15,0,0,4,0],[76574,19,76574,72,0,0,4,1],[76587,7,76587,12,0,0,4,1]],"clearCellOverflow":[[76644,20,76644,65,0,0,4,1],[76662,9,76662,14,0,0,4,1]],"btreePageLookup":[[72089,7,72089,14,0,0,4,1]],"pager_playback_one_page":[[58857,9,58857,32,0,0,4,0]],"sqlite3_create_collation16":[[181282,7,181282,32,0,0,4,1],[181282,36,181282,44,0,0,4,1],[181287,7,181287,13,0,0,4,0]],"pager_truncate":[[59093,26,59093,46,0,0,4,1]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_close","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_value_pointer","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_value_pointer","sqlite3_finalize","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_close"],"visited":1}
/**/


// Custom collation callback
static void  id_000042_collation_needed_callback(void* pCtx, sqlite3* db, int eTextRep, const void* zName) {
    // This callback is invoked when an unknown collation is needed
    // We can register a simple collation here
    sqlite3_create_collation16(db, zName, eTextRep, nullptr, 
        [](void* pArg, int n1, const void* s1, int n2, const void* s2) -> int {
            // Simple binary comparison
            int len = n1 < n2 ? n1 : n2;
            int cmp = memcmp(s1, s2, len);
            if (cmp == 0) return n1 - n2;
            return cmp;
        });
}

// Exec callback function
static int  id_000042_exec_callback(void* data, int argc, char** argv, char** colNames) {
    // For fuzzing, we don't need to do anything meaningful here
    // Just return 0 to continue
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t* f_data, size_t f_size) {
	if(f_size<140) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // Get SQLite version
    const char* version = sqlite3_libversion();
    // We'll use the version string later if needed
    
    // Create a temporary file for database
    const char* filename = "fuzz_db.sqlite";
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* errmsg = nullptr;
    
    // Open database
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set some limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // Register collation needed callback
    sqlite3_collation_needed16(db, nullptr,  id_000042_collation_needed_callback);
    
    // Create a table to work with
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "text_data TEXT);";
    
    rc = sqlite3_exec(db, fuzz_str_7, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    
    // Use input data as SQL commands (with safety checks)
    // We'll use a portion of the input as a potential SQL statement
    size_t sql_len = size > 1024 ? 1024 : size;
    char* sql = new char[sql_len + 1];
    memcpy(sql, data, sql_len);
    sql[sql_len] = '\0';
    
    // Try to execute the SQL
    rc = sqlite3_exec(db, sql,  id_000042_exec_callback, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }
    
    // Also try to prepare and execute the statement
    const char* tail = nullptr;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        // Try to step through the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Get column value
            sqlite3_value* value = sqlite3_column_value(stmt, fuzz_int32_t_8);
            if (value) {
                // Check encoding
                int encoding = sqlite3_value_encoding(value);
                
                // Try to get pointer from value (if it's a pointer type)
                const char* type = "test_type";
                void* ptr = sqlite3_value_pointer(value, fuzz_str_9);
                // We don't actually use the pointer, just checking the API
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // Get WAL filename for the database
    sqlite3_filename db_filename = sqlite3_db_filename(db, fuzz_str_10);
    if (db_filename) {
        const char* wal_filename = sqlite3_filename_wal(db_filename);
        // wal_filename could be null if WAL is not enabled
    }
    
    // Clean up
    delete[] sql;
    
    // Close database
    sqlite3_close(db);
    
    // Remove the temporary file
    remove(filename);
    
    return 0;
}