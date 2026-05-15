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
//<ID> 494
//<Prompt> []
/*<Combination>: [
*/
//<score> 48, nr_unique_branch: 3
//<Quality> {"density":16,"unique_branches":{"sqlite3_result_error_code":[[90136,19,90136,26,0,0,4,1],[90140,7,90140,35,0,0,4,0]],"accessPayload":[[74898,11,74898,32,0,0,4,0]]},"library_calls":["sqlite3_initialize","sqlite3_shutdown","sqlite3_open","sqlite3_shutdown","sqlite3_create_function","sqlite3_close","sqlite3_shutdown","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_create_function","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_close","sqlite3_shutdown"],"visited":0}
/**/


// Custom function implementation
static void  id_000067_custom_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        unsigned int subtype = sqlite3_value_subtype(argv[0]);
        sqlite3_result_error_code(context, (int)subtype);
    } else {
        sqlite3_result_error_code(context, 1);
    }
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize sqlite3
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Create temporary file for database
    const char *db_filename = "fuzz.db";
    FILE *db_file = fopen(db_filename, "wb");
    if (!db_file) {
        sqlite3_shutdown();
        assert_file_closed(&db_file);
	return 0;
    }
    
    // Write input data to file if there's any
    if (size > 0 && data) {
        fwrite(data, 1, size, db_file);
    }
    assert_file_closed(&db_file);;

    // Open database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(db_filename, &db);
    if (rc != SQLITE_OK || !db) {
        remove(db_filename);
        sqlite3_shutdown();
        assert_file_closed(&db_file);
	return 0;
    }

    // Set client data (if available, using data pointer as client data)
    if (size > 0) {
        // Allocate memory for client data
        void *client_data = malloc(size);
        if (client_data) {
            memcpy(client_data, data, size);
            // Note: sqlite3_set_clientdata doesn't exist in standard SQLite
            // We'll use sqlite3_create_function_v2 with xDestroy instead
        }
        // We'll demonstrate client data concept via create_function
    }

    // Create custom function
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, nullptr,
                                 id_000067_custom_function, nullptr, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        remove(db_filename);
        sqlite3_shutdown();
        assert_file_closed(&db_file);
	return 0;
    }

    // Execute SQL statements
    char *errmsg = nullptr;
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS fuzz_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }

    // Prepare statement for insertion
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO fuzz_table (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind integer parameter (use first 4 bytes if available)
        int bind_value = 0;
        if (size >= sizeof(int)) {
            memcpy(&bind_value, data, sizeof(int));
        }
        sqlite3_bind_int(stmt, 1, bind_value);
        
        // Execute statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
            // Handle error
        }
        
        // Finalize statement
        sqlite3_finalize(stmt);
    }

    // Execute a query using the custom function
    const char *select_sql = "SELECT fuzz_func(data) FROM fuzz_table;";
    rc = sqlite3_exec(db, select_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }

    // Prepare and execute another statement
    const char *select_sql2 = "SELECT * FROM fuzz_table WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, select_sql2, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Bind integer parameter
        sqlite3_bind_int(stmt, 1, 1);
        
        // Step through results
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process row if needed
        }
        
        sqlite3_finalize(stmt);
    }

    // Clean up
    sqlite3_close(db);
    remove(db_filename);
    sqlite3_shutdown();

    assert_file_closed(&db_file);
	return 0;
}