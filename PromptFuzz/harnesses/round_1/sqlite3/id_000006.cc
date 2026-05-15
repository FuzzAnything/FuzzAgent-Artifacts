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
//<ID> 39
//<Prompt> []
/*<Combination>: [
*/
//<score> 140, nr_unique_branch: 12
//<Quality> {"density":20,"unique_branches":{"sqlite3VdbeDeleteAuxData":[[87637,14,87637,30,0,0,4,0],[87638,15,87638,31,0,0,4,1],[87638,35,87638,69,0,0,4,0]],"sqlite3_unlock_notify":[[182811,7,182811,32,0,0,4,1],[182816,7,182816,17,0,0,4,1],[182822,13,182822,39,0,0,4,0],[182845,32,182845,34,0,0,4,1]],"blobSeekToRow":[[101398,9,101398,22,0,0,4,1],[14098,27,14098,28,3,0,4,1]],"sqlite3Step":[[90254,32,90254,39,0,0,4,1]],"sqlite3_profile":[[179855,7,179855,19,0,0,4,1]],"sqlite3Close":[[178872,9,178872,23,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close","sqlite3_set_clientdata","sqlite3_profile","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_result_null","sqlite3_value_encoding","sqlite3_get_auxdata","sqlite3_set_auxdata","sqlite3_result_int","sqlite3_result_null","sqlite3_value_encoding","sqlite3_get_auxdata","sqlite3_set_auxdata","sqlite3_result_int","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_unlock_notify","sqlite3_drop_modules","sqlite3_profile","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_set_clientdata","sqlite3_profile","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_prepare_v2","sqlite3_bind_blob64","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_result_int","sqlite3_set_auxdata","sqlite3_get_auxdata","sqlite3_value_encoding","sqlite3_result_null","sqlite3_result_int","sqlite3_set_auxdata","sqlite3_get_auxdata","sqlite3_value_encoding","sqlite3_result_null","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_step","sqlite3_finalize","sqlite3_blob_open","sqlite3_blob_read","sqlite3_blob_close","sqlite3_unlock_notify","sqlite3_drop_modules","sqlite3_profile","sqlite3_close","sqlite3_close"],"visited":0}
/**/


// Custom structure to hold data for auxdata callbacks
struct AuxDataInfo {
    uint8_t* data;
    size_t size;
    int encoding;
};

// Callback for sqlite3_unlock_notify
static void  id_000006_unlock_notify_callback(void** block_arg, int count) {
    // This callback is called when database becomes unlocked
    // We can use auxdata stored in the first argument if available
    if (count > 0 && block_arg[0]) {
        // Just mark as processed, no actual operation needed for fuzzing
        AuxDataInfo* info = static_cast<AuxDataInfo*>(block_arg[0]);
        info->encoding = 1; // Mark as notified
    }
}

// Custom function for sqlite3_set_clientdata callback
static void  id_000006_clientdata_destructor(void* data) {
    if (data) {
        free(data);
    }
}

// Profile callback for sqlite3_profile
static void  id_000006_profile_callback(void* context, const char* sql, sqlite3_uint64 elapsed) {
    // Store profile information in client data if available
    if (context) {
        int* counter = static_cast<int*>(context);
        (*counter)++;
    }
}

