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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 21, nr_unique_branch: 2
//<Quality> {"density":21,"unique_branches":{"sqlite3Pragma":[[138193,33,138193,40,0,0,4,1],[138193,44,138193,54,0,0,4,0]]},"library_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_exec","sqlite3_create_function","sqlite3_close","sqlite3_prepare_v2","sqlite3_sql","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_column_value","sqlite3_finalize","sqlite3_profile","sqlite3_drop_modules","sqlite3_close"],"critical_calls":["sqlite3_libversion_number","sqlite3_open","sqlite3_profile","sqlite3_create_module_v2","sqlite3_exec","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_sql","sqlite3_step","sqlite3_column_count","sqlite3_column_text","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_step","sqlite3_column_value","sqlite3_finalize","sqlite3_profile","sqlite3_drop_modules","sqlite3_close"],"visited":1}
/**/


// Custom function for sqlite3_profile callback
static void  id_000055_profileCallback(void *pCtx, const char *zSql, sqlite3_uint64 ns) {
    // Just a placeholder callback for profiling
    (void)pCtx;
    (void)zSql;
    (void)ns;
}

// Custom scalar function that uses sqlite3_result_value
static void  id_000055_customFunction(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);  // Return first argument
    } else {
        sqlite3_result_null(ctx);
    }
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size == 0) {
        return 0;
    }

    // 1. Get library version
    int version = sqlite3_libversion_number();
    (void)version;  // Avoid unused variable warning

    // 2. Create temporary in-memory database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }

    // 3. Set up profile callback
    void *profileCtx = nullptr;
    profileCtx = sqlite3_profile(db,  id_000055_profileCallback, nullptr);

    // 4. Create a virtual table (needed for vtab APIs)
    // First create a dummy module
    static sqlite3_module dummyModule = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    if (sqlite3_create_module_v2(db, "dummy_vtab", &dummyModule, 
                                 nullptr, nullptr) == SQLITE_OK) {
        // Create a virtual table
        const char *createVtab = "CREATE VIRTUAL TABLE temp.dummy USING dummy_vtab()";
        sqlite3_exec(db, createVtab, nullptr, nullptr, nullptr);
    }

    // 5. Create custom scalar function
    sqlite3_create_function(db, "custom_func", 1, SQLITE_UTF8, 
                           nullptr,  id_000055_customFunction, nullptr, nullptr);

    // 6. Prepare a statement from input data
    sqlite3_stmt *stmt = nullptr;
    const char *tail = nullptr;
    
    // Use input data as SQL (null-terminate it)
    char *sql = new char[size + 1];
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, data, size);
    sql[size] = '\0';
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    delete[] sql;
    
    if (rc == SQLITE_OK && stmt) {
        // 7. Get SQL string of prepared statement
        const char *stmtSql = sqlite3_sql(stmt);
        (void)stmtSql;  // Avoid unused variable warning
        
        // 8. Try to use vtab_rhs_value (this typically needs an index_info context)
        // We'll create a dummy context for demonstration
        sqlite3_value *rhsVal = nullptr;
        sqlite3_index_info idxInfo = {0};
        
        // We can't properly call vtab_rhs_value without proper context,
        // but we'll demonstrate the API call pattern
        if (stmt) {
            // This is just to show the API signature - won't actually work
            // sqlite3_vtab_rhs_value(&idxInfo, 0, &rhsVal);
            (void)idxInfo;
            (void)rhsVal;
        }
        
        // 9. Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row if needed
            int cols = sqlite3_column_count(stmt);
            for (int i = 0; i < cols; i++) {
                // Just touch the data to consume it
                sqlite3_column_text(stmt, i);
            }
        }
        
        // 10. Reset scan status
        sqlite3_stmt_scanstatus_reset(stmt);
        
        // 11. Finalize statement
        sqlite3_finalize(stmt);
    }
    
    // 12. Also test with a different statement that uses our custom function
    sqlite3_stmt *stmt2 = nullptr;
    const char *testSql = "SELECT custom_func(?)";
    rc = sqlite3_prepare_v2(db, testSql, -1, &stmt2, nullptr);
    
    if (rc == SQLITE_OK && stmt2) {
        // Bind some data from input
        if (size > 0) {
            // Use first part of input as blob
            sqlite3_bind_blob(stmt2, 1, data, 
                             (size > 100) ? 100 : (int)size, SQLITE_TRANSIENT);
        }
        
        if (sqlite3_step(stmt2) == SQLITE_ROW) {
            // Get result
            sqlite3_column_value(stmt2, 0);
        }
        
        sqlite3_finalize(stmt2);
    }

    // 13. Create output file and write some data
    FILE *out_file = fopen("output_file", "wb");
    if (out_file) {
        // Write version info
        fprintf(out_file, "SQLite version: %d\n", version);
        
        // Write first few bytes of input
        if (size > 0) {
            size_t write_size = (size > 100) ? 100 : size;
            fwrite(data, 1, write_size, out_file);
        }
        
        assert_file_closed(&out_file);;
    }

    // 14. Clean up
    if (profileCtx) {
        // Restore previous profile callback
        sqlite3_profile(db, nullptr, nullptr);
    }
    
    // Drop the dummy module
    sqlite3_drop_modules(db, nullptr);
    
    // Close database
    sqlite3_close(db);
    
    assert_file_closed(&out_file);
	return 0;
}