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
//<ID> 48
//<Prompt> []
/*<Combination>: [
*/
//<score> 361, nr_unique_branch: 71
//<Quality> {"density":19,"unique_branches":{"sqlite3ExprDeleteNN":[[109312,38,109312,54,0,0,4,1],[109338,7,109338,23,0,0,4,1]],"sqlite3VdbeExec":[[99219,9,99219,25,0,0,4,1],[101225,7,101225,25,0,0,4,0],[101225,29,101225,46,0,0,4,1]],"sqlite3AffinityType":[[121803,13,121803,36,0,0,4,1],[121803,40,121803,60,0,0,4,1]],"sqlite3AddDefaultValue":[[121873,7,121873,11,0,0,4,0],[121874,18,121874,31,0,0,4,1],[121876,9,121876,56,0,0,4,1],[121880,15,121880,49,0,0,4,1],[19761,28,19761,67,7,0,4,1]],"sqlite3Pragma":[[138955,13,138955,21,0,0,4,0]],"yy_reduce":[[174024,7,174024,14,0,0,4,0],[174051,7,174051,14,0,0,4,0],[174103,7,174103,14,0,0,4,0],[174109,7,174109,14,0,0,4,0],[174128,7,174128,14,0,0,4,0],[174131,7,174131,8,0,0,4,0],[174144,7,174144,14,0,0,4,0],[174154,7,174154,14,0,0,4,0],[174157,7,174157,14,0,0,4,0],[174205,7,174205,14,0,0,4,0],[174395,7,174395,15,0,0,4,0],[14098,27,14098,28,25,0,4,1],[14098,27,14098,28,252,0,4,0],[14098,27,14098,28,259,0,4,1],[14098,27,14098,28,260,0,4,1],[14098,27,14098,28,261,0,4,1],[14098,27,14098,28,276,0,4,0],[14098,27,14098,28,337,0,4,0]],"callCollNeeded":[[125861,7,125861,22,0,0,4,1]],"keywordCode":[[14098,27,14098,28,34,0,4,0]],"dupedExprNodeSize":[[109460,7,109460,23,0,0,4,0],[109460,43,109460,54,0,0,4,0]],"sqlite3GetInt32":[[35347,7,35347,19,0,0,4,0],[35350,13,35350,25,0,0,4,0],[35391,7,35391,10,0,0,4,0]],"dupedExprStructSize":[[109437,7,109437,15,0,0,4,1],[109443,9,109443,17,0,0,4,0],[109443,9,109443,17,0,0,4,1],[109443,21,109443,31,0,0,4,1],[18919,34,18919,55,1,0,4,1]],"sqlite3LocateCollSeq":[[126099,21,126099,27,0,0,4,0]],"dupedExprSize":[[109481,7,109481,15,0,0,4,0],[109481,7,109481,15,0,0,4,1],[109482,7,109482,16,0,0,4,1]],"sqlite3AddCollateType":[[122080,7,122080,33,0,0,4,1],[122084,7,122084,13,0,0,4,1],[122086,7,122086,42,0,0,4,1],[19761,28,19761,67,1,0,4,1]],"exprDup":[[109526,7,109526,15,0,0,4,0],[109536,9,109536,17,0,0,4,0],[109564,9,109564,17,0,0,4,0],[109565,11,109565,27,0,0,4,0],[109565,47,109565,58,0,0,4,0],[109571,9,109571,17,0,0,4,0],[109591,9,109591,17,0,0,4,0],[109622,11,109622,19,0,0,4,0],[109623,13,109623,36,0,0,4,1],[109629,25,109629,33,0,0,4,0],[109632,24,109632,33,0,0,4,1],[109647,7,109647,15,0,0,4,0]],"sqlite3DeferForeignKey":[[123868,7,123868,36,0,0,4,1],[123870,7,123870,37,0,0,4,0],[14156,25,14156,46,1,0,4,1],[14156,26,14156,29,1,0,4,1]],"sqlite3ColumnSetExpr":[[120842,7,120842,21,0,0,4,0],[120846,19,120846,27,0,0,4,0]],"sqlite3GetCollSeq":[[126061,7,126061,8,0,0,4,1],[126065,7,126065,11,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_sourceid","sqlite3_open","sqlite3_shutdown","sqlite3_exec","sqlite3_prepare_v2","sqlite3_close","sqlite3_shutdown","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_mutex_leave","sqlite3_commit_hook","sqlite3_step","sqlite3_column_decltype16","sqlite3_bind_parameter_name","sqlite3_str_new","sqlite3_str_append","sqlite3_str_errcode","sqlite3_str_finish","sqlite3_last_insert_rowid","sqlite3_reset","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_sourceid","sqlite3_open","sqlite3_exec","sqlite3_prepare_v2","sqlite3_db_mutex","sqlite3_mutex_try","sqlite3_mutex_leave","sqlite3_commit_hook","sqlite3_step","sqlite3_column_decltype16","sqlite3_bind_parameter_name","sqlite3_str_new","sqlite3_str_append","sqlite3_str_errcode","sqlite3_str_finish","sqlite3_last_insert_rowid","sqlite3_reset","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<8) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeIntegral(int32_t, fuzz_int32_t_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Get SQLite source ID
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Use variable to avoid unused warning

    // Create in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }

    // Create table using input data as table name
    if (size > 0) {
        // Ensure null termination for table name
        char *table_name = new char[size + 1];
        memcpy(table_name, data, size);
        table_name[size] = '\0';
        
        char create_table_sql[256];
        snprintf(create_table_sql, sizeof(create_table_sql),
                "CREATE TABLE IF NOT EXISTS %s (id INTEGER PRIMARY KEY, name TEXT)", table_name);
        
        sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
        delete[] table_name;
    }

    // Prepare a statement
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test VALUES (?, ?)";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }

    // Try to get mutex
    sqlite3_mutex *mutex = sqlite3_db_mutex(db);
    if (mutex) {
        int mutex_result = sqlite3_mutex_try(mutex);
        if (mutex_result == SQLITE_OK) {
            sqlite3_mutex_leave(mutex);
        }
    }

    // Set commit hook
    static int commit_counter = 0;
    auto commit_hook_callback = [](void *data) -> int {
        int *counter = static_cast<int*>(data);
        (*counter)++;
        return 0;
    };
    void *old_commit_hook = sqlite3_commit_hook(db, commit_hook_callback, &commit_counter);

    // Get column decltype16 (need to execute statement first)
    sqlite3_step(stmt);
    const void *decltype16 = sqlite3_column_decltype16(stmt, fuzz_int32_t_1);
    (void)decltype16; // Use variable

    // Get bind parameter name
    const char *param_name = sqlite3_bind_parameter_name(stmt, fuzz_int32_t_2);
    (void)param_name; // Use variable

    // Create and use sqlite3_str
    sqlite3_str *str = sqlite3_str_new(db);
    if (str) {
        if (size > 0) {
            sqlite3_str_append(str, reinterpret_cast<const char*>(data), 
                              static_cast<int>(size < 100 ? size : 100));
        }
        int str_err = sqlite3_str_errcode(str);
        (void)str_err; // Use variable
        sqlite3_str_finish(str);
    }

    // Get last insert rowid
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    (void)last_rowid; // Use variable

    // Reset statement
    sqlite3_reset(stmt);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    sqlite3_shutdown();

    return 0;
}