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
//<ID> 135
//<Prompt> []
/*<Combination>: [
*/
//<score> 10, nr_unique_branch: 0
//<Quality> {"density":10,"unique_branches":{},"library_calls":["sqlite3_open","sqlite3_create_function_v2","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_finalize","sqlite3_cancel_auto_extension","sqlite3_changes64","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_create_function_v2","sqlite3_exec","sqlite3_exec","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_text","sqlite3_finalize","sqlite3_cancel_auto_extension","sqlite3_changes64","sqlite3_close"],"visited":0}
/**/


// Custom UDF callback function that uses the required APIs
static void  id_000017_customUdfCallback(sqlite3_context* ctx, int argc, sqlite3_value** argv) {
    // 1. Use sqlite3_get_auxdata
    void* auxData = sqlite3_get_auxdata(ctx, 0);
    
    // 2. Use sqlite3_malloc for temporary storage
    void* tempBuffer = sqlite3_malloc(256);
    if (tempBuffer) {
        // 3. Use sqlite3_result_zeroblob64
        sqlite3_result_zeroblob64(ctx, 1024);  // Return 1KB zeroblob
        
        // 4. Use sqlite3_result_double
        sqlite3_result_double(ctx, 3.14159);
        
        sqlite3_free(tempBuffer);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t* f_data, size_t f_size) {
	if(f_size<92) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_5, fuzz_str_sz_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file
    FILE* out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Register custom UDF
    sqlite3_create_function_v2(
        db, fuzz_str_1, fuzz_int32_t_2, fuzz_int32_t_3, nullptr,
         id_000017_customUdfCallback, nullptr, nullptr, nullptr
    );
    
    // Create a table
    const char* createTableSql = "CREATE TABLE test (id INTEGER, name TEXT);";
    sqlite3_exec(db, fuzz_str_4, nullptr, nullptr, nullptr);
    
    // Insert data using input bytes
    if (size > 0) {
        char insertSql[256];
        snprintf(insertSql, sizeof(insertSql), 
                "INSERT INTO test VALUES (%d, 'test');", (int)data[0]);
        sqlite3_exec(db, insertSql, nullptr, nullptr, nullptr);
    }
    
    // Prepare a statement to query the table
    const char* selectSql = "SELECT * FROM test;";
    if (sqlite3_prepare_v2(db, fuzz_str_5, -1, &stmt, nullptr) == SQLITE_OK) {
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 5. Use sqlite3_column_table_name16 (simulated with column_text)
            const unsigned char* tableName = sqlite3_column_text(stmt, fuzz_int32_t_6);
            
            // Write results to output file
            if (tableName) {
                fwrite(tableName, 1, strlen((const char*)tableName), out_file);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // 6. Use sqlite3_cancel_auto_extension
    sqlite3_cancel_auto_extension(nullptr);
    
    // 7. Use sqlite3_changes64
    sqlite3_int64 changes = sqlite3_changes64(db);
    
    // Write changes count to output file
    fwrite(&changes, sizeof(changes), 1, out_file);
    
    // Clean up
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}