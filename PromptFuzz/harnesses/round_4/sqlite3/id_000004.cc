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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 25.5, nr_unique_branch: 2
//<Quality> {"density":17,"unique_branches":{"sqlite3_profile":[[179855,7,179855,19,0,0,4,1]],"sqlite3_bind_text64":[[91257,7,91257,23,0,0,4,1]]},"library_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_close","sqlite3_status","sqlite3_status","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_text64","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_create_function_v2","sqlite3_exec","sqlite3_free","sqlite3_profile","sqlite3_close_v2"],"critical_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_status","sqlite3_status","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_text64","sqlite3_bind_blob","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_create_function_v2","sqlite3_exec","sqlite3_free","sqlite3_profile","sqlite3_close_v2"],"visited":1}
/**/


// Profile callback function
static void  id_000004_profile_callback(void *user_data, const char *sql, sqlite3_uint64 elapsed_time) {
    // Do nothing in fuzzing - just needed for API call
    (void)user_data;
    (void)sql;
    (void)elapsed_time;
}

// Custom function that uses sqlite3_user_data
static void  id_000004_custom_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    void *user_data = sqlite3_user_data(context);
    if (user_data) {
        sqlite3_result_int(context, 42);
    } else {
        sqlite3_result_null(context);
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* f_data, size_t f_size) {
	if(f_size<239) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_2, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_3, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_6, fuzz_str_sz_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_10, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_11, fuzz_str_sz_11, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_12, fuzz_str_sz_12, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_13, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_14, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_15, fuzz_str_sz_15, fdp);
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_shutdown() != SQLITE_OK) {
        // This is expected - shutdown returns SQLITE_OK
    }
    
    // Create a temporary database file
    const char *db_filename = "fuzz_test.db";
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(fuzz_str_1, &db);
    
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Use sqlite3_status to get memory usage statistics
    int current = 0, highwater = 0;
    sqlite3_status(fuzz_int32_t_2, &current, &highwater, fuzz_int32_t_3);
    sqlite3_status(fuzz_int32_t_4, &current, &highwater, fuzz_int32_t_5); // Reset flag = 1
    
    // Set up profile callback
    void *profile_data = (void*)0x1234; // Arbitrary user data
    sqlite3_profile(db,  id_000004_profile_callback, profile_data);
    
    // Create a simple table
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value BLOB);";
    
    char *err_msg = nullptr;
    rc = sqlite3_exec(db, fuzz_str_6, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
        err_msg = nullptr;
    }
    
    // Prepare a statement for inserting data
    sqlite3_stmt *stmt = nullptr;
    const char *insert_sql = "INSERT INTO test_table (name, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v3(db, fuzz_str_7, -1, fuzz_uint32_t_8, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Use sqlite3_bind_text64 to bind data from input
        if (size > 0) {
            // Convert input data to a null-terminated string for binding
            char *text_data = (char *)malloc(size + 1);
            if (text_data) {
                memcpy(text_data, data, size);
                text_data[size] = '\0';
                
                // Bind the text data using sqlite3_bind_text64
                rc = sqlite3_bind_text64(stmt, 1, text_data, size, 
                                        [](void *p) { free(p); }, 
                                        SQLITE_UTF8);
                
                // If binding failed, free the memory manually
                if (rc != SQLITE_OK) {
                    free(text_data);
                }
                
                // Bind a small blob from input data
                if (size > 10) {
                    rc = sqlite3_bind_blob(stmt, fuzz_int32_t_9, data, 
                                          (size > 100) ? 100 : (int)size, 
                                          SQLITE_TRANSIENT);
                } else {
                    rc = sqlite3_bind_null(stmt, fuzz_int32_t_10);
                }
                
                // Execute the statement
                if (rc == SQLITE_OK) {
                    sqlite3_step(stmt);
                }
            }
        }
        
        // Reset and finalize the statement
        sqlite3_reset(stmt);
        sqlite3_finalize(stmt);
    }
    
    // Check transaction state using sqlite3_txn_state
    int txn_state = sqlite3_txn_state(db, fuzz_str_11);
    (void)txn_state; // Use the value to avoid unused variable warning
    
    // Create a custom function that uses sqlite3_user_data
    void *user_data = (void*)0x5678;
    rc = sqlite3_create_function_v2(
        db, 
        fuzz_str_12, 
        fuzz_int32_t_13,  // Number of arguments
        fuzz_int32_t_14, 
        user_data, 
         id_000004_custom_function, 
        nullptr, 
        nullptr, 
        nullptr
    );
    
    // Use the custom function in a query
    if (rc == SQLITE_OK) {
        const char *select_sql = "SELECT custom_func();";
        rc = sqlite3_exec(db, fuzz_str_15, nullptr, nullptr, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
    
    // Clean up
    // Remove profile callback
    sqlite3_profile(db, nullptr, nullptr);
    
    // Close database
    sqlite3_close_v2(db);
    
    // Remove temporary database file
    remove(db_filename);
    
    return 0;
}