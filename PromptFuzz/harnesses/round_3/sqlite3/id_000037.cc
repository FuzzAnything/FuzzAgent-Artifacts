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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 238, nr_unique_branch: 48
//<Quality> {"density":34,"unique_branches":{"sqlite3ErrorToParser":[[34710,16,34710,40,0,0,4,0]],"sqlite3VdbeMemSetStr":[[83140,11,83140,31,0,0,4,1]],"patternCompare":[[128157,28,128157,56,0,0,4,0],[128163,17,128163,30,0,0,4,0],[128164,13,128164,31,0,0,4,1],[128171,18,128171,26,0,0,4,0],[128171,18,128171,26,0,0,4,1],[128173,17,128173,39,0,0,4,0],[128173,17,128173,39,0,0,4,1],[128218,11,128218,29,0,0,4,1],[128227,13,128227,17,0,0,4,0],[128227,13,128227,17,0,0,4,1],[128229,13,128229,20,0,0,4,0],[128229,13,128229,20,0,0,4,1],[128233,13,128233,20,0,0,4,0],[128233,13,128233,20,0,0,4,1],[128234,15,128234,21,0,0,4,0],[128234,15,128234,21,0,0,4,1],[128237,16,128237,18,0,0,4,0],[128237,16,128237,18,0,0,4,1],[128237,22,128237,29,0,0,4,0],[128237,22,128237,29,0,0,4,1],[128238,15,128238,22,0,0,4,0],[128238,15,128238,22,0,0,4,1],[128238,26,128238,42,0,0,4,0],[128238,26,128238,42,0,0,4,1],[128238,46,128238,60,0,0,4,0],[128238,46,128238,60,0,0,4,1],[128238,64,128238,73,0,0,4,0],[128238,64,128238,73,0,0,4,1],[128240,17,128240,27,0,0,4,0],[128240,17,128240,27,0,0,4,1],[128240,31,128240,36,0,0,4,0],[128240,31,128240,36,0,0,4,1],[128243,17,128243,22,0,0,4,0],[128243,17,128243,22,0,0,4,1],[128250,13,128250,18,0,0,4,0],[128250,13,128250,18,0,0,4,1],[128250,22,128250,40,0,0,4,0],[128250,22,128250,40,0,0,4,1],[20329,7,20329,23,6,0,4,0],[20329,7,20329,23,6,0,4,1],[20330,12,20330,31,6,0,4,0],[20330,12,20330,31,6,0,4,1]],"sqlite3_clear_bindings":[[89629,12,89629,21,0,0,4,0]],"bindText":[[91148,11,91148,24,0,0,4,1],[91151,11,91151,13,0,0,4,0]],"sqlite3VdbeExec":[[95807,9,95807,52,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_open_v2","sqlite3_shutdown","sqlite3_limit","sqlite3_limit","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_bind_blob64","sqlite3_malloc64","sqlite3_bind_text","sqlite3_bind_text","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_clear_bindings","sqlite3_reset","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_column_bytes","sqlite3_column_blob","sqlite3_column_text","sqlite3_column_int","sqlite3_stmt_isexplain","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_malloc","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_finalize","sqlite3_malloc","sqlite3_malloc","sqlite3_strglob","sqlite3_complete","sqlite3_free","sqlite3_free","sqlite3_keyword_count","sqlite3_keyword_check","sqlite3_collation_needed16","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_libversion","sqlite3_open_v2","sqlite3_limit","sqlite3_limit","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_malloc64","sqlite3_bind_text","sqlite3_bind_int","sqlite3_step","sqlite3_clear_bindings","sqlite3_reset","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_int64","sqlite3_column_bytes","sqlite3_column_blob","sqlite3_column_text","sqlite3_column_int","sqlite3_stmt_isexplain","sqlite3_blob_open","sqlite3_blob_bytes","sqlite3_malloc","sqlite3_blob_read","sqlite3_free","sqlite3_blob_close","sqlite3_finalize","sqlite3_malloc","sqlite3_malloc","sqlite3_strglob","sqlite3_complete","sqlite3_free","sqlite3_free","sqlite3_keyword_count","sqlite3_keyword_check","sqlite3_collation_needed16","sqlite3_close","sqlite3_shutdown"],"visited":0}
/*Here's a C++ fuzz driver that uses all the specified SQLite3 APIs in a realistic scenario:

*/


extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* f_data, size_t f_size) {
	if(f_size<109) return 0;

	
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
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_18, fuzz_str_sz_18, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_19, fuzz_str_sz_19, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_20, fuzz_str_sz_20, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_21, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_22, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_23, fuzz_str_sz_23, fdp);
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }
    
    // Get library version
    const char *version = sqlite3_libversion();
    
    // Create in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Set some limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    
    // Create table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "text_data TEXT, "
        "int_val INTEGER)";
    
    sqlite3_stmt *create_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &create_stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(create_stmt);
        sqlite3_finalize(create_stmt);
    }
    
    // Insert data using prepared statement
    const char *insert_sql = "INSERT INTO test (data, text_data, int_val) VALUES (?, ?, ?)";
    sqlite3_stmt *insert_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &insert_stmt, nullptr);
    
    if (rc == SQLITE_OK && insert_stmt != nullptr) {
        // Split input data for different bindings
        size_t blob_size = size > 100 ? 100 : size;
        size_t text_size = size > blob_size ? size - blob_size : 0;
        
        // Bind blob (first 100 bytes or less)
        if (blob_size > 0) {
            sqlite3_bind_blob64(insert_stmt, fuzz_int32_t_5, data, blob_size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_blob64(insert_stmt, fuzz_int32_t_6, nullptr, 0, SQLITE_TRANSIENT);
        }
        
        // Bind text (remaining bytes)
        if (text_size > 0) {
            char *text_data = (char*)sqlite3_malloc64(text_size + 1);
            if (text_data) {
                memcpy(text_data, data + blob_size, text_size);
                text_data[text_size] = '\0';
                sqlite3_bind_text(insert_stmt, fuzz_int32_t_7, text_data, -1, sqlite3_free);
            } else {
                sqlite3_bind_text(insert_stmt, fuzz_int32_t_8, "", -1, SQLITE_STATIC);
            }
        } else {
            sqlite3_bind_text(insert_stmt, fuzz_int32_t_9, "default", -1, SQLITE_STATIC);
        }
        
        // Bind integer
        sqlite3_bind_int(insert_stmt, fuzz_int32_t_10, static_cast<int>(size));
        
        // Execute insert
        sqlite3_step(insert_stmt);
        
        // Clear bindings and reuse statement
        sqlite3_clear_bindings(insert_stmt);
        sqlite3_reset(insert_stmt);
        
        // Rebind with different values
        sqlite3_bind_int(insert_stmt, fuzz_int32_t_11, fuzz_int32_t_12);
        sqlite3_step(insert_stmt);
        
        sqlite3_finalize(insert_stmt);
    }
    
    // Query data back
    const char *select_sql = "SELECT id, data, text_data, int_val FROM test";
    sqlite3_stmt *select_stmt = nullptr;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
    
    if (rc == SQLITE_OK && select_stmt != nullptr) {
        while (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // Get column values
            sqlite3_int64 id = sqlite3_column_int64(select_stmt, fuzz_int32_t_13);
            int blob_size = sqlite3_column_bytes(select_stmt, fuzz_int32_t_14);
            const void *blob_data = sqlite3_column_blob(select_stmt, fuzz_int32_t_15);
            const unsigned char *text = sqlite3_column_text(select_stmt, fuzz_int32_t_16);
            int int_val = sqlite3_column_int(select_stmt, fuzz_int32_t_17);
            
            // Use sqlite3_stmt_isexplain
            int is_explain = sqlite3_stmt_isexplain(select_stmt);
            
            // Open blob for reading if we have blob data
            if (blob_data != nullptr && blob_size > 0) {
                sqlite3_blob *blob = nullptr;
                rc = sqlite3_blob_open(db, fuzz_str_18, fuzz_str_19, fuzz_str_20, id, fuzz_int32_t_21, &blob);
                if (rc == SQLITE_OK && blob != nullptr) {
                    // Get blob size
                    int blob_bytes = sqlite3_blob_bytes(blob);
                    
                    // Read blob content
                    if (blob_bytes > 0) {
                        void *buffer = sqlite3_malloc(blob_bytes);
                        if (buffer) {
                            sqlite3_blob_read(blob, buffer, blob_bytes, fuzz_int32_t_22);
                            sqlite3_free(buffer);
                        }
                    }
                    
                    sqlite3_blob_close(blob);
                }
            }
        }
        sqlite3_finalize(select_stmt);
    }
    
    // Test string functions
    if (size > 10) {
        char *pattern = (char*)sqlite3_malloc(10);
        char *str = (char*)sqlite3_malloc(10);
        if (pattern && str) {
            memcpy(pattern, data, 9);
            memcpy(str, data + (size > 20 ? 10 : 0), 9);
            pattern[9] = '\0';
            str[9] = '\0';
            
            // Test string glob matching
            int glob_result = sqlite3_strglob(pattern, str);
            
            // Test if string is complete SQL
            int is_complete = sqlite3_complete(fuzz_str_23);
            
            sqlite3_free(pattern);
            sqlite3_free(str);
        }
    }
    
    // Test keyword functions
    int keyword_count = sqlite3_keyword_count();
    if (keyword_count > 0) {
        // Check if "SELECT" is a keyword
        int is_keyword = sqlite3_keyword_check("SELECT", 6);
    }
    
    // Set collation needed callback
    sqlite3_collation_needed16(db, nullptr, nullptr);
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}