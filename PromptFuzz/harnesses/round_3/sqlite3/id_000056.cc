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
//<ID> 339
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 5
//<Quality> {"density":24,"unique_branches":{"sqlite3VdbeMemTranslate":[[34173,9,34173,35,0,0,4,1],[34182,14,34182,23,0,0,4,0],[34182,14,34182,23,0,0,4,1],[34047,7,34047,14,11,0,4,1],[34007,7,34007,16,12,0,4,0]]},"library_calls":["sqlite3_libversion","sqlite3_log","sqlite3_open","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_log","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_log","sqlite3_step","sqlite3_data_count","sqlite3_column_table_name","sqlite3_log","sqlite3_column_value","sqlite3_value_encoding","sqlite3_log","sqlite3_finalize","sqlite3_prepare16_v3","sqlite3_step","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_log","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_log","sqlite3_open","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_log","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_log","sqlite3_step","sqlite3_data_count","sqlite3_column_table_name","sqlite3_log","sqlite3_column_value","sqlite3_value_encoding","sqlite3_log","sqlite3_finalize","sqlite3_prepare16_v3","sqlite3_step","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_log","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* f_data, size_t f_size) {
	if(f_size<71) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Get library version and log it
    const char *version = sqlite3_libversion();
    sqlite3_log(fuzz_int32_t_1, "SQLite version: %s", version);
    
    // 2. Write input data to a file for database operations
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // 3. Create database from input file
    sqlite3 *db = nullptr;
    const char *db_filename = "input_file";
    
    // Write data to file first
    FILE *db_file = fopen(db_filename, "wb");
    if (db_file) {
        fwrite(data, 1, size, db_file);
        assert_file_closed(&db_file);;
    }
    
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK) {
        // Try in-memory database if file open fails
        sqlite3_open(":memory:", &db);
    }
    
    if (!db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 4. Set some limits
    sqlite3_limit(db, fuzz_int32_t_2, 1000000);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 5. Execute SQL using input data as SQL string (with bounds checking)
    char *errmsg = nullptr;
    char *sql = nullptr;
    if (size > 0 && size < 1000000) {
        sql = (char *)malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            
            // Try to execute as SQL
            rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
            if (errmsg) {
                sqlite3_log(rc, "SQL error: %s", errmsg);
                sqlite3_free(errmsg);
            }
            free(sql);
        }
    }
    
    // 6. Prepare and execute a statement using prepare_v2
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    const char *simple_sql = "SELECT name FROM sqlite_master WHERE type='table';";
    
    rc = sqlite3_prepare_v2(db, simple_sql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt) {
        // Check if statement is EXPLAIN
        int is_explain = sqlite3_stmt_isexplain(stmt);
        if (is_explain) {
            sqlite3_log(fuzz_int32_t_7, "Statement is EXPLAIN");
        }
        
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_data_count(stmt);
            if (col_count > 0) {
                // Get table name
                const char *table_name = sqlite3_column_table_name(stmt, fuzz_int32_t_8);
                if (table_name) {
                    sqlite3_log(fuzz_int32_t_9, "Table: %s", table_name);
                }
                
                // Get column value and check encoding
                sqlite3_value *col_value = sqlite3_column_value(stmt, fuzz_int32_t_10);
                if (col_value) {
                    int encoding = sqlite3_value_encoding(col_value);
                    sqlite3_log(fuzz_int32_t_11, "Value encoding: %d", encoding);
                }
            }
        }
        
        sqlite3_finalize(stmt);
    }
    
    // 7. Prepare statement using prepare16_v3 (UTF-16)
    sqlite3_stmt *stmt16 = nullptr;
    const void *tail16 = nullptr;
    
    // Create a simple UTF-16 SQL string
    const unsigned short utf16_sql[] = {
        'S','E','L','E','C','T',' ','1',';',0
    };
    
    rc = sqlite3_prepare16_v3(db, utf16_sql, 
                              sizeof(utf16_sql), 
                              fuzz_uint32_t_12, 
                              &stmt16, 
                              &tail16);
    if (rc == SQLITE_OK && stmt16) {
        if (sqlite3_step(stmt16) == SQLITE_ROW) {
            // Process row if any
        }
        sqlite3_finalize(stmt16);
    }
    
    // 8. Set client data
    static int client_data = 42;
    sqlite3_set_clientdata(db, fuzz_str_13, &client_data, nullptr);
    
    // 9. Get database filename and WAL filename
    sqlite3_filename db_file_name = sqlite3_db_filename(db, fuzz_str_14);
    if (db_file_name) {
        const char *wal_name = sqlite3_filename_wal(db_file_name);
        if (wal_name) {
            sqlite3_log(fuzz_int32_t_15, "WAL file: %s", wal_name);
        }
    }
    
    // 10. Cleanup
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    
    // Remove temporary files
    remove(db_filename);
    remove("output_file");
    
    assert_file_closed(&db_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}