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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 105, nr_unique_branch: 12
//<Quality> {"density":15,"unique_branches":{"sqlite3_deserialize":[[53065,7,53065,9,0,0,4,0],[53093,7,53093,12,0,0,4,0],[53093,16,53093,60,0,0,4,1]],"sqlite3WindowRewrite":[[168342,9,168342,20,0,0,4,0]],"sqlite3_backup_pagecount":[[81817,7,81817,11,0,0,4,1]],"sqlite3_limit":[[180525,7,180525,18,0,0,4,0],[180526,9,180526,37,0,0,4,1],[180528,15,180528,25,0,0,4,1]],"windowArgCount":[[168807,11,168807,16,0,0,4,1]],"sqlite3VdbeExec":[[99902,9,99902,24,0,0,4,0],[99906,9,99906,23,0,0,4,1],[99915,9,99915,11,0,0,4,0]]},"library_calls":["sqlite3_open","sqlite3_deserialize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_snprintf","sqlite3_limit","sqlite3_prepare_v2","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_user_data","sqlite3_result_zeroblob64","sqlite3_result_error_toobig","sqlite3_user_data","sqlite3_result_zeroblob64","sqlite3_result_error_toobig","sqlite3_user_data","sqlite3_result_zeroblob64","sqlite3_user_data","sqlite3_result_zeroblob64","sqlite3_create_window_function","sqlite3_exec","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"critical_calls":["sqlite3_open","sqlite3_deserialize","sqlite3_open","sqlite3_backup_init","sqlite3_backup_pagecount","sqlite3_snprintf","sqlite3_limit","sqlite3_prepare_v2","sqlite3_stmt_scanstatus_reset","sqlite3_finalize","sqlite3_result_error_toobig","sqlite3_result_zeroblob64","sqlite3_user_data","sqlite3_result_error_toobig","sqlite3_result_zeroblob64","sqlite3_user_data","sqlite3_result_zeroblob64","sqlite3_user_data","sqlite3_result_zeroblob64","sqlite3_user_data","sqlite3_create_window_function","sqlite3_exec","sqlite3_backup_finish","sqlite3_close","sqlite3_close"],"visited":0}
/**/


extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* f_data, size_t f_size) {
	if(f_size<0) return 0;

	
	//fuzzer vars shim {
		FuzzedDataProvider fdp(f_data, f_size);
		FDPConsumeRawBytes(const uint8_t *, data, size, fdp)
	//fuzzer shim end}
	FILE *input_file_ptr = fopen("input_file", "wb");
	if (input_file_ptr == NULL) {return 0;}
	fwrite(data, sizeof(uint8_t), size, input_file_ptr);
	fclose(input_file_ptr);





    // Create input file from the data
    FILE *in_file = fmemopen((void *)data, size, "rb");
    if (!in_file) {
	assert_file_closed(&in_file);
	return 0;
	}
	
    
    // Open the input database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open("input_file", &db);
    if (rc != SQLITE_OK) {
        assert_file_closed(&in_file);;
        assert_file_closed(&in_file);
	assert_file_name_closed("input_file");
	return 0;
    }
    
    // Read database content from input file
    fseek(in_file, 0, SEEK_SET);
    char *buffer = (char *)malloc(size);
    if (buffer) {
        fread(buffer, 1, size, in_file);
        // Use sqlite3_deserialize to load database
        sqlite3_deserialize(db, "main", (unsigned char *)buffer, size, size, 0);
        free(buffer);
    }
    assert_file_closed(&in_file);;
    
    // Create a backup to output database
    sqlite3 *db2 = nullptr;
    sqlite3_open("output_file", &db2);
    sqlite3_backup *backup = sqlite3_backup_init(db2, "main", db, "main");
    
    if (backup) {
        // Use sqlite3_backup_pagecount API
        int pagecount = sqlite3_backup_pagecount(backup);
        
        // Use sqlite3_snprintf API
        char buf[256];
        sqlite3_snprintf(256, buf, "Backup pagecount: %d", pagecount);
        
        // Use sqlite3_limit API
        int old_limit = sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 10000);
        
        // Create a prepared statement
        sqlite3_stmt *stmt = nullptr;
        rc = sqlite3_prepare_v2(db, "SELECT 1", -1, &stmt, nullptr);
        
        if (stmt) {
            // Use sqlite3_stmt_scanstatus_reset API
            sqlite3_stmt_scanstatus_reset(stmt);
            sqlite3_finalize(stmt);
        }
        
        // Simulate a context for result APIs (requires a custom function context)
        // Create an aggregate function to get a context
        struct FuncContext {
            sqlite3_uint64 blob_size;
            int error_triggered;
        };
        
        FuncContext ctx_data = {0, 0};
        
        // Window function callbacks
        auto xStep = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            FuncContext *data = (FuncContext *)sqlite3_user_data(ctx);
            if (data && !data->error_triggered) {
                // Use sqlite3_result_zeroblob64 API
                sqlite3_result_zeroblob64(ctx, 1024); // 1KB zeroblob
                data->blob_size = 1024;
                
                // Use sqlite3_result_error_toobig API
                // Trigger based on some condition (e.g., if blob too large)
                if (data->blob_size > 1000) {
                    sqlite3_result_error_toobig(ctx);
                    data->error_triggered = 1;
                }
            }
        };
        
        auto xFinal = [](sqlite3_context *ctx) {
            FuncContext *data = (FuncContext *)sqlite3_user_data(ctx);
            if (data && !data->error_triggered) {
                sqlite3_result_zeroblob64(ctx, data->blob_size);
            }
        };
        
        auto xValue = [](sqlite3_context *ctx) {
            // Optional: return current value
        };
        
        auto xInverse = [](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
            // Optional: inverse operation
        };
        
        auto xDestroy = [](void *data) {
            // Cleanup
        };
        
        // Create window function
        sqlite3_create_window_function(
            db, "test_func", 0, SQLITE_UTF8, &ctx_data,
            xStep, xFinal, xValue, xInverse, xDestroy
        );
        
        // Execute the function
        sqlite3_exec(db, "SELECT test_func() OVER ()", nullptr, nullptr, nullptr);
        
        sqlite3_backup_finish(backup);
    }
    
    // Cleanup
    sqlite3_close(db2);
    sqlite3_close(db);
    
    // Remove temporary files
    remove("input_file");
    remove("output_file");
    
    assert_file_closed(&in_file);
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	assert_file_name_closed("output_file");
	assert_file_name_closed("input_file");
	return 0;
}