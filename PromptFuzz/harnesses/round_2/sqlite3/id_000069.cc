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
//<ID> 497
//<Prompt> []
/*<Combination>: [
*/
//<score> 42, nr_unique_branch: 13
//<Quality> {"density":14,"unique_branches":{"sqlite3_get_clientdata":[[181345,22,181345,23,0,0,4,0],[181346,9,181346,35,0,0,4,0]],"sqlite3Close":[[178868,10,178868,21,0,0,4,0],[178872,9,178872,23,0,0,4,0]],"codeInteger":[[111894,42,111894,49,0,0,4,0],[111894,53,111894,74,0,0,4,1],[111909,11,111909,18,0,0,4,0],[111909,30,111909,34,0,0,4,1]],"sqlite3_set_clientdata":[[181368,22,181368,23,0,0,4,1],[181371,7,181371,8,0,0,4,1],[181380,13,181380,21,0,0,4,1],[181386,9,181386,13,0,0,4,1]],"sqlite3GetInt32":[[14098,27,14098,28,6,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_set_clientdata","sqlite3_get_clientdata","sqlite3_context_db_handle","sqlite3_value_subtype","sqlite3_result_error_code","sqlite3_result_int","sqlite3_result_null","sqlite3_get_clientdata","sqlite3_context_db_handle","sqlite3_value_subtype","sqlite3_result_error_code","sqlite3_result_int","sqlite3_result_null","sqlite3_create_function","sqlite3_close","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_set_clientdata","sqlite3_result_null","sqlite3_result_int","sqlite3_result_error_code","sqlite3_value_subtype","sqlite3_get_clientdata","sqlite3_context_db_handle","sqlite3_result_null","sqlite3_result_int","sqlite3_result_error_code","sqlite3_value_subtype","sqlite3_get_clientdata","sqlite3_context_db_handle","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_close"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early return for insufficient data
    if (size < sizeof(int)) {
	return 0;
	}
	

    // Step 1: Open a database
    sqlite3 *db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }

    // Step 2: Create a custom function that uses client data
    struct ClientData {
        int value;
        int calls;
    };
    ClientData *client_data = new ClientData{0, 0};

    // Set client data (assuming sqlite3_set_clientdata exists - it's in our API list)
    // Note: This function might not exist in standard SQLite; we treat it as given
    sqlite3_set_clientdata(db, "fuzz_key", client_data, [](void *p) { delete static_cast<ClientData*>(p); });

    // Define the custom function implementation
    auto custom_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        // Get client data back
        ClientData *cd = static_cast<ClientData*>(sqlite3_get_clientdata(sqlite3_context_db_handle(ctx), "fuzz_key"));
        if (cd) cd->calls++;
        
        // Check subtype of first argument if present
        if (argc > 0) {
            unsigned int subtype = sqlite3_value_subtype(argv[0]);
            // Use subtype to decide error code (example logic)
            if (subtype % 2 == 0) {
                sqlite3_result_error_code(ctx, SQLITE_ERROR);
            } else {
                sqlite3_result_int(ctx, cd ? cd->value : -1);
            }
        } else {
            sqlite3_result_null(ctx);
        }
    };

    // Register the custom function
    if (sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, nullptr, 
                                custom_func, nullptr, nullptr) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    // Step 3: Execute SQL using the custom function
    char *errmsg = nullptr;
    const char *sql1 = "CREATE TABLE test (id INTEGER, value BLOB);";
    sqlite3_exec(db, sql1, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }

    // Step 4: Use the input data to bind parameters
    // Extract an integer from the beginning of data
    int bound_value = 0;
    if (size >= sizeof(int)) {
        memcpy(&bound_value, data, sizeof(int));
        bound_value = abs(bound_value); // Avoid large negatives
    }

    // Create a statement with the custom function
    sqlite3_stmt *stmt = nullptr;
    const char *sql2 = "SELECT fuzz_func(?);";
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt, nullptr) == SQLITE_OK) {
        // Bind the integer extracted from input
        sqlite3_bind_int(stmt, 1, bound_value % 1000); // Keep within reasonable range
        
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row if needed
        }
        
        sqlite3_finalize(stmt);
    }

    // Step 5: Insert some data using exec
    char insert_sql[256];
    snprintf(insert_sql, sizeof(insert_sql), 
             "INSERT INTO test VALUES (%d, x'%02x%02x%02x');", 
             bound_value, 
             size > 0 ? data[0] : 0,
             size > 1 ? data[1] : 0,
             size > 2 ? data[2] : 0);
    sqlite3_exec(db, insert_sql, nullptr, nullptr, &errmsg);
    if (errmsg) {
        sqlite3_free(errmsg);
        errmsg = nullptr;
    }

    // Step 6: Clean up
    // Note: sqlite3_set_clientdata's destructor will clean up client_data
    sqlite3_close(db);
    
    return 0;
}