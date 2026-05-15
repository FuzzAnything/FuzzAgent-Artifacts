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
//<ID> 392
//<Prompt> []
/*<Combination>: [
*/
//<score> 120, nr_unique_branch: 15
//<Quality> {"density":30,"unique_branches":{"patternCompare":[[128161,11,128161,15,0,0,4,1],[128163,17,128163,30,0,0,4,1],[128189,11,128189,17,0,0,4,0],[128192,13,128192,19,0,0,4,1],[128202,15,128202,28,0,0,4,1],[128205,15,128205,37,0,0,4,0],[128205,15,128205,37,0,0,4,1]],"sqlite3_blob_open":[[101633,7,101633,20,0,0,4,1],[101636,9,101636,14,0,0,4,0],[101636,18,101636,30,0,0,4,1],[101639,32,101639,36,0,0,4,0]],"blobSeekToRow":[[101370,7,101370,21,0,0,4,1],[101393,7,101393,21,0,0,4,1],[101395,13,101395,21,0,0,4,0],[101398,9,101398,22,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_exec","sqlite3_free","sqlite3_close","sqlite3_prepare_v2","sqlite3_bind_zeroblob","sqlite3_blob_open","sqlite3_last_insert_rowid","sqlite3_blob_write","sqlite3_blob_close","sqlite3_bind_int","sqlite3_bind_text","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_value_type","sqlite3_value_int","sqlite3_result_int","sqlite3_result_null","sqlite3_result_zeroblob","sqlite3_result_int","sqlite3_result_null","sqlite3_value_type","sqlite3_value_int","sqlite3_result_int","sqlite3_result_null","sqlite3_result_zeroblob","sqlite3_result_int","sqlite3_result_null","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_strglob","sqlite3_free","sqlite3_step","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_text","sqlite3_reset","sqlite3_strglob","sqlite3_errcode","sqlite3_finalize","sqlite3_finalize","sqlite3_free","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_rollback_hook","sqlite3_exec","sqlite3_prepare_v2","sqlite3_bind_zeroblob","sqlite3_blob_open","sqlite3_last_insert_rowid","sqlite3_blob_write","sqlite3_blob_close","sqlite3_bind_int","sqlite3_bind_text","sqlite3_step","sqlite3_reset","sqlite3_result_null","sqlite3_result_int","sqlite3_result_zeroblob","sqlite3_result_null","sqlite3_result_int","sqlite3_value_int","sqlite3_value_type","sqlite3_result_null","sqlite3_result_int","sqlite3_result_zeroblob","sqlite3_result_null","sqlite3_result_int","sqlite3_value_int","sqlite3_value_type","sqlite3_create_function","sqlite3_prepare_v2","sqlite3_expanded_sql","sqlite3_strglob","sqlite3_free","sqlite3_step","sqlite3_column_int","sqlite3_column_blob","sqlite3_column_bytes","sqlite3_column_text","sqlite3_reset","sqlite3_strglob","sqlite3_errcode","sqlite3_finalize","sqlite3_finalize","sqlite3_free","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}




    // Create input file from fuzzer data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
        assert_file_closed(&in_file);
	return 0;
    }
    
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt *stmt2 = nullptr;
    char *errmsg = nullptr;
    int rc;
    
    // 1. Open database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // 2. Set rollback hook
    sqlite3_rollback_hook(db, [](void *) {
        // Empty callback for demonstration
    }, nullptr);
    
    // 3. Create table and insert data using input
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS fuzz_data ("
        "id INTEGER PRIMARY KEY, "
        "content BLOB, "
        "value INTEGER, "
        "text_content TEXT);";
    
    rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        if (errmsg) sqlite3_free(errmsg);
        sqlite3_close(db);
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	return 0;
    }
    
    // Read data from input file
    fseek(in_file, 0, SEEK_END);
    long file_size = ftell(in_file);
    fseek(in_file, 0, SEEK_SET);
    
    if (file_size > 0) {
        // 4. Prepare INSERT statement
        const char *insert_sql = 
            "INSERT INTO fuzz_data (content, value, text_content) "
            "VALUES (?, ?, ?);";
        
        rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, nullptr);
        if (rc == SQLITE_OK) {
            // Use first 100 bytes for blob, or all if smaller
            int blob_size = (file_size > 100) ? 100 : file_size;
            uint8_t *blob_data = new uint8_t[blob_size];
            fread(blob_data, 1, blob_size, in_file);
            
            // Bind blob
            rc = sqlite3_bind_zeroblob(stmt, 1, blob_size);
            if (rc == SQLITE_OK) {
                // Update zeroblob with actual data
                sqlite3_blob *blob_handle;
                rc = sqlite3_blob_open(db, "main", "fuzz_data", "content", 
                                      sqlite3_last_insert_rowid(db), 0, &blob_handle);
                if (rc == SQLITE_OK) {
                    sqlite3_blob_write(blob_handle, blob_data, blob_size, 0);
                    sqlite3_blob_close(blob_handle);
                }
            }
            
            // Bind integer value (use file size)
            sqlite3_bind_int(stmt, 2, file_size);
            
            // Bind text (remaining data or placeholder)
            if (file_size > blob_size) {
                int text_size = file_size - blob_size;
                char *text_data = new char[text_size + 1];
                fread(text_data, 1, text_size, in_file);
                text_data[text_size] = '\0';
                sqlite3_bind_text(stmt, 3, text_data, text_size, SQLITE_TRANSIENT);
                delete[] text_data;
            } else {
                sqlite3_bind_text(stmt, 3, "fuzz_data", -1, SQLITE_TRANSIENT);
            }
            
            // Execute insert
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            
            delete[] blob_data;
        }
        
        // 5. Create custom function
        auto custom_func = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            if (argc == 1) {
                int val_type = sqlite3_value_type(argv[0]);
                switch (val_type) {
                    case SQLITE_INTEGER: {
                        int val = sqlite3_value_int(argv[0]);
                        sqlite3_result_int(ctx, val * val); // Square the value
                        break;
                    }
                    case SQLITE_NULL:
                        sqlite3_result_null(ctx);
                        break;
                    case SQLITE_BLOB:
                        sqlite3_result_zeroblob(ctx, 16); // Return 16-byte zeroblob
                        break;
                    default:
                        sqlite3_result_int(ctx, 0);
                }
            } else {
                sqlite3_result_null(ctx);
            }
        };
        
        sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, 
                               nullptr, custom_func, nullptr, nullptr);
        
        // 6. Use the custom function in a query
        const char *select_sql = 
            "SELECT fuzz_func(value), content, text_content FROM fuzz_data;";
        
        rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt2, nullptr);
        if (rc == SQLITE_OK) {
            // Get expanded SQL
            char *expanded_sql = sqlite3_expanded_sql(stmt2);
            if (expanded_sql) {
                // Use strglob to check if expanded SQL contains "fuzz_func"
                int glob_result = sqlite3_strglob("*fuzz_func*", expanded_sql);
                sqlite3_free(expanded_sql);
            }
            
            // Step through results
            while (sqlite3_step(stmt2) == SQLITE_ROW) {
                // Get column values
                int result = sqlite3_column_int(stmt2, 0);
                
                const void *blob = sqlite3_column_blob(stmt2, 1);
                int blob_bytes = sqlite3_column_bytes(stmt2, 1);
                
                const char *text = (const char *)sqlite3_column_text(stmt2, 2);
                
                // Just use the values to avoid unused variable warnings
                (void)result;
                (void)blob;
                (void)blob_bytes;
                (void)text;
            }
            
            sqlite3_reset(stmt2);
        }
        
        // 7. Test strglob with some patterns
        const char *patterns[] = {"*test*", "*data*", "*fuzz*"};
        for (auto pattern : patterns) {
            sqlite3_strglob(pattern, "fuzz_data_test");
        }
        
        // 8. Check error code
        int errcode = sqlite3_errcode(db);
        (void)errcode;
        
        // 9. Finalize statements
        if (stmt) sqlite3_finalize(stmt);
        if (stmt2) sqlite3_finalize(stmt2);
        
        // 10. Clean up
        if (errmsg) sqlite3_free(errmsg);
    }
    
    assert_file_closed(&in_file);;
    sqlite3_close(db);
    
    assert_file_closed(&in_file);
	return 0;
}