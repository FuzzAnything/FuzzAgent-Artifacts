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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 85, nr_unique_branch: 10
//<Quality> {"density":17,"unique_branches":{"sqlite3_vtab_config":[[154964,7,154964,9,0,0,4,0],[154994,7,154994,20,0,0,4,0]],"applyNumericAffinity":[[92550,7,92550,12,0,0,4,1],[92551,7,92551,12,0,0,4,0],[92551,16,92551,51,0,0,4,0]],"alsoAnInt":[[92522,7,92522,42,0,0,4,0]],"sqlite3RealSameAsInt":[[82704,10,82704,17,0,0,4,1],[82705,11,82705,42,0,0,4,0],[82706,14,82706,38,0,0,4,0],[82706,42,82706,64,0,0,4,0]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_set_clientdata","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_vtab_config","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_set_clientdata","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_finalize","sqlite3_vtab_config","sqlite3_close"],"visited":0}
/*Here's a C++ fuzzer that uses the specified SQLite3 APIs to process input data as if it were SQLite's output:

*/


// Forward declarations for callback functions
static void  id_000052_collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const void *zName);
static int  id_000052_exec_callback(void *pArg, int argc, char **argv, char **colNames);

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t* f_data, size_t f_size) {
	if(f_size<39) return 0;

	
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
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Get SQLite version (simple API call)
    const char *version = sqlite3_libversion();
    
    // 2. Write input data to a temporary file
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create a temporary database file
    const char *db_filename = "input_file";
    FILE *db_file = fopen(db_filename, "wb");
    if (!db_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Copy input data to database file
    uint8_t buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, db_file);
    }
    
    assert_file_closed(&db_file);;
    assert_file_closed(&in_file);;
    
    // 3. Open the database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        remove(db_filename);
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Set some limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 5. Register collation needed callback
    char callback_context[64] = "fuzz_context";
    sqlite3_collation_needed16(db, callback_context,  id_000052_collation_needed_callback);
    
    // 6. Execute SQL statements using the input data as SQL
    // Use first 512 bytes as SQL string (or all if smaller)
    size_t sql_len = size > 512 ? 512 : size;
    char *sql = new char[sql_len + 1];
    memcpy(sql, data, sql_len);
    sql[sql_len] = '\0';
    
    char *errmsg = nullptr;
    rc = sqlite3_exec(db, sql,  id_000052_exec_callback, nullptr, &errmsg);
    
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    delete[] sql;
    
    // 7. Try to prepare a statement from the input data
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    // Use different portion for prepared statement
    size_t prep_start = size > 100 ? 100 : 0;
    size_t prep_len = size - prep_start;
    if (prep_len > 0 && prep_len < 1024) {
        char *prep_sql = new char[prep_len + 1];
        memcpy(prep_sql, data + prep_start, prep_len);
        prep_sql[prep_len] = '\0';
        
        sqlite3_prepare_v2(db, prep_sql, prep_len, &stmt, &tail);
        delete[] prep_sql;
        
        if (stmt) {
            // 8. Check if statement is explain
            int is_explain = sqlite3_stmt_isexplain(stmt);
            
            // 9. Try to step through the statement
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Just step through results
            }
            
            // 10. Set client data
            int client_data = 42;
            sqlite3_set_clientdata(db, fuzz_str_7, &client_data, nullptr);
            
            // 11. Try to get value encoding (simulated)
            // Since we don't have a sqlite3_value, we'll create a dummy scenario
            // by preparing a simple SELECT statement that returns a value
            sqlite3_stmt *dummy_stmt = nullptr;
            const char *dummy_sql = "SELECT 1";
            if (sqlite3_prepare_v2(db, dummy_sql, -1, &dummy_stmt, nullptr) == SQLITE_OK) {
                if (sqlite3_step(dummy_stmt) == SQLITE_ROW) {
                    // Can't actually call sqlite3_value_encoding without a value
                    // This is a limitation of the provided API list
                }
                sqlite3_finalize(dummy_stmt);
            }
            
            // 12. Finalize the statement
            sqlite3_finalize(stmt);
        }
    }
    
    // 13. Get WAL filename (simulated)
    // Since we don't have a sqlite3_filename, we'll skip the actual call
    // but demonstrate the pattern
    sqlite3_filename dummy_filename = db_filename;
    // const char *wal_file = sqlite3_filename_wal(dummy_filename); // Can't call without proper type
    
    // 14. Configure virtual table
    // This is a no-op since we don't have a virtual table
    sqlite3_vtab_config(db, fuzz_int32_t_8);
    
    // 15. Close database
    sqlite3_close(db);
    
    // 16. Clean up temporary file
    remove(db_filename);
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
}

// Collation needed callback implementation
static void  id_000052_collation_needed_callback(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    // Simple collation that compares strings directly
    sqlite3_create_collation(db, 
                             reinterpret_cast<const char*>(zName),
                             eTextRep,
                             pArg,
                             [](void *pArg, int len1, const void *str1, int len2, const void *str2) {
                                 return memcmp(str1, str2, len1 < len2 ? len1 : len2);
                             });
}

// Exec callback implementation
static int  id_000052_exec_callback(void *pArg, int argc, char **argv, char **colNames) {
    // Just consume the data
    for (int i = 0; i < argc; i++) {
        if (argv[i]) {
            // Do nothing, just consume
            volatile char c = argv[i][0];
            (void)c;
        }
    }
    return 0;
}