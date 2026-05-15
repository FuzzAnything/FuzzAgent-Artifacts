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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 19, nr_unique_branch: 0
//<Quality> {"density":19,"unique_branches":{},"library_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_result_text","sqlite3_result_text","sqlite3_create_function","sqlite3_exec","sqlite3_prepare","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_column_value","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_exec","sqlite3_drop_modules","sqlite3_close","sqlite3_free","sqlite3_free"],"critical_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_result_text","sqlite3_result_text","sqlite3_create_function","sqlite3_exec","sqlite3_prepare","sqlite3_bind_blob","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_column_value","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_exec","sqlite3_drop_modules","sqlite3_close","sqlite3_free","sqlite3_free"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // 1. Get SQLite version
    int version = sqlite3_libversion_number();
    
    // 2. Create memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // 3. Setup profile callback
    sqlite3_profile(db, [](void *p, const char *sql, sqlite3_uint64 ns) {
        // Simple profile logging
    }, nullptr);
    
    // 4. Create dummy virtual table module
    sqlite3_module dummy_module = {0};
    const char *keep_modules[] = {nullptr};
    sqlite3_create_module_v2(db, "dummy_vtab", &dummy_module, nullptr, nullptr);
    
    // 5. Create custom function
    auto dummy_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        sqlite3_result_text(ctx, "dummy", 5, SQLITE_TRANSIENT);
    };
    sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, nullptr, 
                          dummy_func, nullptr, nullptr);
    
    // 6. Execute SQL from input data
    char *errmsg = nullptr;
    if (size > 0) {
        // Use input data as SQL query
        char *sql = (char*)malloc(size + 1);
        if (sql) {
            memcpy(sql, data, size);
            sql[size] = '\0';
            sqlite3_exec(db, sql, nullptr, nullptr, &errmsg);
            free(sql);
        }
    }
    
    // 7. Prepare and execute another statement
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    const char *select_sql = "SELECT dummy_func(?)";
    if (sqlite3_prepare(db, select_sql, -1, &stmt, &tail) == SQLITE_OK) {
        // Bind blob from input data
        if (size > 0) {
            size_t bind_size = size > 100 ? 100 : size; // Limit size
            sqlite3_bind_blob(stmt, 1, data, bind_size, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 1);
        }
        
        // Execute statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int col_count = sqlite3_column_count(stmt);
            for (int i = 0; i < col_count; i++) {
                const unsigned char *text = sqlite3_column_text(stmt, i);
                sqlite3_value *val = sqlite3_column_value(stmt, i);
                // Use values to avoid optimization
                if (text && val) {
                    // Do nothing, just reference them
                }
            }
        }
        
        // Reset scan status and finalize
        sqlite3_stmt_scanstatus_reset(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 8. Execute another SQL statement
    char *errmsg2 = nullptr;
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER);", 
                nullptr, nullptr, &errmsg2);
    
    // 9. Drop modules
    sqlite3_drop_modules(db, keep_modules);
    
    // 10. Close database
    sqlite3_close(db);
    
    // Free error messages
    if (errmsg) sqlite3_free(errmsg);
    if (errmsg2) sqlite3_free(errmsg2);
    
    return 0;
}