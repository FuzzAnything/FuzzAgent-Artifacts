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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 40, nr_unique_branch: 1
//<Quality> {"density":20,"unique_branches":{"walIndexRecover":[[65630,7,65630,9,0,0,4,0]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare16_v3","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_column_table_name","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_log","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare16_v3","sqlite3_step","sqlite3_column_value","sqlite3_value_encoding","sqlite3_column_table_name","sqlite3_finalize","sqlite3_set_clientdata","sqlite3_db_filename","sqlite3_filename_wal","sqlite3_log","sqlite3_close"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite APIs in a realistic scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<149) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // 1. Get SQLite library version
    const char *version = sqlite3_libversion();
    
    // Create a temporary file for database operations
    const char *temp_filename = "input_file";
    
    // Write input data to temporary file
    FILE *temp_file = fopen(temp_filename, "wb");
    if (!temp_file) {
	assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
	}
	
    fwrite(data, 1, size, temp_file);
    assert_file_closed(&temp_file);;
    
    // 2. Open database from the input file
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(temp_filename, &db);
    
    if (rc == SQLITE_OK && db) {
        // 3. Set some limits on the database
        sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
        sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
        
        // 4. Execute a simple SQL statement
        char *errmsg = nullptr;
        const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, name TEXT);";
        rc = sqlite3_exec(db, fuzz_str_5, nullptr, nullptr, &errmsg);
        
        if (errmsg) {
            sqlite3_free(errmsg);
        }
        
        // Insert some data using prepared statement
        const char *insert_sql = "INSERT INTO test(name) VALUES(?);";
        sqlite3_stmt *stmt = nullptr;
        const char *tail = nullptr;
        
        // 5. Prepare a statement in UTF-8
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, &tail);
        
        if (rc == SQLITE_OK && stmt) {
            // 6. Check if statement is explain
            int is_explain = sqlite3_stmt_isexplain(stmt);
            
            // Bind some data from input
            if (size > 0) {
                sqlite3_bind_text(stmt, fuzz_int32_t_6, reinterpret_cast<const char*>(data), 
                                 (size > 100 ? 100 : size), SQLITE_TRANSIENT);
            }
            
            // 7. Step through the statement
            rc = sqlite3_step(stmt);
            
            // Finalize the statement
            sqlite3_finalize(stmt);
        }
        
        // 8. Prepare another statement in UTF-16
        const void *select_sql16 = L"SELECT * FROM test;";
        sqlite3_stmt *stmt16 = nullptr;
        const void *tail16 = nullptr;
        
        rc = sqlite3_prepare16_v3(db, fuzz_str_7, static_cast<int>(fuzz_str_sz_7), fuzz_uint32_t_8, &stmt16, &tail16);
        
        if (rc == SQLITE_OK && stmt16) {
            // Step through results
            while (sqlite3_step(stmt16) == SQLITE_ROW) {
                // 9. Get column value
                sqlite3_value *value = sqlite3_column_value(stmt16, fuzz_int32_t_9);
                
                if (value) {
                    // 10. Get value encoding
                    int encoding = sqlite3_value_encoding(value);
                    
                    // 11. Get column table name
                    const char *table_name = sqlite3_column_table_name(stmt16, fuzz_int32_t_10);
                }
            }
            
            sqlite3_finalize(stmt16);
        }
        
        // 12. Set client data
        int client_data = 42;
        rc = sqlite3_set_clientdata(db, fuzz_str_11, &client_data, nullptr);
        
        // 13. Get database filename
        sqlite3_filename db_filename = sqlite3_db_filename(db, fuzz_str_12);
        
        // 14. Get WAL filename
        const char *wal_filename = sqlite3_filename_wal(db_filename);
        
        // 15. Log some information
        sqlite3_log(fuzz_int32_t_13, "Fuzzing test: version=%s", version);
        
        // Clean up
        remove(temp_filename);
        
        // 16. Close database
        sqlite3_close(db);
    }
    
    // Clean up temporary file if it still exists
    remove(temp_filename);
    
    assert_file_closed(&temp_file);
	assert_file_name_closed("input_file");
	return 0;
}