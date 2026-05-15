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
//<ID> 247
//<Prompt> []
/*<Combination>: [
*/
//<score> 64, nr_unique_branch: 7
//<Quality> {"density":16,"unique_branches":{"sqlite3Prepare16":[[141220,15,141220,24,0,0,4,1]],"sqlite3_table_column_metadata":[[181571,15,181571,32,0,0,4,0],[181571,36,181571,74,0,0,4,1]],"sqlite3Utf8CharLen":[[34335,19,34335,26,0,0,4,1],[20329,7,20329,23,1,0,4,0],[20330,12,20330,31,1,0,4,0],[20330,12,20330,31,1,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_close_v2","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_table_column_metadata","sqlite3_prepare16","sqlite3_prepare16","sqlite3_step","sqlite3_column_database_name16","sqlite3_reset","sqlite3_bind_int","sqlite3_transfer_bindings","sqlite3_stmt_scanstatus_v2","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_create_function","sqlite3_exec","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_exec","sqlite3_exec","sqlite3_exec","sqlite3_table_column_metadata","sqlite3_prepare16","sqlite3_prepare16","sqlite3_step","sqlite3_column_database_name16","sqlite3_reset","sqlite3_bind_int","sqlite3_transfer_bindings","sqlite3_stmt_scanstatus_v2","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_result_int","sqlite3_result_error_nomem","sqlite3_create_function","sqlite3_exec","sqlite3_finalize","sqlite3_finalize","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* f_data, size_t f_size) {
	if(f_size<311) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(unsigned char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_14, fuzz_str_sz_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_17, fuzz_str_sz_17, fdp);
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	  // Need minimum data for meaningful operations

    // Step 1: Create an in-memory database
    sqlite3* db = nullptr;
    if (sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        return 0;
    }

    // Step 2: Create a table using the first part of input data as SQL
    // We'll use the first 50 bytes as table creation SQL
    size_t sql_len = size > 50 ? 50 : size;
    char* create_sql = (char*)malloc(sql_len + 1);
    if (!create_sql) {
        sqlite3_close_v2(db);
        return 0;
    }
    memcpy(create_sql, data, sql_len);
    create_sql[sql_len] = '\0';
    
    // Create a simple test table
    const char* safe_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL)";
    sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    // Insert some test data
    sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, nullptr);
    sqlite3_exec(db, fuzz_str_3, nullptr, nullptr, nullptr);

    // Step 3: Use sqlite3_table_column_metadata API
    const char* data_type = nullptr;
    const char* coll_seq = nullptr;
    int not_null = 0, primary_key = 0, autoinc = 0;
    
    sqlite3_table_column_metadata(db, fuzz_str_4, fuzz_str_5, fuzz_str_6, 
                                  &data_type, &coll_seq, &not_null, &primary_key, &autoinc);

    // Step 4: Prepare statements using sqlite3_prepare16
    // Use the next part of input data as SQL query (if valid UTF-16)
    sqlite3_stmt* stmt1 = nullptr;
    sqlite3_stmt* stmt2 = nullptr;
    const void* tail = nullptr;
    
    // Convert some data to UTF-16 for testing (simplified conversion)
    size_t utf16_size = (size - sql_len) / 2;
    if (utf16_size > 100) utf16_size = 100;
    
    uint16_t* utf16_sql = (uint16_t*)malloc((utf16_size + 1) * sizeof(uint16_t));
    if (utf16_sql) {
        // Simple ASCII to UTF-16 conversion
        for (size_t i = 0; i < utf16_size && (sql_len + i * 2) < size; i++) {
            utf16_sql[i] = data[sql_len + i * 2];
        }
        utf16_sql[utf16_size] = 0;
        
        // Try to prepare with the UTF-16 SQL
        sqlite3_prepare16(db, utf16_sql, utf16_size * sizeof(uint16_t), &stmt1, &tail);
        
        // Also prepare a valid statement
        const char* valid_sql = "SELECT * FROM test_table";
        sqlite3_prepare16(db, fuzz_str_7, static_cast<int>(fuzz_str_sz_7), &stmt2, &tail);
        
        free(utf16_sql);
    }

    // Step 5: Use sqlite3_column_database_name16
    if (stmt2) {
        sqlite3_step(stmt2);
        const void* db_name = sqlite3_column_database_name16(stmt2, fuzz_int32_t_8);
        // We don't need to use the result, just call the API
        sqlite3_reset(stmt2);
    }

    // Step 6: Create a sqlite3_str and use sqlite3_str_value
    // Note: sqlite3_str is typically created via sqlite3_str_new, but we'll simulate
    // Since we don't have sqlite3_str_new in provided APIs, we'll skip actual creation
    // but demonstrate the API usage pattern
    sqlite3_str* str_obj = nullptr;
    // In real usage: str_obj = sqlite3_str_new(db);
    // char* str_value = sqlite3_str_value(str_obj);
    // sqlite3_str_free(str_obj);

    // Step 7: Use sqlite3_transfer_bindings
    if (stmt1 && stmt2) {
        // Create bindings on stmt1
        sqlite3_bind_int(stmt1, fuzz_int32_t_9, fuzz_int32_t_10);
        // Transfer bindings from stmt1 to stmt2
        sqlite3_transfer_bindings(stmt1, stmt2);
    }

    // Step 8: Use sqlite3_stmt_scanstatus_v2
    if (stmt2) {
        int scan_status = 0;
        sqlite3_stmt_scanstatus_v2(stmt2, fuzz_int32_t_11, fuzz_int32_t_12, fuzz_int32_t_13, &scan_status);
    }

    // Step 9: Simulate sqlite3_result_error_nomem usage
    // This would typically be used in a user-defined function callback
    // We'll create a simple user-defined function that might use it
    struct UserData {
        const uint8_t* data;
        size_t size;
    };
    
    auto udf_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        // Simulate out of memory condition
        void* large_allocation = malloc(1024 * 1024 * 1024); // Try to allocate 1GB
        if (!large_allocation) {
            sqlite3_result_error_nomem(ctx);
            return;
        }
        free(large_allocation);
        sqlite3_result_int(ctx, 42);
    };
    
    sqlite3_create_function(db, fuzz_str_14, fuzz_int32_t_15, fuzz_int32_t_16, nullptr, udf_func, nullptr, nullptr);
    
    // Call the user-defined function
    sqlite3_exec(db, fuzz_str_17, nullptr, nullptr, nullptr);

    // Step 10: Clean up
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    
    free(create_sql);
    sqlite3_close_v2(db);

    return 0;
}