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
//<ID> 376
//<Prompt> []
/*<Combination>: [
*/
//<score> 63, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"balance_nonroot":[[14823,57,14823,78,34,0,4,1]],"sqlite3CompareAffinity":[[18155,38,18155,63,4,0,4,1]]},"library_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_close","sqlite3_trace","sqlite3_str_new","sqlite3_str_finish","sqlite3_close","sqlite3_str_appendall","sqlite3_complete16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_stmt_status","sqlite3_finalize","sqlite3_str_finish","sqlite3_free","sqlite3_errmsg16","sqlite3_close"],"critical_calls":["sqlite3_threadsafe","sqlite3_open","sqlite3_trace","sqlite3_str_new","sqlite3_str_appendall","sqlite3_complete16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_stmt_status","sqlite3_finalize","sqlite3_str_finish","sqlite3_free","sqlite3_errmsg16","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t* f_data, size_t f_size) {
	if(f_size<24) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Step 1: Check if SQLite is threadsafe
    int thread_safe = sqlite3_threadsafe();
    
    // Step 2: Open an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Step 3: Set up trace callback
    sqlite3_trace(db, [](void*, const char* sql) {
        // Empty trace callback
    }, nullptr);
    
    // Step 4: Create a string builder
    sqlite3_str *str = sqlite3_str_new(db);
    
    // Step 5: Process input data
    if (size > 0) {
        // Copy input data to heap for safer manipulation
        char *input_copy = (char*)malloc(size + 1);
        if (!input_copy) {
            sqlite3_str_finish(str);
            sqlite3_close(db);
            return 0;
        }
        memcpy(input_copy, data, size);
        input_copy[size] = '\0';
        
        // Step 6: Append input to string builder
        sqlite3_str_appendall(str, input_copy);
        
        // Step 7: Check if input is complete SQL statement (UTF-16)
        // Convert to UTF-16 for checking
        wchar_t *utf16_sql = (wchar_t*)malloc((size + 1) * sizeof(wchar_t));
        if (utf16_sql) {
            for (size_t i = 0; i < size && i < 1024; i++) {
                utf16_sql[i] = data[i];
            }
            utf16_sql[size < 1024 ? size : 1024] = L'\0';
            
            int is_complete = sqlite3_complete16(utf16_sql);
            free(utf16_sql);
        }
        
        // Step 8: Execute SQL if we have enough data
        if (size > 10) {
            char *err_msg = nullptr;
            
            // Try executing as SQL statement
            rc = sqlite3_exec(db, input_copy, nullptr, nullptr, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
            }
            
            // Step 9: Prepare and execute a statement with zeroblob
            sqlite3_stmt *stmt = nullptr;
            const char *tail = nullptr;
            
            // Prepare a statement with placeholder
            const char *test_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER, data BLOB); "
                                   "INSERT INTO test VALUES(?, ?); "
                                   "SELECT * FROM test;";
            
            rc = sqlite3_prepare_v2(db, test_sql, -1, &stmt, &tail);
            
            if (stmt) {
                // Bind integer parameter
                sqlite3_bind_int(stmt, fuzz_int32_t_1, fuzz_int32_t_2);
                
                // Bind zeroblob parameter
                sqlite3_bind_zeroblob64(stmt, fuzz_int32_t_3, size < 1000 ? size : 1000);
                
                // Step through statement
                while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                    // Get column text if available
                    if (sqlite3_column_count(stmt) > 0) {
                        const unsigned char *col_text = sqlite3_column_text(stmt, fuzz_int32_t_4);
                        // Use column text (even if NULL)
                    }
                }
                
                // Get statement status
                int status = sqlite3_stmt_status(stmt, fuzz_int32_t_5, fuzz_int32_t_6);
                
                // Finalize statement
                sqlite3_finalize(stmt);
            }
        }
        
        free(input_copy);
    }
    
    // Step 10: Finish string builder and free result
    char *str_result = sqlite3_str_finish(str);
    if (str_result) {
        sqlite3_free(str_result);
    }
    
    // Step 11: Get error message (UTF-16 version)
    const void *err_msg16 = sqlite3_errmsg16(db);
    
    // Step 12: Clean up
    sqlite3_close(db);
    
    return 0;
}