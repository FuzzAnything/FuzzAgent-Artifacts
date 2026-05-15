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
//<ID> 252
//<Prompt> []
/*<Combination>: [
*/
//<score> 32, nr_unique_branch: 1
//<Quality> {"density":16,"unique_branches":{"sqlite3BtreeBeginTrans":[[73509,7,73509,21,0,0,4,1]]},"library_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_libversion","sqlite3_open","sqlite3_limit","sqlite3_limit","sqlite3_limit","sqlite3_collation_needed16","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_value","sqlite3_value_subtype","sqlite3_snapshot_get","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


extern "C" {
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<35) return 0;

	
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
		FDPConsumeRandomLengthString(char, fuzz_str_8, fuzz_str_sz_8, fdp);
	//fuzzer shim end}




    // Create a temporary file to use as database
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
    
    // Get libversion first
    const char *version = sqlite3_libversion();
    if (out_file) {
        fwrite(version, strlen(version), 1, out_file);
    }
    
    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set some limits
    sqlite3_limit(db, fuzz_int32_t_1, fuzz_int32_t_2);
    sqlite3_limit(db, fuzz_int32_t_3, fuzz_int32_t_4);
    sqlite3_limit(db, fuzz_int32_t_5, fuzz_int32_t_6);
    
    // Set collation needed callback (dummy implementation)
    sqlite3_collation_needed16(db, nullptr, [](void*, sqlite3*, int, const void*) {});
    
    // Read SQL from input data
    fseek(in_file, 0, SEEK_END);
    long sql_len = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    char *sql = (char*)malloc(sql_len + 1);
    if (!sql) {
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    fread(sql, 1, sql_len, in_file);
    sql[sql_len] = '\0';
    
    // Execute SQL
    char *errmsg = nullptr;
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
    
    if (rc != SQLITE_OK && errmsg) {
        if (out_file) {
            fwrite(errmsg, strlen(errmsg), 1, out_file);
        }
        sqlite3_free(errmsg);
    }
    
    // Prepare and execute a SELECT statement
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    const char *select_sql = "SELECT * FROM sqlite_master WHERE type='table'";
    
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                sqlite3_value *value = sqlite3_column_value(stmt, fuzz_int32_t_7);
                if (value) {
                    unsigned int subtype = sqlite3_value_subtype(value);
                    if (out_file) {
                        fwrite(&subtype, sizeof(subtype), 1, out_file);
                    }
                }
            }
        }
        
        // Get a snapshot
        sqlite3_snapshot *snapshot = nullptr;
        sqlite3_snapshot_get(db, fuzz_str_8, &snapshot);
        
        sqlite3_finalize(stmt);
    }
    
    // Cleanup
    free(sql);
    sqlite3_close(db);
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
}