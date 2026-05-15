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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 128, nr_unique_branch: 16
//<Quality> {"density":32,"unique_branches":{"sqlite3_blob_open":[[101633,7,101633,20,0,0,4,1],[101636,9,101636,14,0,0,4,0],[101636,18,101636,30,0,0,4,1],[101639,32,101639,36,0,0,4,0]],"blobSeekToRow":[[101378,9,101378,16,0,0,4,0],[101380,11,101380,18,0,0,4,0],[101393,7,101393,21,0,0,4,1],[101395,13,101395,21,0,0,4,1]],"sqlite3_complete":[[177353,44,177353,56,0,0,4,1],[177354,13,177354,23,0,0,4,1],[177399,29,177399,66,0,0,4,0],[177407,29,177407,67,0,0,4,0],[177409,35,177409,70,0,0,4,0],[177411,35,177411,75,0,0,4,0],[177419,29,177419,63,0,0,4,0],[177423,29,177423,67,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_open_v2","sqlite3_shutdown","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_bind_text","sqlite3_free","sqlite3_malloc64","sqlite3_bind_blob64","sqlite3_bind_int","sqlite3_step","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_int64","sqlite3_column_text","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_int","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_malloc","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_malloc","sqlite3_malloc","sqlite3_strglob","sqlite3_free","sqlite3_free","sqlite3_keyword_count","sqlite3_keyword_check","sqlite3_malloc","sqlite3_complete","sqlite3_free","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_open_v2","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_malloc","sqlite3_bind_text","sqlite3_free","sqlite3_malloc64","sqlite3_bind_blob64","sqlite3_bind_int","sqlite3_step","sqlite3_clear_bindings","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_stmt_isexplain","sqlite3_step","sqlite3_column_int64","sqlite3_column_text","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_int","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_malloc","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_malloc","sqlite3_malloc","sqlite3_strglob","sqlite3_free","sqlite3_free","sqlite3_keyword_count","sqlite3_keyword_check","sqlite3_malloc","sqlite3_complete","sqlite3_free","sqlite3_close","sqlite3_shutdown"],"visited":0}
/*Here's a C++ fuzz driver that uses the specified SQLite3 APIs to simulate a database operation scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* f_data, size_t f_size) {
	if(f_size<93) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_16, fuzz_str_sz_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
		FDPConsumeIntegral(int64_t, fuzz_int64_t_18, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_19, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_20, fdp);
	//fuzzer shim end}




    if (size < 1) {
	return 0;
	}
	
    
    // 1. Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Get library version
    const char* lib_version = sqlite3_libversion();
    
    // 2. Open database (in-memory for fuzzing safety)
    sqlite3* db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, 
                            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                            nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // 3. Set database limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // 4. Register collation handler
    sqlite3_collation_needed16(db, nullptr, 
        [](void*, sqlite3*, int eTextRep, const void* zName) {});
    
    // 5. Create test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "data BLOB, "
        "value INTEGER);";
    
    sqlite3_stmt* create_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &create_stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(create_stmt);
        sqlite3_finalize(create_stmt);
    }
    
    // 6. Insert data using prepared statement
    const char* insert_sql = "INSERT INTO test (name, data, value) VALUES (?, ?, ?);";
    sqlite3_stmt* insert_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr);
    
    if (rc == SQLITE_OK && insert_stmt != nullptr) {
        // Bind parameters using input data
        size_t data_offset = 0;
        
        // Bind text parameter (name)
        if (data_offset < size) {
            size_t text_len = data[data_offset] % 100 + 1;
            if (text_len > size - data_offset - 1) text_len = size - data_offset - 1;
            if (text_len > 0) {
                char* text_buf = (char*)sqlite3_malloc(text_len + 1);
                if (text_buf) {
                    memcpy(text_buf, data + data_offset + 1, text_len);
                    text_buf[text_len] = '\0';
                    sqlite3_bind_text(insert_stmt, fuzz_int32_t_7, text_buf, -1, SQLITE_TRANSIENT);
                    sqlite3_free(text_buf);
                }
                data_offset += text_len + 1;
            }
        }
        
        // Bind blob parameter (data)
        if (data_offset < size) {
            size_t blob_len = data[data_offset] % 1000 + 1;
            if (blob_len > size - data_offset - 1) blob_len = size - data_offset - 1;
            if (blob_len > 0) {
                void* blob_data = sqlite3_malloc64(blob_len);
                if (blob_data) {
                    memcpy(blob_data, data + data_offset + 1, blob_len);
                    sqlite3_bind_blob64(insert_stmt, fuzz_int32_t_8, blob_data, blob_len, sqlite3_free);
                    data_offset += blob_len + 1;
                }
            }
        }
        
        // Bind integer parameter (value)
        if (data_offset < size) {
            int int_value = data[data_offset];
            sqlite3_bind_int(insert_stmt, fuzz_int32_t_9, int_value);
            data_offset++;
        }
        
        // Execute insert
        sqlite3_step(insert_stmt);
        sqlite3_clear_bindings(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // 7. Query and process data
    const char* select_sql = "SELECT id, name, data, value FROM test;";
    sqlite3_stmt* select_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
    
    if (rc == SQLITE_OK && select_stmt != nullptr) {
        // Check if statement is an EXPLAIN
        int is_explain = sqlite3_stmt_isexplain(select_stmt);
        
        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // Read column values
            sqlite3_int64 id = sqlite3_column_int64(select_stmt, fuzz_int32_t_10);
            const unsigned char* name = sqlite3_column_text(select_stmt, fuzz_int32_t_11);
            const void* blob_data = sqlite3_column_blob(select_stmt, fuzz_int32_t_12);
            int blob_size = sqlite3_column_bytes(select_stmt, fuzz_int32_t_13);
            int value = sqlite3_column_int(select_stmt, fuzz_int32_t_14);
        }
        sqlite3_finalize(select_stmt);
    }
    
    // 8. Test blob operations
    sqlite3_blob* blob_handle = nullptr;
    rc = sqlite3_blob_open(db, fuzz_str_15, fuzz_str_16, fuzz_str_17, fuzz_int64_t_18, fuzz_int32_t_19, &blob_handle);
    
    if (rc == SQLITE_OK && blob_handle != nullptr) {
        int blob_size = sqlite3_blob_bytes(blob_handle);
        if (blob_size > 0 && blob_size <= 1024) {
            void* read_buffer = sqlite3_malloc(blob_size);
            if (read_buffer) {
                sqlite3_blob_read(blob_handle, read_buffer, blob_size, fuzz_int32_t_20);
                sqlite3_free(read_buffer);
            }
        }
        sqlite3_blob_close(blob_handle);
    }
    
    // 9. Test string functions
    if (size > 10) {
        char* pattern = (char*)sqlite3_malloc(6);
        char* test_str = (char*)sqlite3_malloc(6);
        if (pattern && test_str) {
            memcpy(pattern, data, 5);
            memcpy(test_str, data + 5, 5);
            pattern[5] = '\0';
            test_str[5] = '\0';
            
            int glob_result = sqlite3_strglob(pattern, test_str);
            sqlite3_free(pattern);
            sqlite3_free(test_str);
        }
    }
    
    // 10. Test keyword functions
    int keyword_count = sqlite3_keyword_count();
    if (keyword_count > 0 && size > 0) {
        // Use first byte to select a keyword to check
        int keyword_idx = data[0] % keyword_count;
        const char* keywords[] = {"SELECT", "INSERT", "UPDATE", "DELETE", "CREATE"};
        if (keyword_idx < 5) {
            int is_keyword = sqlite3_keyword_check(keywords[keyword_idx], 
                                                  (int)strlen(keywords[keyword_idx]));
        }
    }
    
    // 11. Test SQL completeness
    if (size > 0) {
        char* test_sql = (char*)sqlite3_malloc(size + 1);
        if (test_sql) {
            memcpy(test_sql, data, size);
            test_sql[size] = '\0';
            int is_complete = sqlite3_complete(test_sql);
            sqlite3_free(test_sql);
        }
    }
    
    // 12. Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}