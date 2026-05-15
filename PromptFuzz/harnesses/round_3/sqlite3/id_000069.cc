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
//<ID> 453
//<Prompt> []
/*<Combination>: [
*/
//<score> 306, nr_unique_branch: 51
//<Quality> {"density":17,"unique_branches":{"sqlite3_uri_key":[[182342,7,182342,19,0,0,4,1],[182342,23,182342,26,0,0,4,1],[182345,31,182345,43,0,0,4,1],[182349,10,182349,22,0,0,4,1],[14155,25,14155,46,1,0,4,0],[14155,26,14155,29,1,0,4,0]],"sqlite3BitvecSet":[[53299,7,53299,11,0,0,4,0],[53303,37,53303,48,0,0,4,0],[53306,9,53306,27,0,0,4,0],[53306,9,53306,27,0,0,4,1],[53308,11,53308,29,0,0,4,1],[53320,7,53320,21,0,0,4,1],[53330,9,53330,25,0,0,4,1],[53332,9,53332,23,0,0,4,0],[53332,9,53332,23,0,0,4,1],[53333,12,53333,25,0,0,4,0],[53333,12,53333,25,0,0,4,1],[53338,7,53338,29,0,0,4,0],[53338,7,53338,29,0,0,4,1],[53342,9,53342,20,0,0,4,1],[53349,16,53349,29,0,0,4,0],[53349,16,53349,29,0,0,4,1],[53350,13,53350,24,0,0,4,0],[53350,13,53350,24,0,0,4,1]],"sqlite3VdbeMemValidStrRep":[[82124,7,82124,26,0,0,4,0],[82126,9,82126,31,0,0,4,1]],"sqlite3VdbeExec":[[93339,7,93339,17,0,0,4,0]],"xferCompatibleIndex":[[134386,7,134386,36,0,0,4,0]],"sqlite3_wal_checkpoint_v2":[[180092,7,180092,12,0,0,4,0],[180093,7,180093,13,0,0,4,0]],"xferOptimization":[[134620,11,134620,49,0,0,4,1],[134751,11,134751,49,0,0,4,1]],"sqlite3_result_zeroblob":[[90111,35,90111,38,0,0,4,1]],"sqlite3BtreeInsert":[[79148,11,79148,13,0,0,4,0]],"sqlite3_keyword_name":[[176468,7,176468,10,0,0,4,1],[176468,14,176468,33,0,0,4,1]],"sqlite3BitvecDestroy":[[53407,7,53407,18,0,0,4,0],[53409,14,53409,27,0,0,4,0],[53409,14,53409,27,0,0,4,1]],"sqlite3BtreeIndexMoveto":[[75811,13,75811,24,0,0,4,0]],"vdbeMergeEngineCompare":[[103857,7,103857,31,0,0,4,1],[103870,13,103870,23,0,0,4,0]],"sqlite3VdbeRecordCompareWithSkip":[[88738,15,88738,33,0,0,4,1],[88740,21,88740,39,0,0,4,1]],"sqlite3BitvecTestNotNull":[[53262,10,53262,21,0,0,4,0],[53266,9,53266,11,0,0,4,0],[53266,9,53266,11,0,0,4,1],[53274,12,53274,25,0,0,4,0],[53275,11,53275,27,0,0,4,1]],"sqlite3ParseUri":[[180581,7,180581,14,0,0,4,1]],"vdbeMergeEngineNew":[[102997,10,102997,19,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_create_function_v2","sqlite3_value_text16","sqlite3_value_int","sqlite3_result_zeroblob","sqlite3_value_text16","sqlite3_value_int","sqlite3_result_zeroblob","sqlite3_exec","sqlite3_error_offset","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text64","sqlite3_step","sqlite3_step","sqlite3_finalize","sqlite3_total_changes64","sqlite3_keyword_name","sqlite3_snapshot_open","sqlite3_wal_checkpoint_v2","sqlite3_db_filename","sqlite3_uri_key","sqlite3_free","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_create_function_v2","sqlite3_result_zeroblob","sqlite3_value_int","sqlite3_value_text16","sqlite3_result_zeroblob","sqlite3_value_int","sqlite3_value_text16","sqlite3_exec","sqlite3_error_offset","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_text64","sqlite3_step","sqlite3_step","sqlite3_finalize","sqlite3_total_changes64","sqlite3_keyword_name","sqlite3_snapshot_open","sqlite3_wal_checkpoint_v2","sqlite3_db_filename","sqlite3_uri_key","sqlite3_free","sqlite3_close_v2"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<72) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_9, fuzz_str_sz_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
	//fuzzer shim end}




    // Create a temporary file with the input data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create output file for database operations
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    char *errmsg = nullptr;
    int rc;
    
    // 1. Open database with URI using output file
    rc = sqlite3_open_v2("file:output_file?mode=memory&cache=shared", 
                         &db, 
                         SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI,
                         nullptr);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Create a custom function that uses sqlite3_result_zeroblob and sqlite3_value_text16
    rc = sqlite3_create_function_v2(
        db,
        fuzz_str_1,
        fuzz_int32_t_2,
        fuzz_int32_t_3,
        nullptr,
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            if (argc == 2) {
                // Use sqlite3_value_text16 on first argument
                const void *text16 = sqlite3_value_text16(argv[0]);
                if (text16) {
                    // Return zeroblob of size from second argument
                    int n = sqlite3_value_int(argv[1]);
                    sqlite3_result_zeroblob(ctx, n);
                }
            }
        },
        nullptr,
        nullptr,
        nullptr
    );
    
    // 3. Execute SQL commands from input data
    if (size > 0) {
        // Read SQL from input file
        char *sql = (char*)malloc(size + 1);
        if (sql) {
            fread(sql, 1, size, in_file);
            sql[size] = '\0';
            
            // Use sqlite3_exec
            rc = sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
            
            // Get error offset if any
            if (rc != SQLITE_OK) {
                int offset = sqlite3_error_offset(db);
                (void)offset; // Use offset to avoid unused variable warning
            }
            
            free(sql);
        }
    }
    
    // 4. Prepare and execute a statement with binding
    const char *test_sql = "SELECT fuzz_func(?, ?)";
    rc = sqlite3_prepare_v2(db, test_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind parameters
        sqlite3_bind_int(stmt, fuzz_int32_t_4, fuzz_int32_t_5);
        sqlite3_bind_text64(stmt, fuzz_int32_t_7, fuzz_str_6, 4, SQLITE_TRANSIENT, SQLITE_UTF8);
        
        // Execute
        rc = sqlite3_step(stmt);
        while (rc == SQLITE_ROW) {
            rc = sqlite3_step(stmt);
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // 5. Get total changes
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    (void)total_changes; // Use to avoid unused variable warning
    
    // 6. Get keyword name
    const char *keyword = nullptr;
    int keylen = 0;
    sqlite3_keyword_name(fuzz_int32_t_8, &keyword, &keylen);
    
    // 7. Try to open snapshot (may fail if no write transaction)
    sqlite3_snapshot *snapshot = nullptr;
    sqlite3_snapshot_open(db, fuzz_str_9, snapshot);
    
    // 8. Perform WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, fuzz_str_10, fuzz_int32_t_11, &pnLog, &pnCkpt);
    
    // 9. Get URI key from filename
    sqlite3_filename db_file = sqlite3_db_filename(db, fuzz_str_12);
    if (db_file) {
        const char *uri_key = sqlite3_uri_key(db_file, fuzz_int32_t_13);
        (void)uri_key; // Use to avoid unused variable warning
    }
    
    // 10. Free any error message
    if (errmsg) {
        sqlite3_free(errmsg);
    }
    
    // 11. Close database
    sqlite3_close_v2(db);
    
    // Clean up
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove temporary output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}