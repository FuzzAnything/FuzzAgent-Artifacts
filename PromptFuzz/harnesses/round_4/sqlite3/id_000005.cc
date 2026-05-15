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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 9, nr_unique_branch: 0
//<Quality> {"density":18,"unique_branches":{},"library_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_close","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_text64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_status","sqlite3_create_function_v2","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_profile","sqlite3_close"],"critical_calls":["sqlite3_shutdown","sqlite3_open","sqlite3_profile","sqlite3_exec","sqlite3_free","sqlite3_prepare_v3","sqlite3_bind_text64","sqlite3_bind_int","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_txn_state","sqlite3_status","sqlite3_create_function_v2","sqlite3_prepare_v3","sqlite3_step","sqlite3_finalize","sqlite3_profile","sqlite3_close"],"visited":1}
/**/


// Custom function to demonstrate sqlite3_user_data
static void  id_000005_custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Retrieve user data passed during function creation
    void* user_data = sqlite3_user_data(context);
    if (user_data) {
        const char* message = static_cast<const char*>(user_data);
        sqlite3_result_text(context, message, -1, SQLITE_TRANSIENT);
    } else {
        sqlite3_result_null(context);
    }
}

// Profile callback for sqlite3_profile
static void  id_000005_profile_callback(void* user_data, const char* sql, sqlite3_uint64 elapsed_time) {
    // This callback is called after each SQL statement execution
    // We don't need to do anything specific for fuzzing
    (void)user_data;
    (void)sql;
    (void)elapsed_time;
}

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t* f_data, size_t f_size) {
	if(f_size<240) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
		FDPConsumeRandomLengthString(char, fuzz_str_1, fuzz_str_sz_1, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_2, fuzz_str_sz_2, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_3, fuzz_str_sz_3, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_4, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_5, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_6, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_7, fuzz_str_sz_7, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_8, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_9, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_10, fuzz_str_sz_10, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_11, fdp);
		FDPConsumeIntegral(int32_t, fuzz_int32_t_12, fdp);
		FDPConsumeRandomLengthString(char, fuzz_str_13, fuzz_str_sz_13, fdp);
		FDPConsumeIntegral(uint32_t, fuzz_uint32_t_14, fdp);
	//fuzzer shim end}




    // Initialize SQLite
    if (sqlite3_shutdown() != SQLITE_OK) {
        // This might fail if SQLite wasn't initialized, but we continue anyway
    }
    
    // Create a temporary file for database operations
    const char* db_filename = "fuzz_db.sqlite";
    sqlite3* db = nullptr;
    int rc;
    
    // Open database
    rc = sqlite3_open(fuzz_str_1, &db);
    if (rc != SQLITE_OK || !db) {
        // If we can't open the database, clean up and return
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable profile hook
    void* profile_user_data = nullptr;
    sqlite3_profile(db,  id_000005_profile_callback, profile_user_data);
    
    // Create a table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_table ("
        "id INTEGER PRIMARY KEY, "
        "data TEXT, "
        "value INTEGER);";
    
    char* err_msg = nullptr;
    rc = sqlite3_exec(db, fuzz_str_2, nullptr, nullptr, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Prepare an INSERT statement
    sqlite3_stmt* stmt = nullptr;
    const char* insert_sql = "INSERT INTO fuzz_table (data, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v3(db, fuzz_str_3, -1, fuzz_uint32_t_4, &stmt, nullptr);
    
    if (rc == SQLITE_OK && stmt) {
        // Bind text data from input
        if (size > 0) {
            // Convert data to null-terminated string for binding
            char* text_data = static_cast<char*>(malloc(size + 1));
            if (text_data) {
                memcpy(text_data, data, size);
                text_data[size] = '\0';
                
                // Use sqlite3_bind_text64 to bind the text data
                rc = sqlite3_bind_text64(stmt, fuzz_int32_t_5, text_data, size, 
                                         [](void* ptr) { free(ptr); }, 
                                         SQLITE_UTF8);
                
                // Bind a random integer value
                int int_value = size > 0 ? data[0] : 0;
                sqlite3_bind_int(stmt, fuzz_int32_t_6, int_value);
                
                // Execute the statement
                rc = sqlite3_step(stmt);
                
                // Reset statement for potential reuse
                sqlite3_reset(stmt);
            }
        }
        
        // Finalize the statement
        sqlite3_finalize(stmt);
    }
    
    // Check transaction state
    const char* schema_name = "main";
    int txn_state = sqlite3_txn_state(db, fuzz_str_7);
    (void)txn_state; // Mark as used to avoid compiler warning
    
    // Get SQLite status information
    int current_memory = 0;
    int highwater_memory = 0;
    int reset_flag = 0; // Don't reset highwater mark
    
    // Get memory usage statistics
    sqlite3_status(fuzz_int32_t_8, &current_memory, &highwater_memory, fuzz_int32_t_9);
    
    // Create a custom function with user data
    const char* user_data_message = "Custom function user data";
    rc = sqlite3_create_function_v2(
        db,
        fuzz_str_10,
        fuzz_int32_t_11,  // Number of arguments
        fuzz_int32_t_12,
        const_cast<char*>(user_data_message), // User data
         id_000005_custom_function,
        nullptr, // No step function (scalar function)
        nullptr, // No finalize function
        nullptr  // No destructor
    );
    
    // Use the custom function
    if (rc == SQLITE_OK) {
        const char* select_sql = "SELECT custom_func();";
        sqlite3_stmt* select_stmt = nullptr;
        rc = sqlite3_prepare_v3(db, fuzz_str_13, -1, fuzz_uint32_t_14, &select_stmt, nullptr);
        
        if (rc == SQLITE_OK && select_stmt) {
            if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                // Function executed successfully
            }
            sqlite3_finalize(select_stmt);
        }
    }
    
    // Clean up
    // Remove profile hook
    sqlite3_profile(db, nullptr, nullptr);
    
    // Close database
    sqlite3_close(db);
    
    // Remove temporary database file
    remove(db_filename);
    
    return 0;
}