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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 104, nr_unique_branch: 25
//<Quality> {"density":16,"unique_branches":{"vdbeUnbind":[[91122,7,91122,20,0,0,4,0],[91122,24,91122,74,0,0,4,0],[91122,39,91122,44,0,0,4,1]],"sqlite3VdbeMemCopy":[[83049,9,83049,37,0,0,4,0]],"sqlite3_serialize":[[52970,7,52970,17,0,0,4,0],[52988,7,52988,13,0,0,4,0]],"memdbFromDbSchema":[[52935,7,52935,9,0,0,4,0]],"applyAffinity":[[92603,13,92603,38,0,0,4,1]],"exprAnalyze":[[159805,13,159805,21,0,0,4,0]],"sqlite3Step":[[90226,11,90226,21,0,0,4,0],[90229,13,90229,55,0,0,4,0]],"sqlite3_file_control":[[181647,7,181647,13,0,0,4,1]],"isLikeOrGlob":[[158643,9,158643,13,0,0,4,0],[158643,17,158643,54,0,0,4,0],[158643,17,158643,54,0,0,4,1],[158732,11,158732,26,0,0,4,0],[158736,13,158736,25,0,0,4,0],[158736,13,158736,25,0,0,4,1],[158736,29,158736,48,0,0,4,1]],"sqlite3TransferBindings":[[91384,12,91384,25,0,0,4,0]],"sqlite3_step":[[90395,9,90395,19,0,0,4,1]],"sqlite3VdbeGetBoundValue":[[89271,7,89271,8,0,0,4,0],[89274,9,89274,36,0,0,4,0],[89274,9,89274,36,0,0,4,1],[89276,11,89276,15,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_close_v2","sqlite3_rollback_hook","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_bind_int","sqlite3_bind_double","sqlite3_bind_text","sqlite3_step","sqlite3_finalize","sqlite3_serialize","sqlite3_strglob","sqlite3_free","sqlite3_global_recover","sqlite3_close_v2"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_exec","sqlite3_free","sqlite3_prepare_v2","sqlite3_bind_parameter_count","sqlite3_bind_parameter_name","sqlite3_bind_int","sqlite3_step","sqlite3_finalize","sqlite3_serialize","sqlite3_strglob","sqlite3_free","sqlite3_global_recover","sqlite3_close_v2"],"visited":1}
/**/


extern "C" int LLVMFuzzerTestOneInput_3(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create a temporary database in memory
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }

    // Set rollback hook to monitor rollbacks
    void* rollback_data = malloc(16); // Dummy data for hook
    void (*rollback_callback)(void*) = [](void* p) { /* Do nothing */ };
    sqlite3_rollback_hook(db, rollback_callback, rollback_data);

    // Create a table and insert some data to have content to serialize
    char* errmsg = nullptr;
    const char* create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, name TEXT);"
        "INSERT INTO test (name) VALUES ('fuzz'), ('test'), ('data');";
    rc = sqlite3_exec(db, create_table, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }

    // Prepare a statement with bind parameters
    sqlite3_stmt *stmt = nullptr;
    const char* sql = "SELECT * FROM test WHERE name LIKE ?;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Get bind parameter count and names
        int param_count = sqlite3_bind_parameter_count(stmt);
        for (int i = 1; i <= param_count; i++) {
            const char* param_name = sqlite3_bind_parameter_name(stmt, i);
            // Use the input data for bind value if we have enough data
            if (size > 0) {
                // Take first byte as bind type selector
                uint8_t bind_type = data[0];
                size_t data_offset = 1;
                
                if (bind_type == 0 && size > data_offset + sizeof(int)) {
                    int int_val = *reinterpret_cast<const int*>(data + data_offset);
                    sqlite3_bind_int(stmt, i, int_val);
                } else if (bind_type == 1 && size > data_offset + sizeof(double)) {
                    double dbl_val = *reinterpret_cast<const double*>(data + data_offset);
                    sqlite3_bind_double(stmt, i, dbl_val);
                } else if (size > data_offset) {
                    // Bind as text using remaining data
                    size_t text_len = (size - data_offset) > 100 ? 100 : (size - data_offset);
                    char* text_buf = reinterpret_cast<char*>(malloc(text_len + 1));
                    if (text_buf) {
                        memcpy(text_buf, data + data_offset, text_len);
                        text_buf[text_len] = '\0';
                        sqlite3_bind_text(stmt, i, text_buf, -1, SQLITE_TRANSIENT);
                        free(text_buf);
                    }
                }
            }
        }
        
        // Execute the statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            // Process rows if needed
        }
        sqlite3_finalize(stmt);
    }

    // Serialize the database using input data for schema selection
    sqlite3_int64 serialized_size = 0;
    unsigned int serialize_flags = 0;
    
    // Use input data to determine which schema to serialize
    const char* schema = "main"; // Default schema
    if (size > 0) {
        // Use first byte to decide schema
        if (data[0] % 3 == 0) {
            schema = "main";
        } else if (data[0] % 3 == 1) {
            schema = "temp";
        } else {
            schema = nullptr; // Will serialize all attached databases
        }
        
        // Use second byte for flags if available
        if (size > 1) {
            serialize_flags = data[1] % 4; // Valid flags are 0-3
        }
    }
    
    unsigned char* serialized_db = sqlite3_serialize(db, schema, &serialized_size, serialize_flags);
    
    // Use sqlite3_strglob with the serialized data
    if (serialized_db && serialized_size > 0) {
        // Convert serialized data to string for pattern matching
        char* serialized_str = reinterpret_cast<char*>(malloc(serialized_size + 1));
        if (serialized_str) {
            memcpy(serialized_str, serialized_db, serialized_size);
            serialized_str[serialized_size] = '\0';
            
            // Create pattern from input data
            char* pattern = reinterpret_cast<char*>(malloc(size + 1));
            if (pattern) {
                memcpy(pattern, data, size > 100 ? 100 : size);
                pattern[size > 100 ? 100 : size] = '\0';
                
                // Perform pattern matching
                int match_result = sqlite3_strglob(pattern, serialized_str);
                (void)match_result; // Use result to avoid unused variable warning
                
                free(pattern);
            }
            free(serialized_str);
        }
        sqlite3_free(serialized_db);
    }

    // Attempt global recovery
    rc = sqlite3_global_recover();
    (void)rc; // Use result to avoid unused variable warning

    // Cleanup
    free(rollback_data);
    sqlite3_close_v2(db);
    
    return 0;
}