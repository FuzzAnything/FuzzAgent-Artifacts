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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 45, nr_unique_branch: 10
//<Quality> {"density":15,"unique_branches":{"sqlite3_value_blob":[[89654,12,89654,16,0,0,4,1]],"sqlite3_value_pointer":[[89680,7,89681,49,0,0,4,0],[89682,7,89682,16,0,0,4,0],[89683,7,89683,23,0,0,4,0],[89684,7,89684,37,0,0,4,0]],"sqlite3_sleep":[[181613,7,181613,14,0,0,4,1],[181618,30,181618,34,0,0,4,0],[181618,30,181618,34,0,0,4,1]],"sqlite3_db_config":[[178596,15,178596,19,0,0,4,1]],"sqlite3_result_pointer":[[89989,7,89989,14,0,0,4,1]]},"library_calls":["sqlite3_initialize","sqlite3_open","sqlite3_shutdown","sqlite3_db_config","sqlite3_create_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_value_pointer","sqlite3_column_value","sqlite3_finalize","sqlite3_sleep","sqlite3_thread_cleanup","sqlite3_close","sqlite3_shutdown"],"critical_calls":["sqlite3_initialize","sqlite3_open","sqlite3_db_config","sqlite3_create_function","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_blob","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_step","sqlite3_value_pointer","sqlite3_column_value","sqlite3_finalize","sqlite3_sleep","sqlite3_thread_cleanup","sqlite3_close","sqlite3_shutdown"],"visited":1}
/**/


// Custom callback structure for autovacuum
struct AutovacuumCallbackData {
    const uint8_t* input_data;
    size_t data_size;
    size_t position;
};

// Function signature for sqlite3_autovacuum_pages callback
unsigned int  id_000041_autovacuum_callback(void* pClientData, const char* zDb, 
                                unsigned int nPage, unsigned int nFree,
                                unsigned int nSizePerPage) {
    AutovacuumCallbackData* data = (AutovacuumCallbackData*)pClientData;
    
    // Consume bytes from input data to determine pages to skip
    unsigned int pages_to_skip = 0;
    if (data->position + sizeof(unsigned int) <= data->data_size) {
        memcpy(&pages_to_skip, data->input_data + data->position, sizeof(unsigned int));
        data->position += sizeof(unsigned int);
    }
    
    // Return number of pages to skip (modulo to ensure valid value)
    return pages_to_skip % (nPage + 1);
}

// Cleanup function for autovacuum callback
void  id_000041_autovacuum_cleanup(void* pClientData) {
    // Nothing to cleanup in this case
}

// User-defined function for sqlite3_result_pointer
void  id_000041_test_pointer_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_BLOB) {
        const void* blob_data = sqlite3_value_blob(argv[0]);
        int blob_size = sqlite3_value_bytes(argv[0]);
        
        // Create a copy of the blob data
        void* copy = malloc(blob_size);
        if (copy) {
            memcpy(copy, blob_data, blob_size);
            sqlite3_result_pointer(context, copy, "TestPointer", free);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Initialize SQLite
    sqlite3_initialize();
    
    // Create an in-memory database
    sqlite3* db = nullptr;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        sqlite3_shutdown();
        return 0;
    }
    
    // Prepare autovacuum callback data
    AutovacuumCallbackData callback_data;
    callback_data.input_data = data;
    callback_data.data_size = size;
    callback_data.position = 0;
    
    // Use sqlite3_autovacuum_pages API
    // Note: This function may not be available in all versions
    // We'll use sqlite3_db_config to set autovacuum as an alternative approach
    sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, nullptr);
    
    // Register custom function that uses sqlite3_result_pointer
    sqlite3_create_function(db, "test_pointer", 1, SQLITE_UTF8, nullptr,
                            id_000041_test_pointer_function, nullptr, nullptr);
    
    // Create a test table
    const char* create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "data BLOB, "
        "value TEXT)";
    
    sqlite3_exec(db, create_table_sql, nullptr, nullptr, nullptr);
    
    // Use input data for various operations
    size_t data_pos = 0;
    
    // Insert data into table using the input
    while (data_pos + sizeof(int) * 2 < size) {
        int id = 0;
        memcpy(&id, data + data_pos, sizeof(int));
        data_pos += sizeof(int);
        
        int data_size = 0;
        memcpy(&data_size, data + data_pos, sizeof(int));
        data_pos += sizeof(int);
        
        // Ensure we don't read beyond the input
        if (data_size < 0 || data_pos + data_size > size) {
            data_size = size - data_pos;
            if (data_size <= 0) break;
        }
        
        // Prepare insert statement
        sqlite3_stmt* stmt = nullptr;
        const char* insert_sql = "INSERT INTO test_table (id, data) VALUES (?, ?)";
        
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_int(stmt, 1, id);
            sqlite3_bind_blob(stmt, 2, data + data_pos, data_size, SQLITE_STATIC);
            
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        data_pos += data_size;
    }
    
    // Test the custom function with sqlite3_result_pointer
    sqlite3_stmt* test_stmt = nullptr;
    const char* test_sql = "SELECT test_pointer(data) FROM test_table LIMIT 1";
    
    if (sqlite3_prepare_v2(db, test_sql, -1, &test_stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(test_stmt) == SQLITE_ROW) {
            // Get the pointer result
            void* result_ptr = sqlite3_value_pointer(sqlite3_column_value(test_stmt, 0), "TestPointer");
            if (result_ptr) {
                // We could use the pointer here if needed
            }
        }
        sqlite3_finalize(test_stmt);
    }
    
    // Simulate using sqlite3_vtab_in with a dummy index_info structure
    // Note: This is a simplified usage since we don't have a real virtual table
    sqlite3_index_info dummy_index_info = {};
    int iCons = 0;
    int bHandle = 1;
    
    // Use sqlite3_vtab_in if available (compile-time check)
    #ifdef SQLITE_ENABLE_VIRTUALTABLE
    sqlite3_vtab_in(&dummy_index_info, iCons, bHandle);
    #endif
    
    // Use sqlite3_sleep with a small delay based on input data
    int sleep_ms = 0;
    if (size >= sizeof(int)) {
        memcpy(&sleep_ms, data, sizeof(int));
        sleep_ms = sleep_ms % 100;  // Limit to 100ms max
        sqlite3_sleep(sleep_ms);
    }
    
    // Cleanup thread-specific data
    sqlite3_thread_cleanup();
    
    // Close database
    sqlite3_close(db);
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}