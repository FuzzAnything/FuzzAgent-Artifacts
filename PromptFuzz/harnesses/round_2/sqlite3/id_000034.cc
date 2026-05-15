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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 237.5, nr_unique_branch: 70
//<Quality> {"density":25,"unique_branches":{"lockBtree":[[73115,9,73115,37,0,0,4,0],[73142,9,73142,23,0,0,4,0]],"sqlite3PagerCheckpoint":[[63923,7,63923,22,0,0,4,1],[63935,7,63935,19,0,0,4,0],[63937,10,63937,42,0,0,4,0]],"sqlite3_test_control":[[181698,11,181698,13,0,0,4,1],[181703,5,181703,35,0,0,4,1],[181713,5,181713,38,0,0,4,1],[181739,5,181739,35,0,0,4,1],[181762,5,181762,38,0,0,4,1],[181781,5,181781,37,0,0,4,1],[181799,5,181799,39,0,0,4,1],[181820,5,181820,45,0,0,4,1],[181842,5,181842,38,0,0,4,1],[181864,5,181864,32,0,0,4,1],[181923,5,181923,32,0,0,4,1],[181940,5,181940,35,0,0,4,1],[181954,5,181954,39,0,0,4,0],[181973,5,181973,41,0,0,4,1],[181989,5,181989,44,0,0,4,1],[182003,5,182003,39,0,0,4,1],[182020,5,182020,45,0,0,4,1],[182030,5,182030,46,0,0,4,1],[182040,5,182040,39,0,0,4,1],[182051,5,182051,37,0,0,4,1],[182062,5,182062,32,0,0,4,1],[182083,5,182083,34,0,0,4,1],[182126,5,182126,40,0,0,4,1],[182141,5,182141,36,0,0,4,1],[182158,5,182158,36,0,0,4,1],[182179,5,182179,32,0,0,4,1],[182199,5,182199,39,0,0,4,1],[182225,5,182225,30,0,0,4,1]],"getAndInitPage":[[72121,7,72121,31,0,0,4,0]],"pager_playback_one_page":[[58837,16,58837,30,0,0,4,1],[58837,34,58837,66,0,0,4,1],[58842,8,58842,42,0,0,4,1],[58842,46,58842,72,0,0,4,1],[57236,21,57236,41,10,0,4,0]],"sqlite3_db_config":[[178537,7,178537,32,0,0,4,1],[178542,5,178542,36,0,0,4,1],[178549,5,178549,35,0,0,4,1],[178556,5,178556,12,0,0,4,0],[178583,16,178583,27,0,0,4,0],[178584,13,178584,30,0,0,4,0],[178588,15,178588,22,0,0,4,0],[178593,15,178593,34,0,0,4,0],[178596,15,178596,19,0,0,4,0]],"schemaIsValid":[[17310,34,17310,79,7,0,4,1]],"connectionIsBusy":[[178814,7,178814,16,0,0,4,0]],"moveToLeftmost":[[75307,10,75307,23,0,0,4,1]],"yy_reduce":[[174206,7,174206,14,0,0,4,0],[174246,7,174246,14,0,0,4,0],[14098,27,14098,28,260,0,4,0],[14098,27,14098,28,261,0,4,1]],"sqlite3PagerSharedLock":[[61689,9,61689,22,0,0,4,0],[61768,11,61768,24,0,0,4,1]],"sqlite3_wal_checkpoint_v2":[[180092,7,180092,12,0,0,4,0],[180093,7,180093,13,0,0,4,0]],"sqlite3WalCheckpoint":[[68505,13,68505,18,0,0,4,0],[68507,13,68507,19,0,0,4,0]],"syncJournal":[[60742,13,60742,26,0,0,4,0],[60742,30,60742,65,0,0,4,1],[60746,13,60746,26,0,0,4,1]],"sqlite3Close":[[178854,23,178854,43,0,0,4,0]],"sqlite3InitOne":[[140519,11,140519,22,0,0,4,0]],"hasHotJournal":[[61601,15,61601,28,0,0,4,1],[61611,21,61611,40,0,0,4,1]],"vdbeCommit":[[86946,14,86946,27,0,0,4,1],[86952,9,86952,22,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_result_int","sqlite3_result_int","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_clear_bindings","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_result_int","sqlite3_result_int","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_clear_bindings","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_finalize","sqlite3_wal_checkpoint_v2","sqlite3_db_config","sqlite3_test_control","sqlite3_errcode","sqlite3_finalize","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create a temporary file to use as SQLite database
    const char* db_filename = "input_file";
    FILE* db_file = fopen(db_filename, "wb");
    if (!db_file) {
	assert_file_closed(&db_file);
	assert_file_name_closed("input_file");
	return 0;
	}
	
    fwrite(data, 1, size, db_file);
    assert_file_closed(&db_file);;

    sqlite3* db = nullptr;
    char* errmsg = nullptr;
    sqlite3_stmt* stmt = nullptr;
    int rc = 0;

    // 1. Open database
    rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        remove(db_filename);
        assert_file_closed(&db_file);
	assert_file_name_closed("input_file");
	return 0;
    }

    // 2. Set rollback hook
    sqlite3_rollback_hook(db, [](void*){}, nullptr);

    // 3. Create a custom function
    rc = sqlite3_create_function(
        db, "custom_func", 2, SQLITE_UTF8, nullptr,
        [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
            sqlite3_result_int(ctx, 42);
        },
        nullptr, nullptr
    );

    // 4. Execute SQL statements using sqlite3_exec
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT OR IGNORE INTO test(data) VALUES(?1);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }

    // 5. Prepare a statement
    const char* insert_sql = "INSERT INTO test(data) VALUES(?1)";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 6. Get bind parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // 7. Bind blob data if we have parameters
        if (param_count > 0 && size > 0) {
            // Use first part of input as blob
            size_t blob_size = size > 1024 ? 1024 : size;
            rc = sqlite3_bind_blob(stmt, 1, data, static_cast<int>(blob_size), SQLITE_STATIC);
            
            // 8. Execute the statement
            rc = sqlite3_step(stmt);
            
            // 9. Reset statement for potential reuse
            sqlite3_reset(stmt);
            
            // 10. Clear bindings
            sqlite3_clear_bindings(stmt);
        }
    }

    // 11. Prepare and execute a SELECT statement to retrieve data
    const char* select_sql = "SELECT * FROM test";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // 12. Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // 13. Get column count
            int col_count = sqlite3_column_count(stmt);
            
            for (int i = 0; i < col_count; i++) {
                // 14. Get column type
                int col_type = sqlite3_column_type(stmt, i);
                
                if (col_type == SQLITE_BLOB) {
                    // 15. Get blob data
                    const void* blob = sqlite3_column_blob(stmt, i);
                    
                    // 16. Get blob size
                    int blob_bytes = sqlite3_column_bytes(stmt, i);
                    
                    // Write blob to output file
                    FILE* out_file = fopen("output_file", "wb");
                    if (out_file && blob && blob_bytes > 0) {
                        fwrite(blob, 1, blob_bytes, out_file);
                        assert_file_closed(&out_file);;
                    }
                    if (out_file) assert_file_closed(&out_file);;
                }
            }
        }
        
        // 17. Finalize statement
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // 18. Perform WAL checkpoint
    int pnLog = 0, pnCkpt = 0;
    sqlite3_wal_checkpoint_v2(db, "main", SQLITE_CHECKPOINT_PASSIVE, &pnLog, &pnCkpt);

    // 19. Configure database
    rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, &rc);

    // 20. Use test control API
    sqlite3_test_control(SQLITE_TESTCTRL_OPTIMIZATIONS, db, 0xFF);

    // 21. Get error code
    int errcode = sqlite3_errcode(db);

    // Cleanup
    if (stmt) sqlite3_finalize(stmt);
    
    // 22. Close database
    sqlite3_close(db);
    
    // Remove temporary files
    remove(db_filename);
    remove("output_file");

    assert_file_closed(&db_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}