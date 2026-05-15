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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 26, nr_unique_branch: 0
//<Quality> {"density":26,"unique_branches":{},"library_calls":["sqlite3_auto_extension","sqlite3_open","sqlite3_close_v2","sqlite3_rollback_hook","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_null","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_column_value","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_value_type","sqlite3_malloc","sqlite3_strglob","sqlite3_free","sqlite3_finalize","sqlite3_serialize","sqlite3_malloc","sqlite3_global_recover","sqlite3_free","sqlite3_free","sqlite3_close_v2","sqlite3_cancel_auto_extension"],"critical_calls":["sqlite3_auto_extension","sqlite3_open","sqlite3_rollback_hook","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_text","sqlite3_step","sqlite3_column_value","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_value_type","sqlite3_malloc","sqlite3_strglob","sqlite3_free","sqlite3_finalize","sqlite3_serialize","sqlite3_malloc","sqlite3_global_recover","sqlite3_free","sqlite3_free","sqlite3_close_v2","sqlite3_cancel_auto_extension"],"visited":0}
/**/


// Define a dummy extension function for sqlite3_auto_extension
static void  id_000007_dummy_extension() {
    // This is just a placeholder extension function
}

// Define a rollback hook callback function
static void  id_000007_rollback_hook_callback(void *arg) {
    // This callback will be invoked when a transaction is rolled back
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Early exit if input is too small
    if (size < 1) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    const char *zTail = nullptr;
    unsigned char *serialized_data = nullptr;
    sqlite3_int64 db_size = 0;
    void *malloced_ptr = nullptr;
    
    // 1. Register an auto-extension (dummy)
    sqlite3_auto_extension((void (*)()) id_000007_dummy_extension);
    
    // 2. Open an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }
    
    // 3. Set up a rollback hook
    sqlite3_rollback_hook(db,  id_000007_rollback_hook_callback, nullptr);
    
    // 4. Create a table and insert data using input
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_test ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "pattern TEXT"
        ");";
    
    rc = sqlite3_prepare_v2(db, create_table_sql, -1, &stmt, &zTail);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 5. Insert data using a prepared statement
    // We'll use the first byte as pattern, rest as blob data
    if (size > 0) {
        const char *insert_sql = 
            "INSERT INTO fuzz_test (data, pattern) VALUES (?, ?);";
        
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, &zTail);
        if (rc == SQLITE_OK) {
            // Bind blob data (skip first byte for pattern)
            size_t blob_size = (size > 1) ? size - 1 : 0;
            const void *blob_data = (size > 1) ? data + 1 : nullptr;
            
            if (blob_size > 0) {
                sqlite3_bind_blob(stmt, 1, blob_data, static_cast<int>(blob_size), SQLITE_STATIC);
            } else {
                sqlite3_bind_null(stmt, 1);
            }
            
            // Bind pattern (use first byte as pattern character)
            char pattern[2] = {static_cast<char>(data[0]), '\0'};
            sqlite3_bind_text(stmt, 2, pattern, -1, SQLITE_STATIC);
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 6. Use sqlite3_bind_parameter_count on a dummy statement
    const char *dummy_sql = "SELECT * FROM fuzz_test WHERE pattern GLOB ?;";
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, &zTail);
    if (rc == SQLITE_OK) {
        int param_count = sqlite3_bind_parameter_count(stmt);
        (void)param_count; // Use variable to avoid unused warning
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 7. Query data using strglob pattern matching
    if (size > 0) {
        const char *query_sql = 
            "SELECT data, pattern FROM fuzz_test WHERE pattern GLOB ?;";
        
        rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, &zTail);
        if (rc == SQLITE_OK) {
            // Use first byte as pattern for GLOB matching
            char pattern[2] = {static_cast<char>(data[0]), '\0'};
            sqlite3_bind_text(stmt, 1, pattern, -1, SQLITE_STATIC);
            
            // Execute query and process results
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Get column values
                sqlite3_value *value = sqlite3_column_value(stmt, 0);
                const void *blob_data = sqlite3_column_blob(stmt, 0);
                int blob_size = sqlite3_column_bytes(stmt, 0);
                
                // Use the value (even if just to ensure it's accessed)
                if (value != nullptr) {
                    int value_type = sqlite3_value_type(value);
                    (void)value_type;
                }
                
                // Test strglob with pattern from first byte and data from column
                if (blob_data != nullptr && blob_size > 0) {
                    // Convert blob to string for strglob comparison
                    char *blob_str = static_cast<char*>(sqlite3_malloc(blob_size + 1));
                    if (blob_str) {
                        memcpy(blob_str, blob_data, blob_size);
                        blob_str[blob_size] = '\0';
                        
                        char pattern_str[2] = {static_cast<char>(data[0]), '\0'};
                        int strglob_result = sqlite3_strglob(pattern_str, blob_str);
                        (void)strglob_result; // Use variable
                        
                        sqlite3_free(blob_str);
                    }
                }
            }
            
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // 8. Serialize the database
    serialized_data = sqlite3_serialize(db, "main", &db_size, 0);
    
    // 9. Allocate memory using sqlite3_malloc
    malloced_ptr = sqlite3_malloc(256);
    if (malloced_ptr) {
        // Initialize with some data
        memset(malloced_ptr, 0, 256);
        
        // Test global recover (this is typically for error recovery)
        int recover_result = sqlite3_global_recover();
        (void)recover_result;
        
        // Free the allocated memory
        sqlite3_free(malloced_ptr);
    }
    
    // 10. Clean up serialized data if any
    if (serialized_data) {
        sqlite3_free(serialized_data);
    }
    
    // 11. Close the database
    sqlite3_close_v2(db);
    
    // 12. Cancel the auto extension
    sqlite3_cancel_auto_extension((void (*)()) id_000007_dummy_extension);
    
    return 0;
}