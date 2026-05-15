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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 102, nr_unique_branch: 3
//<Quality> {"density":34,"unique_branches":{"sqlite3ValueBytes":[[83963,9,83963,28,0,0,4,0]],"numericType":[[14098,27,14098,28,12,0,4,1],[14098,27,14098,28,13,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_close","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_finalize","sqlite3_close","sqlite3_bind_zeroblob","sqlite3_finalize","sqlite3_close","sqlite3_bind_blob","sqlite3_bind_zeroblob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_close","sqlite3_reset","sqlite3_errcode","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_close","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_exec","sqlite3_free","sqlite3_finalize","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_blob","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_errcode","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_column_count","sqlite3_column_type","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_exec","sqlite3_free","sqlite3_finalize","sqlite3_close"],"visited":0}
/**/


// Custom function that uses various sqlite3_value_* and sqlite3_result_* APIs
static void  id_000031_custom_function(
    sqlite3_context* context,
    int argc,
    sqlite3_value** argv
) {
    if (argc == 0) {
        sqlite3_result_null(context);
        return;
    }

    int value_type = sqlite3_value_type(argv[0]);
    
    switch (value_type) {
        case SQLITE_INTEGER: {
            int int_value = sqlite3_value_int(argv[0]);
            sqlite3_result_int(context, int_value * 2); // Return double the value
            break;
        }
        case SQLITE_BLOB: {
            int blob_size = sqlite3_value_bytes16(argv[0]);
            sqlite3_result_zeroblob(context, blob_size);
            break;
        }
        case SQLITE_NULL: {
            sqlite3_result_null(context);
            break;
        }
        default: {
            // For other types, return a zeroblob of size 10
            sqlite3_result_zeroblob(context, 10);
            break;
        }
    }
}

// Rollback hook function
static void  id_000031_rollback_hook(void* data) {
    // This function is called when a rollback occurs
    // We don't need to do anything specific for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Limit size to avoid excessive memory usage
    if (size > 1024 * 1024) {
        return 0;
    }

    sqlite3* db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    char* err_msg = nullptr;
    int rc;
    
    // 1. Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    // 2. Set rollback hook
    sqlite3_rollback_hook(db,  id_000031_rollback_hook, nullptr);

    // 3. Create custom function
    rc = sqlite3_create_function(
        db,
        "myfunc",
        1,  // Number of arguments
        SQLITE_UTF8,
        nullptr,
         id_000031_custom_function,
        nullptr,
        nullptr
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 4. Execute SQL to create table
    const char* create_table_sql = 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB, value INTEGER);";
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // 5. Prepare statement for insertion
    const char* insert_sql = 
        "INSERT INTO test (data, value) VALUES (?, myfunc(?));";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 6. Check parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    
    // 7. Bind parameters
    if (size > 0 && param_count >= 1) {
        // Bind blob from input data
        rc = sqlite3_bind_blob(stmt, 1, data, 
                              (size > 1000) ? 1000 : (int)size, 
                              SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    } else {
        // Bind zeroblob if no data
        rc = sqlite3_bind_zeroblob(stmt, 1, 10);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    if (param_count >= 2) {
        // Bind integer value derived from input size
        int int_value = (size > 0 && data) ? (int)data[0] : 0;
        // Use the blob binding API for the second parameter too
        if (size > 1) {
            sqlite3_bind_blob(stmt, 2, data + 1, 
                             (size > 100) ? 100 : (int)size - 1, 
                             SQLITE_STATIC);
        } else {
            sqlite3_bind_zeroblob(stmt, 2, 5);
        }
    }

    // 8. Execute the statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    // 9. Reset statement for potential reuse
    sqlite3_reset(stmt);
    
    // 10. Check error code
    int errcode = sqlite3_errcode(db);

    // 11. Prepare statement for querying
    sqlite3_finalize(stmt);
    const char* select_sql = "SELECT * FROM test;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // 12. Execute query and process results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        
        for (int i = 0; i < col_count; i++) {
            int col_type = sqlite3_column_type(stmt, i);
            
            switch (col_type) {
                case SQLITE_INTEGER: {
                    int int_val = sqlite3_column_int(stmt, i);
                    // Do something with the integer value
                    break;
                }
                case SQLITE_BLOB: {
                    const void* blob_data = sqlite3_column_blob(stmt, i);
                    int blob_size = sqlite3_column_bytes(stmt, i);
                    // Do something with blob data
                    break;
                }
            }
        }
    }

    // 13. Execute another SQL command using sqlite3_exec
    const char* update_sql = 
        "UPDATE test SET value = value + 1 WHERE id = 1;";
    rc = sqlite3_exec(db, update_sql, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }

    // 14. Clean up
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    
    // Final close
    sqlite3_close(db);

    return 0;
}