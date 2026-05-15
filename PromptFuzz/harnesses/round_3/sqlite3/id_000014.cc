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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 34, nr_unique_branch: 14
//<Quality> {"density":17,"unique_branches":{"sqlite3_stmt_scanstatus_v2":[[91875,16,91875,28,0,0,4,0],[91877,11,91877,23,0,0,4,0],[91879,13,91879,20,0,0,4,0],[91883,7,91883,20,0,0,4,1],[91886,5,91886,31,0,0,4,0],[91887,11,91887,28,0,0,4,0],[91894,5,91894,32,0,0,4,1],[91902,5,91902,29,0,0,4,1],[91912,5,91912,30,0,0,4,1],[91916,5,91916,33,0,0,4,1],[91924,5,91924,34,0,0,4,1],[91932,5,91932,34,0,0,4,1],[91940,5,91940,32,0,0,4,1],[91971,5,91971,12,0,0,4,1]]},"library_calls":["sqlite3_open_v2","sqlite3_db_config","sqlite3_db_config","sqlite3_db_config","sqlite3_soft_heap_limit","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_pointer","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open_v2","sqlite3_db_config","sqlite3_db_config","sqlite3_db_config","sqlite3_soft_heap_limit","sqlite3_exec","sqlite3_prepare_v3","sqlite3_bind_pointer","sqlite3_bind_double","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_stmt_scanstatus_v2","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t* f_data, size_t f_size) {
	if(f_size<234) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_3, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_4, fuzz_str_sz_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_15, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_16, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_17, fdp);
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Create output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;
    
    // 1. Open database from input file
    rc = sqlite3_open_v2("input_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // 2. Configure database using sqlite3_db_config
    // Set multiple configurations (using common op values)
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &rc);
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, &rc);
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, 1, &rc);
    
    // 3. Set soft heap limit
    sqlite3_soft_heap_limit(1024 * 1024); // 1MB limit
    
    // 4. Create a table and insert data from input
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "value REAL);";
    
    rc = sqlite3_exec(db, fuzz_str_1, nullptr, nullptr, nullptr);
    
    // 5. Prepare a statement with pointer binding
    const char *insert_sql = "INSERT INTO test_table (data, value) VALUES (?, ?)";
    rc = sqlite3_prepare_v3(db, fuzz_str_2, -1, fuzz_uint32_t_3, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Create some data to bind as pointer
        struct CustomData {
            int id;
            char name[32];
        };
        
        CustomData *custom_ptr = new CustomData{1, "test"};
        
        // 6. Bind pointer to statement
        // Note: sqlite3_bind_pointer requires a valid pointer type name
        rc = sqlite3_bind_pointer(stmt, fuzz_int32_t_5, custom_ptr, fuzz_str_4, 
                                  [](void *ptr) { delete static_cast<CustomData*>(ptr); });
        
        // Bind a double value
        sqlite3_bind_double(stmt, fuzz_int32_t_6, 3.14159);
        
        // Execute statement
        rc = sqlite3_step(stmt);
        
        // 7. Use sqlite3_stmt_scanstatus_v2 to get scan status
        if (rc == SQLITE_DONE) {
            int64_t nLoop = 0;
            // Use common scan status operations
            sqlite3_stmt_scanstatus_v2(stmt, fuzz_int32_t_7, fuzz_int32_t_8, fuzz_int32_t_9, &nLoop); // SQLITE_SCANSTAT_NLOOP
            sqlite3_stmt_scanstatus_v2(stmt, fuzz_int32_t_10, fuzz_int32_t_11, fuzz_int32_t_12, &nLoop); // SQLITE_SCANSTAT_NSELECT
        }
        
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 8. Prepare another statement for querying
    const char *select_sql = "SELECT * FROM test_table";
    rc = sqlite3_prepare_v3(db, fuzz_str_13, -1, fuzz_uint32_t_14, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Execute and get scan status
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int64_t nLoop = 0;
            sqlite3_stmt_scanstatus_v2(stmt, fuzz_int32_t_15, fuzz_int32_t_16, fuzz_int32_t_17, &nLoop);
        }
        sqlite3_finalize(stmt);
    }
    
    // 9. Test control operations (using common test control ops)
    // Note: sqlite3_test_control is not in the provided API list, so we'll skip it
    // as per the instruction to use only provided APIs
    
    // 10. Clean up
    // Close database
    if (db) {
        sqlite3_close(db);
    }
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("input_file");
	return 0;
}