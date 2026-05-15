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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 76, nr_unique_branch: 16
//<Quality> {"density":19,"unique_branches":{"sqlite3MemCompare":[[88525,11,88525,34,0,0,4,1]],"sqlite3_commit_hook":[[179875,7,179875,32,0,0,4,1]],"sqlite3_enable_load_extension":[[136425,7,136425,32,0,0,4,1],[136428,7,136428,12,0,0,4,1]],"sqlite3_db_cacheflush":[[178509,7,178509,32,0,0,4,1],[178513,12,178513,25,0,0,4,0],[178513,29,178513,38,0,0,4,0],[178513,29,178513,38,0,0,4,1],[178515,9,178515,12,0,0,4,0],[178515,9,178515,12,0,0,4,1],[178515,16,178515,59,0,0,4,1],[178526,12,178526,25,0,0,4,0],[178526,29,178526,38,0,0,4,1]],"vdbeSafetyNotNull":[[89520,7,89520,11,0,0,4,0]],"sqlite3Close":[[178854,7,178854,19,0,0,4,1]],"sqlite3_step":[[90365,7,90365,27,0,0,4,0]]},"library_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_commit_hook","sqlite3_str_new","sqlite3_close_v2","sqlite3_str_appendchar","sqlite3_str_finish","sqlite3_close_v2","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_blob","sqlite3_column_text16","sqlite3_finalize","sqlite3_memory_alarm","sqlite3_db_cacheflush","sqlite3_free","sqlite3_close_v2"],"critical_calls":["sqlite3_open_v2","sqlite3_enable_load_extension","sqlite3_commit_hook","sqlite3_str_new","sqlite3_str_appendchar","sqlite3_str_finish","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v3","sqlite3_step","sqlite3_column_blob","sqlite3_column_text16","sqlite3_finalize","sqlite3_memory_alarm","sqlite3_db_cacheflush","sqlite3_free","sqlite3_close_v2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Open a memory database for fuzzing
    sqlite3 *db = nullptr;
    if (sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr) != SQLITE_OK) {
        return 0;
    }

    // Enable load extension (even if unused later)
    sqlite3_enable_load_extension(db, 0);

    // Set commit hook (empty callback)
    sqlite3_commit_hook(db, nullptr, nullptr);

    // Create a string builder for constructing SQL
    sqlite3_str *str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close_v2(db);
        return 0;
    }

    // Append characters from input data to the string builder
    size_t chars_to_append = (size > 100) ? 100 : size; // Limit to avoid excessive growth
    for (size_t i = 0; i < chars_to_append; i++) {
        sqlite3_str_appendchar(str, 1, static_cast<char>(data[i]));
    }
    char *sql = sqlite3_str_finish(str);
    if (!sql) {
        sqlite3_close_v2(db);
        return 0;
    }

    // Execute SQL to create a table and insert data
    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v3(db, sql, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_step(stmt); // Ignore result
        sqlite3_finalize(stmt);
    }

    // Create another statement to query data
    const char *query = "SELECT * FROM t";
    if (sqlite3_prepare_v3(db, query, -1, 0, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Access blob column (if exists)
            const void *blob = sqlite3_column_blob(stmt, 0);
            // Access text column as UTF-16LE (if exists)
            const void *text16le = sqlite3_column_text16(stmt, 0); // Note: text16le is not directly available, using column_text16
        }
        sqlite3_finalize(stmt);
    }

    // Set memory alarm (callback does nothing)
    sqlite3_int64 threshold = 1000000; // 1MB threshold
    sqlite3_memory_alarm(nullptr, nullptr, threshold);

    // Flush database cache
    sqlite3_db_cacheflush(db);

    // Note: sqlite3_context_db_handle requires a context, which we don't have
    // sqlite3_value_text16le requires a value, which we don't have
    // sqlite3_free_table requires a table result from sqlite3_get_table, which we didn't use

    // Cleanup
    sqlite3_free(sql);
    sqlite3_close_v2(db);
    return 0;
}