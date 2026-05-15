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
//<ID> 68
//<Prompt> []
/*<Combination>: [
*/
//<score> 30, nr_unique_branch: 1
//<Quality> {"density":15,"unique_branches":{"memdbClose":[[52434,9,52434,51,0,0,4,1]]},"library_calls":["sqlite3_open","sqlite3_update_hook","sqlite3_malloc","sqlite3_deserialize","sqlite3_errmsg16","sqlite3_create_function_v2","sqlite3_value_type","sqlite3_value_int","sqlite3_result_int","sqlite3_result_subtype","sqlite3_result_null","sqlite3_value_type","sqlite3_value_int","sqlite3_result_int","sqlite3_result_subtype","sqlite3_result_null","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_update_hook","sqlite3_malloc","sqlite3_deserialize","sqlite3_errmsg16","sqlite3_create_function_v2","sqlite3_result_null","sqlite3_result_subtype","sqlite3_result_int","sqlite3_value_int","sqlite3_value_type","sqlite3_result_null","sqlite3_result_subtype","sqlite3_result_int","sqlite3_value_int","sqlite3_value_type","sqlite3_prepare_v2","sqlite3_bind_int","sqlite3_bind_zeroblob64","sqlite3_step","sqlite3_reset","sqlite3_finalize","sqlite3_exec","sqlite3_free","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Check for minimum input size
    if (size < 10) {
	return 0;
	}
	
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    unsigned char *deserialize_data = nullptr;
    
    // Create a temporary file to work with
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Create an output file
    FILE *out_file = fopen("output_file", "wb");
    if (!out_file) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Open an in-memory database
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&out_file);;
        assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	return 0;
    }
    
    // Set update hook to monitor database changes
    void *update_hook_arg = static_cast<void*>(out_file);
    sqlite3_update_hook(db, 
        [](void *arg, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
            FILE *f = static_cast<FILE*>(arg);
            if (f) {
                const char *op_str = "";
                switch(op) {
                    case SQLITE_INSERT: op_str = "INSERT"; break;
                    case SQLITE_UPDATE: op_str = "UPDATE"; break;
                    case SQLITE_DELETE: op_str = "DELETE"; break;
                }
                fprintf(f, "Hook: %s on %s.%s rowid=%lld\n", 
                        op_str, db_name, table_name, rowid);
            }
        }, 
        update_hook_arg);
    
    // Allocate buffer for deserialization
    size_t data_size = size > 100 ? 100 : size; // Limit size for safety
    deserialize_data = static_cast<unsigned char*>(sqlite3_malloc(data_size));
    if (deserialize_data) {
        // Copy input data to buffer
        memcpy(deserialize_data, data, data_size);
        
        // Attempt to deserialize database from memory buffer
        sqlite3_int64 szDb = static_cast<sqlite3_int64>(data_size);
        sqlite3_int64 szBuf = static_cast<sqlite3_int64>(data_size * 2);
        
        rc = sqlite3_deserialize(db, "main", deserialize_data, 
                                 szDb, szBuf, 0);
        
        // Check error if deserialization failed
        if (rc != SQLITE_OK) {
            const void *err_msg = sqlite3_errmsg16(db);
            if (err_msg) {
                // Write error message to output file
                fwrite(err_msg, sizeof(uint16_t), 
                       wcslen(static_cast<const wchar_t*>(err_msg)), out_file);
            }
        }
    }
    
    // Create a custom function that uses sqlite3_result_subtype
    rc = sqlite3_create_function_v2(
        db, "test_func", 1, SQLITE_UTF8, nullptr,
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            if (argc == 1 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
                int val = sqlite3_value_int(argv[0]);
                sqlite3_result_int(ctx, val * 2);
                // Set subtype to indicate transformed value
                sqlite3_result_subtype(ctx, 1);
            } else {
                sqlite3_result_null(ctx);
            }
        },
        nullptr, nullptr, nullptr);
    
    if (rc == SQLITE_OK) {
        // Prepare a statement with zeroblob binding
        const char *sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, data BLOB); "
                          "INSERT INTO test VALUES (test_func(?), ?);";
        rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        
        if (rc == SQLITE_OK && stmt) {
            // Bind parameters
            sqlite3_bind_int(stmt, 1, static_cast<int>(size % 100));
            
            // Bind zeroblob64 - use input size if reasonable, otherwise limit
            sqlite3_uint64 blob_size = (size < 1024 * 1024) ? 
                static_cast<sqlite3_uint64>(size) : 1024;
            rc = sqlite3_bind_zeroblob64(stmt, 2, blob_size);
            
            if (rc == SQLITE_OK) {
                // Execute the statement
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    // Process rows if any
                }
                
                // Reset statement for potential reuse
                sqlite3_reset(stmt);
            }
            
            // Finalize the statement
            sqlite3_finalize(stmt);
            stmt = nullptr;
        }
    }
    
    // Perform some operations to trigger update hook
    const char *trigger_sql = 
        "CREATE TABLE IF NOT EXISTS trigger_table (id INTEGER PRIMARY KEY, value TEXT);"
        "INSERT INTO trigger_table (value) VALUES ('test1'), ('test2');"
        "UPDATE trigger_table SET value = 'updated' WHERE id = 1;"
        "DELETE FROM trigger_table WHERE id = 2;";
    
    char *err_msg = nullptr;
    rc = sqlite3_exec(db, trigger_sql, nullptr, nullptr, &err_msg);
    if (err_msg) {
        sqlite3_free(err_msg);
    }
    
    // Clean up
    if (deserialize_data) {
        sqlite3_free(deserialize_data);
    }
    
    // Close database
    sqlite3_close(db);
    
    // Close files
    assert_file_closed(&in_file);;
    assert_file_closed(&out_file);;
    
    // Remove output file
    remove("output_file");
    
    assert_file_closed(&out_file);
	assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	return 0;
}