// Helper function to create a temporary database file from input data
static int  id_000006_create_temp_db(const uint8_t* data, size_t size, sqlite3** db) {
    // Use fmemopen to create a FILE* from input data
    FILE* in_file = fmemopen((void*)data, size, "rb");
    if (!in_file) return SQLITE_ERROR;
    
    // Create a temporary file to write database to
    FILE* tmp_file = fopen("temp_fuzz.db", "wb");
    if (!tmp_file) {
        assert_file_closed(&in_file);;
        return SQLITE_ERROR;
    }
    
    // Copy data from memory to temporary file
    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), in_file)) > 0) {
        fwrite(buffer, 1, bytes_read, tmp_file);
    }
    
    assert_file_closed(&in_file);;
    assert_file_closed(&tmp_file);;
    
    // Open the database from temporary file
    return sqlite3_open("temp_fuzz.db", db);
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    if (size < 100) {
	return 0;
	}
	 // Need minimum data for meaningful fuzzing
    
    sqlite3* db = nullptr;
    sqlite3* blocked_db = nullptr;
    sqlite3_stmt* stmt = nullptr;
    sqlite3_blob* blob = nullptr;
    int rc = 0;
    
    // 1. Create temporary database from input data
    rc =  id_000006_create_temp_db(data, size, &db);
    if (rc != SQLITE_OK || !db) {
        remove("temp_fuzz.db");
        return 0;
    }
    
    // 2. Create a second database connection for unlock_notify testing
    rc = sqlite3_open(":memory:", &blocked_db);
    if (rc != SQLITE_OK || !blocked_db) {
        sqlite3_close(db);
        remove("temp_fuzz.db");
        return 0;
    }
    
    // 3. Setup client data (sqlite3_set_clientdata)
    int* client_data = static_cast<int*>(malloc(sizeof(int)));
    *client_data = 42;
    rc = sqlite3_set_clientdata(db, "fuzz_client", client_data,  id_000006_clientdata_destructor);
    
    // 4. Setup profile callback (sqlite3_profile)
    int profile_counter = 0;
    sqlite3_profile(db,  id_000006_profile_callback, &profile_counter);
    
    // 5. Prepare a statement to create table and insert data
    const char* create_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_data ("
        "id INTEGER PRIMARY KEY,"
        "content BLOB,"
        "value TEXT"
        ");";
    
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 6. Insert some data into the table
    const char* insert_sql = "INSERT INTO fuzz_data (content, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Use first 50 bytes as blob content
        size_t blob_size = size > 50 ? 50 : size;
        sqlite3_bind_blob64(stmt, 1, data, blob_size, SQLITE_STATIC);
        
        // Use next 50 bytes as text (if available)
        const char* text_data = reinterpret_cast<const char*>(data + blob_size);
        size_t text_size = size - blob_size > 50 ? 50 : size - blob_size;
        sqlite3_bind_text(stmt, 2, text_data, text_size, SQLITE_STATIC);
        
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 7. Create a custom function to test sqlite3_set_auxdata and sqlite3_value_encoding
    struct FuncContext {
        int call_count = 0;
    };
    
    auto custom_func = [](sqlite3_context* ctx, int argc, sqlite3_value** argv) {
        if (argc != 1) {
            sqlite3_result_null(ctx);
            return;
        }
        
        // Get encoding of the value (sqlite3_value_encoding)
        int encoding = sqlite3_value_encoding(argv[0]);
        
        // Get or create auxdata (sqlite3_set_auxdata)
        FuncContext* func_ctx = static_cast<FuncContext*>(sqlite3_get_auxdata(ctx, 0));
        if (!func_ctx) {
            func_ctx = new FuncContext();
            sqlite3_set_auxdata(ctx, 0, func_ctx, [](void* data) {
                delete static_cast<FuncContext*>(data);
            });
        }
        func_ctx->call_count++;
        
        sqlite3_result_int(ctx, encoding * 1000 + func_ctx->call_count);
    };
    
    sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, nullptr, 
                           custom_func, nullptr, nullptr);
    
    // 8. Test the custom function
    const char* func_test = "SELECT fuzz_func(content) FROM fuzz_data;";
    rc = sqlite3_prepare_v2(db, func_test, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Function executed, auxdata and encoding were tested
        }
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }
    
    // 9. Open a blob handle for reading (sqlite3_blob_open)
    rc = sqlite3_blob_open(db, "main", "fuzz_data", "content", 1, 0, &blob);
    if (rc == SQLITE_OK && blob) {
        // Read from blob (sqlite3_blob_read)
        char buffer[100];
        int bytes_to_read = size > sizeof(buffer) ? sizeof(buffer) : size;
        if (bytes_to_read > 0) {
            // Use part of input data to determine read parameters
            size_t offset = data[0] % 20; // Small offset
            if (offset + bytes_to_read <= 100) { // Within blob bounds
                sqlite3_blob_read(blob, buffer, bytes_to_read, offset);
            }
        }
        sqlite3_blob_close(blob);
        blob = nullptr;
    }
    
    // 10. Test unlock_notify (sqlite3_unlock_notify)
    AuxDataInfo notify_info;
    notify_info.data = const_cast<uint8_t*>(data);
    notify_info.size = size;
    notify_info.encoding = 0;
    
    void* notify_arg = &notify_info;
    rc = sqlite3_unlock_notify(blocked_db,  id_000006_unlock_notify_callback, notify_arg);
    
    // 11. Drop modules (sqlite3_drop_modules)
    // Drop all modules except essential ones
    const char* keep_modules[] = {nullptr}; // Drop all
    rc = sqlite3_drop_modules(db, keep_modules);
    
    // 12. Clean up profile callback
    sqlite3_profile(db, nullptr, nullptr);
    
    // 13. Clean up
    sqlite3_close(blocked_db);
    sqlite3_close(db);
    
    // Remove temporary files
    remove("temp_fuzz.db");
    
    // Free client data (destructor should be called by sqlite3)
    // Note:  id_000006_clientdata_destructor will free the memory
    
    return 0;
}